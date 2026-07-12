# Teste bancada — Hall 4 pisos (sobe / deteta / desce)

Motor **sobe** continuamente. Cada vez que **entra** numa zona Hall → informa no Serial.  
**Ignora** o Hall já activo no arranque (ex.: parado no piso 1).  
Ao detetar **piso 4** → **desce**. Na descida informa cada piso; ao detetar **piso 1** → para.

## Pinagem maquete

| Piso | Hall |
|------|------|
| 1 | **D7** |
| 2 | **D8** |
| 3 | **D11** |
| 4 | **D12** |

## Upload

```powershell
cd elevador/testes/bancada
.\upload.ps1 hall_sequencia
pio device monitor -d hall_sequencia
```

## Serial (exemplo)

```
Arranque: Hall piso 1 activo — ignorado, motor sobe...
>> Motor SOBE
>>> SUBIDA — PISO 2
>>> SUBIDA — PISO 3
>>> SUBIDA — PISO 4
=== PISO 4 DETETADO — comeca descida ===
>> Motor DESCE
>>> DESCIDA — PISO 3
>>> DESCIDA — PISO 2
>>> DESCIDA — PISO 1
=== PISO 1 DETETADO — teste completo ===
```

## Notas

- Só reporta na **borda** (entra na zona), não fica repetindo enquanto parado no piso.
- Motor ao contrário: `MOTOR_INVERTIDO 0` em `src/main.ino`.
