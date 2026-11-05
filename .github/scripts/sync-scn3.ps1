param([switch]$IncludeSources)

$ErrorActionPreference = 'Stop'
$work = 'build/windows-capture/scenarios/roundtrip'
$badges = @('s1', 'ws' | Where-Object { Test-Path "$work/$_-roundtrip.svg" })
if (!$badges.Count) { exit 0 }
$head = git rev-parse HEAD
if ($LASTEXITCODE -ne 0) { throw 'Cannot read checkout commit' }
$tip = git ls-remote --refs origin refs/heads/master
if ($LASTEXITCODE -ne 0) { throw 'Cannot read master' }
if ($head -ne $env:GITHUB_SHA -or !$tip -or ($tip -split '\s+')[0] -ne $env:GITHUB_SHA) {
  Write-Host 'master has advanced; leaving generated files for its next run'
  exit 0
}
$paths = @($badges | ForEach-Object { "docs/$_-roundtrip.svg" })
$manifest = "$work/sync-files.txt"
if ($IncludeSources -and (Test-Path $manifest)) {
  $paths += @(Get-Content -LiteralPath $manifest | Where-Object { $_ })
}
foreach ($path in $paths) {
  if ($path -notmatch '^docs/(s1|ws)-roundtrip[.]svg$' -and
      ($path -notmatch '^src/(s1|ws)/scenario/(?:[A-Za-z0-9_-]+/)*[A-Za-z0-9_.-]+\.(c|h)$' -or $path -match '(?:^|/)\.\.(?:/|$)')) {
    throw 'Unexpected generated path in scenario sync'
  }
}
foreach ($game in $badges) {
  Copy-Item -LiteralPath "$work/$game-roundtrip.svg" -Destination "docs/$game-roundtrip.svg" -Force
}
git add -- @paths
if ($LASTEXITCODE -ne 0) { throw 'Cannot stage generated files' }
git diff --cached --quiet -- @paths
if ($LASTEXITCODE -eq 0) { exit 0 }
if ($LASTEXITCODE -ne 1) { throw 'Cannot compare generated files' }
$message = "chore(scn3-ci): update roundtrip results from $env:GITHUB_SHA"
if ($IncludeSources -and (Test-Path $manifest)) { $message = "chore(scn3-ci): sync decompiled scripts from $env:GITHUB_SHA" }
git -c user.name=LemonHaze420 -c user.email=35783139+LemonHaze420@users.noreply.github.com `
  -c commit.gpgsign=false commit --only -m $message -- @paths
if ($LASTEXITCODE -ne 0) { throw 'Cannot commit generated files' }
git push --no-follow-tags --recurse-submodules=no origin HEAD:refs/heads/master
if ($LASTEXITCODE -ne 0) { throw 'Cannot push generated files' }
