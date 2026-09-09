# =============================================================================
#  build_windows.ps1 - Windows 원클릭 빌드 (PowerShell)
#
#  [인코딩] 이 파일은 반드시 UTF-8 BOM으로 저장되어야 한다.
#  Windows PowerShell 5.1은 BOM이 없으면 스크립트를 시스템 코드페이지(한국어는
#  CP949)로 읽는다. 그러면 UTF-8로 쓴 한글이 깨지고, 깨진 바이트 중 일부가
#  따옴표로 해석되어 파서가 죽는다.
#
#  사용:  powershell -ExecutionPolicy Bypass -File build_windows.ps1
#         powershell -ExecutionPolicy Bypass -File build_windows.ps1 -Run
# =============================================================================
param(
    [switch]$Run,                       # 빌드 후 바로 실행
    [switch]$SkipTests,
    # [중요] 반드시 ASCII 전용 경로여야 한다.
    # vcpkg는 경로에 한글 등 비ASCII 문자가 있으면 내부 도구(ninja 등) 취득 단계에서
    # "no such file or directory"로 실패한다. 한국어 Windows의 기본 사용자 폴더
    # (C:\Users\홍길동)가 그대로 걸리는 함정이다.
    [string]$VcpkgRoot = "C:\vcpkg"
)
$ErrorActionPreference = "Stop"
$root = $PSScriptRoot

# 콘솔 출력 인코딩을 UTF-8로 맞춘다(한글 메시지가 깨지지 않도록).
try { [Console]::OutputEncoding = [System.Text.Encoding]::UTF8 } catch { }

function Step([string]$m) { Write-Host "`n=== $m ===" -ForegroundColor Cyan }
function Info([string]$m) { Write-Host "  $m" }
function Ok  ([string]$m) { Write-Host "  $m" -ForegroundColor Green }

Step "선행 도구 확인"

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    throw "CMake가 없습니다.  winget install Kitware.CMake"
}
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    throw "Git이 없습니다.  winget install Git.Git"
}
Info ("CMake : " + ((cmake --version) | Select-Object -First 1))

# Visual Studio 2022 빌드도구(C++ 워크로드) 확인
$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vswhere) {
    $vs = & $vswhere -latest -products '*' `
            -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
            -property displayName
    if ($vs) {
        Info ("VS    : " + $vs)
    } else {
        Write-Warning ("VC++ 빌드도구를 찾지 못했습니다.`n" +
            "  winget install Microsoft.VisualStudio.2022.BuildTools`n" +
            "  설치 시 'C++를 사용한 데스크톱 개발' 워크로드를 반드시 선택하세요.")
    }
} else {
    Write-Warning "vswhere를 찾지 못했습니다. Visual Studio 설치 여부를 확인하세요."
}

Step "경로 점검 (비ASCII 문자)"
function Test-Ascii([string]$p) { return ($p -notmatch '[^\x00-\x7F]') }

if (-not (Test-Ascii $VcpkgRoot)) {
    Write-Warning "vcpkg 경로에 비ASCII 문자가 있습니다: $VcpkgRoot"
    $VcpkgRoot = "C:\vcpkg"
    Info "ASCII 경로로 대체합니다: $VcpkgRoot"
}
if (-not (Test-Ascii $root)) {
    Write-Warning ("프로젝트 경로에 한글이 포함되어 있습니다:`n  $root`n" +
        "  MSVC/CMake는 대개 문제없지만, 빌드가 계속 실패하면 " +
        "C:\dev\ 같은 ASCII 경로로 저장소를 옮겨 다시 시도하세요.")
}
Info "vcpkg 경로 : $VcpkgRoot"

