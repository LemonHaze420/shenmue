param([Parameter(Mandatory = $true)][string]$BuildDirectory, [switch]$Capture,
  [ValidateSet("S1", "WHATS")][string]$Game = "S1")

$ErrorActionPreference = 'Stop'
$vcpkg = $env:VCPKG_INSTALLATION_ROOT
if (!$vcpkg) { $vcpkg = $env:VCPKG_ROOT }
if (!$vcpkg) { $vcpkg = 'C:\dev\vcpkg' }
$arguments = @('-S', '.', '-B', $BuildDirectory, '-A', 'x64',
  "-DCMAKE_TOOLCHAIN_FILE=$(Join-Path $vcpkg 'scripts/buildsystems/vcpkg.cmake')",
  '-DVCPKG_TARGET_TRIPLET=x64-windows-static', "-DSHENMUE_S1_GAME=$Game",
  '-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>',
  '-DSHENMUE_ENHANCEMENT_S2_MAP_SEAMS=ON', '-DSHENMUE_RT_ARCADE=ON', '-DSHENMUE_LOAD_HANGON_DLL=ON')
if ($Capture) {
  $arguments += '-DClang_DIR=C:/dev/llvm-20.1.0/lib/cmake/clang', '-DSHENMUE_STEAM=OFF', '-DSHEN_AUTO_TEST_ENABLED=ON', '-DSHENMUE_BUILD_SCN3CC=ON',
    "-DSHENMUE_BUILD_SCENARIOS=$(if ($Game -eq 'S1') { 'ON' } else { 'OFF' })"
} else {
  $arguments += "-DSHENMUE_STEAM=$(if ($Game -eq 'WHATS') { 'OFF' } else { 'ON' })", '-DSHEN_AUTO_TEST_ENABLED=OFF', '-DSHENMUE_BUILD_SCENARIOS=OFF'
}
& cmake @arguments
if ($LASTEXITCODE -ne 0) { throw 'Game configuration failed' }
