# Upload automatico — testes bancada elevador
# Uso: .\upload.ps1 motor
#      .\upload.ps1 integrado
#      .\upload.ps1 pedidos_exteriores

param(
    [Parameter(Mandatory = $true, Position = 0)]
    [ValidateSet("pedidos_exteriores", "oled", "sensores_hall", "motor", "integrado", "real")]
    [string]$Teste
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path

$map = @{
    pedidos_exteriores = Join-Path $root "pedidos_exteriores"
    oled               = Join-Path $root "oled"
    sensores_hall      = Join-Path $root "sensores_hall"
    motor              = Join-Path $root "motor"
    integrado          = Join-Path $root "integrado"
    real               = (Join-Path (Split-Path -Parent (Split-Path -Parent $root)) "real\l298n_sh1106_hall")
}

$proj = $map[$Teste]
if (-not (Test-Path $proj)) {
    Write-Error "Pasta nao encontrada: $proj"
}

function Get-MegaPort {
    $ports = python -c @"
import serial.tools.list_ports as p
for x in p.comports():
    d = (x.description or '').lower()
    if 'usb' in d or 'arduino' in d or 'mega' in d or 'ch340' in d or 'serial' in d:
        if 'management' not in d and 'bluetooth' not in d:
            print(x.device)
"@ 2>$null
    $list = @($ports -split "`n" | Where-Object { $_ -match '^COM\d+$' })
    return $list
}

$ports = Get-MegaPort
if ($ports.Count -eq 0) {
    Write-Error "Nenhuma porta USB encontrada. Liga o Mega por USB."
}

$uploaded = $false
foreach ($port in $ports) {
    Write-Host ">> Tentativa upload $Teste em $port ..."
    Push-Location $proj
    try {
        python -m platformio run -t upload --upload-port $port
        if ($LASTEXITCODE -eq 0) {
            Write-Host "OK — $Teste enviado para $port"
            $uploaded = $true
            break
        }
    } finally {
        Pop-Location
    }
}

if (-not $uploaded) {
    Write-Error "Upload falhou em todas as portas ($($ports -join ', ')). Fecha Serial Monitor / Arduino IDE e tenta de novo."
}
