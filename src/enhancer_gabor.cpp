#include "cfp/enhancer.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <vector>

#include <opencv2/imgproc.hpp>
#include "cfp/thinning.hpp"

namespace cfp {

namespace {
constexpr double kPi = 3.14159265358979323846;

// 안전한 bilinear 샘플링. 경계 밖은 0을 반환한다.
inline float sample(const cv::Mat& f32, double x, double y) {
    const int x0 = static_cast<int>(std::floor(x));
    const int y0 = static_cast<int>(std::floor(y));
    if (x0 < 0 || y0 < 0 || x0 + 1 >= f32.cols || y0 + 1 >= f32.rows) return 0.0f;
    const double ax = x - x0, ay = y - y0;
    const float* r0 = f32.ptr<float>(y0);
    const float* r1 = f32.ptr<float>(y0 + 1);
    const double top = r0[x0] * (1 - ax) + r0[x0 + 1] * ax;
    const double bot = r1[x0] * (1 - ax) + r1[x0 + 1] * ax;
    return static_cast<float>(top * (1 - ay) + bot * ay);
}
}  // namespace

// =============================================================================
//  1) 정규화 — 평균 0, 분산 1
//
//  조명 불균일(비접촉 촬영의 고질병)을 제거해 이후 단계의 임계값이
//  영상마다 달라지지 않게 만든다. CLAHE를 먼저 걸어 국소 대비를 살린 뒤
//  전역 표준화를 한다. 순서가 반대면 CLAHE가 표준화로 만든 분포를 다시 흩는다.
// =============================================================================
cv::Mat GaborRidgeEnhancer::normalize(const cv::Mat& gray) const {
    cv::Mat g8;
    if (gray.type() != CV_8UC1) gray.convertTo(g8, CV_8UC1);
    else                        g8 = gray;

    if (cfg_.invert_polarity) cv::bitwise_not(g8, g8);   // 융선이 항상 "밝게"

    auto clahe = cv::createCLAHE(cfg_.clahe_clip, {cfg_.clahe_tile, cfg_.clahe_tile});
    cv::Mat eq;
    clahe->apply(g8, eq);

    cv::Mat f;
    eq.convertTo(f, CV_32F);
    cv::Scalar mu, sd;
    cv::meanStdDev(f, mu, sd);
    const double s = (sd[0] > 1e-6) ? sd[0] : 1.0;
    return (f - mu[0]) / s;
}

// =============================================================================
//  2) 방향장(orientation field) — 구조텐서 기반
//
//  [핵심 로직 해설 ①: 왜 각도를 직접 평균내면 안 되는가]
//  융선 방향은 "방향성 없는 축"이다. 0도와 179도는 사실상 같은 방향인데
//  산술평균을 내면 89.5도라는 엉뚱한 값이 나온다(wraparound 문제).
//  해결책은 각도를 2배로 만들어 벡터로 바꾸는 것이다.
//      theta -> (cos 2theta, sin 2theta)
//  2배로 만들면 theta와 theta+pi가 같은 벡터로 사상되므로,
//  이 벡터를 평균낸 뒤 다시 atan2/2 로 되돌리면 올바른 평균 방향이 나온다.
//
//  [핵심 로직 해설 ②: Vx, Vy가 이미 배각 벡터다]
//  gx, gy를 영상 기울기라 할 때
//      Vx = 2*gx*gy       = |g|^2 * sin(2*phi)
//      Vy = gx^2 - gy^2   = |g|^2 * cos(2*phi)     (phi = 기울기 방향)
//  즉 (Vy, Vx)는 크기가 |g|^2인 배각 벡터다. 따라서 Vx, Vy를 블록 평균하는 것
//  자체가 "기울기 크기로 가중된 배각 공간 평균"이 된다.
//  강한 기울기(선명한 융선)가 자동으로 더 큰 표를 행사한다.
//
//  [핵심 로직 해설 ③: 왜 +pi/2 인가]
//  영상 기울기는 융선을 가로지르는 방향(법선)을 가리킨다.
//  우리가 원하는 것은 융선을 따라가는 방향이므로 90도를 더한다.
//  검증: 세로로 흐르는 융선은 밝기가 x축으로만 변하므로 gx만 크다.
//        -> Vx=0, Vy>0 -> phi=0(수평 기울기) -> +pi/2 = pi/2 (수직 융선) OK
// =============================================================================
cv::Mat GaborRidgeEnhancer::orientation_field(const cv::Mat& norm) const {
    cv::Mat gx, gy;
    cv::Sobel(norm, gx, CV_32F, 1, 0, 3);
    cv::Sobel(norm, gy, CV_32F, 0, 1, 3);

    cv::Mat vx = 2.0 * gx.mul(gy);
    cv::Mat vy = gx.mul(gx) - gy.mul(gy);

    // 블록 평균 = 배각 공간에서의 가중 평균 (해설 ②)
    const cv::Size b(cfg_.block, cfg_.block);
    cv::boxFilter(vx, vx, CV_32F, b);
    cv::boxFilter(vy, vy, CV_32F, b);

    // 한 번 더 평활 — 방향장은 원래 매끄러워야 한다.
    // 여기서도 반드시 배각 공간(vx, vy)에서 해야 wraparound가 없다. (해설 ①)
    if (cfg_.orient_blur > 1) {
        const int k = cfg_.orient_blur | 1;   // 홀수 강제
        cv::GaussianBlur(vx, vx, {k, k}, 0);
        cv::GaussianBlur(vy, vy, {k, k}, 0);
    }

    cv::Mat orient(norm.size(), CV_32F);
    for (int y = 0; y < norm.rows; ++y) {
        const float* pvx = vx.ptr<float>(y);
        const float* pvy = vy.ptr<float>(y);
        float* po = orient.ptr<float>(y);
        for (int x = 0; x < norm.cols; ++x) {
            // 0.5*atan2 로 배각을 원래 각으로 되돌리고, +pi/2 로 법선->융선 (해설 ③)
            double th = 0.5 * std::atan2(static_cast<double>(pvx[x]),
                                         static_cast<double>(pvy[x])) + kPi / 2.0;
            // [0, pi) 로 접기 — 융선 방향은 pi 주기
            th = std::fmod(th, kPi);
            if (th < 0) th += kPi;
            po[x] = static_cast<float>(th);
        }
    }
    return orient;
}

// =============================================================================
//  3) 융선 주파수 — x-signature 법 (Hong, Wan & Jain 1998)
//
//  [핵심 로직 해설]
//  블록 중심에서 융선을 "가로지르는" 방향(법선)으로 한 줄을 뜨면
//  밝기가 융선-골-융선-골 로 주기적으로 오르내린다.
//  이 1차원 신호의 봉우리 간 평균 간격이 곧 융선 주기다.
//
//  노이즈를 줄이기 위해 법선 방향 각 위치에서 "융선을 따라" L픽셀을 평균낸다.
//  융선을 따라가면 값이 거의 일정하므로 이 평균은 신호를 죽이지 않고
//  잡음만 1/sqrt(L) 로 줄인다 — 신호대잡음비를 공짜로 올리는 지점.
//
//  FFT를 쓰지 않는 이유: 블록이 작아(32px) 주파수 분해능이 2~3 bin밖에 안 나온다.
//  봉우리 간격을 직접 재는 편이 이 크기에서 더 정확하다.
// =============================================================================
cv::Mat GaborRidgeEnhancer::frequency_field(const cv::Mat& norm,
                                            const cv::Mat& orient) const {
    const int W = cfg_.block * 2;                 // 법선 방향 창 길이
    const int L = cfg_.block;                     // 융선 방향 평균 길이
    const int step = std::max(4, cfg_.block / 2);

    cv::Mat freq = cv::Mat::zeros(norm.size(), CV_32F);
    std::vector<float> sig(static_cast<std::size_t>(W));

    for (int cy = 0; cy < norm.rows; cy += step) {
        for (int cx = 0; cx < norm.cols; cx += step) {
            const double th = orient.at<float>(cy, cx);      // 융선 방향
            const double tx = std::cos(th),      ty = std::sin(th);      // 융선 따라
            const double nx = std::cos(th + kPi / 2.0), ny = std::sin(th + kPi / 2.0); // 가로질러

            // --- x-signature 생성 ---
            for (int k = 0; k < W; ++k) {
                const double off = k - W / 2.0;
                double acc = 0.0;
                for (int l = 0; l < L; ++l) {
                    const double along = l - L / 2.0;
                    acc += sample(norm,
                                  cx + off * nx + along * tx,
                                  cy + off * ny + along * ty);
                }
                sig[static_cast<std::size_t>(k)] = static_cast<float>(acc / L);
            }

            // --- 국소 최대(봉우리) 위치 수집 ---
            std::vector<int> peaks;
            for (int k = 1; k + 1 < W; ++k) {
                if (sig[k] > sig[k - 1] && sig[k] >= sig[k + 1]) peaks.push_back(k);
            }
            if (peaks.size() < 2) continue;       // 주기 추정 불가 → 0으로 남겨 나중에 보간

            // --- 평균 봉우리 간격 = 융선 주기 ---
            double sum = 0.0;
            for (std::size_t i = 1; i < peaks.size(); ++i)
                sum += peaks[i] - peaks[i - 1];
            const double period = sum / static_cast<double>(peaks.size() - 1);

            // 생리학적으로 불가능한 주기는 버린다(잡음/모아레 방지)
            if (period < cfg_.min_period || period > cfg_.max_period) continue;

            const float f = static_cast<float>(1.0 / period);
            cv::Rect cell(cx - step / 2, cy - step / 2, step, step);
            cell &= cv::Rect(0, 0, freq.cols, freq.rows);
            if (cell.area() > 0) freq(cell).setTo(f);
        }
    }

    // 추정 실패한 구멍을 주변 값으로 메운다.
    // 중앙값 필터를 쓰면 0(실패)이 다수일 때 0이 전파되므로,
    // 유효값만 팽창(dilate)시켜 채운 뒤 평활한다.
    cv::Mat valid = (freq > 0);
    cv::Mat filled = freq.clone();
    for (int i = 0; i < 3; ++i) {
        cv::Mat d;
        cv::dilate(filled, d, cv::getStructuringElement(cv::MORPH_RECT, {5, 5}));
        d.copyTo(filled, ~valid);
        valid = (filled > 0);
    }
    cv::GaussianBlur(filled, filled, {7, 7}, 0);
    return filled;
}

// =============================================================================
//  4) 유효영역 마스크
//  정규화 영상의 국소 분산이 낮은 곳 = 융선 구조가 없는 곳(배경/포화/흐림).
//  E[x^2] - (E[x])^2 를 boxFilter 두 번으로 계산한다.
// =============================================================================
cv::Mat GaborRidgeEnhancer::region_mask(const cv::Mat& norm) const {
    const cv::Size b(cfg_.block, cfg_.block);
    cv::Mat mean, sq, mean_sq;
    cv::boxFilter(norm, mean, CV_32F, b);
    cv::multiply(norm, norm, sq);
    cv::boxFilter(sq, mean_sq, CV_32F, b);
    cv::Mat var = mean_sq - mean.mul(mean);

    cv::Mat mask;
    cv::threshold(var, mask, cfg_.mask_var_thresh, 255, cv::THRESH_BINARY);
    mask.convertTo(mask, CV_8UC1);

    // 구멍 메우기 + 가장자리 정리
    const cv::Mat k = cv::getStructuringElement(cv::MORPH_ELLIPSE, {9, 9});
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, k);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,  k);
    cv::erode(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, {5, 5}));
    return mask;
}

