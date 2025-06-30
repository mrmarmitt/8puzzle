# 8Puzzle

Um jogo de quebra-cabeça deslizante (sliding puzzle) 3x3 implementado em C++ com arquitetura hexagonal. O objetivo é organizar as peças numeradas de 1 a 8 em ordem, deixando o espaço vazio no canto inferior direito.

## 🧩 Sobre o Projeto

Este projeto utiliza o CMake como sistema de build e é dividido em múltiplos módulos, com uma separação clara entre domínio, infraestrutura e interface. O foco é manter a lógica de negócio desacoplada da plataforma.

### Funcionalidades:

- Arquitetura hexagonal
- Jogo 8 Puzzle funcional via terminal
- Suporte a testes com GoogleTest
- Preparado para interface interativa com FTXUI (descomentável)

---



    // m_terminalScreenCache = std::make_unique<TerminalScreenCache>();
    //não vamos mais trabalhar com as dependencias do Terminal, vamos tornar esse processo injetável.

    // m_recordService = std::make_unique<RecordService>();
    //
    // m_recordService->loadInitialRecords({
    //     Record("Mateus", 34, "2025-06-23 15:00", 91000),
    //     Record("Carine", 27, "2025-06-22 18:10", 87000),
    //     Record("Benicio", 16, "2025-06-22 18:10", 95000)
    // });