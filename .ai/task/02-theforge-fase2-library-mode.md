# 02 — The-Forge fase 2: cengine dona do loop (modo biblioteca)

- **Status:** in-progress (degraus 0, 1 e 2 ✅; falta o degrau 3 — o jogo —
  e o registro comparativo final)
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
3. **O jogo**: 8Puzzle completo — as cenas de `src/platform/theforge/src/
   8PuzzleForge/scene/` são reaproveitadas (falam só com `forgeui`); muda o
   casco: sai o `IApp`, entra `main()` + `EngineManager` + window manager
   (fiação idêntica ao `main_ftxui.cpp`).

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
- [ ] Degrau 3: 8Puzzle jogável de ponta a ponta, cenas da fase 1
      reaproveitadas sem mudança de contrato, `records.tsv` funcionando.
- [ ] Registro comparativo: fase 1 (hospedado) × fase 2 (biblioteca) — custo
      real da cola, recomendação de qual modo usar por padrão.

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
