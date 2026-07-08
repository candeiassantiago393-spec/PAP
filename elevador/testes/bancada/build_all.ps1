# Compila todos os testes de bancada + firmware real
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$real = Join-Path (Split-Path -Parent (Split-Path -Parent $root)) "real\l298n_sh1106_hall"

$projects = @(
    "pedidos_exteriores",
    "oled",
    "sensores_hall",
    "motor",
    "integrado",
    $real
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
