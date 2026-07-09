# 8Puzzle

Um jogo de quebra-cabeça deslizante (sliding puzzle) 3x3 implementado em C++ com arquitetura hexagonal. O objetivo é organizar as peças numeradas de 1 a 8 em ordem, deixando o espaço vazio no canto inferior direito.

## 🧩 Sobre o Projeto

Este projeto utiliza o CMake como sistema de build e é dividido em múltiplos módulos, com uma separação clara entre domínio, infraestrutura e interface. O foco é manter a lógica de negócio desacoplada da plataforma.

O game loop é da engine [cengine](https://github.com/mrmarmitt/cengine) (consumida via FetchContent); o domínio do jogo (tabuleiro, partidas, recordes) é compartilhado por **três interfaces**, cada uma vivendo em `src/platform/`:

| Executável | Interface | Descrição |
|------------|-----------|-----------|
| `8Puzzle.exe` | Terminal clássico | Saída via `std::cout`, input bloqueante (`_getch`) — a versão original de estudo |
| `8PuzzleFtxui.exe` | [FTXUI](https://github.com/ArthurSonzogni/FTXUI) | Tabuleiro desenhado com bordas e cores, menu navegável, cronômetro ao vivo a ~60 FPS, input não-bloqueante |
| `8PuzzleForge.exe` | [The-Forge](https://github.com/ConfettiFX/The-Forge) | Janela gráfica de verdade (D3D12): texto renderizado na GPU, engine em modo hospedado (`frame(dt)`) — build separado, via MSBuild |

### Funcionalidades:

- Arquitetura hexagonal — mesmo domínio, três plataformas de UI
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

## Interface The-Forge (8PuzzleForge)

O mesmo jogo numa **plataforma gráfica de verdade**: janela nativa com renderização D3D12 via [The-Forge](https://github.com/ConfettiFX/The-Forge) v1.63. Os controles são os mesmos da interface FTXUI (tabela acima).

A arquitetura é o resultado da PoC documentada em [`.ai/task/01-theforge-poc.md`](.ai/task/01-theforge-poc.md): o The-Forge é dono do loop (framework `IApp`), e a cengine roda em **modo hospedado** — o `Draw()` do app chama `EngineManager::frame(dt)` (cengine 0.4.0), que executa o quadro completo com fixed timestep. Domínio, serviços e máquina de estados são os mesmos arquivos das outras plataformas, compilados sem alteração; a camada da plataforma vive em `src/platform/theforge/src/8PuzzleForge/` (`ForgeUi` = fila de teclado + desenho de texto; cenas em `scene/`).

### Pré-requisitos (além dos gerais)

Este alvo **não** faz parte do build CMake — é um `.vcxproj` (MSBuild), porque reutiliza a cadeia de build do The-Forge (props, FSL, libs). Ele espera o seguinte layout de workspace, com os repositórios **irmãos**:

```
<workspace>/
├── 8puzzle/      # este repo
├── cengine/      # checkout na mesma versão do FetchContent (tag 0.4.0)
└── The-Forge/    # v1.63, com assets baixados (PRE_BUILD.bat)
```

1. **Visual Studio 2019** (toolset v142) — as solutions do The-Forge são `PC_VS2019`.
2. **The-Forge buildado uma vez** na mesma Configuration: abrir `The-Forge/Examples_3/Unit_Tests/PC_VS2019/Unit_Tests.sln` e buildar (gera `OS.lib`/`Renderer.lib` e os shaders de fonte/UI que o jogo copia).
3. **cengine** clonada ao lado, no tag usado pelo 8puzzle (as fontes dela são compiladas direto no vcxproj).

### Build

```powershell
MSBuild src\platform\theforge\PC_VS2019\8PuzzleForge.vcxproj /p:Configuration=Release /p:Platform=x64
```

O executável e todos os artefatos (shaders compilados, DLLs de runtime, configs) saem em `out/theforge/x64/Release/8PuzzleForge/8PuzzleForge.exe`. O passo FSL compila as root signatures por-app; o `PathStatement.txt` próprio resolve fontes/texturas na árvore do The-Forge irmão.

> Também existem na mesma pasta os projetos `HelloForge.vcxproj` e `CengineAdapter.vcxproj` — degraus 1 e 2 da PoC, mantidos como referência mínima de boilerplate e de integração cengine↔IApp.

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

   > O `8PuzzleForge.exe` (The-Forge) não é um alvo CMake — ver a seção
   > [Interface The-Forge](#interface-the-forge-8puzzleforge) para o build via MSBuild.

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