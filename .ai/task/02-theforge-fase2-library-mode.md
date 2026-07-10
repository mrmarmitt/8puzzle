# 02 — The-Forge fase 2: cengine dona do loop (modo biblioteca)

- **Status:** ✅ done (2026-07-10) — os 4 degraus validados em runtime e o
  registro comparativo abaixo. A visão original do projeto está concluída:
  a cengine gerencia o loop e o The-Forge é só o motor gráfico.
- **Prioridade:** exploratória (aprendizado de plataforma/GPU)
- **Categoria:** Plataforma
- **Depende de:** task 01 ✅ (fase 1 — modo hospedado); task 16 da cengine
  (`IWindowManager::present()`) para o degrau 2 em diante.
- **Relacionada:** task 15 da cengine ✅ (o modo hospedado continua sendo uma
  alternativa válida — esta task NÃO o substitui).

## Visão

Concluir a visão original do projeto: **a cengine gerencia o loop e o
The-Forge é só o motor gráfico**. Na fase 1 o The-Forge (framework `IApp`) é
dono do loop e chama `EngineManager::frame(dt)`; nesta fase invertemos —
`EngineManager::start()` dirige, e o The-Forge entra como biblioteca
(`IGraphics.h`: `initRenderer`, `addSwapChain(HWND)`, `queuePresent`...), sem
`IApp`, sem `DEFINE_APPLICATION_MAIN`, sem `WindowsMain`.

## O que o WindowsMain dá de graça (mapeado em 2026-07-09)

Inventário de `WindowsBase.cpp` (v1.63) — o que a fase 2 assume:

| Responsabilidade | Custo de substituir |
|---|---|
| `initMemAlloc`/`initFileSystem`/`initLog` | Baixo — funções públicas, basta chamar na ordem |
| Janela (`openWindow`), monitores, DPI | Médio — janela Win32 própria (`CreateWindowEx` + WndProc) |
| Message pump (`handleMessages`) + input system | Médio — pump próprio; `WM_KEYDOWN` alimenta a fila do `ForgeUi` (substitui os custom bindings) |
| Resize → `Unload`/`Load` (recriar swapchain) | **Alto** — a parte mais delicada |
| Device lost → reinit completo | **Alto** — fora do escopo da PoC (log + sair) |
| Minimizado → `threadSleep(1)` | Baixo |

Nota: as funções de plataforma (`openWindow`, `handleMessages`,
`platformUpdateInput`...) existem no OS.lib como `extern`, mas orbitam
globals amarrados ao `IApp` (`pApp`, `pWindowAppRef`, `gReloadDescriptor`
estático). Reutilizá-las sem `IApp` é frágil — janela própria é o caminho.

## Lacuna de API na cengine (→ task 16)

O ciclo do `run()` é `window.update()` → fases → fim do quadro. GPU exige
trabalho DEPOIS do `render()` (endCmd, submit, present) e não existe esse
gancho. A task 16 da cengine adiciona `IWindowManager::present()` chamado ao
fim do quadro (candidata a 0.5.0, breaking pequeno nas plataformas). Modelo
do quadro na fase 2:

```
window.update()  -> pump de mensagens, acquire da imagem, beginCmd
fases do jogo    -> render() desenha via forgeui (mesmo contrato de hoje)
window.present() -> endCmd, submit, queuePresent          (task 16)
```

## Degraus

0. **Spike "renderer sem IApp"** ✅ (2026-07-09): `ForgeLibSpike.vcxproj` —
   `main()` próprio com janela Win32 crua (tamanho fixo, WndProc próprio,
   pump por `PeekMessage`), `initMemAlloc → initFileSystem → initLog` na
   ordem do `WindowsMain`, `initGPUConfiguration`/`initRenderer` +
   `addSwapChain(HWND)` na nossa janela, clear azul-noite, ESC/X fecham
   limpo. Validado em runtime. Aprendizados:
   - o link nem puxa o `WindowsBase.obj` — nenhum símbolo do framework é
     necessário para o renderer como biblioteca (tese central confirmada);
   - o `DEFINE_APPLICATION_MAIN` era quem exportava
     `D3D12SDKVersion`/`D3D12SDKPath` (Agility SDK) — sem `IApp`, o exe
     exporta por conta própria (`D3D12_AGILITY_SDK_VERSION` vem do
     `TF_Shared.props`);
   - sem resource loader o submit espera só o semáforo do acquire (não há
     `flushResourceUpdates`);
   - `PathStatement.txt`/`gpu.cfg`/`gpu.data` seguem necessários (filesystem
     e GPU config são subsistemas independentes do framework).
