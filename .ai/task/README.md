# Plano de trabalho — 8Puzzle

Tasks do projeto, no mesmo formato do plano da
[cengine](https://github.com/cengine-dev/cengine) (`.ai/task/`): um arquivo por
task, com contexto, degraus e critérios de aceite. Status:
`todo` → `in-progress` → `done`.

## Índice

| # | Task | Status | Categoria |
|---|------|--------|-----------|
| 01 | [PoC The-Forge — plataforma gráfica](01-theforge-poc.md) | done | Plataforma |
| 02 | [The-Forge fase 2 — cengine dona do loop (modo biblioteca)](02-theforge-fase2-library-mode.md) | done | Plataforma |

## Contexto do projeto

O 8Puzzle é o consumidor de referência da cengine e o laboratório das
plataformas de UI: o mesmo domínio (Board/GamePlay/services/GameRouter) roda
hoje em três interfaces — `src/platform/terminal/`, `src/platform/ftxui/` e
`src/platform/theforge/` (gráfica, via The-Forge). As duas fases da PoC estão
concluídas: na fase 1 o The-Forge hospeda a cengine; na fase 2 a cengine é dona
do loop e usa o The-Forge como biblioteca. A segunda fase validou o
`IWindowManager::present()` introduzido pela task 16 da cengine.
