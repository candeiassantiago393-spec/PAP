# Compila testes bancada activos + firmware real
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$realRoot = Join-Path (Split-Path -Parent (Split-Path -Parent $root)) "real"

$projects = @(
    "pedidos_exteriores",
    "oled",
    "sensores_hall",
    "hall_porta",
    "hall_diag",
    "hall_sequencia",
    "reset_d10",
    "motor",
    "motor_hall_pedidos",
    (Join-Path $realRoot "codigo_final_3_0_maquete\ext_porta"),
    (Join-Path $realRoot "codigo_final_3_0")
)

$fail = 0
foreach ($proj in $projects) {
    Write-Host "`n=== BUILD $proj ===" -ForegroundColor Cyan
    Push-Location $proj
    python -m platformio run
    if ($LASTEXITCODE -ne 0) { $fail = 1 }
    Pop-Location
}

if ($fail) { exit 1 }
Write-Host "`nTodos os projectos compilaram com sucesso." -ForegroundColor Green
