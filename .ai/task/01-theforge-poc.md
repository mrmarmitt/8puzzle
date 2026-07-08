# 01 — PoC The-Forge: o 8Puzzle numa plataforma gráfica

- **Status:** in-progress (degrau 0 ✅ em 2026-07-08)
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
1. **Hello Forge**: vcxproj novo na solution de exemplos com um `IApp` mínimo
   desenhando texto via UI middleware do The-Forge (sem 8puzzle). Valida o
   boilerplate (Init/Load/Draw, FSL, fontes).
2. **O adaptador**: o `IApp` hospeda o `GameManager` da cengine (mapeamento
   acima) com uma cena de teste trivial. Fontes da cengine (core + routing) e
   do domínio compilados direto no vcxproj (C++ puro, sem dependência de
   plataforma). ⚠️ risco C++23 no VS2019 — ver Riscos.
3. **O 8Puzzle de verdade**: cenas The-Forge (splash/menu/jogo/recordes)
   desenhadas com a UI middleware (painéis/texto/widgets — sem shader
   próprio), domínio e `GameRouter` intocados. Recordes no mesmo
   `records.tsv`.

## Critérios de aceite

- [x] Exemplo oficial roda no ambiente atual (degrau 0) — validado com
      `10_ScreenSpaceReflections` em 2026-07-08.
- [ ] `IApp` próprio compila e desenha na solution do The-Forge (degrau 1).
- [ ] Navegação de cenas da cengine funcionando dentro do `IApp` (degrau 2).
- [ ] 8Puzzle jogável de ponta a ponta no The-Forge, com domínio inalterado
      (degrau 3).
- [ ] Registro do aprendizado: o que o framework deu de graça e o que a
      fase 2 precisará substituir (alimenta a task 15 da cengine).

## Riscos

- **C++23 no VS2019** (16.11 tem suporte parcial via `/std:c++latest`): a
  cengine exige `cxx_std_23`. Se não compilar, instalar **VS2022 Community**
  (abre as solutions PC_VS2019; o CI da cengine já builda com ele). Testar
  cedo, no degrau 2.
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
