# 02 — The-Forge fase 2: cengine dona do loop (modo biblioteca)

- **Status:** todo
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

0. **Spike "renderer sem IApp"**: `main()` próprio — janela Win32 crua +
   `initMemAlloc/FileSystem/Log` + `initRenderer` + `addSwapChain(HWND)` +
   clear azul-noite + pump mínimo (fechar no X). Prova a tese central antes
   de tocar na cengine. Sem resize, sem fontes, sem cengine.
1. **Texto + input**: sistema de fontes (receita FSL da fase 1: rootsigs
   por-app + shaders do OS) e teclado via WndProc alimentando a fila do
   `ForgeUi` (a API `forgeui` não muda — cenas não percebem).
2. **cengine assume o loop**: `TheForgeWindowManager : IWindowManager`
   (janela + pump + boilerplate GPU em `update()`/`present()`) +
   `EngineManager::start()` com cena de teste. Consome a task 16 da cengine.
   Resize básico aqui (recriar swapchain no `WM_SIZE`).
3. **O jogo**: 8Puzzle completo — as cenas de `src/platform/theforge/src/
   8PuzzleForge/scene/` são reaproveitadas (falam só com `forgeui`); muda o
   casco: sai o `IApp`, entra `main()` + `EngineManager` + window manager
   (fiação idêntica ao `main_ftxui.cpp`).

## Critérios de aceite

- [ ] Degrau 0: janela própria com clear via The-Forge, sem nenhum símbolo
      de `IApp`/`WindowsBase` linkado na inicialização.
- [ ] Degrau 1: texto renderizado + teclado próprio na fila do `ForgeUi`.
- [ ] Degrau 2: `EngineManager::start()` dirigindo (fixed timestep incluso),
      navegação entre cenas de teste, resize sem crash.
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
