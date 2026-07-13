# Upload firmware + dashboard local (Central de Controlo)
# Uso: .\run_dashboard.ps1 [-Port COMx] [-SkipUpload]
# Sem -Port: deteta automaticamente a Arduino Mega ligada por USB.

param(
  [string]$Port = "",
  [switch]$SkipUpload
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$Firmware = Join-Path $Root "testes\bancada\dht_gas_oled_next_buzzer"
$Dashboard = Join-Path $Root "dashboard"

function Find-MegaComPort {
  $found = @()

  $devices = Get-CimInstance Win32_PnPEntity -ErrorAction SilentlyContinue |
    Where-Object { $_.Caption -match 'Arduino Mega' -or $_.PNPDeviceID -match 'VID_2341&PID_0042' }

  foreach ($dev in $devices) {
    if ($dev.Caption -match '\((COM\d+)\)') {
      $found += $Matches[1]
    }
  }

  if (Get-Command pio -ErrorAction SilentlyContinue) {
    Push-Location $Firmware
    try {
      $raw = pio device list 2>$null | Out-String
      $blocks = $raw -split '(?=^COM\d+)' | Where-Object { $_.Trim() }
      foreach ($block in $blocks) {
        if ($block -notmatch 'Arduino Mega|VID:PID=2341:0042') { continue }
        if ($block -match '^(COM\d+)') {
          $found += $Matches[1]
        }
      }
    } finally {
      Pop-Location
    }
  }

  return @($found | Select-Object -Unique)
}

if (-not $Port) {
  Write-Host "A procurar Arduino Mega..." -ForegroundColor Yellow
  $ports = Find-MegaComPort
  if ($ports.Count -eq 1) {
    $Port = $ports[0]
    Write-Host "Mega encontrada em $Port" -ForegroundColor Green
  } elseif ($ports.Count -gt 1) {
    Write-Host "Varias Megas detetadas: $($ports -join ', ')" -ForegroundColor Red
    Write-Host "Especifica uma: .\run_dashboard.ps1 -Port COMx" -ForegroundColor Yellow
    exit 1
  } else {
    Write-Host "Nenhuma Arduino Mega encontrada." -ForegroundColor Red
    Write-Host "Liga a Mega por USB e fecha Serial Monitor / Arduino IDE." -ForegroundColor Yellow
    Write-Host "Portas disponiveis: $([string]::Join(', ', [System.IO.Ports.SerialPort]::GetPortNames()))" -ForegroundColor Yellow
    exit 1
  }
}

Write-Host "=== Central de Controlo - Dashboard ===" -ForegroundColor Cyan
Write-Host "Porta: $Port" -ForegroundColor Cyan

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
