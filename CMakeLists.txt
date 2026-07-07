# CMakeList.txt: projeto de CMake para 8Puzzle, incluir origem e definir
# lógica específica a um projeto aqui.
#
# A cengine (via FetchContent) exige CMake >= 3.23; alinhamos o mínimo aqui.
cmake_minimum_required(VERSION 3.23)



project (8Puzzle)

# A cengine 0.1.0 exige C++23 e propaga o padrão via target_compile_features
# PUBLIC; alinhamos o projeto ao mesmo padrão.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PUZZLE_BUILD_TESTS "Build the test suite" ON)

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

# -----------------------------
# 8puzzle_lib — lógica do jogo + plataforma (tudo menos o main),
# linkável tanto pelo executável quanto pela suíte de testes.
# -----------------------------
file(GLOB_RECURSE PROJECT_SOURCES
        "src/*.cpp" "src/*.h"
)
list(REMOVE_ITEM PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_library(8puzzle_lib STATIC ${PROJECT_SOURCES})
target_include_directories(8puzzle_lib PUBLIC src)
target_link_libraries(8puzzle_lib PUBLIC
    cengine::core        # game loop + ports
    cengine::routing     # roteamento de cenas/estados
)

# -----------------------------
# Executável principal
# -----------------------------
add_executable(8Puzzle src/main.cpp)
target_link_libraries(8Puzzle PRIVATE 8puzzle_lib)

# -----------------------------
# Testes (GoogleTest via CTest)
# -----------------------------
if(PUZZLE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
