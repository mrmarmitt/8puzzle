# CMakeList.txt: projeto de CMake para 8Puzzle, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required(VERSION 3.16)



project (8Puzzle)

# A cengine 0.1.0 exige C++23 e propaga o padrão via target_compile_features
# PUBLIC; alinhamos o projeto ao mesmo padrão.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Inclui todos os diretórios de headers dentro de src/
include_directories(src)

# -----------------------------
# Dependência: cengine (game loop + módulo de routing)
# -----------------------------
include(FetchContent)

FetchContent_Declare(
  cengine
  GIT_REPOSITORY https://github.com/mrmarmitt/cengine.git
  GIT_TAG 0.1.0
)

# Não precisamos da suíte de testes da cengine; desligar antes do MakeAvailable
# evita o fetch do GoogleTest no configure (falha offline).
set(CENGINE_BUILD_TESTS OFF)
FetchContent_MakeAvailable(cengine)

# Coleta os arquivos fonte do projeto
file(GLOB_RECURSE PROJECT_SOURCES
        "src/*.cpp" "src/*.h"
)

# Cria o executável principal usando todos os arquivos fonte
add_executable(8Puzzle ${PROJECT_SOURCES})
target_link_libraries(8Puzzle PRIVATE
    cengine::core        # game loop + ports
    cengine::routing     # roteamento de cenas/estados
)
target_include_directories(8Puzzle PRIVATE src)
