# =============================================================================
#  build_windows.ps1 - 빌드 스크립트 (Windows 기본 / Linux·macOS 지원)
#
#  [인코딩] 이 파일은 반드시 UTF-8 BOM으로 저장되어야 한다.
#  Windows PowerShell 5.1은 BOM이 없으면 스크립트를 시스템 코드페이지(한국어는
#  CP949)로 읽는다. 그러면 UTF-8로 쓴 한글이 깨지고, 깨진 바이트 중 일부가
#  따옴표로 해석되어 파서가 죽는다.
#
#  사용:
#    Windows  powershell -ExecutionPolicy Bypass -File build_windows.ps1 -Run
#    Linux    pwsh ./build_windows.ps1 -OpenCVDir <opencv4 cmake 디렉터리>
# =============================================================================
param(
    [switch]$Run,                       # 빌드 후 바로 실행
    [switch]$SkipTests,
    # OpenCV 위치. Windows는 프리빌트 루트(예: C:\opencv), 다른 OS는
    # OpenCVConfig.cmake가 있는 디렉터리를 지정한다.
    [string]$OpenCVDir = "",
    # [중요] ASCII 전용 경로여야 한다. vcpkg는 경로에 한글 등 비ASCII 문자가
    # 있으면 내부 도구(ninja 등) 취득 단계에서 실패한다.
    [string]$VcpkgRoot = "C:\vcpkg"
)
$ErrorActionPreference = "Stop"
$root = $PSScriptRoot

try { [Console]::OutputEncoding = [System.Text.Encoding]::UTF8 } catch { }

function Step([string]$m) { Write-Host "`n=== $m ===" -ForegroundColor Cyan }
function Info([string]$m) { Write-Host "  $m" }
function Ok  ([string]$m) { Write-Host "  $m" -ForegroundColor Green }

# -----------------------------------------------------------------------------
#  플랫폼 판별
#
#  [핵심] $IsWindows는 PowerShell 6+ 자동 변수다. Windows PowerShell 5.1에는
#  존재하지 않는데, 5.1은 Windows에서만 돌므로 "없으면 Windows"로 본다.
#  이 판별 없이 Windows 전용 코드를 그대로 두면 Linux에서
#  ${env:ProgramFiles(x86)} 가 null이 되어 엉뚱한 곳에서 죽는다.
# -----------------------------------------------------------------------------
$onWindows = if ($null -eq $IsWindows) { $true } else { $IsWindows }

function Test-Ascii([string]$p) { return ($p -notmatch '[^\x00-\x7F]') }

# =============================================================================
Step "환경 확인"
Info ("플랫폼 : " + $(if ($onWindows) { "Windows" } elseif ($IsMacOS) { "macOS" } else { "Linux" }))

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    throw "CMake가 없습니다.  (Windows: winget install Kitware.CMake)"
}
Info ("CMake  : " + ((cmake --version) | Select-Object -First 1))

if ($onWindows) {
    if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
        throw "Git이 없습니다.  winget install Git.Git"
    }
    # ${env:ProgramFiles(x86)} 는 일부 환경에서 비어 있을 수 있으므로 그대로 쓰지 않는다.
    $pf86 = ${env:ProgramFiles(x86)}
    if ([string]::IsNullOrWhiteSpace($pf86)) { $pf86 = "C:\Program Files (x86)" }
    $vswhere = Join-Path $pf86 "Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $vs = & $vswhere -latest -products '*' `
                -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
                -property displayName
        if ($vs) { Info ("VS     : " + $vs) }
        else {
            Write-Warning ("VC++ 빌드도구를 찾지 못했습니다.`n" +
                "  winget install Microsoft.VisualStudio.2022.BuildTools`n" +
                "  설치 시 'C++를 사용한 데스크톱 개발' 워크로드를 반드시 선택하세요.")
        }
    } else {
        Write-Warning "vswhere를 찾지 못했습니다. Visual Studio 설치 여부를 확인하세요."
    }
} else {
    Info "GUI 앱(ContactlessFP)은 Windows 전용입니다. 여기서는 CLI와 테스트만 빌드합니다."
}

