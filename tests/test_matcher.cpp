#include "cfp/matcher.hpp"
#include "test_util.hpp"

#include <cmath>
#include <numbers>
#include <random>

// kPi는 POSIX 확장이라 표준 C++에 없다. MSVC는 _USE_MATH_DEFINES 없이는
// 정의하지 않으므로 이식성이 없다. C++20의 std::numbers::pi를 쓴다.
inline constexpr double kPi = std::numbers::pi;

using namespace cfp;

static Template make_template(int n, unsigned seed, int w = 256, int h = 352) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> ux(20, w - 20), uy(20, h - 20),
                                           ut(0, 2 * kPi);
    Template t; t.width = w; t.height = h; t.subject_id = "s" + std::to_string(seed);
    for (int i = 0; i < n; ++i) {
        Minutia m;
        m.x = (float)ux(rng); m.y = (float)uy(rng); m.theta = (float)ut(rng);
        m.quality = 0.8f; m.type = MinutiaType::RidgeEnding;
        t.minutiae.push_back(m);
    }
    return t;
}

// 강체변환을 가한 사본 (+ 관측잡음)
static Template transform(const Template& src, double deg, double tx, double ty,
                          double noise_px, unsigned seed) {
    std::mt19937 rng(seed);
    std::normal_distribution<double> nz(0.0, noise_px);
    const double r = deg * kPi / 180.0, cs = std::cos(r), sn = std::sin(r);
    Template t = src; t.subject_id = src.subject_id + "_x";
    for (auto& m : t.minutiae) {
        const double x = m.x, y = m.y;
        m.x = (float)(x * cs - y * sn + tx + nz(rng));
        m.y = (float)(x * sn + y * cs + ty + nz(rng));
        m.theta = (float)std::fmod(m.theta + r + 2 * kPi, 2 * kPi);
    }
    return t;
}

int main() {
    std::printf("== matcher ==\n");
    MinutiaMatcher matcher;

    // --- 각도 정규화 ---
    tu::near(wrap_pi(3 * kPi), kPi, 1e-9, "wrap_pi(3pi) == pi");
    tu::near(wrap_pi(-3 * kPi / 2), kPi / 2, 1e-9, "wrap_pi(-3pi/2) == pi/2");
    tu::near(std::fabs(wrap_half_pi(kPi - 0.01)), 0.01, 1e-9,
             "wrap_half_pi: 179도와 0도는 사실상 같은 융선 방향");

    const Template a = make_template(45, 1);

    // --- 자기 자신과의 매칭 ---
    {
        auto r = matcher.match(a, a);
        tu::check(r.score > 0.7, "동일 템플릿 스코어 > 0.7");
        tu::check(r.inliers >= 40, "동일 템플릿 inlier >= 40");
    }

    // --- 회전 + 이동 + 잡음 ---
    {
        const Template b = transform(a, 18.0, 12.0, -7.0, 1.2, 99);
        auto r = matcher.match(a, b);
        tu::check(r.score > 0.5, "18도 회전 + 이동 후에도 스코어 > 0.5");
        tu::near(r.rotation * 180.0 / kPi, 18.0, 6.0, "추정 회전각 ~= 18도");
    }

    // --- 타인 ---
    {
        const Template c = make_template(45, 777);
        auto r = matcher.match(a, c);
        tu::check(r.score < 0.15, "무관한 템플릿 스코어 < 0.15");
    }

    // --- 본인 > 타인 분리 (여러 시드) ---
    {
        int ok = 0;
        for (unsigned s = 10; s < 30; ++s) {
            const Template p = make_template(40, s);
            const Template q = transform(p, 12.0, 6.0, 4.0, 1.0, s + 500);
            const Template o = make_template(40, s + 9000);
            if (matcher.match(p, q).score > matcher.match(p, o).score) ++ok;
        }
        tu::check(ok >= 19, "20회 중 19회 이상 본인쌍 > 타인쌍 (실제 " +
                            std::to_string(ok) + "/20)");
    }

    // --- 빈 템플릿 방어 ---
    {
        Template empty;
        tu::near(matcher.match(a, empty).score, 0.0, 1e-12, "빈 템플릿 → 스코어 0");
    }
    return tu::summary("matcher");
}