1. **Texto + input** ✅ (2026-07-10): sistema de fontes (receita FSL da fase
   1: rootsigs por-app + shaders do OS) e teclado via WndProc alimentando a
   fila do `ForgeUi` (a API `forgeui` não muda — cenas não percebem).
   Validado em runtime (texto TitilliumText, digitação via `WM_CHAR`,
   setas/enter/backspace via `WM_KEYDOWN`). Aprendizados:
   - **dependência oculta confirmada** (risco previsto): o
     `platformInitFontSystem()` dimensiona o contexto fontstash pelo DPI do
     monitor e para isso dereferencia o global `gWindow` e a lista de
     monitores do window system (`getActiveMonitorIdx()` /
     `getMonitorDpiScale()`, `WindowsWindow.cpp`);
   - a cola mínima replica o que o `WindowsBase` faz: chamar
     `initWindowSystem()` (autocontido — preenche os monitores via
     `collectMonitorInfo()`) e apontar `gWindow` (extern) para um
     `WindowDesc` próprio com o nosso HWND, ANTES do
     `platformInitFontSystem()`. São 5 símbolos `extern` do OS.lib, todos
     documentados no spike;
   - a cadeia completa de fontes em modo biblioteca: resource loader →
     rootsigs (`INIT_RS_DESC`) → `fntDefineFonts` → `initFontSystem` →
     swapchain → `loadFontSystem(RELOAD_TYPE_ALL)` →
     `waitForAllResourceLoads`; com resource loader ativo, o submit passa a
     esperar TAMBÉM o semáforo do `flushResourceUpdates` (além do acquire);
   - o `ForgeUi` ganhou `pushKey(KeyEvent)` como entrada do modo biblioteca:
     o WndProc enfileira na MESMA fila que o `beginInput()` (modo hospedado)
     alimenta — o `ForgeUi.cpp` é compilado nos dois cascos e as cenas do
     degrau 3 não vão perceber a troca.
2. **cengine assume o loop** ✅ (2026-07-10): `TheForgeWindowManager :
   IWindowManager` (janela + pump + boilerplate GPU em
   `update()`/`present()`) + `EngineManager::start()` com duas cenas de
   teste navegáveis. Consome a cengine 0.5.0 (task 16) — primeira validação
   real do gancho `present()`. Resize recriando o swapchain no `WM_SIZE`.
   Validado em runtime (fixed timestep a 60 passos/s, navegação A↔B,
   resize/maximizar/minimizar sem crash, ESC e X encerram limpo).
   Aprendizados:
   - o par `update()`/`present()` da 0.5.0 mapeia 1:1 no quadro de GPU:
     `update()` = pump + resize pendente + acquire + beginCmd + barriers +
     `forgeui::beginDraw`; `present()` = endCmd + submit + queuePresent — o
     command buffer fica ABERTO entre os dois e as cenas desenham no meio,
     sem conhecer o window manager;
   - fechar a janela vira INPUT: o `WM_CLOSE` (X) entra na fila do forgeui
     como Escape e quem decide sair é a cena/router — a janela só é
     destruída no `cleanup()`, depois do loop terminar (o último quadro é
     apresentado, confirmando a decisão da task 16);
   - resize: `WM_SIZE` só grava pending (nunca 0x0 do minimizar); o
     `update()` aplica com `waitQueueIdle` →
     `unloadFontSystem(RELOAD_TYPE_RESIZE)` → remove/addSwapChain →
     `loadFontSystem(RELOAD_TYPE_RESIZE)` — o protocolo RELOAD dos apps
     IApp, sem framework. Limitação conhecida: durante o arrasto o modal
     loop do Win32 segura o pump (imagem congela; ajusta ao soltar);
   - a cengine roda inteira no MSVC do projeto (exceções ligadas só neste
     vcxproj — `/EH` — porque `EngineManager`/`SceneRepository` usam throw;
     RTTI não foi necessário: o spike não usa o `GameRouter` do domínio);
   - a fiação do `main()` ficou idêntica à do `main_ftxui.cpp` — a tese da
     fase 2 (trocar plataforma sem tocar domínio/engine) se sustenta.
3. **O jogo** ✅ (2026-07-10): 8Puzzle completo — `8PuzzleForgeLib` — as
   cenas de `src/platform/theforge/src/8PuzzleForge/scene/` reaproveitadas
   com **diff zero** (falam só com `forgeui`); mudou só o casco: sai o
   `IApp`, entra `main()` + `EngineManager` + `TheForgeWindowManager`
   (fiação idêntica ao `main_ftxui.cpp`). Validado em runtime: fluxo
   completo (splash → menu → jogo → recordes/game over), `records.tsv` no
   mesmo formato das outras plataformas, resize durante o jogo, saída pelo
   menu e pelo X. Aprendizados:
   - a tese central da PoC se confirmou de ponta a ponta: as 7 cenas
     compilam idênticas nos DOIS cascos (hospedado e biblioteca) — a ponte
     `forgeui` isolou a plataforma por completo; a única diferença é quem
     alimenta a fila de teclado (WndProc próprio × custom bindings);
   - o casco novo custou 1 arquivo de main (~100 linhas, união da fiação do
     `main_ftxui.cpp` com os subsistemas de processo do WindowsMain) + 1
     vcxproj (união mecânica das receitas do spike e da fase 1: RTTI +
     `/EH` + supressões do domínio);
   - o `TheForgeWindowManager` do degrau 2 foi reaproveitado como está
     (compilado de `ForgeLibSpike/` — mesmo padrão do `ForgeUi`
     compartilhado); o middleware de UI da fase 1 (`initUserInterface`/
     `uiIsFocused`) não fez falta: o jogo é 100% texto via fontstash.

