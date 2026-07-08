# Plano de trabalho — 8Puzzle

Tasks do projeto, no mesmo formato do plano da
[cengine](https://github.com/mrmarmitt/cengine) (`.ai/task/`): um arquivo por
task, com contexto, degraus e critérios de aceite. Status:
`todo` → `in-progress` → `done`.

## Índice

| # | Task | Status | Categoria |
|---|------|--------|-----------|
| 01 | [PoC The-Forge — plataforma gráfica](01-theforge-poc.md) | todo | Plataforma |

## Contexto do projeto

O 8Puzzle é o consumidor de referência da cengine e o laboratório das
plataformas de UI: o mesmo domínio (Board/GamePlay/services/GameRouter) roda
hoje em duas interfaces (`src/platform/terminal/` e `src/platform/ftxui/`).
A próxima fronteira é a plataforma gráfica de verdade (The-Forge), em
`src/platform/theforge/` (pasta reservada desde o início do projeto).
