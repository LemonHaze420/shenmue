param([Parameter(Mandatory = $true)][string]$BuildDirectory, [switch]$Capture,
  [ValidateSet("S1", "WHATS")][string]$Game = "S1")

$ErrorActionPreference = 'Stop'
$vcpkg = $env:VCPKG_INSTALLATION_ROOT
if (!$vcpkg) { $vcpkg = $env:VCPKG_ROOT }
if (!$vcpkg) { $vcpkg = 'C:\dev\vcpkg' }
if ($env:GITHUB_ACTIONS -eq 'true' -and !$Capture) {
  $tools = Get-Content (Join-Path $vcpkg 'scripts/vcpkg-tools.json') -Raw | ConvertFrom-Json
  $cmakeTools = @($tools.tools | Where-Object { $_.name -eq 'cmake' -and $_.os -eq 'windows' -and $_.arch -eq 'amd64' })
  if ($cmakeTools.Count -ne 1) { throw 'Cannot identify the vcpkg CMake tool' }
  $cmakeTool = $cmakeTools[0]
  $downloads = $env:VCPKG_DOWNLOADS
  if (!$downloads) { $downloads = Join-Path $vcpkg 'downloads' }
  $archive = Join-Path $downloads $cmakeTool.archive
  if (!(Test-Path -LiteralPath $archive) -or (Get-FileHash -LiteralPath $archive -Algorithm SHA512).Hash -ne $cmakeTool.sha512) {
    New-Item -ItemType Directory -Force $downloads | Out-Null
    $temporaryArchive = "$archive.$PID.tmp"
    try {
      $cmakeVersion = [version]$cmakeTool.version
      $uri = "https://cmake.org/files/v$($cmakeVersion.Major).$($cmakeVersion.Minor)/$($cmakeTool.archive)"
      Invoke-WebRequest -UseBasicParsing -Uri $uri -OutFile $temporaryArchive
      if ((Get-FileHash -LiteralPath $temporaryArchive -Algorithm SHA512).Hash -ne $cmakeTool.sha512) {
        throw 'CMake archive checksum mismatch'
      }
      Move-Item -LiteralPath $temporaryArchive -Destination $archive -Force
    } finally {
      Remove-Item -LiteralPath $temporaryArchive -Force -ErrorAction SilentlyContinue
    }
  }
}
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
