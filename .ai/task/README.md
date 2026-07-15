# Plano de trabalho — 8Puzzle

Tasks do projeto, no mesmo formato do plano da
[cengine](https://github.com/cengine-dev/cengine) (`.ai/task/`): um arquivo por
task, com contexto, degraus e critérios de aceite. Status:
`todo` → `in-progress` → `done`.

## Índice

| # | Task | Status | Categoria |
|---|------|--------|-----------|
| 01 | [PoC The-Forge — plataforma gráfica](01-theforge-poc.md) | done | Plataforma |
| 02 | [The-Forge fase 2 — cengine dona do loop (modo biblioteca)](02-theforge-fase2-library-mode.md) | todo | Plataforma |

## Contexto do projeto

O 8Puzzle é o consumidor de referência da cengine e o laboratório das
plataformas de UI: o mesmo domínio (Board/GamePlay/services/GameRouter) roda
hoje em três interfaces — `src/platform/terminal/`, `src/platform/ftxui/` e
`src/platform/theforge/` (gráfica, via The-Forge; fase 1 da PoC concluída —
ver task 01). A fase 2 (cengine dona do loop, The-Forge como biblioteca) está
planejada na task 02 e depende da task 16 da cengine
(`IWindowManager::present()`).