// =============================================================================
//  5) Gabor 필터뱅크
//
//  [핵심 로직 해설 ①: 커널 정의와 축 방향]
//      u =  x cos(th) + y sin(th)    (융선을 따라가는 축)
//      v = -x sin(th) + y cos(th)    (융선을 가로지르는 축 = 법선)
//      h(x,y) = exp(-0.5 (u^2/su^2 + v^2/sv^2)) * cos(2*pi*f*v)
//  코사인이 v(법선)를 따라 진동해야 융선-골의 주기와 맞물린다.
//  u를 따라 진동시키면 융선을 따라가며 켜졌다 꺼지므로 완전히 반대다.
//  (문헌마다 th를 융선 방향으로 두느냐 법선으로 두느냐가 달라 부호 실수가 잦다.
//   검증: 수직 융선 th=pi/2 -> v = -x -> cos(2*pi*f*(-x)) 는 x축을 따라 진동 = 맞음)
//
//  [핵심 로직 해설 ②: 왜 필터뱅크인가]
//  이상적으로는 픽셀마다 (th, f)에 맞는 커널을 만들어 컨볼루션해야 한다.
//  그러면 커널 생성 비용이 픽셀 수만큼 든다.
//  대신 (th, f)를 각각 양자화해 커널을 미리 만들어 두고,
//  bin별로 영상 전체를 한 번씩 필터링한 뒤 해당 bin 픽셀만 골라 담는다.
//  연산량이 (bin 수) x (영상 크기 x 커널 크기)로 묶인다.
//  실제로 등장하는 bin 조합만 만들어 낭비를 더 줄인다.
// =============================================================================
cv::Mat GaborRidgeEnhancer::gabor_filter(const cv::Mat& norm, const cv::Mat& orient,
                                         const cv::Mat& freq, const cv::Mat& mask) const {
    const int    nO = std::max(4, cfg_.gabor_orient_n);
    const int    nF = std::max(1, cfg_.freq_bins);
    const double fmin = 1.0 / cfg_.max_period;
    const double fmax = 1.0 / cfg_.min_period;

    // --- 픽셀별 bin 인덱스 계산 ---
    cv::Mat obin(norm.size(), CV_8U, cv::Scalar(0));
    cv::Mat fbin(norm.size(), CV_8U, cv::Scalar(255));   // 255 = 무효
    for (int y = 0; y < norm.rows; ++y) {
        const float* po = orient.ptr<float>(y);
        const float* pf = freq.ptr<float>(y);
        const uchar* pm = mask.ptr<uchar>(y);
        uchar* pob = obin.ptr<uchar>(y);
        uchar* pfb = fbin.ptr<uchar>(y);
        for (int x = 0; x < norm.cols; ++x) {
            if (!pm[x] || pf[x] <= 0.0f) continue;
            int oi = static_cast<int>(std::lround(po[x] / kPi * nO)) % nO;
            if (oi < 0) oi += nO;
            const double t = (pf[x] - fmin) / (fmax - fmin);
            int fi = static_cast<int>(std::lround(std::clamp(t, 0.0, 1.0) * (nF - 1)));
            pob[x] = static_cast<uchar>(oi);
            pfb[x] = static_cast<uchar>(fi);
        }
    }

    cv::Mat out = cv::Mat::zeros(norm.size(), CV_32F);

    // --- 실제 등장하는 (o, f) 조합만 처리 ---
    std::map<std::pair<int, int>, bool> present;
    for (int y = 0; y < norm.rows; ++y) {
        const uchar* pob = obin.ptr<uchar>(y);
        const uchar* pfb = fbin.ptr<uchar>(y);
        for (int x = 0; x < norm.cols; ++x)
            if (pfb[x] != 255) present[{pob[x], pfb[x]}] = true;
    }

    for (const auto& kv : present) {
        const int oi = kv.first.first, fi = kv.first.second;
        const double th = (oi + 0.5) * kPi / nO;
        const double f  = (nF == 1) ? 0.5 * (fmin + fmax)
                                    : fmin + (fmax - fmin) * fi / (nF - 1);
        const double period = 1.0 / f;
        const double sv = cfg_.gabor_sigma_mul * period;   // 법선 방향 폭
        const double su = cfg_.gabor_sigma_mul * period;   // 융선 방향 폭

        // 커널 반경은 sigma의 3배까지 — 그 밖은 가우시안이 0에 수렴해 기여가 없다.
        const int rad = std::max(3, static_cast<int>(std::ceil(3.0 * std::max(su, sv))));
        const int ks  = 2 * rad + 1;
        cv::Mat kern(ks, ks, CV_32F);
        const double ct = std::cos(th), st = std::sin(th);
        for (int y = -rad; y <= rad; ++y) {
            for (int x = -rad; x <= rad; ++x) {
                const double u =  x * ct + y * st;    // 융선 따라
                const double v = -x * st + y * ct;    // 융선 가로질러
                const double g = std::exp(-0.5 * (u * u / (su * su) + v * v / (sv * sv)))
                                 * std::cos(2.0 * kPi * f * v);
                kern.at<float>(y + rad, x + rad) = static_cast<float>(g);
            }
        }
        // DC 성분 제거 — 커널 합이 0이 아니면 밝기 오프셋이 응답에 섞인다.
        kern -= static_cast<float>(cv::mean(kern)[0]);

        cv::Mat resp;
        cv::filter2D(norm, resp, CV_32F, kern, {-1, -1}, 0, cv::BORDER_REPLICATE);

        cv::Mat sel = (obin == oi) & (fbin == fi);
        resp.copyTo(out, sel);
    }
    return out;
}

