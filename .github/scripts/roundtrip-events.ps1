param(
  [string]$BuildDirectory = 'build/s2-event-roundtrip',
  [string]$ReferenceDirectories = $env:SHENMUE_DC_EVENT_REFERENCE_DIR,
  [string]$ToolchainRoot = $env:SHENMUE_DC_TOOLCHAIN_ROOT,
  [string]$HostCompiler = $env:SHENMUE_EVENT_HOST_CC
)

$ErrorActionPreference = 'Stop'
if (!$ToolchainRoot) { $ToolchainRoot = [Environment]::GetEnvironmentVariable('SHENMUE_DC_TOOLCHAIN_ROOT', 'User') }
if (!$HostCompiler) { $HostCompiler = [Environment]::GetEnvironmentVariable('SHENMUE_EVENT_HOST_CC', 'User') }
if (!$ReferenceDirectories) {
  if (!$env:SHEN_ROOT) { throw 'SHEN_ROOT is not configured' }
  $discRoot = Join-Path $env:SHEN_ROOT '../../output/orig_iso/DC/Shenmue II v1.001/_JP'
  $ReferenceDirectories = (1..4 | ForEach-Object { Join-Path $discRoot "DISC $_" }) -join ';'
}
if (!$ToolchainRoot) { throw 'SHENMUE_DC_TOOLCHAIN_ROOT is not configured (SH-4 binutils prefix)' }

function ConvertTo-WslPath([string]$path) {
  if ($path.StartsWith('/')) { return $path }
  $converted = & wsl.exe --exec wslpath -a -u $path
  if ($LASTEXITCODE -ne 0 -or !$converted) { throw "Cannot resolve WSL path: $path" }
  return $converted.Trim()
}

$source = ConvertTo-WslPath $PWD.ProviderPath
$build = ConvertTo-WslPath (New-Item -ItemType Directory -Force $BuildDirectory).FullName
$references = @($ReferenceDirectories.Split(';') | ForEach-Object {
  if (![string]::IsNullOrWhiteSpace($_)) { ConvertTo-WslPath $_ }
}) -join ';'
if (!$references) { throw 'No event reference directories were supplied' }
$toolchain = ConvertTo-WslPath $ToolchainRoot
$arguments = @('-S', $source, '-B', $build, '-G', 'Unix Makefiles', '-DSHENMUE_DC_EVENT_OVERLAYS=ON',
  "-DSHENMUE_DC_EVENT_REFERENCE_DIR=$references", "-DSHENMUE_DC_TOOLCHAIN_ROOT=$toolchain")
if ($HostCompiler) { $arguments += "-DSHENMUE_EVENT_HOST_CC=$HostCompiler" }
& wsl.exe --exec cmake @arguments
if ($LASTEXITCODE -ne 0) { throw 'S2 event overlay configuration failed' }

$report = Join-Path $BuildDirectory 'event-results.xml'
Remove-Item -LiteralPath $report -Force -ErrorAction SilentlyContinue
& wsl.exe --exec cmake --build $build --parallel 4 -- -k
$buildResult = $LASTEXITCODE
& wsl.exe --exec ctest --test-dir $build -L '^s2-events$' --output-on-failure --parallel 4 --no-tests=error `
  --output-junit "$build/event-results.xml"
$testResult = $LASTEXITCODE
if (Test-Path -LiteralPath $report) {
  [xml]$results = Get-Content -LiteralPath $report -Raw
  $total = [int]$results.testsuite.tests
  $failed = [int]$results.testsuite.failures + [int]$results.testsuite.errors + [int]$results.testsuite.skipped
  Write-Host "[event] $($total - $failed)/$total overlays rebuilt byte-identically"
}
if ($buildResult -ne 0 -or $testResult -ne 0) { throw 'S2 event overlay roundtrip verification failed' }
