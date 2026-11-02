$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
$reference = $null
$image = $null
try {
  $reference = [Drawing.Bitmap]::new((Join-Path $PWD 'docs/hang_on_standalone.png'))
  $image = [Drawing.Bitmap]::new((Join-Path $PWD 'dist/screens/hangon-pc.png'))
  if ($image.Width -ne $reference.Width -or $image.Height -ne $reference.Height) {
    throw 'HangOn reference dimensions differ'
  }
  $tolerance = 8
  $different = 0
  for ($y = 0; $y -lt $image.Height; ++$y) {
    for ($x = 0; $x -lt $image.Width; ++$x) {
      $actual = $image.GetPixel($x, $y)
      $expected = $reference.GetPixel($x, $y)
      if ([Math]::Abs([int]$actual.R - $expected.R) -gt $tolerance -or
          [Math]::Abs([int]$actual.G - $expected.G) -gt $tolerance -or
          [Math]::Abs([int]$actual.B - $expected.B) -gt $tolerance) { ++$different }
    }
  }
  $similarity = 1 - $different / ($image.Width * $image.Height)
  Write-Host ('HangOn reference similarity: {0:P3}' -f $similarity)
  if ($similarity -lt 0.999) { throw 'HangOn screenshot differs from the reference' }
} finally {
  if ($image) { $image.Dispose() }
  if ($reference) { $reference.Dispose() }
}
