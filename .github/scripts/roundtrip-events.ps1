param(
  [string]$BuildDirectory = 'build/s2-event-roundtrip',
  [string]$ReferenceDirectories = $env:SHENMUE_DC_EVENT_REFERENCE_DIR,
  [string]$ToolchainRoot = $env:SHENMUE_DC_TOOLCHAIN_ROOT,
  [string]$HostCompiler = $env:SHENMUE_EVENT_HOST_CC
)

$ErrorActionPreference = 'Stop'
$work = (New-Item -ItemType Directory -Force $BuildDirectory).FullName
$sets = @(
  @{ name = 'jp'; label = 'S2 JP V1.003'; target = 'S2_JP_1003'; expected = 333; directory = $work },
  @{ name = 'gamejam'; label = 'S2 GameJam'; target = 'S2_JP_GAMEJAM'; expected = 137; directory = "$work/gamejam" },
  @{ name = 'us'; label = 'S2 US'; target = 'S2_US'; expected = 333; directory = "$work/us" },
  @{ name = 'eu'; label = 'S2 EU'; target = 'S2_EU'; expected = 334; directory = "$work/eu" }
)
foreach ($set in $sets) {
  $set.successful = 0; $set.total = 0; $set.implemented = 0; $set.complete = $false; $set.error = $null
  Remove-Item "$work/s2-$($set.name)-ev-roundtrip.svg" -Force -ErrorAction SilentlyContinue
}
Remove-Item "$work/results.json" -Force -ErrorAction SilentlyContinue

function ConvertTo-WslPath([string]$path) {
  if ($path.StartsWith('/')) { return $path }
  $converted = & wsl.exe --exec wslpath -a -u $path
  if ($LASTEXITCODE -ne 0 -or !$converted) { throw "Cannot resolve WSL path: $path" }
  return $converted.Trim()
}