Step "vcpkg 준비"
if (-not (Test-Path (Join-Path $VcpkgRoot "vcpkg.exe"))) {
    if (-not (Test-Path $VcpkgRoot)) {
        Info "vcpkg 클론 중... ($VcpkgRoot)"
        try {
            New-Item -ItemType Directory -Force -Path $VcpkgRoot | Out-Null
        } catch {
            throw ("$VcpkgRoot 를 만들 수 없습니다. 관리자 권한으로 한 번 실행하거나 " +
                   "-VcpkgRoot D:\vcpkg 처럼 쓰기 가능한 ASCII 경로를 지정하세요.")
        }
        git clone --depth 1 https://github.com/microsoft/vcpkg.git $VcpkgRoot
        if ($LASTEXITCODE -ne 0) { throw "vcpkg 클론 실패" }
    }
    & (Join-Path $VcpkgRoot "bootstrap-vcpkg.bat") -disableMetrics
    if ($LASTEXITCODE -ne 0) { throw "vcpkg 부트스트랩 실패" }
}
Info "vcpkg : $VcpkgRoot"

Step "OpenCV 설치 (최초 1회 5~15분 소요)"
# [왜 classic 모드인가]
# manifest 모드는 설치 트리를 <repo>/vcpkg_installed/ 에 만든다. 저장소가
# 한글 경로(C:\Users\이상진\...)에 있으면 그 경로가 vcpkg 내부 도구 취득 단계에서
# 깨진다. classic 모드로 ASCII 경로(C:\vcpkg\installed)에 설치하면 이 문제를 피한다.
# vcpkg.json은 의존성 문서로 남기고, CMake에는 VCPKG_MANIFEST_MODE=OFF를 넘긴다.
$env:VCPKG_DOWNLOADS = Join-Path $VcpkgRoot "downloads"
$env:VCPKG_DEFAULT_TRIPLET = "x64-windows"

Push-Location $VcpkgRoot          # 매니페스트가 없는 위치에서 실행
try {
    # [최소 기능] 기본 기능(dnn/gapi/highgui/tiff/webp/quirc...)을 모두 끈다.
    # dnn -> protobuf -> abseil 연쇄가 빌드 실패의 원인이었고, 우리는 dnn을 쓰지 않는다.
    # core/imgproc/imgcodecs만 있으면 되며 png/jpeg는 libpng/libjpeg-turbo만 추가한다.
    # 빌드 시간이 30~40분에서 5~10분 수준으로 줄어든다.
    & (Join-Path $VcpkgRoot "vcpkg.exe") install --classic `
        "opencv4[png,jpeg,fs,thread,intrinsics]:x64-windows"
    if ($LASTEXITCODE -ne 0) { throw "OpenCV 설치 실패 (vcpkg install)" }
} finally {
    Pop-Location
}

Step "CMake 구성"
$toolchain = Join-Path $VcpkgRoot "scripts\buildsystems\vcpkg.cmake"
$buildDir  = Join-Path $root "build"
cmake -S $root -B $buildDir -A x64 `
      -DCMAKE_TOOLCHAIN_FILE="$toolchain" `
      -DVCPKG_TARGET_TRIPLET=x64-windows `
      -DVCPKG_MANIFEST_MODE=OFF
if ($LASTEXITCODE -ne 0) { throw "CMake 구성 실패" }

Step "빌드"
cmake --build $buildDir --config Release
if ($LASTEXITCODE -ne 0) { throw "빌드 실패" }

if (-not $SkipTests) {
    Step "테스트"
    ctest --test-dir $buildDir -C Release --output-on-failure
}

Step "완료"
$exe = Join-Path $buildDir "Release\ContactlessFP.exe"
$cli = Join-Path $buildDir "Release\cfp_cli.exe"
if (Test-Path $exe) {
    Ok "앱  : $exe"
    if (Test-Path $cli) { Ok "CLI : $cli" }
    if ($Run) {
        Info "앱을 실행합니다..."
        Start-Process $exe
    } else {
        Write-Host "`n  실행:  .\build\Release\ContactlessFP.exe" -ForegroundColor Yellow
    }
} else {
    Write-Warning "실행 파일을 찾지 못했습니다: $exe"
}
