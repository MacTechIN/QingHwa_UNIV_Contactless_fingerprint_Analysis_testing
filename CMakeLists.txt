cmake_minimum_required(VERSION 3.20)
project(ContactlessFp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

find_package(OpenCV 4.5 REQUIRED COMPONENTS core imgproc imgcodecs ximgproc)

# -----------------------------------------------------------------------------
# cfp_core — OpenCV 비의존. 매칭/평가/해설 텍스트.
#   [의도] 이 타깃만 있으면 데이터셋도 카메라도 없이 CI에서 로직을 검증할 수 있다.
# -----------------------------------------------------------------------------
add_library(cfp_core STATIC
  src/metrics.cpp
  src/matcher_minutiae.cpp
  src/narration.cpp
)
target_include_directories(cfp_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# -----------------------------------------------------------------------------
# cfp_imaging — 영상 파이프라인 (OpenCV 의존)
# -----------------------------------------------------------------------------
add_library(cfp_imaging STATIC
  src/quality.cpp
  src/segmenter_ycbcr.cpp
  src/enhancer_gabor.cpp
  src/minutiae_cn.cpp
  src/pipeline.cpp
  src/narrate_stages.cpp
  src/report_html.cpp
  src/report_pdf.cpp
)
target_link_libraries(cfp_imaging PUBLIC cfp_core ${OpenCV_LIBS})
target_include_directories(cfp_imaging PUBLIC ${OpenCV_INCLUDE_DIRS})

# OpenCV 4.10 헤더가 C++20에서 내는 enum-enum 변환 경고를 억제한다.
# 우리 코드의 문제가 아니라 서드파티 헤더 문제이므로 전역 억제가 타당하다.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(cfp_imaging PRIVATE -Wno-deprecated-enum-enum-conversion)
endif()

# -----------------------------------------------------------------------------
# CLI (모든 플랫폼)
# -----------------------------------------------------------------------------
add_executable(cfp_cli tools/cfp_cli.cpp)
target_link_libraries(cfp_cli PRIVATE cfp_imaging)
target_include_directories(cfp_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(cfp_cli PRIVATE -Wno-deprecated-enum-enum-conversion)
endif()

# -----------------------------------------------------------------------------
# Windows 네이티브 앱 (WinUI 3 셸은 별도 vcxproj, 여기서는 캡처 백엔드만)
# -----------------------------------------------------------------------------
if(WIN32)
  add_library(cfp_capture STATIC src/capture_mf.cpp)
  target_link_libraries(cfp_capture PUBLIC cfp_imaging
    mf mfplat mfreadwrite mfuuid ole32 shlwapi)
endif()

# -----------------------------------------------------------------------------
# 테스트
# -----------------------------------------------------------------------------
enable_testing()
add_executable(test_metrics tests/test_metrics.cpp)
target_link_libraries(test_metrics PRIVATE cfp_core)
target_include_directories(test_metrics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_test(NAME metrics COMMAND test_metrics)

add_executable(test_matcher tests/test_matcher.cpp)
target_link_libraries(test_matcher PRIVATE cfp_core)
target_include_directories(test_matcher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_test(NAME matcher COMMAND test_matcher)

add_executable(test_pipeline tests/test_pipeline.cpp)
target_link_libraries(test_pipeline PRIVATE cfp_imaging)
target_include_directories(test_pipeline PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(test_pipeline PRIVATE -Wno-deprecated-enum-enum-conversion)
endif()
add_test(NAME pipeline COMMAND test_pipeline)

# -----------------------------------------------------------------------------
# 개발 환경 보정 (Linux/CI 전용)
#   conda-forge로 설치한 OpenCV는 시스템 GCC보다 새 libstdc++로 빌드되어 있어
#   시스템 링커가 CXXABI 심볼을 못 찾는 경우가 있다.
#   OpenCV가 있는 prefix의 런타임을 우선 링크하도록 rpath를 지정해 해결한다.
#   Windows/vcpkg 빌드에는 영향이 없다.
# -----------------------------------------------------------------------------
if(UNIX AND NOT APPLE AND DEFINED CMAKE_PREFIX_PATH)
  foreach(_t cfp_imaging cfp_cli test_pipeline)
    if(TARGET ${_t})
      target_link_directories(${_t} PUBLIC ${CMAKE_PREFIX_PATH}/lib)
      set_target_properties(${_t} PROPERTIES
        BUILD_RPATH   "${CMAKE_PREFIX_PATH}/lib"
        INSTALL_RPATH "${CMAKE_PREFIX_PATH}/lib")
    endif()
  endforeach()
endif()
