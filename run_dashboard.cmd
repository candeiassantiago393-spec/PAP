@echo off
REM Um comando fixo: upload + dashboard (deteta a porta COM automaticamente)
powershell -ExecutionPolicy Bypass -File "%~dp0logicas_extras\central_de_controlo\scripts\run_dashboard.ps1" %*
