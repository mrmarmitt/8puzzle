# 01 — PoC The-Forge: o 8Puzzle numa plataforma gráfica

- **Status:** in-progress (degraus 0, 1 e 2 ✅ — último em 2026-07-09)
- **Prioridade:** exploratória (aprendizado de renderização)
- **Categoria:** Plataforma
- **Pré-requisitos no ambiente:** ✅ já satisfeitos (ver inventário abaixo)
- **Relacionada:** task 15 da cengine (modo hospedado do loop) — a fase 2
  desta PoC; **não** é pré-requisito da PoC.

## Visão e decisão de integração

A visão original do projeto: **a cengine gerencia o loop e o The-Forge é o
motor gráfico**. O The-Forge, porém, tem duas camadas separáveis:

1. **Framework** (`IApp` + `DEFINE_APPLICATION_MAIN` + `WindowsBase.cpp`):
   dono do loop (`while (!quit)` em `WindowsBase.cpp:536`), da janela, do
   message pump, do input, da UI middleware e do reload de device/shader.
   `Update(float dt)`/`Draw()` das apps de exemplo são **callbacks** — quem os
   chama é o `WindowsMain`.
2. **Renderer como biblioteca** (`IGraphics.h`): API estilo C
   (`initRenderer`, `addSwapChain(WindowHandle)`, `acquireNextImage`,
   `queueSubmit`, `queuePresent`) que **não exige `IApp`**; os subsistemas de
   base (`initMemAlloc`/`initFileSystem`/`initLog`) são funções públicas.

**Decisão: duas fases.**

- **Fase 1 (esta task)** — caminho oficial do framework: um `IApp` adaptador
  hospeda as fases do `cengine::routing::GameManager`. Rende rápido, ganha o
  input/UI middleware/reload de graça, e o aprendizado de renderização
  acontece sem brigar com a cola de SO. A arquitetura continua mandando: quem
  decide cenas/estados/regras é a cengine — o loop vira detalhe mecânico da
  plataforma (como o pacing de 60 FPS é detalhe do `FtxuiWindowManager`).
- **Fase 2 (futura, a visão original)** — modo biblioteca: `EngineManager`
  dono do loop, janela própria (HWND passado ao swapchain), message pump no
  `TheForgeWindowManager::update()`. Custo: reescrever a cola que o
  `WindowsBase` já resolve (wndproc, resize→recriar swapchain, device lost) e
  dirigir manualmente input/UI middleware. Depende do aprendizado da fase 1 e
  da task 15 da cengine (`frame(dt)` hospedado). Cada upgrade do The-Forge
  pode quebrar essa cola — decidir se o ganho vale depois da fase 1.

### Mapeamento das fases (fase 1)

| The-Forge (`IApp`) | cengine |
|--------------------|---------|
| `Init()` / `Exit()` | montagem (repositório+router+GameManager) / teardown |
| `Load()` / `Unload()` | recursos de GPU da plataforma (a cengine não participa) |
| `Update(float dt)` | `gameManager.onEnter()` + `input()` + `update(Seconds{dt})`* |
| `Draw()` | `gameManager.render()` + ao fim: `onExit()`; `shouldExit()` → shutdown |

\* O `dt` do The-Forge é variável com clamp (0,05 s se > 0,15 s — análogo ao
nosso anti-espiral). O adaptador pode replicar o acumulador de fixed timestep
da cengine por fora, ou aceitar dt variável na PoC (jogo de tabuleiro não
sofre). Formalizar isso é a task 15 da cengine.

## Inventário do ambiente (verificado em 2026-07-08)

| Item | Estado |
|------|--------|
| The-Forge clonado | `The-Forge/` no workspace, **v1.63** (abr/2025) |
| Assets de arte | Baixados (`PRE_BUILD.bat` já rodou; `Art/` presente) |
| Exemplos compilados | jun/2025: 01, 09, 10, 16, 39 em Release x64 + `OS.lib`/`Renderer.lib` |
| Toolchain | VS 2019 Community 16.11 (o que as solutions `PC_VS2019` pedem) |
| Vulkan SDK | Ausente — irrelevante no Windows (D3D12 é o default) |
| Pasta da plataforma | `src/platform/theforge/` existe (vazia) |

## Degraus

0. **Revalidar o ambiente** ✅ (2026-07-08): exemplos rebuildados e executados
   no VS2019 atual, incluindo o `10_ScreenSpaceReflections` (um dos mais
   complexos — visibility buffer + reflexões + PBR), rodando e interativo.
   Toolchain, GPU e driver confirmados.