try {
  if (!$ToolchainRoot) { $ToolchainRoot = [Environment]::GetEnvironmentVariable('SHENMUE_DC_TOOLCHAIN_ROOT', 'User') }
  if (!$HostCompiler) { $HostCompiler = [Environment]::GetEnvironmentVariable('SHENMUE_EVENT_HOST_CC', 'User') }
  $hostRoot = $env:SHEN_ROOT
  if (!$hostRoot) { $hostRoot = [Environment]::GetEnvironmentVariable('SHEN_ROOT', 'User') }
  if (!$hostRoot) { throw 'SHEN_ROOT is not configured' }
  if (!$ToolchainRoot) { throw 'SHENMUE_DC_TOOLCHAIN_ROOT is not configured (SH-4 binutils prefix)' }
  $discRoot = Join-Path $hostRoot '../../output/orig_iso/DC'
  $source = ConvertTo-WslPath $PWD.ProviderPath
  $cache = ConvertTo-WslPath $work
  $toolchain = ConvertTo-WslPath $ToolchainRoot
  foreach ($set in $sets) {
    Write-Host "[event] $($set.label) started"
    try {
      $directory = (New-Item -ItemType Directory -Force $set.directory).FullName
      $build = ConvertTo-WslPath $directory
      $report = Join-Path $directory 'event-results.xml'
      Remove-Item -LiteralPath $report -Force -ErrorAction SilentlyContinue
      if ($set.name -eq 'gamejam') {
        $inputs = @(Join-Path $discRoot 'Shenmue II Game Jam (JP) (Apr 11, 2001 prototype) Disc 1/Shenmue2.GDI')
      } elseif ($set.name -eq 'jp' -and $ReferenceDirectories) {
        $inputs = @($ReferenceDirectories.Split(';') | Where-Object { ![string]::IsNullOrWhiteSpace($_) })
      } elseif ($set.name -eq 'jp') {
        $edition = Join-Path $discRoot 'Shenmue II v1.001/_JP'
        $inputs = @(1..4 | ForEach-Object {
          $disc = $_
          $archives = @(Get-ChildItem -LiteralPath $edition -File -Filter '*.zip' |
            Where-Object { $_.Name -match "^Shenmue II v1[.]003 [(]2001[)][(]Sega[)][(]JP[)][(]Disc $disc of 4[)]" })
          if ($archives.Count -ne 1) { throw "Expected one JP V1.003 archive for disc $disc" }
          $archives[0].FullName
        })
      } elseif ($set.name -eq 'us') {
        $inputs = @(1..4 | ForEach-Object { Join-Path $discRoot "Shenmue II US v0.991/archives/disc$_.7z" })
      } else {
        $inputs = @(1..4 | ForEach-Object { Join-Path $discRoot "Shenmue II v1.001/DISC $_" })
      }
      $byPath = @{}; $hashes = @{}; $roots = @(); $disc = 0
      foreach ($inputPath in $inputs) {
        $disc++
        if ($inputPath.StartsWith('/')) {
          $inputPath = & wsl.exe --exec wslpath -w $inputPath
          if ($LASTEXITCODE -ne 0 -or !$inputPath) { throw 'Cannot resolve event reference input' }
          $inputPath = $inputPath.Trim()
        }
        $inputPath = (Resolve-Path -LiteralPath $inputPath).Path
        $reference = (New-Item -ItemType Directory -Force "$directory/reference/DISC $disc").FullName
        $wslRoot = (ConvertTo-WslPath $reference).TrimEnd('/')
        $manifestPath = Join-Path $reference 'event-manifest.json'
        Remove-Item -LiteralPath $manifestPath -Force -ErrorAction SilentlyContinue
        Write-Host "[event] $($set.label) extracting disc $disc"
        & wsl.exe --exec python3 "$source/tools/dc/extract_events.py" (ConvertTo-WslPath $inputPath) $wslRoot
        if ($LASTEXITCODE -ne 0) { throw "Event extraction failed for disc $disc" }
        $manifest = Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json
        $roots += $wslRoot
        foreach ($event in $manifest.events) {
          $relative = [string]$event.path
          if ($relative -match '(^/|\\|(^|/)\.\.?(/|$))' -or $relative -notmatch '[.]EV[123]$') {
            throw 'Invalid event extraction path'
          }
          $hash = ([string]$event.sha256).ToUpperInvariant()
          if ($hash -notmatch '^[0-9A-F]{64}$') { throw 'Invalid extracted event hash' }
          $byPath["$wslRoot/$relative"] = $hash
          $hashes[$hash] = $true
        }
      }
      $set.total = $hashes.Count
      if ($set.total -ne $set.expected) { throw "Expected $($set.expected) distinct EV files; found $($set.total)" }
      $arguments = @('-S', $source, '-B', $build, '-G', 'Unix Makefiles', '-DSHENMUE_DC_EVENT_OVERLAYS=ON',
        "-DSHENMUE_DC_EVENT_TARGET=$($set.target)", "-DSHENMUE_DC_EVENT_REFERENCE_DIR=$($roots -join ';')",
        "-DSHENMUE_DC_TOOLCHAIN_ROOT=$toolchain")
      if ($set.name -ne 'jp') { $arguments += "-DSHENMUE_EVENT_TOOLCHAIN_CACHE=$cache" }
      if ($set.name -eq 'eu') { $arguments += "-DSHENMUE_EVENT_RUNTIME_TOOLCHAIN_CACHE=$cache/us" }
      if ($HostCompiler) { $arguments += "-DSHENMUE_EVENT_HOST_CC=$HostCompiler" }
      & wsl.exe --exec cmake @arguments
      if ($LASTEXITCODE -ne 0) { throw 'Event overlay configuration failed' }
      # Retain object/compiler caches, but never count a stale binary after a failed rebuild.
      if (Test-Path "$directory/events") {
        Get-ChildItem "$directory/events" -File | Where-Object { $_.Extension -match '^\.EV[123]$' } | Remove-Item -Force
      }
      & wsl.exe --exec cmake --build $build --parallel 4 -- -k
      $buildResult = $LASTEXITCODE
      & wsl.exe --exec ctest --test-dir $build -L '^s2-events$' --output-on-failure --parallel 4 --no-tests=error `
        --output-junit "$build/event-results.xml"
      $testResult = $LASTEXITCODE
      $testData = & wsl.exe --exec ctest --test-dir $build -L '^s2-events$' --show-only=json-v1
      if ($LASTEXITCODE -ne 0) { throw 'Cannot read event test inventory' }
      $tests = @((($testData -join "`n") | ConvertFrom-Json).tests)
      [xml]$results = Get-Content -LiteralPath $report -Raw
      $cases = @{}
      foreach ($case in $results.testsuite.testcase) { $cases[$case.name] = $case }
      $passed = @{}; $tested = @{}; $failed = @{}
      foreach ($test in $tests) {
        if ($test.command[1] -ne '-E' -or $test.command[2] -ne 'compare_files' -or !$byPath.ContainsKey($test.command[4])) {
          throw 'Unexpected event comparison in test inventory'
        }
        $hash = $byPath[$test.command[4]]
        $tested[$hash] = $true
        $case = $cases[$test.name]
        $output = Join-Path "$directory/events" (Split-Path $test.command[3] -Leaf)
        $matches = $false
        if ($case -and !$case.SelectSingleNode('failure|error|skipped') -and
            $case.GetAttribute('status') -notin @('notrun', 'disabled') -and (Test-Path -LiteralPath $output)) {
          $matches = (Get-FileHash -LiteralPath $output -Algorithm SHA256).Hash -eq $hash
        }
        if ($matches) { $passed[$hash] = $true } else { $failed[$hash] = $true }
      }
      foreach ($hash in $failed.Keys) { $passed.Remove($hash) }
      $set.successful = $passed.Count
      $set.implemented = $tested.Count
      $set.complete = $cases.Count -eq $tests.Count -and $tests.Count -gt 0 -and
        $failed.Count -eq 0 -and $set.successful -eq $set.implemented
      if ($buildResult -ne 0 -or $testResult -ne 0 -or !$set.complete) {
        throw 'Event overlay roundtrip verification failed'
      }
    } catch {
      $set.error = $_.Exception.Message
      Write-Host "[event] $($set.label) failed: $($set.error)"
    }
    Write-Host "[event] $($set.label) $($set.successful)/$($set.total) overlays rebuilt byte-identically"
  }
} finally {
  $statistics = foreach ($set in $sets) {
    $value = 'not available'; $percentage = $null; $colour = '#9f9f9f'
    if ($set.total -gt 0) {
      $percentage = 100.0 * $set.successful / $set.total
      $value = $percentage.ToString('0.##', [Globalization.CultureInfo]::InvariantCulture) + '%'
      $colour = if ($set.error -or !$set.complete) { '#e05d44' }
        elseif ($set.successful -eq $set.total) { '#4c1' } else { '#dfb317' }
    }
    $title = "$($set.label) - EV Roundtrip: $value ($($set.successful)/$($set.total))"
    $svg = @"
<svg xmlns="http://www.w3.org/2000/svg" width="280" height="20" role="img" aria-label="$title">
  <title>$title</title>
  <rect width="280" height="20" rx="3" fill="#555"/>
  <path fill="$colour" d="M200 0h77q3 0 3 3v14q0 3-3 3h-77z"/>
  <g fill="#fff" text-anchor="middle" font-family="Verdana,DejaVu Sans,sans-serif" font-size="11">
    <text x="100" y="14">$($set.label) - EV Roundtrip</text><text x="240" y="14">$value</text>
  </g>
</svg>
"@
    [IO.File]::WriteAllText("$work/s2-$($set.name)-ev-roundtrip.svg", $svg.Replace("`r`n", "`n") + "`n", [Text.UTF8Encoding]::new($false))
    @{ build = $set.target; successful = $set.successful; total = $set.total; percentage = $percentage;
      implemented = $set.implemented; complete = $set.complete;
      coverageComplete = $set.successful -eq $set.total; error = $set.error }
  }
  @($statistics) | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath "$work/results.json" -Encoding UTF8
}
if (@($sets | Where-Object { $_.error -or !$_.complete }).Count) {
  throw 'S2 event overlay roundtrip verification failed'
}