if (-not (Test-Ascii $root)) {
    Write-Warning ("프로젝트 경로에 비ASCII 문자가 있습니다:`n  $root`n" +
        "  MSVC/CMake는 대개 문제없지만, 빌드가 계속 실패하면 " +
        "ASCII 경로로 저장소를 옮겨 다시 시도하세요.")
}

# =============================================================================
#  OpenCV 확보
#   1순위: 프리빌트/기존 설치 (-OpenCVDir 또는 자동 탐색) -> 빌드 불필요
#   2순위: vcpkg 소스 빌드 (Windows 전용)                 -> 5~15분
# =============================================================================
Step "OpenCV 탐색"

# [주의] 공식 Windows 배포판에는 OpenCVConfig.cmake가 여러 곳에 있다:
#   <root>\build\OpenCVConfig.cmake               <- 이걸 써야 한다(툴셋 자동 선택)
#   <root>\build\x64\vc16\lib\OpenCVConfig.cmake  <- vc16 전용
#   <root>\build\x64\vc17\lib\OpenCVConfig.cmake  <- vc17 전용
# 아무거나 집으면 툴셋이 어긋나 링크가 깨진다. 또 압축 해제 방식에 따라
# C:\opencv\build 일 수도, C:\opencv\opencv\build 일 수도 있다.
# 그래서 재귀 탐색 후 (1) 부모 폴더가 build 인 것을 우선하고 (2) 가장 얕은 것을 고른다.
function Find-OpenCVConfig([string]$base) {
    if ([string]::IsNullOrWhiteSpace($base) -or -not (Test-Path $base)) { return $null }
    $hits = Get-ChildItem -Path $base -Filter "OpenCVConfig.cmake" -File -Recurse `
              -Depth 5 -ErrorAction SilentlyContinue
    if (-not $hits) { return $null }
    Info "발견된 OpenCVConfig.cmake:"
    foreach ($h in $hits) { Info ("  - " + $h.FullName) }
    $ranked = $hits | Sort-Object `
        @{ Expression = { if ($_.Directory.Name -ieq "build") { 0 } else { 1 } } }, `
        @{ Expression = { ($_.FullName -split '[\\/]').Count } }
    return $ranked[0].Directory.FullName
}

$candidates = @($OpenCVDir)
if ($onWindows) {
    $candidates += @("C:\opencv", "C:\opencv\opencv",
                     "C:\opencv\build", "C:\opencv\opencv\build")
} else {
    # conda/micromamba 환경에 설치한 경우를 흔한 위치에서 가볍게 탐색한다.
    $candidates += @("$HOME/micromamba/envs", "$HOME/miniconda3/envs",
                     "/usr/local/lib/cmake", "/usr/lib/x86_64-linux-gnu/cmake")
}

$cvDir = $null
foreach ($cand in $candidates) {
    if ([string]::IsNullOrWhiteSpace($cand)) { continue }
    $found = Find-OpenCVConfig $cand
    if ($found) { $cvDir = $found; break }
}

$useVcpkg = $false
if ($cvDir) {
    Ok "OpenCV 사용: $cvDir"
    if (-not (Test-Ascii $cvDir)) {
        Write-Warning "OpenCV 경로에 비ASCII 문자가 있습니다. 빌드가 실패하면 ASCII 경로로 옮기세요."
    }
} elseif ($onWindows) {
    $useVcpkg = $true
    Info "프리빌트 OpenCV를 찾지 못했습니다. vcpkg로 빌드합니다."
    Info "(이미 받아두셨다면 -OpenCVDir <경로> 로 지정하세요)"
} else {
    Info "OpenCV를 자동 탐색하지 못했습니다. CMake의 기본 탐색에 맡깁니다."
    Info "실패하면 -OpenCVDir <OpenCVConfig.cmake 가 있는 디렉터리> 로 지정하세요."
}

if ($useVcpkg) {
    Step "vcpkg 준비"
    if (-not (Test-Ascii $VcpkgRoot)) {
        Write-Warning "vcpkg 경로에 비ASCII 문자가 있습니다: $VcpkgRoot"
        $VcpkgRoot = "C:\vcpkg"
        Info "ASCII 경로로 대체합니다: $VcpkgRoot"
    }
    if (-not (Test-Path (Join-Path $VcpkgRoot "vcpkg.exe"))) {
        if (-not (Test-Path $VcpkgRoot)) {
            Info "vcpkg 클론 중... ($VcpkgRoot)"
            try { New-Item -ItemType Directory -Force -Path $VcpkgRoot | Out-Null }
            catch {
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
    # [최소 기능] 기본 기능(dnn/gapi/highgui/tiff/webp/quirc/directml...)을 모두 끈다.
    # dnn -> protobuf -> abseil 연쇄가 빌드 실패의 원인이었고, 우리는 dnn을 쓰지 않는다.
    $env:VCPKG_DOWNLOADS = Join-Path $VcpkgRoot "downloads"
    $env:VCPKG_DEFAULT_TRIPLET = "x64-windows"
    Push-Location $VcpkgRoot          # 매니페스트가 없는 위치에서 실행
    try {
        & (Join-Path $VcpkgRoot "vcpkg.exe") install --classic `
            "opencv4[png,jpeg,fs,thread,intrinsics]:x64-windows"
        if ($LASTEXITCODE -ne 0) { throw "OpenCV 설치 실패 (vcpkg install)" }
    } finally { Pop-Location }
}