1. **Hello Forge** ✅ (2026-07-09): `IApp` mínimo desenhando texto via UI
   middleware (sem 8puzzle). Diferente do plano original (vcxproj na solution
   de exemplos), o projeto E o output vivem no repo do 8puzzle
   (`src/platform/theforge/PC_VS2019/HelloForge.vcxproj` → `out/theforge/`),
   com o The-Forge como dependência irmã. Aprendizados registrados:
   - a pasta do vcxproj PRECISA chamar `PC_VS2019` (o `TF_Shared.props`
     seleciona os props de plataforma testando o caminho do projeto);
   - os shaders de fonte/UI vêm prontos do projeto OS (copiados no pre-link),
     mas `default.rootsig`/`compute.rootsig` são compilados **por app** pelo
     passo FSL — sem `Shaders.list` + `fsl.targets` o exe abre e fecha
     (`Failed to open shader binary` → pipelines falham com `0x80070057`);
   - output fora da árvore do The-Forge exige `PathStatement.txt` próprio
     (os mounts padrão assumem o exe 6 níveis dentro da árvore dele) e
     pre-link/post-build sobrescritos com cópias explícitas.
2. **O adaptador** ✅ (2026-07-09): `CengineAdapter.vcxproj` — o `IApp` hospeda
   o `GameManager` da cengine (mapeamento acima) com cenas de teste A↔B +
   exit, validando navegação, reativação A→B→A (cena recriada, estado
   zerado) e `shouldExit()` → `requestShutdown()`. Fontes do módulo routing
   compiladas direto do repo irmão `../cengine` (sem `EngineManager` — o
   loop é do The-Forge nesta fase). Aprendizados:
   - ✅ **risco C++23 dissipado**: as fontes da cengine compilaram limpas no
     VS2019 16.11 com `/std:c++latest`; só foi preciso `/Zc:char8_t-` porque
     o `DEFINE_APPLICATION_MAIN` atribui `u8""` a `const char*` (regra nova
     do C++20, problema do The-Forge, não da cengine);
   - input do app via `inputAddCustomBindings("scene_next; button; K_ENTER;
     released")` + `inputGetCustomBindingEnum` — NÃO incluir
     `OS/Input/InputCommon.h` (define globals `gInputValues` etc. e
     duplicaria símbolos do OS.lib);
   - ponte cenas↔plataforma via snapshot por quadro (`ForgeFrame`: input
     bools no `Update`, `Cmd*`/dimensões no `Draw`) — no degrau 3 vira
     contexto injetado via factory, como no FtxuiWindowManager.
3. **O 8Puzzle de verdade**: cenas The-Forge (splash/menu/jogo/recordes)
   desenhadas com a UI middleware (painéis/texto/widgets — sem shader
   próprio), domínio e `GameRouter` intocados. Recordes no mesmo
   `records.tsv`.

## Critérios de aceite

- [x] Exemplo oficial roda no ambiente atual (degrau 0) — validado com
      `10_ScreenSpaceReflections` em 2026-07-08.
- [x] `IApp` próprio compila e desenha (degrau 1) — validado em 2026-07-09:
      janela com título "HELLO FORGE" e subtítulo pulsando, buildado do repo
      do 8puzzle com output em `out/theforge/`.
- [x] Navegação de cenas da cengine funcionando dentro do `IApp` (degrau 2) —
      validado em 2026-07-09: A↔B via ENTER com recriação de cena, ESC
      encerra via estado `exit`; ciclo `onEnter`/`onExit` visível no log.
- [ ] 8Puzzle jogável de ponta a ponta no The-Forge, com domínio inalterado
      (degrau 3).
- [ ] Registro do aprendizado: o que o framework deu de graça e o que a
      fase 2 precisará substituir (alimenta a task 15 da cengine).

## Riscos

- ~~**C++23 no VS2019**~~ resolvido no degrau 2: a cengine compila limpa com
  `/std:c++latest` no 16.11 (fallback VS2022 não foi necessário).
- **Boilerplate de renderização** (FSL, ReloadDesc, resource loading): mitigado
  usando a UI middleware e o `01_Transformations` como template — sem shader
  próprio na PoC.
- **Build híbrido** (vcxproj com fontes CMake-first): fontes do domínio são
  C++ puro; listar no vcxproj é suficiente para a PoC. Integração de build
  definitiva é problema da fase 2.

## Fora do escopo

- Modo biblioteca (fase 2) — janela própria + `EngineManager` dono do loop.
- Shaders próprios / renderização 3D do tabuleiro.
- CMake para o The-Forge.
