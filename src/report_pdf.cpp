#include "cfp/report.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#ifndef _WIN32
#  include <sys/wait.h>
#endif

#ifdef _WIN32
#  include <windows.h>
#endif

namespace fs = std::filesystem;

namespace cfp::report {

namespace {

// -----------------------------------------------------------------------------
//  HTML 인쇄 엔진 탐색
//
//  [설계 근거] PDF를 직접 그리는 라이브러리를 넣지 않는 이유:
//  ① 화면(HTML)과 인쇄(PDF)를 각각 그리면 반드시 어긋난다.
//  ② 한글 폰트 임베딩/서브셋팅을 직접 구현하는 비용이 매우 크다.
//  브라우저 엔진에 인쇄를 맡기면 화면과 100% 동일하고 한글도 그대로 나온다.
//
//  Windows 타깃에서는 Edge(WebView2)가 OS에 기본 탑재되므로 항상 존재한다.
//  개발/CI 환경(리눅스)에서는 chromium을 찾아 같은 CLI 규약으로 처리한다.
// -----------------------------------------------------------------------------
std::string find_engine() {
#ifdef _WIN32
    // Edge는 Windows 10/11에 기본 포함 — 사실상 항상 hit.
    const char* candidates[] = {
        "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe",
        "C:\\Program Files\\Microsoft\\Edge\\Application\\msedge.exe",
        "C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe",
        "C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe",
    };
    for (const char* c : candidates)
        if (fs::exists(c)) return c;
    return {};
#else
    const char* names[] = {"chromium", "chromium-browser", "google-chrome",
                           "google-chrome-stable", "microsoft-edge"};
    // PATH 탐색은 which에 맡긴다(POSIX 한정 경로 규약을 직접 구현할 이유가 없다).
    for (const char* n : names) {
        const std::string cmd = std::string("command -v ") + n + " 2>/dev/null";
        if (FILE* p = popen(cmd.c_str(), "r")) {
            std::array<char, 512> buf{};
            std::string out;
            if (std::fgets(buf.data(), static_cast<int>(buf.size()), p)) out = buf.data();
            pclose(p);
            while (!out.empty() && (out.back() == '\n' || out.back() == '\r')) out.pop_back();
            if (!out.empty()) return out;
        }
    }
    return {};
#endif
}

std::string quote(const std::string& s) { return "\"" + s + "\""; }

}  // namespace

// =============================================================================
//  PDF 내보내기
//
//  [핵심 로직 해설 — 왜 file:// URL인가]
//  HTML에 이미지를 base64 data URI로 인라인했기 때문에 외부 파일 참조가 없다.
//  따라서 임시 디렉터리 어디에 두어도 렌더 결과가 동일하고,
//  헤드리스 브라우저의 로컬 파일 접근 제약(--allow-file-access-from-files 등)에
//  걸리지 않는다. 리포트를 자족형 HTML로 만든 설계가 여기서 값을 한다.
// =============================================================================
bool export_pdf(const std::string& html_path, const std::string& pdf_path,
                std::string& why) {
    const std::string engine = find_engine();
    if (engine.empty()) {
        why = "HTML 인쇄 엔진(Edge/Chrome/Chromium)을 찾지 못했습니다. "
              "HTML 보고서는 정상 생성되었으므로 브라우저에서 직접 인쇄(Ctrl+P)하면 됩니다.";
        return false;
    }

    // 프로필 디렉터리를 임시로 분리한다.
    // 사용자의 기본 브라우저 프로필을 건드리면 실행 중인 브라우저와 충돌한다.
    const fs::path tmp_profile =
        fs::temp_directory_path() / ("cfp_print_" + std::to_string(
            std::hash<std::string>{}(pdf_path)));

    std::string cmd = quote(engine)
        + " --headless=new"
        + " --disable-gpu"
        + " --no-sandbox"
        + " --disable-dev-shm-usage"
        + " --no-pdf-header-footer"
        // 애니메이션/폰트 로딩을 기다리다 무한 대기하는 것을 막는다.
        + " --virtual-time-budget=8000"
        + " --user-data-dir=" + quote(tmp_profile.string())
        + " --print-to-pdf=" + quote(pdf_path)
        + " " + quote("file://" + fs::absolute(html_path).string());

    // -------------------------------------------------------------------------
    //  [핵심] 외부 프로세스에는 반드시 타임아웃을 건다.
    //  헤드리스 브라우저는 디스플레이/IPC가 막힌 환경(CI 컨테이너, 원격 세션 등)에서
    //  종료하지 않고 매달리는 사례가 흔하다. 타임아웃이 없으면 앱 전체가 멈춘다.
    //  보고서 생성은 부가 기능이므로, 실패하더라도 앱은 계속 살아 있어야 한다.
    // -------------------------------------------------------------------------
    const int kTimeoutSec = 60;
#ifdef _WIN32
    // Windows: CreateProcess + WaitForSingleObject 로 대기 시간을 제어한다.
    int rc = -1;
    {
        STARTUPINFOA si{}; si.cb = sizeof(si);
        si.dwFlags = STARTF_USESHOWWINDOW; si.wShowWindow = SW_HIDE;
        PROCESS_INFORMATION pi{};
        std::string mutable_cmd = cmd;
        if (CreateProcessA(nullptr, mutable_cmd.data(), nullptr, nullptr, FALSE,
                           CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
            const DWORD w = WaitForSingleObject(pi.hProcess, kTimeoutSec * 1000);
            if (w == WAIT_TIMEOUT) {
                TerminateProcess(pi.hProcess, 1);
                rc = -2;                       // 타임아웃임을 구분
            } else {
                DWORD code = 1;
                GetExitCodeProcess(pi.hProcess, &code);
                rc = static_cast<int>(code);
            }
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
        }
    }
#else
    // POSIX: timeout(1)로 감싼다. coreutils는 사실상 어디에나 있다.
    const std::string wrapped =
        "timeout -k 5 " + std::to_string(kTimeoutSec) + " " + cmd + " >/dev/null 2>&1";
    int rc = std::system(wrapped.c_str());
    if (rc != 0) {
        const int exit_code = (rc == -1) ? -1 : WEXITSTATUS(rc);
        // timeout(1)은 시간 초과 시 124를 반환한다.
        if (exit_code == 124) rc = -2;
    }
#endif
    if (rc == -2) {
        std::error_code ec2;
        fs::remove_all(tmp_profile, ec2);
        why = "인쇄 엔진이 " + std::to_string(kTimeoutSec) +
              "초 내에 끝나지 않아 중단했습니다. HTML 보고서는 정상 생성되었습니다.";
        return false;
    }
    std::error_code ec;
    fs::remove_all(tmp_profile, ec);

    if (rc != 0 || !fs::exists(pdf_path)) {
        why = "인쇄 엔진 실행에 실패했습니다 (exit=" + std::to_string(rc) +
              ", engine=" + engine + ")";
        return false;
    }
    why.clear();
    return true;
}

bool save(const ReportInput& in, const std::string& out_dir,
          std::string& html_path_out, std::string& pdf_path_out, std::string& why) {
    std::error_code ec;
    fs::create_directories(out_dir, ec);

    html_path_out = (fs::path(out_dir) / "report.html").string();
    pdf_path_out  = (fs::path(out_dir) / "report.pdf").string();

    // HTML은 어떤 경우에도 먼저 저장한다.
    // PDF 변환이 실패해도 사용자는 결과물을 잃지 않아야 한다.
    {
        std::ofstream f(html_path_out, std::ios::binary);
        if (!f) { why = "HTML 파일을 쓸 수 없습니다: " + html_path_out; return false; }
        f << build_html(in);
    }
    return export_pdf(html_path_out, pdf_path_out, why);
}

}  // namespace cfp::report
