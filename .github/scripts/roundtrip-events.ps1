param(
  [string]$BuildDirectory = 'build/s2-event-roundtrip',
  [string]$ReferenceDirectories = $env:SHENMUE_DC_EVENT_REFERENCE_DIR,
  [string]$ToolchainRoot = $env:SHENMUE_DC_TOOLCHAIN_ROOT,
  [string]$HostCompiler = $env:SHENMUE_EVENT_HOST_CC
)

$ErrorActionPreference = 'Stop'
$work = (New-Item -ItemType Directory -Force $BuildDirectory).FullName
$sets = @(
  @{ name = 'jp'; label = 'S2 JP'; target = 'S2_JP_1001'; expected = 115; directory = $work },
  @{ name = 'gamejam'; label = 'S2 GameJam'; target = 'S2_JP_GAMEJAM'; expected = 50; directory = "$work/gamejam" },
  @{ name = 'us'; label = 'S2 US'; target = 'S2_US'; expected = 115; directory = "$work/us" },
  @{ name = 'eu'; label = 'S2 EU'; target = 'S2_EU'; expected = 116; directory = "$work/eu" }
)
foreach ($set in $sets) {
  $set.successful = 0; $set.total = 0; $set.complete = $false; $set.error = $null
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
        $gdi = Join-Path $discRoot 'Shenmue II Game Jam (JP) (Apr 11, 2001 prototype) Disc 1/Shenmue2.GDI'
        $references = @((New-Item -ItemType Directory -Force "$directory/reference").FullName)
        & wsl.exe --exec python3 "$source/tools/dc/extract_events.py" (ConvertTo-WslPath $gdi) (ConvertTo-WslPath $references[0])
        if ($LASTEXITCODE -ne 0) { throw 'GameJam EV extraction failed' }
      } elseif ($set.name -eq 'jp' -and $ReferenceDirectories) {
        $references = @($ReferenceDirectories.Split(';') | Where-Object { ![string]::IsNullOrWhiteSpace($_) })
      } else {
        $edition = switch ($set.name) {
          'jp' { 'Shenmue II v1.001/_JP' }
          'us' { 'Shenmue II US v0.991' }
          'eu' { 'Shenmue II v1.001' }
        }
        $references = @(1..4 | ForEach-Object { Join-Path $discRoot "$edition/DISC $_" })
      }
      $byPath = @{}; $hashes = @{}; $roots = @()
      foreach ($reference in $references) {
        if ($reference.StartsWith('/')) {
          $reference = & wsl.exe --exec wslpath -w $reference
          if ($LASTEXITCODE -ne 0 -or !$reference) { throw 'Cannot resolve event reference directory' }
          $reference = $reference.Trim()
        }
        $root = (Resolve-Path -LiteralPath $reference).Path.TrimEnd('\', '/')
        $wslRoot = (ConvertTo-WslPath $root).TrimEnd('/')
        $roots += $wslRoot
        foreach ($file in Get-ChildItem -LiteralPath $root -Recurse -File | Where-Object { $_.Extension -match '^\.EV[123]$' }) {
          $hash = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash
          $relative = $file.FullName.Substring($root.Length + 1).Replace('\', '/')
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
      $passed = @{}
      foreach ($test in $tests) {
        if ($test.command[1] -ne '-E' -or $test.command[2] -ne 'compare_files' -or !$byPath.ContainsKey($test.command[4])) {
          throw 'Unexpected event comparison in test inventory'
        }
        $hash = $byPath[$test.command[4]]
        $case = $cases[$test.name]
        $output = Join-Path "$directory/events" (Split-Path $test.command[3] -Leaf)
        if ($case -and !$case.SelectSingleNode('failure|error|skipped') -and
            $case.GetAttribute('status') -notin @('notrun', 'disabled') -and (Test-Path -LiteralPath $output)) {
          if ((Get-FileHash -LiteralPath $output -Algorithm SHA256).Hash -eq $hash) { $passed[$hash] = $true }
        }
      }
      $set.successful = $passed.Count
      $set.complete = $tests.Count -eq $set.total -and $cases.Count -eq $tests.Count
      if ($buildResult -ne 0 -or $testResult -ne 0 -or !$set.complete -or $set.successful -ne $set.total) {
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
      $colour = if (!$set.error -and $set.complete -and $set.successful -eq $set.total) { '#4c1' } else { '#e05d44' }
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
      complete = $set.complete; error = $set.error }
  }
  @($statistics) | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath "$work/results.json" -Encoding UTF8
}
if (@($sets | Where-Object { $_.error -or !$_.complete -or $_.successful -ne $_.total }).Count) {
  throw 'S2 event overlay roundtrip verification failed'
}