## Critérios de aceite

- [x] Degrau 0: janela própria com clear via The-Forge, sem nenhum símbolo
      de `IApp`/`WindowsBase` linkado na inicialização — validado em
      2026-07-09 (janela estável, encerramento limpo por ESC/X).
- [x] Degrau 1: texto renderizado + teclado próprio na fila do `ForgeUi` —
      validado em 2026-07-10 (demo de digitação + contador de setas, ESC/X
      encerram limpo).
- [x] Degrau 2: `EngineManager::start()` dirigindo (fixed timestep incluso),
      navegação entre cenas de teste, resize sem crash — validado em
      2026-07-10 (relógio simulado a 60 passos/s, A↔B, resize/maximizar/
      minimizar, ESC e X saindo limpo).
- [x] Degrau 3: 8Puzzle jogável de ponta a ponta, cenas da fase 1
      reaproveitadas sem mudança de contrato (diff zero), `records.tsv`
      funcionando — validado em 2026-07-10.
- [x] Registro comparativo: fase 1 (hospedado) × fase 2 (biblioteca) — custo
      real da cola, recomendação de qual modo usar por padrão (seção
      abaixo, 2026-07-10).

## Registro comparativo: fase 1 (hospedado) × fase 2 (biblioteca)

O que ficou IGUAL nos dois modos (e é a conclusão mais importante da PoC):
domínio, serviços, máquina de estados, as 7 cenas (`forgeui` isolou a
plataforma com diff zero), a receita de build do The-Forge (FSL + shaders do
OS + DLLs) e o fixed timestep (dentro do `EngineManager` desde a 0.4.0).

| | Fase 1 — hospedado (`8PuzzleForge`) | Fase 2 — biblioteca (`8PuzzleForgeLib`) |
|---|---|---|
| Dono do loop | `IApp`/WindowsMain; cengine via `frame(dt)` | cengine (`start()` + `update()`/`present()` da 0.5.0) |
| Cola fora de API pública | **zero** extern | **5 externs** do OS.lib (window system + fontstash) |
| Código de casco | ~330 linhas (IApp com GPU no Draw) | ~100 (main) + ~450 (TheForgeWindowManager, reutilizável) |
| Janela/input/resize | do framework (de graça, incl. fullscreen) | próprios (WndProc → fila do forgeui; resize recria swapchain; modal loop congela no arrasto) |
| Device lost / GPU switch | tratado pelo WindowsMain | fora do escopo (log + sair) |
| UI middleware (imgui) | disponível (`initUserInterface`) | não integrado (exigiria mais cola) |
| Risco em upgrade do The-Forge | baixo (APIs públicas) | médio (externs podem sumir/mudar — refazer a cola) |
| Fidelidade à arquitetura | cengine é convidada do framework | The-Forge é detalhe atrás do port `IWindowManager` |

**Recomendação — modo padrão: fase 2 (biblioteca).** É a que cumpre a visão
do projeto (cengine no comando, plataforma intercambiável — o `main()` é
idêntico ao do FTXUI) e o custo real da cola ficou baixo e localizado (5
externs documentados num único .cpp). O modo hospedado continua sendo a
escolha certa quando o jogo precisar do que o framework dá de graça —
fullscreen/device lost robustos ou UI middleware (imgui) — ou para minimizar
manutenção sob upgrades frequentes do The-Forge. Os dois cascos coexistem no
repo e compilam as mesmas cenas, então a decisão é por projeto, não
definitiva.

## Riscos

- **Manutenção da cola**: cada upgrade do The-Forge pode quebrar janela/
  resize/input próprios — o registro da task 01 já avisa; esta PoC mede o
  custo real.
- **Dependências ocultas do renderer**: `initRenderer`/fontes podem assumir
  estado que só o `WindowsBase` monta (ex.: `platformInitFontSystem` roda
  antes do `Init()` das apps). O degrau 0/1 existe para expor isso cedo.
- **Resize/fullscreen**: recriar swapchain corretamente (waitQueueIdle +
  remove + add) — escopo do degrau 2, sem fullscreen exclusivo na PoC.

## Fora do escopo

- Device lost / troca de GPU (log + encerrar).
- Fullscreen, multi-monitor, DPI dinâmico.
- UI middleware (imgui) — as cenas usam só texto via `forgeui`.
- Substituir a fase 1: o `8PuzzleForge` (modo hospedado) continua existindo;
  ao final decidimos qual é o recomendado.
