cmake_minimum_required(VERSION 3.20)
project(ContactlessFp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# [의존성 최소화] ximgproc(opencv_contrib)을 쓰지 않는다.
# 세선화 함수 하나 때문에 contrib 전체를 끌어오면 vcpkg에서
# contrib -> dnn -> protobuf -> abseil 연쇄가 발생해 빌드가 수십 분 늘고
# 실패 지점도 늘어난다(실제로 Windows 빌드가 abseil에서 실패했다).
# Zhang-Suen은 src/thinning.cpp에 직접 구현했고 OpenCV 구현과 대조 검증했다.
# 버전을 못박지 않는다. OpenCV 4.x 프리빌트/vcpkg 빌드와 5.x 프리빌트를 모두 받는다.
# 사용하는 API가 core/imgproc/imgcodecs의 안정 표면뿐이라 4.5~5.x에서 동일하게 동작한다.
# -----------------------------------------------------------------------------
# [필수] MSVC에 소스가 UTF-8임을 알린다.
#
# 이 옵션이 없으면 MSVC는 소스를 시스템 코드페이지(한국어 Windows는 CP949)로 읽는다.
# 우리 소스의 한글 주석이 깨지고, 깨진 바이트가 뒤따르는 코드를 삼켜
# "cfp::cfp::cv::Mat 재정의" 같은 전혀 무관해 보이는 오류가 연쇄적으로 터진다.
#
# [주의] 예전에는 타깃마다 target_compile_options로 붙였는데 테스트 타깃 3개를
# 빠뜨렸고, 정확히 그 3개만 깨졌다. 전역 옵션으로 두어 새 타깃을 추가해도
# 자동으로 적용되게 한다.
if(MSVC)
  add_compile_options(/utf-8)
endif()

find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)
message(STATUS "OpenCV ${OpenCV_VERSION}  (${OpenCV_DIR})")

# OpenCV 5.x는 2D 기하 함수(minAreaRect / contourArea / getRotationMatrix2D)를
# imgproc에서 새 geometry 모듈로 옮겼다. 5 이상이면 그 컴포넌트를 추가로 요구한다.
# (4.x에는 geometry 모듈이 없으므로 조건부로만 넣는다)
if(OpenCV_VERSION VERSION_GREATER_EQUAL 5)
  message(STATUS "OpenCV 5.x 감지 - geometry 모듈 추가")
  find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs geometry)
endif()
if(OpenCV_VERSION VERSION_LESS 4.5)
  message(FATAL_ERROR "OpenCV 4.5 이상이 필요합니다. 발견된 버전: ${OpenCV_VERSION}")
