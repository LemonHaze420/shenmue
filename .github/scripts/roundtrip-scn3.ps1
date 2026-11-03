$ErrorActionPreference = 'Stop'
$work = (New-Item -ItemType Directory -Force 'build/windows-capture/scenarios/roundtrip').FullName
$report = Join-Path $work 'results.json'
$badge = Join-Path $work 's1-roundtrip.svg'
$manifest = Join-Path $work 'sync-files.txt'
Remove-Item $report, $badge, $manifest -Force -ErrorAction SilentlyContinue
$results = [Collections.Generic.List[object]]::new()
$updates = [Collections.Generic.List[object]]::new()
$total = 0
$complete = $false

function Invoke-Scn3([string[]]$arguments, [string]$log) {
  $quoted = @($arguments | ForEach-Object { '"{0}"' -f $_ })
  $process = Start-Process -FilePath $compiler -ArgumentList $quoted -Wait -PassThru -NoNewWindow `
    -RedirectStandardOutput "$log.log" -RedirectStandardError "$log.err.log"
  if ($process.ExitCode -ne 0) { throw "SCN3 failed: $log (exit $($process.ExitCode))" }
}

try {
  cmake --build build/windows-capture --config Release --target scn3cc tacextract motextract --parallel 4
  if ($LASTEXITCODE -ne 0) { throw 'Scenario tool build failed' }
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
  $total = $scripts.Count
  if (!$total) { throw 'No SCN3 scripts found' }
  foreach ($file in $scripts) {
    $relative = $file.FullName.Substring($include.Length + 1)
    $directory = (New-Item -ItemType Directory -Force (Join-Path $work (Split-Path $relative))).FullName
    $source = Join-Path $directory "$($file.BaseName).c"
    $header = Join-Path $directory "$($file.BaseName).h"
    $rebuilt = Join-Path $directory "$($file.BaseName).rebuilt.scn"
    $stage = 'export'
    Write-Host "[scenario] $relative started"
    try {
      Remove-Item $source, $header, $rebuilt -Force -ErrorAction SilentlyContinue
      Invoke-Scn3 @($file.FullName, '--export-c', $source) (Join-Path $directory "$($file.BaseName).export")
      if (!(Test-Path $source) -or !(Test-Path $header)) { throw 'Generated scenario source is missing' }
      $stage = 'compile'
      Invoke-Scn3 @($source, '-I', $include, '--target', 'pc', '--emit', 'vm', '-o', $rebuilt) `
        (Join-Path $directory "$($file.BaseName).compile")
      if (!(Test-Path $rebuilt)) { throw 'Rebuilt scenario is missing' }
      $stage = 'compare'
      $arguments = @('/b', ('"{0}"' -f $file.FullName), ('"{0}"' -f $rebuilt))
      $process = Start-Process -FilePath "$env:SystemRoot/System32/fc.exe" -ArgumentList $arguments -Wait -PassThru -NoNewWindow `
        -RedirectStandardOutput (Join-Path $directory "$($file.BaseName).compare.log")
      if ($process.ExitCode -ne 0) { throw "SCN3 round-trip mismatch: $($file.Name)" }
      $results.Add(@{ script = $relative; success = $true; stage = 'complete' })
      Write-Host "[scenario] $relative successfully round-tripped"
    } catch {
      $results.Add(@{ script = $relative; success = $false; stage = $stage; error = $_.Exception.Message })
      Write-Host "[scenario] $relative failed ($stage): $($_.Exception.Message)"
      continue
    }
    $target = Join-Path $file.DirectoryName "$($file.BaseName).c"
    $alternate = "$($file.FullName).c"
    $override = $false
    foreach ($candidate in @($target, $alternate)) {
      if ((Test-Path $candidate) -and (Get-Content -LiteralPath $candidate -TotalCount 1) -match '^\s*//\s*override\s*$') {
        $override = $true
      }
    }
    if ($override) { continue }
    if (Test-Path $alternate) { $target = $alternate }
    $updates.Add(@{ source = $source; target = $target })
    $updates.Add(@{ source = $header; target = (Join-Path $file.DirectoryName "$($file.BaseName).h") })
  }
  $complete = $true
} finally {
  $successful = @($results | Where-Object { $_.success }).Count
  $percentage = $null
  $value = 'not available'
  $colour = '#9f9f9f'
  if ($complete -and $total -gt 0) {
    $percentage = 100.0 * $successful / $total
    $value = $percentage.ToString('0.##', [Globalization.CultureInfo]::InvariantCulture) + '%'
    $colour = if ($successful -eq $total) { '#4c1' } else { '#e05d44' }
  }
  @{ successful = $successful; total = $total; failed = $total - $successful; percentage = $percentage;
    complete = $complete; results = @($results.ToArray()) } | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $report -Encoding UTF8
  $title = "S1 - Roundtrip: $value ($successful/$total)"
  @"
<svg xmlns="http://www.w3.org/2000/svg" width="190" height="20" role="img" aria-label="$title">
  <title>$title</title>
  <rect width="190" height="20" rx="3" fill="#555"/>
  <path fill="$colour" d="M110 0h77q3 0 3 3v14q0 3-3 3h-77z"/>
  <g fill="#fff" text-anchor="middle" font-family="Verdana,DejaVu Sans,sans-serif" font-size="11">
    <text x="55" y="14">S1 - Roundtrip</text><text x="150" y="14">$value</text>
  </g>
</svg>
"@ | Set-Content -LiteralPath $badge -Encoding UTF8
  Write-Host "SCN3 round-trips: $successful/$total ($value)"
}
if ($successful -ne $total) { throw "$($total - $successful) of $total SCN3 round-trips failed" }
foreach ($update in $updates) {
  if (!(Test-Path $update.source)) { throw 'Generated scenario source is missing' }
}
$paths = foreach ($update in $updates) {
  Copy-Item -LiteralPath $update.source -Destination $update.target -Force
  $update.target.Substring($PWD.Path.Length + 1).Replace('\', '/')
}
@($paths) | Set-Content -LiteralPath $manifest -Encoding UTF8
