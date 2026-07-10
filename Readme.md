# 8Puzzle

Um jogo de quebra-cabeça deslizante (sliding puzzle) 3x3 implementado em C++ com arquitetura hexagonal. O objetivo é organizar as peças numeradas de 1 a 8 em ordem, deixando o espaço vazio no canto inferior direito.

## 🧩 Sobre o Projeto

Este projeto utiliza o CMake como sistema de build e é dividido em múltiplos módulos, com uma separação clara entre domínio, infraestrutura e interface. O foco é manter a lógica de negócio desacoplada da plataforma.

O game loop é da engine [cengine](https://github.com/mrmarmitt/cengine) (consumida via FetchContent); o domínio do jogo (tabuleiro, partidas, recordes) é compartilhado por **três interfaces** (quatro executáveis), cada uma vivendo em `src/platform/`:

| Executável | Interface | Descrição |
|------------|-----------|-----------|
| `8Puzzle.exe` | Terminal clássico | Saída via `std::cout`, input bloqueante (`_getch`) — a versão original de estudo |
| `8PuzzleFtxui.exe` | [FTXUI](https://github.com/ArthurSonzogni/FTXUI) | Tabuleiro desenhado com bordas e cores, menu navegável, cronômetro ao vivo a ~60 FPS, input não-bloqueante |
| `8PuzzleForge.exe` | [The-Forge](https://github.com/ConfettiFX/The-Forge) | Janela gráfica de verdade (D3D12), **modo hospedado**: o framework (`IApp`) é dono do loop e chama `EngineManager::frame(dt)` — build separado, via MSBuild |
| `8PuzzleForgeLib.exe` | [The-Forge](https://github.com/ConfettiFX/The-Forge) | Mesma janela D3D12, **modo biblioteca**: a cengine é dona do loop (`EngineManager::start()`) e o The-Forge entra só como motor gráfico — build separado, via MSBuild |

### Funcionalidades:

- Arquitetura hexagonal — mesmo domínio, três plataformas de UI (e as
  **mesmas cenas The-Forge em dois cascos**: hospedado e biblioteca)
- Tabuleiro embaralhado a cada partida (sempre solucionável)
- Recordes (top 10 por tempo ou movimentos) **persistidos em `records.tsv`** (mesmo formato TSV em todas as interfaces; o arquivo é criado no diretório de onde o jogo roda)
- Suporte a testes com GoogleTest

---

## Interface FTXUI (8PuzzleFtxui)

A interface recomendada para jogar. Fluxo: splash → instruções → menu → jogo → fim de jogo/recorde → recordes.

**Controles:**

| Tela | Teclas |
|------|--------|
| Splash / Instruções | `ENTER` avança |
| Menu | `↑`/`↓` navegar, `ENTER` selecionar, `ESC` sair |
| Jogo | `↑`/`↓`/`←`/`→` desliza a peça vizinha para o vazio (a seta indica a direção em que a **peça** se move), ou `1`-`8` move a peça pelo número; `ESC` volta ao menu |
| Fim de jogo | digitação livre do nome (se recorde), `BACKSPACE` apaga, `ENTER` confirma |
| Recordes | `T` ordena por tempo, `M` por movimentos, `ESC` volta |

O alvo é controlado pela opção CMake `PUZZLE_BUILD_FTXUI` (default `ON`); o FTXUI (v6.1.9) é baixado via FetchContent no configure. A biblioteca é usada em **modo imediato** (só `ftxui::dom`): o game loop continua sendo o da cengine, e cada cena desenha um `Element` por quadro — o pacing de ~60 FPS é responsabilidade da plataforma (`FtxuiWindowManager`).

> Os recordes ficam em `records.tsv`, criado no diretório de onde o jogo é executado (uma linha por recorde, campos separados por TAB).

---

## Interface The-Forge (8PuzzleForge e 8PuzzleForgeLib)

O mesmo jogo numa **plataforma gráfica de verdade**: janela nativa com renderização D3D12 via [The-Forge](https://github.com/ConfettiFX/The-Forge) v1.63. Os controles são os mesmos da interface FTXUI (tabela acima).

São **dois executáveis com as mesmas cenas** (`src/platform/theforge/src/8PuzzleForge/scene/`, que falam com a plataforma só através do `ForgeUi` — fila de teclado + desenho de texto), diferindo apenas no casco, ou seja, em **quem é dono do game loop**:

- **`8PuzzleForge` — modo hospedado** (PoC fase 1, [`.ai/task/01-theforge-poc.md`](.ai/task/01-theforge-poc.md)): o The-Forge é dono do loop (framework `IApp` + `DEFINE_APPLICATION_MAIN`), e a cengine roda hospedada — o `Draw()` do app chama `EngineManager::frame(dt)` (cengine 0.4.0), que executa o quadro completo com fixed timestep. Janela, input e resize vêm de graça do framework.
- **`8PuzzleForgeLib` — modo biblioteca** (PoC fase 2, [`.ai/task/02-theforge-fase2-library-mode.md`](.ai/task/02-theforge-fase2-library-mode.md)): a **cengine é dona do loop** — o entry point segue o padrão dos irmãos (`src/main_theforge.cpp`, ao lado de `main.cpp` e `main_ftxui.cpp`, mesma fiação) e chama `EngineManager::start()`, e todo o boilerplate de janela/GPU (janela Win32 própria, WndProc alimentando a fila do `ForgeUi`, swapchain, resize) vive no `TheForgeWindowManager : IWindowManager`, sobre o par `update()`/`present()` da cengine 0.5.0. O The-Forge vira só o motor gráfico, sem `IApp`/WindowsMain. É o **modo recomendado** pela PoC (registro comparativo na task 02); o hospedado permanece como referência de quando o framework precisa estar no comando (fullscreen/device lost robustos, UI middleware).

Domínio, serviços e máquina de estados são os mesmos arquivos das outras plataformas, compilados sem alteração nos dois cascos.

### Pré-requisitos (além dos gerais)

Este alvo **não** faz parte do build CMake — é um `.vcxproj` (MSBuild), porque reutiliza a cadeia de build do The-Forge (props, FSL, libs). Ele espera o seguinte layout de workspace, com os repositórios **irmãos**:

```
<workspace>/
├── 8puzzle/      # este repo
├── cengine/      # checkout na mesma versão do FetchContent (tag 0.5.0)
└── The-Forge/    # v1.63, com assets baixados (PRE_BUILD.bat)
```

1. **Visual Studio 2019** (toolset v142) — as solutions do The-Forge são `PC_VS2019`.
2. **The-Forge buildado uma vez** na mesma Configuration: abrir `The-Forge/Examples_3/Unit_Tests/PC_VS2019/Unit_Tests.sln` e buildar (gera `OS.lib`/`Renderer.lib` e os shaders de fonte/UI que o jogo copia).
3. **cengine** clonada ao lado, no tag usado pelo 8puzzle (as fontes dela são compiladas direto no vcxproj).

### Build

```powershell
# modo hospedado (fase 1)
MSBuild src\platform\theforge\PC_VS2019\8PuzzleForge.vcxproj /p:Configuration=Release /p:Platform=x64

# modo biblioteca (fase 2 — recomendado)
MSBuild src\platform\theforge\PC_VS2019\8PuzzleForgeLib.vcxproj /p:Configuration=Release /p:Platform=x64
```

O executável e todos os artefatos (shaders compilados, DLLs de runtime, configs) saem em `out/theforge/x64/Release/<Projeto>/<Projeto>.exe`. O passo FSL compila as root signatures por-app; o `PathStatement.txt` próprio resolve fontes/texturas na árvore do The-Forge irmão.

> Também existem na mesma pasta os projetos `HelloForge.vcxproj` e `CengineAdapter.vcxproj` (degraus 1 e 2 da fase 1 — boilerplate mínimo e integração cengine↔IApp) e `ForgeLibSpike.vcxproj` (degraus 0-2 da fase 2 — renderer sem IApp, fontes/input próprios e a cengine assumindo o loop com cenas de teste). São mantidos como referência.

### Distribuição (máquinas sem os repos)

A pasta `out/` **não** é autossuficiente: o `PathStatement.txt` do build resolve a fonte na árvore do The-Forge irmão. O fluxo completo para distribuir é:

```powershell
# 1. buildar (se ainda não buildou)
MSBuild src\platform\theforge\PC_VS2019\8PuzzleForgeLib.vcxproj /p:Configuration=Release /p:Platform=x64

# 2. empacotar — gera dist\8PuzzleForgeLib\ e dist\8PuzzleForgeLib.zip (~22 MB)
powershell -File tools\dist-theforge.ps1 -Zip

# 3. enviar o zip; na máquina de destino: descompactar e rodar 8PuzzleForgeLib.exe
```

O script (`tools/dist-theforge.ps1`, parâmetros `-Project`/`-Configuration`/`-Zip`):

- copia só o necessário: exe, DLLs de runtime (Agility SDK, dxcompiler...), shaders compilados e configs — **sem** `pdb`/`lib`/`exp`, sem a camada de debug do D3D12 e sem o `records.tsv` do desenvolvedor;
- **embute a fonte** TitilliumText (com a licença OFL, exigida por ela) e reescreve o `PathStatement.txt` com mounts locais — era a única referência à árvore do The-Forge;
- gera um `LEIA-ME.txt` com requisitos e controles para quem recebe.

Requisitos na máquina de destino: Windows 10/11 x64, GPU com D3D12 e o [VC++ Redistributable x64](https://aka.ms/vs/17/release/vc_redist.x64.exe) (se faltar, o Windows acusa `VCRUNTIME140.dll não encontrado`).

> **Nota sobre GPUs** (aprendida em teste real): o `gpu.cfg` dos exemplos do The-Forge bane GPUs Intel — e no The-Forge as regras de seleção também valem como *requisito de suporte* da GPU escolhida, então um notebook em que a Intel integrada vence a seleção falhava com `GPU Unsupported`. Os `gpu.cfg` deste repo removem essa regra de propósito (o jogo é só texto; roda em qualquer D3D12, incluindo integradas). Ao atualizar o The-Forge, **não** recopie o `gpu.cfg` dos exemplos por cima — ver o comentário no topo dos nossos arquivos.
>
> Em caso de problema na máquina de destino, o diagnóstico está no `8PuzzleForgeLib.log` criado ao lado do exe.

---

## Como Gerar um Executável

Para gerar um executável do projeto 8Puzzle, você pode seguir os passos abaixo:

### Pré-requisitos

- Visual Studio 2019 ou posterior com o componente "Desenvolvimento para Desktop com C++" instalado
- CMake 3.23 ou superior (exigido pela dependência cengine)
- Ninja build system (instalado automaticamente com o Visual Studio)
- Acesso à internet no primeiro configure (a cengine e o GoogleTest são baixados via FetchContent)

### Usando CMake pela linha de comando

1. Configure o projeto usando um dos presets disponíveis:
   ```bash
   # Para build de debug (x64)
   cmake --preset=x64-debug

   # Para build de release (x64)
   cmake --preset=x64-release
   ```

2. Compile o projeto:
   ```bash
   # Para build de debug
   cmake --build out/build/x64-debug

   # Para build de release
   cmake --build out/build/x64-release
   ```

3. Os executáveis serão gerados em `out/build/x64-debug/` (ou `x64-release/`):
   - `8Puzzle.exe` — interface terminal clássica
   - `8PuzzleFtxui.exe` — interface FTXUI

   > O `8PuzzleForge.exe` e o `8PuzzleForgeLib.exe` (The-Forge) não são alvos CMake — ver a seção
   > [Interface The-Forge](#interface-the-forge-8puzzleforge-e-8puzzleforgelib) para o build via MSBuild.

> Os presets versionados usam o compilador MSVC (`cl.exe`). Para outro
> toolchain (ex.: MinGW/MSYS2), crie um preset de máquina em um
> `CMakeUserPresets.json` (git-ignorado), como no projeto cengine.

### Rodando os testes

A suíte de testes (GoogleTest) é buildada por padrão junto do projeto
(desligável com `-DPUZZLE_BUILD_TESTS=OFF`):

```bash
ctest --test-dir out/build/x64-debug --output-on-failure
```

### Usando o VS Code

1. Abra o projeto no VS Code
2. Pressione `Ctrl+Shift+P` e digite "CMake: Configure"
3. Selecione o preset desejado (x64-debug ou x64-release)
4. Pressione `Ctrl+Shift+P` novamente e digite "CMake: Build"
5. O executável será gerado no diretório correspondente ao preset selecionado

#### Sujestão de extensões
* https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools
* https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
* https://marketplace.visualstudio.com/items?itemName=danielpinto8zz6.c-cpp-compile-run

#### Sujestão de launch.json
```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Executable Debug",
            "type": "cppvsdbg",
            "request": "launch",
            "program": "${workspaceFolder}\\out\\build\\x64-debug\\8Puzzle.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": true
        },
        {
            "name": "Executable",
            "type": "cppvsdbg",
            "request": "launch",
            "program": "${workspaceFolder}\\out\\build\\x64-release\\8Puzzle.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": true
        }
    ]
}
```
### Executando o Jogo

Após a compilação, você pode executar o jogo de duas formas:

1. Diretamente pelo explorador de arquivos, navegando até o diretório de build e clicando no executável desejado — `8PuzzleFtxui.exe` (recomendado) ou `8Puzzle.exe` (terminal clássico)
2. Pelo VS Code, pressionando F5 ou usando o menu de depuração (selecione a configuração "Executable Debug" ou "Executable")