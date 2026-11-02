$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
$screens = New-Item -ItemType Directory -Force dist/screens
$previousRoot = $env:SHEN_ROOT
$previousCapture = $env:SHEN_AUTO_TEST_CAPTURE
try {
  $env:SHEN_ROOT = $env:SHEN_STEAM_ROOT
  foreach ($game in 's1', 's2', 'hangon') {
    $output = Join-Path $screens.FullName "$game-pc.png"
    if (Test-Path $output) { Remove-Item $output }
    $env:SHEN_AUTO_TEST_CAPTURE = $output
    if ($game -eq 'hangon') {
      $exe = (Resolve-Path 'build/hangon-capture/Release/ShenmueHangon.exe').Path
      $directory = Join-Path $env:SHEN_STEAM_ROOT 'sm1'
      if (!(Test-Path "$directory/GameHangon_Winx64_Release.dll")) { throw 'HangOn DLL was not found' }
      $width = 320; $height = 224
    } else {
      $exe = (Resolve-Path "build/windows-capture/Release/shenmue$($game.Substring(1)).exe").Path
      $directory = Split-Path $exe
      $width = 1280; $height = 720
    }
    $process = Start-Process -FilePath $exe -WorkingDirectory $directory -PassThru
    $null = $process.Handle
    if (!$process.WaitForExit(90000)) {
      $process.Kill()
      $process.WaitForExit()
      throw "$game screen capture timed out"
    }
    $process.Refresh()
    if ($process.ExitCode -ne 0 -or !(Test-Path $output)) { throw "$game screen capture failed" }
    $image = [Drawing.Bitmap]::new($output)
    try {
      if ($image.Width -ne $width -or $image.Height -ne $height) { throw "$game screen dimensions differ" }
      $colours = [Collections.Generic.HashSet[int]]::new()
      for ($y = 0; $y -lt $image.Height; $y += 20) {
        for ($x = 0; $x -lt $image.Width; $x += 20) { [void]$colours.Add($image.GetPixel($x, $y).ToArgb()) }
      }
      if ($colours.Count -lt 8) { throw "$game screen is blank" }
    } finally { $image.Dispose() }
  }
} finally {
  $env:SHEN_ROOT = $previousRoot
  $env:SHEN_AUTO_TEST_CAPTURE = $previousCapture
}