endif()

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
  src/thinning.cpp
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
  # 카메라 캡처 (Media Foundation)
  add_library(cfp_capture STATIC src/capture_mf.cpp)
  target_link_libraries(cfp_capture PUBLIC cfp_imaging
    mf mfplat mfreadwrite mfuuid ole32 oleaut32 shlwapi strmiids)
  target_compile_definitions(cfp_capture PUBLIC UNICODE _UNICODE)

  # Win32 네이티브 GUI 앱
  #  WIN32 키워드 = /SUBSYSTEM:WINDOWS (콘솔 창이 뜨지 않는다)
  add_executable(cfp_app WIN32 app/main_win32.cpp app/app.manifest)
  target_link_libraries(cfp_app PRIVATE cfp_capture comctl32 comdlg32 shell32)
  target_compile_definitions(cfp_app PRIVATE UNICODE _UNICODE NOMINMAX
                                             WIN32_LEAN_AND_MEAN)
  set_target_properties(cfp_app PROPERTIES OUTPUT_NAME "ContactlessFP")

  # wWinMain(유니코드 진입점)을 쓰므로 진입점을 명시한다.
  # 생략하면 링커가 WinMain을 찾다가 unresolved external symbol로 실패할 수 있다.
  if(MSVC)
    target_link_options(cfp_app PRIVATE "/ENTRY:wWinMainCRTStartup")
  endif()

  # ---------------------------------------------------------------------------
  # 런타임 DLL을 실행 파일 옆으로 복사한다.
  #
  # [왜 필요한가] OpenCV 공식 Windows 프리빌트는 동적 라이브러리(opencv_worldXXX.dll)다.
  # PATH에 없으면 앱이 "DLL을 찾을 수 없습니다"로 시작조차 못 한다.
  # 사용자가 환경변수를 만지게 하는 대신 빌드 산출물 옆에 복사해 둔다.
  # (vcpkg 툴체인은 이 작업을 알아서 해 주지만, 프리빌트 OpenCV는 해 주지 않는다)
  # ---------------------------------------------------------------------------
  if(NOT CMAKE_VERSION VERSION_LESS 3.21)
    foreach(_exe cfp_app cfp_cli)
      if(TARGET ${_exe})
        add_custom_command(TARGET ${_exe} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
                  "$<TARGET_RUNTIME_DLLS:${_exe}>" "$<TARGET_FILE_DIR:${_exe}>"
          COMMAND_EXPAND_LISTS
          COMMENT "런타임 DLL 복사 → $<TARGET_FILE_DIR:${_exe}>")
      endif()
    endforeach()
  endif()

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
# 개발 환경 보정 (Linux 전용)
#
#   conda/micromamba로 설치한 OpenCV는 시스템 GCC보다 새 libstdc++로 빌드되어 있어
#   시스템 링커가 CXXABI 심볼(__cxa_call_terminate@CXXABI_1.3.15 등)을 찾지 못한다.
#   OpenCV가 설치된 prefix의 lib를 링크 경로와 rpath에 넣어 해결한다.
#
#   [주의] 예전에는 CMAKE_PREFIX_PATH가 정의된 경우에만 동작했다. 그래서
#   -DOpenCV_DIR=... 로만 구성하면 보정이 걸리지 않아 링크가 깨졌다.
#   이제 OpenCV_INSTALL_PATH(OpenCVConfig.cmake가 설정)를 우선 사용하고,
#   없으면 OpenCV_DIR에서 거슬러 올라가 추론한다.
#   Windows/vcpkg 빌드에는 영향이 없다.
# -----------------------------------------------------------------------------
if(UNIX AND NOT APPLE)
  set(_cfp_extra_libdirs "")
  if(DEFINED OpenCV_INSTALL_PATH AND EXISTS "${OpenCV_INSTALL_PATH}/lib")
    list(APPEND _cfp_extra_libdirs "${OpenCV_INSTALL_PATH}/lib")
  endif()
  if(DEFINED CMAKE_PREFIX_PATH AND EXISTS "${CMAKE_PREFIX_PATH}/lib")
    list(APPEND _cfp_extra_libdirs "${CMAKE_PREFIX_PATH}/lib")
  endif()
  if(NOT _cfp_extra_libdirs AND DEFINED OpenCV_DIR)
    # 예: <prefix>/lib/cmake/opencv4 -> <prefix>/lib
    get_filename_component(_p "${OpenCV_DIR}" DIRECTORY)   # .../lib/cmake
    get_filename_component(_p "${_p}" DIRECTORY)           # .../lib
    if(EXISTS "${_p}" AND IS_DIRECTORY "${_p}")
      list(APPEND _cfp_extra_libdirs "${_p}")
    endif()
  endif()
  list(REMOVE_DUPLICATES _cfp_extra_libdirs)

  if(_cfp_extra_libdirs)
    message(STATUS "런타임 라이브러리 경로 보정: ${_cfp_extra_libdirs}")
    foreach(_t cfp_imaging cfp_cli test_pipeline)
      if(TARGET ${_t})
        target_link_directories(${_t} PUBLIC ${_cfp_extra_libdirs})
        set_target_properties(${_t} PROPERTIES
          BUILD_RPATH   "${_cfp_extra_libdirs}"
          INSTALL_RPATH "${_cfp_extra_libdirs}")
      endif()
    endforeach()
  endif()
endif()
