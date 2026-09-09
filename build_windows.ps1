# =============================================================================
#  build_windows.ps1 — Windows 원클릭 빌드 (PowerShell)
#
#  vcpkg가 없으면 받아서 부트스트랩하고, OpenCV를 설치한 뒤 앱까지 빌드한다.
#  최초 실행은 OpenCV 빌드 때문에 20~40분 걸린다(이후에는 캐시되어 수 분).
#
#  사용:  powershell -ExecutionPolicy Bypass -File build_windows.ps1
#         powershell -ExecutionPolicy Bypass -File build_windows.ps1 -Run
# =============================================================================
param(
    [switch]$Run,                       # 빌드 후 바로 실행
    [string]$VcpkgRoot = "$env:USERPROFILE\vcpkg"
)
$ErrorActionPreference = "Stop"
$root = $PSScriptRoot

function Step($m) { Write-Host "`n=== $m ===" -ForegroundColor Cyan }

# --- 선행 도구 확인 ---
Step "선행 도구 확인"
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    throw "CMake가 없습니다. https://cmake.org/download/ 에서 설치하거나 ``winget install Kitware.CMake``"
}
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    throw "Git이 없습니다. ``winget install Git.Git``"
}
# Visual Studio 2022 빌드도구(C++ 워크로드)가 필요하다.
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vswhere) {
    $vs = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property displayName
    if ($vs) { Write-Host "  Visual Studio: $vs" } else {
        Write-Warning "  VC++ 빌드도구를 못 찾았습니다. ``winget install Microsoft.VisualStudio.2022.BuildTools`` 후 'C++ 데스크톱 개발' 워크로드를 설치하세요."
    }
}
Write-Host "  CMake: $((cmake --version | Select-Object -First 1))"

# --- vcpkg ---
Step "vcpkg 준비"
if (-not (Test-Path "$VcpkgRoot\vcpkg.exe")) {
    if (-not (Test-Path $VcpkgRoot)) {
        git clone --depth 1 https://github.com/microsoft/vcpkg.git $VcpkgRoot
    }
    & "$VcpkgRoot\bootstrap-vcpkg.bat" -disableMetrics
}
Write-Host "  vcpkg: $VcpkgRoot"

# --- OpenCV ---
Step "OpenCV 설치 (최초 20~40분 소요)"
& "$VcpkgRoot\vcpkg.exe" install "opencv4[contrib,png,jpeg]:x64-windows"
if ($LASTEXITCODE -ne 0) { throw "OpenCV 설치 실패" }

# --- 빌드 ---
Step "CMake 구성 및 빌드"
$toolchain = "$VcpkgRoot\scripts\buildsystems\vcpkg.cmake"
cmake -S $root -B "$root\build" -A x64 `
      -DCMAKE_TOOLCHAIN_FILE="$toolchain" `
      -DVCPKG_TARGET_TRIPLET=x64-windows
if ($LASTEXITCODE -ne 0) { throw "CMake 구성 실패" }

cmake --build "$root\build" --config Release -j
if ($LASTEXITCODE -ne 0) { throw "빌드 실패" }

# --- 테스트 ---
Step "테스트 실행"
ctest --test-dir "$root\build" -C Release --output-on-failure

# --- 결과 ---
$exe = "$root\build\Release\ContactlessFP.exe"
Step "완료"
if (Test-Path $exe) {
    Write-Host "  앱      : $exe" -ForegroundColor Green
    Write-Host "  CLI     : $root\build\Release\cfp_cli.exe" -ForegroundColor Green
    if ($Run) { Start-Process $exe }
    else { Write-Host "`n  실행:  .\build\Release\ContactlessFP.exe" -ForegroundColor Yellow }
} else {
    Write-Warning "  실행 파일을 찾지 못했습니다: $exe"
}
