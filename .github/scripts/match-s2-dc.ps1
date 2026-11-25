param(
  [string]$BuildDirectory = 'build/s2-dc',
  [string]$EventBuildDirectory = 'build/s2-event-roundtrip',
  [string]$ToolchainRoot = $env:SHENMUE_DC_TOOLCHAIN_ROOT,
  [string]$SdkDirectory = $env:SHENMUE_DC_SDK_LIBRARY_DIR
)

$ErrorActionPreference = 'Stop'

function ConvertTo-WslPath([string]$path) {
  if ($path.StartsWith('/')) { return $path }
  $converted = & wsl.exe --exec wslpath -a -u $path
  if ($LASTEXITCODE -ne 0 -or !$converted) { throw "Cannot resolve WSL path: $path" }
  return $converted.Trim()
}

if (!$ToolchainRoot) { $ToolchainRoot = [Environment]::GetEnvironmentVariable('SHENMUE_DC_TOOLCHAIN_ROOT', 'User') }
if (!$SdkDirectory) { $SdkDirectory = [Environment]::GetEnvironmentVariable('SHENMUE_DC_SDK_LIBRARY_DIR', 'User') }
$hostRoot = $env:SHEN_ROOT
if (!$hostRoot) { $hostRoot = [Environment]::GetEnvironmentVariable('SHEN_ROOT', 'User') }
if (!$hostRoot) { throw 'SHEN_ROOT is not configured' }
if (!$ToolchainRoot) { throw 'SHENMUE_DC_TOOLCHAIN_ROOT is not configured' }
if (!$SdkDirectory) { throw 'SHENMUE_DC_SDK_LIBRARY_DIR is not configured (external Katana GNU archives)' }

$source = ConvertTo-WslPath $PWD.ProviderPath
$work = (New-Item -ItemType Directory -Force $BuildDirectory).FullName
$cache = ConvertTo-WslPath $work
$compiler = ConvertTo-WslPath (Resolve-Path -LiteralPath $EventBuildDirectory).Path
$toolchain = ConvertTo-WslPath $ToolchainRoot
$sdk = ConvertTo-WslPath $SdkDirectory
$discRoot = Join-Path $hostRoot '../../output/orig_iso/DC'
$python = "$cache/python/bin/python"
& wsl.exe --exec python3 -m venv "$cache/python"
if ($LASTEXITCODE -ne 0) { throw 'Cannot prepare the DC build Python environment' }
& wsl.exe --exec $python -m pip install --disable-pip-version-check -r "$source/tools/dc/requirements.txt"
if ($LASTEXITCODE -ne 0) { throw 'Cannot install the pinned DC build dependencies' }

$sets = @(
  @{ target = 'S2_JP_GAMEJAM'; input = 'Shenmue II Game Jam (JP) (Apr 11, 2001 prototype) Disc 1/Shenmue2.GDI' },
  @{ target = 'S2_JP_1001'; input = 'binaries/S2_DC_D1_JP_1ST_READ.BIN' },
  @{ target = 'S2_US'; input = 'Shenmue II US v0.991/DISC 1/1ST_READ.BIN' },
  @{ target = 'S2_EU'; input = 'Shenmue II v1.001/DISC 1/1ST_READ.BIN' }
)
$results = @()
foreach ($set in $sets) {
  $target = $set.target
  $directory = (New-Item -ItemType Directory -Force (Join-Path $work $target)).FullName
  $build = ConvertTo-WslPath $directory
  $report = Join-Path $directory 'matching/image.json'
  Remove-Item -LiteralPath $report -Force -ErrorAction SilentlyContinue
  Write-Host "[s2-dc] $target started"
  try {
    $reference = ConvertTo-WslPath (Resolve-Path -LiteralPath (Join-Path $discRoot $set.input)).Path
    $arguments = @('-S', $source, '-B', $build, '-G', 'Unix Makefiles',
      "-DCMAKE_TOOLCHAIN_FILE=$source/cmake/toolchains/DreamcastS2.cmake", '-DCMAKE_BUILD_TYPE=Release',
      "-DPython3_EXECUTABLE=$python", "-DSHENMUE_DC_TARGET=$target",
      "-DSHENMUE_DC_TOOLCHAIN_ROOT=$toolchain", "-DSHENMUE_DC_SDK_LIBRARY_DIR=$sdk",
      "-DSHENMUE_DC_GCC2952_DIR=$compiler/event-gcc/src/shenmue_event_gcc-build/gcc",
      "-DSHENMUE_DC_GXX2952_SOURCE=$compiler/event-gcc/src/shenmue_event_gxx_source",
      "-DSHENMUE_DC_NEWLIB_SOURCE=$compiler/event-newlib/src/shenmue_event_newlib/newlib/libc",
      "-DSHENMUE_DC_MATCH_REFERENCE=$reference", '-DSHENMUE_DC_DEBUG_FONT_SOURCE=')
    & wsl.exe --exec cmake @arguments
    if ($LASTEXITCODE -ne 0) { throw 'Configuration failed' }
    & wsl.exe --exec cmake --build $build --target shenmue2_dc --parallel 4
    if ($LASTEXITCODE -ne 0) { throw 'Executable build failed' }
    & wsl.exe --exec $python "$source/tools/dc/match_image.py" --config "$source/config/$target/config.yml" `
      --reference $reference --elf "$build/shenmue2_dc.elf" --candidate "$build/1ST_READ.BIN" `
      --output-dir "$build/matching" --summary-only
    $comparison = $LASTEXITCODE
    if ($comparison -notin @(0, 1)) { throw 'Executable comparison failed' }
    $data = Get-Content -LiteralPath $report -Raw | ConvertFrom-Json
    if ($data.match -ne ($comparison -eq 0) -or $data.masked_bytes -ne 0 -or !$data.game_functions.total) {
      throw 'Invalid executable comparison report'
    }
    $game = $data.game_functions
    $results += [PSCustomObject]@{ target = $target; exact = $game.exact; total = $game.total;
      equivalent = $game.exact + $game.instruction_equivalent; bytes = $data.equal_bytes;
      size = [Math]::Max($data.expected_size, $data.actual_size); match = $data.match; error = $null }
    Write-Host "[s2-dc] $target compiled and linked; $($game.exact)/$($game.total) game functions byte-exact"
  } catch {
    $results += [PSCustomObject]@{ target = $target; error = $_.Exception.Message }
    Write-Host "[s2-dc] $target failed: $($_.Exception.Message)"
  }
}
$results | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath (Join-Path $work 'results.json') -Encoding UTF8
if (@($results | Where-Object { $_.error }).Count) { throw 'S2 Dreamcast build or comparison failed' }
