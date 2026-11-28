param([string]$InputDirectory = 'dist/pc', [string]$OutputDirectory = 'dist',
  [Parameter(Mandatory = $true)][ValidatePattern('^[0-9a-fA-F]{40}$')][string]$Commit)

$ErrorActionPreference = 'Stop'
$templates = Join-Path $PSScriptRoot '../release'
$packages = @(
  @{ Name = 'shenmue-pc'; Files = @{
      'Shenmue-PC-1/shenmue1.exe' = 'Shenmue_e.exe'
      'Shenmue-PC-2/shenmue2.exe' = 'Shenmue2_e.exe'
    }; Notices = @('Shenmue-PC-1', 'Shenmue-PC-2') },
  @{ Name = 'shenmue-steam-pc'; Files = @{
      'Shenmue-PC-1/shenmue1e.asi' = 'shenmue1e.asi'
      'Shenmue-PC-2/shenmue2e.asi' = 'shenmue2e.asi'
    }; Notices = @('Shenmue-PC-1', 'Shenmue-PC-2') },
  @{ Name = 'whats-shenmue-pc'; Files = @{
      'WhatsShenmue/WhatsShenmue.exe' = 'WhatsShenmue.exe'
    }; Notices = @('WhatsShenmue') }
)
foreach ($package in $packages) {
  foreach ($source in $package.Files.Keys) {
    if (!(Test-Path -LiteralPath (Join-Path $InputDirectory $source) -PathType Leaf)) { throw "Missing PC binary: $source" }
  }
  foreach ($artifact in $package.Notices) {
    if (!(Test-Path -LiteralPath (Join-Path $InputDirectory "$artifact/THIRD_PARTY_NOTICES.txt") -PathType Leaf)) {
      throw "Missing third-party notices: $artifact"
    }
  }
}

$loaderDirectory = Join-Path $OutputDirectory 'asi-loader'
New-Item -ItemType Directory -Force $loaderDirectory | Out-Null
$loaderArchive = Join-Path $loaderDirectory 'Ultimate-ASI-Loader-NoPDB_x64.zip'
if (!(Test-Path -LiteralPath $loaderArchive)) {
  gh release download v10.0.2 --repo ThirteenAG/Ultimate-ASI-Loader --pattern Ultimate-ASI-Loader-NoPDB_x64.zip --dir $loaderDirectory
  if ($LASTEXITCODE -ne 0) { throw 'ASI loader download failed' }
}
if ((Get-FileHash -LiteralPath $loaderArchive -Algorithm SHA256).Hash -ne 'fe964adf4de7f467ff39c440a4059de02e37d0574a62944eef2f55a35edafa2f') {
  throw 'ASI loader checksum mismatch'
}
Expand-Archive -LiteralPath $loaderArchive -DestinationPath $loaderDirectory -Force

$notes = @()
foreach ($package in $packages) {
  $name = $package.Name
  $directory = Join-Path $OutputDirectory "packages/$name"
  New-Item -ItemType Directory -Force $directory | Out-Null
  foreach ($source in $package.Files.Keys) {
    Copy-Item -LiteralPath (Join-Path $InputDirectory $source) -Destination (Join-Path $directory $package.Files[$source])
  }
  $readme = (Get-Content -LiteralPath (Join-Path $templates "README-$name.txt") -Raw).Replace('@COMMIT@', $Commit)
  $readme | Set-Content (Join-Path $directory 'README.txt') -Encoding utf8
  Copy-Item -LiteralPath (Join-Path $templates 'LICENSE.txt') -Destination $directory
  $notices = foreach ($artifact in $package.Notices) {
    Get-Content -LiteralPath (Join-Path $InputDirectory "$artifact/THIRD_PARTY_NOTICES.txt") -Raw
  }
  ($notices | Select-Object -Unique) -join "`r`n" | Set-Content (Join-Path $directory 'THIRD_PARTY_NOTICES.txt') -Encoding utf8
  $files = @($package.Files.Values) + @('README.txt', 'LICENSE.txt', 'THIRD_PARTY_NOTICES.txt')
  if ($name -eq 'shenmue-steam-pc') {
    Copy-Item -LiteralPath (Join-Path $loaderDirectory 'dinput8.dll') -Destination (Join-Path $directory 'd3d11.dll')
    Copy-Item -LiteralPath (Join-Path $templates 'LICENSE-ASI-Loader.txt') -Destination $directory
    $files += 'd3d11.dll', 'LICENSE-ASI-Loader.txt'
  }
  $files = @($files | ForEach-Object { Join-Path $directory $_ })
  Compress-Archive -LiteralPath $files -DestinationPath (Join-Path $OutputDirectory "$name.zip") -Force
  $notes += Get-Content -LiteralPath (Join-Path $directory 'README.txt') -Raw
}
$notes -join "`r`n`r`n" | Set-Content (Join-Path $OutputDirectory 'release-notes.txt') -Encoding utf8