// =============================================================================
//  파이프라인 조립
// =============================================================================
EnhanceResult GaborRidgeEnhancer::enhance(const cv::Mat& roi_gray,
                                          const cv::Mat& roi_mask) const {
    EnhanceResult r;
    if (roi_gray.empty()) return r;

    r.normalized  = normalize(roi_gray);
    r.orientation = orientation_field(r.normalized);
    r.mask        = region_mask(r.normalized);

    // 세그멘테이션이 넘겨준 손가락 마스크와 교집합을 취한다.
    // 국소분산만으로 만든 마스크는 배경 질감(벽, 그림자 경계)도 통과시키므로,
    // "손가락 안쪽"이라는 상위 단계의 지식이 반드시 필요하다.
    // 실제 스마트폰 촬영 사진 테스트에서 배경이 유효영역으로 새어 들어가
    // 위양성 미뉴셔가 생기는 것을 확인하고 추가한 처리다.
    if (!roi_mask.empty() && roi_mask.size() == r.mask.size()) {
        cv::Mat prior;
        // 경계 근처는 조명 감쇠로 융선이 뭉개지므로 안쪽으로 한 번 더 깎는다.
        cv::erode(roi_mask, prior,
                  cv::getStructuringElement(cv::MORPH_ELLIPSE, {13, 13}));
        cv::bitwise_and(r.mask, prior, r.mask);
    }
    r.frequency   = frequency_field(r.normalized, r.orientation);
    cv::Mat resp  = gabor_filter(r.normalized, r.orientation, r.frequency, r.mask);

    // 표시/저장용 8비트 변환
    cv::normalize(resp, r.enhanced, 0, 255, cv::NORM_MINMAX, CV_8UC1, r.mask);

    // --- 이진화 ---
    // Gabor 응답은 융선에서 양수, 골에서 음수가 되도록 설계됐으므로 0이 자연 임계값이다.
    // (전역 Otsu를 쓰면 마스크 밖 0값들이 히스토그램을 왜곡시킨다)
    r.binary = cv::Mat::zeros(resp.size(), CV_8UC1);
    r.binary.setTo(255, (resp > 0) & (r.mask > 0));

    // --- 세선화 (Zhang-Suen) ---
    // 미뉴셔 정의(Crossing Number)가 1픽셀 두께 스켈레톤을 전제로 한다.
    zhang_suen_thinning(r.binary, r.skeleton);

    // -------------------------------------------------------------------------
    //  [핵심] 마스크 경계선이 만드는 가짜 융선 제거
    //
    //  이진 영상에서 마스크 경계는 그 자체로 하나의 큰 연결 영역의 테두리다.
    //  세선화는 이 테두리를 1픽셀 곡선으로 그대로 추적해 버린다.
    //  그 결과 손가락 윤곽을 따라가는 '가짜 융선'이 생기고,
    //  진짜 융선들이 거기에 T자로 붙으면서 존재하지 않는 분기점(CN=3)이 무더기로 생긴다.
    //
    //  실제 스마트폰 촬영 사진 테스트에서 이 현상을 확인했다.
    //  마스크를 안쪽으로 깎아 골격을 잘라내면 경계선 자체가 사라진다.
    //  (미뉴셔 추출기의 border_margin은 2차 방어선이지, 이 문제의 해결책이 아니다.
    //   경계선이 진짜 융선의 CN 값을 오염시키기 때문에 여기서 끊어야 한다.)
    // -------------------------------------------------------------------------
    if (!r.mask.empty()) {
        cv::Mat inner;
        cv::erode(r.mask, inner,
                  cv::getStructuringElement(cv::MORPH_ELLIPSE, {7, 7}));
        cv::bitwise_and(r.skeleton, inner, r.skeleton);
        cv::bitwise_and(r.binary,   inner, r.binary);
    }
    return r;
}

}  // namespace cfp
