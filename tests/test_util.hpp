// 최소 테스트 하네스 — 외부 의존(gtest 등) 없이 CI를 가볍게 유지한다.
#pragma once
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace tu {
inline int g_fail = 0;
inline int g_total = 0;

inline void check(bool cond, const std::string& what) {
    ++g_total;
    if (!cond) { ++g_fail; std::printf("  [FAIL] %s\n", what.c_str()); }
    else       { std::printf("  [ ok ] %s\n", what.c_str()); }
}
inline void near(double a, double b, double tol, const std::string& what) {
    ++g_total;
    if (std::fabs(a - b) > tol) {
        ++g_fail;
        std::printf("  [FAIL] %s  (got %.6f, want %.6f +-%.6f)\n",
                    what.c_str(), a, b, tol);
    } else {
        std::printf("  [ ok ] %s  (%.6f)\n", what.c_str(), a);
    }
}
inline int summary(const char* suite) {
    std::printf("\n== %s: %d/%d passed ==\n", suite, g_total - g_fail, g_total);
    return g_fail == 0 ? 0 : 1;
}
}  // namespace tu
