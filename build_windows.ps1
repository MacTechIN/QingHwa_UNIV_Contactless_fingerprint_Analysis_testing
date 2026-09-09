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
    [string]$VcpkgRoot = "$env:USERPROFILE\vcpkg"
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

Step "vcpkg 준비"
if (-not (Test-Path (Join-Path $VcpkgRoot "vcpkg.exe"))) {
    if (-not (Test-Path $VcpkgRoot)) {
        Info "vcpkg 클론 중..."
        git clone --depth 1 https://github.com/microsoft/vcpkg.git $VcpkgRoot
    }
    & (Join-Path $VcpkgRoot "bootstrap-vcpkg.bat") -disableMetrics
    if ($LASTEXITCODE -ne 0) { throw "vcpkg 부트스트랩 실패" }
}
Info "vcpkg : $VcpkgRoot"

Step "OpenCV 설치 (최초 1회 20~40분 소요)"
& (Join-Path $VcpkgRoot "vcpkg.exe") install "opencv4[contrib,png,jpeg]:x64-windows"
if ($LASTEXITCODE -ne 0) { throw "OpenCV 설치 실패" }

Step "CMake 구성"
$toolchain = Join-Path $VcpkgRoot "scripts\buildsystems\vcpkg.cmake"
$buildDir  = Join-Path $root "build"
cmake -S $root -B $buildDir -A x64 `
      -DCMAKE_TOOLCHAIN_FILE="$toolchain" `
      -DVCPKG_TARGET_TRIPLET=x64-windows
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
