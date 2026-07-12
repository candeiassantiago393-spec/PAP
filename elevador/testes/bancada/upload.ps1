# Upload automatico — testes bancada + firmware real
# Uso: .\upload.ps1 hall_sequencia
#      .\upload.ps1 maquete

param(
    [Parameter(Mandatory = $true, Position = 0)]
    [ValidateSet(
        "pedidos_exteriores", "oled", "sensores_hall",
        "hall_porta", "hall_diag", "hall_sequencia", "reset_d10",
        "motor", "motor_hall_pedidos",
        "maquete", "completo"
    )]
    [string]$Teste
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$realRoot = Join-Path (Split-Path -Parent (Split-Path -Parent $root)) "real"

$map = @{
    pedidos_exteriores = Join-Path $root "pedidos_exteriores"
    oled               = Join-Path $root "oled"
    sensores_hall      = Join-Path $root "sensores_hall"
    hall_porta         = Join-Path $root "hall_porta"
    hall_diag          = Join-Path $root "hall_diag"
    hall_sequencia     = Join-Path $root "hall_sequencia"
    reset_d10          = Join-Path $root "reset_d10"
    motor              = Join-Path $root "motor"
    motor_hall_pedidos = Join-Path $root "motor_hall_pedidos"
    maquete            = Join-Path $realRoot "codigo_final_3_0_maquete"
    completo           = Join-Path $realRoot "codigo_final_3_0"
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
