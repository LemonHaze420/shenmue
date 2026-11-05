$ErrorActionPreference = 'Stop'
$work = (New-Item -ItemType Directory -Force 'build/windows-capture/scenarios/roundtrip').FullName
$report = Join-Path $work 'results.json'
$manifest = Join-Path $work 'sync-files.txt'
Remove-Item $report, $manifest, "$work/s1-roundtrip.svg", "$work/ws-roundtrip.svg" -Force -ErrorAction SilentlyContinue
$results = [Collections.Generic.List[object]]::new()
$updates = [Collections.Generic.List[object]]::new()
$inputs = [Collections.Generic.List[object]]::new()
$counts = @{ s1 = 0; ws = 0 }
$total = 0
$complete = $false

function Invoke-Scn3([string]$compiler, [string[]]$arguments, [string]$log) {
  $quoted = @($arguments | ForEach-Object { '"{0}"' -f $_ })
  $process = Start-Process -FilePath $compiler -ArgumentList $quoted -Wait -PassThru -NoNewWindow `
    -RedirectStandardOutput "$log.log" -RedirectStandardError "$log.err.log"
  if ($process.ExitCode -ne 0) { throw "SCN3 failed: $log (exit $($process.ExitCode))" }
}

try {
  cmake --build build/windows-capture --config Release --target scn3cc tacextract motextract --parallel 4
  if ($LASTEXITCODE -ne 0) { throw 'Scenario tool build failed' }
  cmake --build build/ws-capture --config Release --target scn3cc --parallel 4
  if ($LASTEXITCODE -ne 0) { throw 'WS scenario compiler build failed' }
  $build = (Resolve-Path 'build/windows-capture').Path
  $include = (Resolve-Path 'src/s1').Path
  $compiler = Join-Path $build 'tools/scn3/Release/scn3cc.exe'
  $arguments = @('-DMODE=assets', "-DSOURCE_DIR=$include/scenario", "-DSOURCE_ROOT=$include",
    "-DWORK_DIR=$build/scenarios", "-DGAME_ROOT=$env:SHEN_STEAM_ROOT/sm1",
    "-DCATALOGUE=$PWD/reference/filename_database.json", "-DEXTRACTOR=$build/tools/archive/Release/tacextract.exe",
    "-DMOTION=$build/tools/motion/Release/motextract.exe", '-P', 'cmake/BuildScenarios.cmake')
  & cmake @arguments
  if ($LASTEXITCODE -ne 0) { throw 'Scenario asset extraction failed' }
  $scripts = @(Get-ChildItem 'src/s1/scenario' -Recurse -File -Filter '*.scn' |
    Where-Object { $_.Name -notlike '*.rebuilt.scn' } | Sort-Object FullName)
  if (!$scripts.Count) { throw 'No S1 SCN3 scripts found' }
  $counts.s1 = $scripts.Count
  foreach ($file in $scripts) {
    $inputs.Add(@{ game = 's1'; file = $file; name = $file.BaseName; include = $include; compiler = $compiler;
      relative = $file.FullName.Substring($include.Length + 1); map = $false })
  }
  $wsInclude = (New-Item -ItemType Directory -Force 'src/ws').FullName
  $wsCompiler = (Resolve-Path 'build/ws-capture/tools/scn3/Release/scn3cc.exe').Path
  $banks = @(Get-ChildItem (Join-Path $env:SHEN_WS_ROOT 'motion') -File -Filter '*.bin' | Sort-Object Name)
  if (!$banks.Count) { throw 'No WS motion banks found' }
  $motionDirectory = (New-Item -ItemType Directory -Force (Join-Path $wsInclude 'motion')).FullName
  foreach ($bank in $banks) {
    $target = Join-Path $motionDirectory ($bank.Name.ToLowerInvariant())
    $header = [IO.Path]::ChangeExtension($target, '.h')
    if (!(Test-Path $target)) { Copy-Item -LiteralPath $bank.FullName -Destination $target }
    if (!(Test-Path $header)) {
      & "$build/tools/motion/Release/motextract.exe" $target
      if ($LASTEXITCODE -ne 0) { throw "WS motion header generation failed: $($bank.Name)" }
    }
  }
  $scene = (Resolve-Path (Join-Path $env:SHEN_WS_ROOT 'scene')).Path
  $maps = @(Get-ChildItem $scene -Recurse -File -Filter 'MAPINFO.BIN' | Sort-Object FullName)
  if (!$maps.Count) { throw 'No WS MAPINFO files found' }
  $counts.ws = $maps.Count
  $names = [Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
  foreach ($file in $maps) {
    $relative = $file.FullName.Substring($scene.Length + 1).Replace('\', '/')
    if ($relative -notmatch '^([0-9]{2})/([A-Za-z0-9_]+)/MAPINFO\.BIN$') { throw "Unexpected WS map path: $relative" }
    $name = "act$($Matches[1])_$($Matches[2].ToLowerInvariant())"
    if (!$names.Add($name)) { throw "Duplicate WS scenario name: $name" }
    $inputs.Add(@{ game = 'ws'; file = $file; name = $name; include = $wsInclude; compiler = $wsCompiler;
      relative = "scenario/$name.scn"; map = $true })
  }
  $total = $inputs.Count
  foreach ($scriptInput in $inputs) {
    $relative = $scriptInput.relative
    $directory = (New-Item -ItemType Directory -Force (Join-Path $work "$($scriptInput.game)/$(Split-Path $relative)")).FullName
    $source = Join-Path $directory "$($scriptInput.name).c"
    $header = Join-Path $directory "$($scriptInput.name).h"
    $rebuilt = Join-Path $directory "$($scriptInput.name).rebuilt.scn"
    $original = $scriptInput.file.FullName
    $stage = if ($scriptInput.map) { 'extract' } else { 'export' }
    Write-Host "[scenario] $($scriptInput.game)/$relative started"
    try {
      Remove-Item $source, $header, $rebuilt -Force -ErrorAction SilentlyContinue
      if ($scriptInput.map) {
        $original = Join-Path $directory "$($scriptInput.name).scn"
        Remove-Item $original -Force -ErrorAction SilentlyContinue
        Invoke-Scn3 $scriptInput.compiler @($scriptInput.file.FullName, '--extract-scn3', $original) (Join-Path $directory "$($scriptInput.name).extract")
      }
      $stage = 'export'
      Invoke-Scn3 $scriptInput.compiler @($original, '-I', $scriptInput.include, '--export-c', $source) (Join-Path $directory "$($scriptInput.name).export")
      if (!(Test-Path $source) -or !(Test-Path $header)) { throw 'Generated scenario source is missing' }
      $stage = 'compile'
      Invoke-Scn3 $scriptInput.compiler @($source, '-I', $scriptInput.include, '--target', 'pc', '--emit', 'vm', '-o', $rebuilt) `
        (Join-Path $directory "$($scriptInput.name).compile")
      if (!(Test-Path $rebuilt)) { throw 'Rebuilt scenario is missing' }
      $stage = 'compare'
      $arguments = @('/b', ('"{0}"' -f $original), ('"{0}"' -f $rebuilt))
      $process = Start-Process -FilePath "$env:SystemRoot/System32/fc.exe" -ArgumentList $arguments -Wait -PassThru -NoNewWindow `
        -RedirectStandardOutput (Join-Path $directory "$($scriptInput.name).compare.log")
      if ($process.ExitCode -ne 0) { throw "SCN3 round-trip mismatch: $($scriptInput.name)" }
      $results.Add(@{ game = $scriptInput.game; script = $relative; success = $true; stage = 'complete' })
      Write-Host "[scenario] $($scriptInput.game)/$relative successfully round-tripped"
    } catch {
      $results.Add(@{ game = $scriptInput.game; script = $relative; success = $false; stage = $stage; error = $_.Exception.Message })
      Write-Host "[scenario] $($scriptInput.game)/$relative failed ($stage): $($_.Exception.Message)"
      continue
    }
    $targetDirectory = Join-Path $scriptInput.include (Split-Path $relative)
    $target = Join-Path $targetDirectory "$($scriptInput.name).c"
    $alternate = Join-Path $targetDirectory "$($scriptInput.name).scn.c"
    $override = $false
    foreach ($candidate in @($target, $alternate)) {
      if ((Test-Path $candidate) -and (Get-Content -LiteralPath $candidate -TotalCount 1) -match '^\s*//\s*override\s*$') {
        $override = $true
      }
    }
    if ($override) { continue }
    if (Test-Path $alternate) { $target = $alternate }
    $updates.Add(@{ source = $source; target = $target })
    $updates.Add(@{ source = $header; target = (Join-Path $targetDirectory "$($scriptInput.name).h") })
  }
  $complete = $true
} finally {
  $successful = @($results | Where-Object { $_.success }).Count
  $games = @{}
  foreach ($game in 's1', 'ws') {
    $gameResults = @($results | Where-Object { $_.game -eq $game })
    $gameTotal = $counts[$game]
    $gameSuccessful = @($gameResults | Where-Object { $_.success }).Count
    $gameComplete = $gameTotal -gt 0 -and $gameResults.Count -eq $gameTotal
    $percentage = $null
    $value = 'not available'
    $colour = '#9f9f9f'
    if ($gameComplete) {
      $percentage = 100.0 * $gameSuccessful / $gameTotal
      $value = $percentage.ToString('0.##', [Globalization.CultureInfo]::InvariantCulture) + '%'
      $colour = if ($gameSuccessful -eq $gameTotal) { '#4c1' } else { '#e05d44' }
    }
    $games[$game] = @{ successful = $gameSuccessful; total = $gameTotal; failed = $gameTotal - $gameSuccessful;
      percentage = $percentage; complete = $gameComplete }
    $label = $game.ToUpperInvariant()
    $title = "$label - Roundtrip: $value ($gameSuccessful/$gameTotal)"
    @"
<svg xmlns="http://www.w3.org/2000/svg" width="190" height="20" role="img" aria-label="$title">
  <title>$title</title>
  <rect width="190" height="20" rx="3" fill="#555"/>
  <path fill="$colour" d="M110 0h77q3 0 3 3v14q0 3-3 3h-77z"/>
  <g fill="#fff" text-anchor="middle" font-family="Verdana,DejaVu Sans,sans-serif" font-size="11">
    <text x="55" y="14">$label - Roundtrip</text><text x="150" y="14">$value</text>
  </g>
</svg>
"@ | Set-Content -LiteralPath (Join-Path $work "$game-roundtrip.svg") -Encoding UTF8
    Write-Host "$label SCN3 round-trips: $gameSuccessful/$gameTotal ($value)"
  }
  @{ successful = $successful; total = $total; failed = $total - $successful; complete = $complete;
    games = $games; results = @($results.ToArray()) } | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $report -Encoding UTF8
}
if (!$complete -or $successful -ne $total) { throw "$($total - $successful) of $total SCN3 round-trips failed" }
foreach ($update in $updates) {
  if (!(Test-Path $update.source)) { throw 'Generated scenario source is missing' }
}
$paths = foreach ($update in $updates) {
  New-Item -ItemType Directory -Force (Split-Path $update.target) | Out-Null
  Copy-Item -LiteralPath $update.source -Destination $update.target -Force
  $update.target.Substring($PWD.Path.Length + 1).Replace('\', '/')
}
@($paths) | Set-Content -LiteralPath $manifest -Encoding UTF8
