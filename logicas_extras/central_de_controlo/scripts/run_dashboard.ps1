# Upload firmware + dashboard local (Central de Controlo)
# Uso: .\run_dashboard.ps1 [-Port COM7] [-SkipUpload]

param(
  [string]$Port = "COM7",
  [switch]$SkipUpload
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$Firmware = Join-Path $Root "testes\bancada\dht_gas_oled_next_buzzer"
$Dashboard = Join-Path $Root "dashboard"

Write-Host "=== Central de Controlo - Dashboard ===" -ForegroundColor Cyan

if (-not $SkipUpload) {
  Write-Host "Upload firmware -> $Port" -ForegroundColor Yellow
  Push-Location $Firmware
  pio run -t upload --upload-port $Port
  if ($LASTEXITCODE -ne 0) { Pop-Location; exit $LASTEXITCODE }
  Pop-Location
  Write-Host "Upload OK. Aguarda 5s (Mega reinicia)..." -ForegroundColor Green
  Start-Sleep -Seconds 5
}

Write-Host "Instalar dependencias Python (se necessario)..." -ForegroundColor Yellow
$pipCmd = Get-Command pip -ErrorAction SilentlyContinue
if ($pipCmd) {
  pip install -q -r (Join-Path $Dashboard "requirements.txt")
} else {
  py -3 -m pip install -q -r (Join-Path $Dashboard "requirements.txt")
}

$env:CENTRAL_COM = $Port
$url = "http://127.0.0.1:8765"

Write-Host "Abrir dashboard: $url" -ForegroundColor Green
Start-Process $url

Push-Location $Dashboard
if (Get-Command py -ErrorAction SilentlyContinue) {
  py -3 server.py
} else {
  python server.py
}
