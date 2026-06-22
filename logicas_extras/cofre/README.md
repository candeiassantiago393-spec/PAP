# Cofre — Código Secreto

Subsistema de **acesso por código numérico** — protótipo de cofre com teclado matricial, servo e feedback visual/sonoro.

Relacionado com a lógica da [porta do prédio](../porta_predio/) (mesmo conceito de PIN + validação), aplicado aqui a um **cofre/fechadura** na maquete do prédio inteligente.

| Campo | Valor |
|-------|-------|
| **Estado** | Simulação Wokwi funcional |
| **MCU** | Arduino Uno |
| **Plataforma** | Wokwi |
| **Etapa** | [E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md) |

---

## Funcionamento

1. Utilizador introduz dígitos no teclado 3×4
2. `#` confirma o código
3. **Código correcto** (`1904`): LED verde, servo abre (90°), bip curto
4. **Código errado**: LED vermelho, bip longo grave
5. `*` limpa a entrada (reset)

---

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/wokwi/](simulacao/wokwi/) | Projeto Wokwi exportado — **abrir esta pasta para simular** |
| [real/](real/) | Implementação física na maquete (futuro) |
| [testes/](testes/) | Testes isolados teclado/servo |
| [documentacao/](documentacao/) | Pinagem e lógica detalhada |
| [cofre_codigo_secreto.ino](cofre_codigo_secreto.ino) | Cópia do firmware (referência) |

---

## Wokwi

| Campo | Valor |
|-------|-------|
| **Link** | https://wokwi.com/projects/462220624733136897 |
| **Export local** | [simulacao/wokwi/](simulacao/wokwi/) |
| **Autor Wokwi** | Santiago Candeias |

### Simular no Cursor

1. Abrir pasta `logicas_extras/cofre/simulacao/wokwi/`
2. Abrir `sketch.ino`
3. `F1` → **Wokwi: Start Simulator**
4. Introduzir `1904` + `#` para testar acesso

---

## Componentes (simulação)

| Componente | Função |
|------------|--------|
| Arduino Uno | Controlador |
| Teclado matricial 3×4 | Entrada do código |
| Servo | Abrir/fechar “porta” do cofre |
| LED vermelho (D11) | Acesso negado |
| LED verde (D12) | Acesso permitido |
| Buzzer (D13) | Feedback sonoro |

---

## Ligação ao prédio inteligente

- **Porta do prédio:** mesma ideia de código PIN — pode reutilizar lógica do `sketch.ino`
- **Hardware comprado (E12):** teclado matricial 3×4 Mauser [096-4642](../../docs/compras/faturas_mauser/2026EC1403323.md) — candidato à implementação real

---

## Próximos passos

- [ ] Adaptar para ESP32 / teclado físico Mauser
- [ ] Integrar na maquete (cofre ou porta de apartamento)
- [ ] Alterar código secreto via configuração (não hardcoded)
