# CMakeList.txt: projeto de CMake para 8Puzzle, incluir origem e definir
# lógica específica a um projeto aqui.
#
# A cengine (via FetchContent) exige CMake >= 3.23; alinhamos o mínimo aqui.
cmake_minimum_required(VERSION 3.23)



project (8Puzzle)

# A cengine 0.5.0 exige C++23 e propaga o padrão via target_compile_features
# PUBLIC; alinhamos o projeto ao mesmo padrão.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PUZZLE_BUILD_TESTS "Build the test suite" ON)
option(PUZZLE_BUILD_FTXUI "Build the FTXUI UI executable (8PuzzleFtxui)" ON)

# -----------------------------
# Dependência: cengine (game loop + módulo de routing)
# -----------------------------
include(FetchContent)

FetchContent_Declare(
  cengine
  GIT_REPOSITORY https://github.com/mrmarmitt/cengine.git
  GIT_TAG 0.5.0
)

# Não precisamos da suíte de testes da cengine; desligar antes do MakeAvailable
# evita o fetch do GoogleTest no configure (falha offline).
set(CENGINE_BUILD_TESTS OFF)
FetchContent_MakeAvailable(cengine)

# -----------------------------
# 8puzzle_lib — lógica do jogo + plataforma terminal (tudo menos os mains),
# linkável tanto pelos executáveis quanto pela suíte de testes.
# A plataforma FTXUI fica fora da lib: só o alvo 8PuzzleFtxui depende dela.
# -----------------------------
file(GLOB_RECURSE PROJECT_SOURCES
        "src/*.cpp" "src/*.h"
)
list(REMOVE_ITEM PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main_ftxui.cpp"
)
# ftxui tem alvo proprio (8PuzzleFtxui); theforge builda via MSBuild/VS
# (src/platform/theforge/PC_VS2019) contra a arvore do The-Forge.
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "/platform/(ftxui|theforge)/")

add_library(8puzzle_lib STATIC ${PROJECT_SOURCES})
target_include_directories(8puzzle_lib PUBLIC src)
target_link_libraries(8puzzle_lib PUBLIC
    cengine::core        # game loop + ports
    cengine::routing     # roteamento de cenas/estados
)

# -----------------------------
# Executável principal (terminal clássico)
# -----------------------------
add_executable(8Puzzle src/main.cpp)
target_link_libraries(8Puzzle PRIVATE 8puzzle_lib)

# -----------------------------
# Executável FTXUI (PoC de interface — mesma engine, plataforma nova)
# -----------------------------
if(PUZZLE_BUILD_FTXUI)
    FetchContent_Declare(
      ftxui
      GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
      GIT_TAG v6.1.9
    )
    FetchContent_MakeAvailable(ftxui)

    file(GLOB_RECURSE FTXUI_PLATFORM_SOURCES
            "src/platform/ftxui/*.cpp" "src/platform/ftxui/*.h"
    )
    add_executable(8PuzzleFtxui src/main_ftxui.cpp ${FTXUI_PLATFORM_SOURCES})
    target_link_libraries(8PuzzleFtxui PRIVATE
        8puzzle_lib
        ftxui::dom       # só o módulo de layout/render — o loop é da cengine
    )
endif()

# -----------------------------
# Testes (GoogleTest via CTest)
# -----------------------------
if(PUZZLE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