# =============================================================================
Step "CMake 구성"
$buildDir = Join-Path $root "build"
$cmArgs = @("-S", $root, "-B", $buildDir)
if ($onWindows) {
    $cmArgs += @("-A", "x64")
} else {
    $cmArgs += @("-DCMAKE_BUILD_TYPE=Release")
}
if ($useVcpkg) {
    $toolchain = Join-Path $VcpkgRoot "scripts\buildsystems\vcpkg.cmake"
    $cmArgs += @("-DCMAKE_TOOLCHAIN_FILE=$toolchain",
                 "-DVCPKG_TARGET_TRIPLET=x64-windows",
                 "-DVCPKG_MANIFEST_MODE=OFF")
} elseif ($cvDir) {
    $cmArgs += @("-DOpenCV_DIR=$cvDir")
}
Info ("cmake " + ($cmArgs -join " "))
cmake @cmArgs
if ($LASTEXITCODE -ne 0) { throw "CMake 구성 실패" }

Step "빌드"
if ($onWindows) { cmake --build $buildDir --config Release }
else            { cmake --build $buildDir -j }
if ($LASTEXITCODE -ne 0) { throw "빌드 실패" }

if (-not $SkipTests) {
    Step "테스트"
    if ($onWindows) { ctest --test-dir $buildDir -C Release --output-on-failure }
    else            { ctest --test-dir $buildDir --output-on-failure }
}

# =============================================================================
Step "완료"
if ($onWindows) {
    $exe = Join-Path $buildDir "Release\ContactlessFP.exe"
    $cli = Join-Path $buildDir "Release\cfp_cli.exe"
} else {
    $exe = $null
    $cli = Join-Path $buildDir "cfp_cli"
}

if ($exe -and (Test-Path $exe)) {
    Ok "앱  : $exe"
    if (Test-Path $cli) { Ok "CLI : $cli" }
    if ($Run) { Info "앱을 실행합니다..."; Start-Process $exe }
    else { Write-Host "`n  실행:  .\build\Release\ContactlessFP.exe" -ForegroundColor Yellow }
} elseif (Test-Path $cli) {
    Ok "CLI : $cli"
    if ($Run) {
        Info "합성 지문으로 데모를 실행합니다..."
        & $cli --demo --no-pdf --out (Join-Path $root "out")
    } else {
        Write-Host "`n  실행:  $cli --demo --no-pdf --out out" -ForegroundColor Yellow
    }
} else {
    Write-Warning "실행 파일을 찾지 못했습니다."
}
