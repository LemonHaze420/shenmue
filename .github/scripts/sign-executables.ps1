param([Parameter(Mandatory = $true)][string[]]$Files)

$ErrorActionPreference = 'Stop'
$pfx = Join-Path $env:RUNNER_TEMP 'lemonhaze-ci.pfx'
$imported = @()
try {
  if (!$env:SIGNING_PFX) { throw 'SIGNING_PFX is not configured' }
  [IO.File]::WriteAllBytes($pfx, [Convert]::FromBase64String($env:SIGNING_PFX))
  $password = [Security.SecureString]::new()
  if ($env:SIGNING_PASSWORD) { $password = ConvertTo-SecureString $env:SIGNING_PASSWORD -AsPlainText -Force }
  $imported = @(Import-PfxCertificate $pfx -CertStoreLocation Cert:\CurrentUser\My -Password $password)
  $certificate = @($imported | Where-Object HasPrivateKey)
  if ($certificate.Count -ne 1) { throw 'Expected one signing certificate with a private key' }
  Remove-Item Env:SIGNING_PFX, Env:SIGNING_PASSWORD -ErrorAction SilentlyContinue
  $signtool = (Get-ChildItem "${env:ProgramFiles(x86)}/Windows Kits/10/bin/*/x64/signtool.exe" | Select-Object -First 1).FullName
  if (!$signtool) { throw 'Windows SDK SignTool was not found' }
  foreach ($file in $Files) {
    & $signtool sign /s My /sha1 $certificate[0].Thumbprint /fd SHA256 $file
    if ($LASTEXITCODE -ne 0) { throw "Signing failed: $file" }
  }
} finally {
  Remove-Item Env:SIGNING_PFX, Env:SIGNING_PASSWORD -ErrorAction SilentlyContinue
  foreach ($certificate in $imported) { Remove-Item "Cert:\CurrentUser\My\$($certificate.Thumbprint)" -DeleteKey -Force }
  if (Test-Path $pfx) { Remove-Item $pfx -Force }
}
