# CMakeList.txt: projeto de CMake para 8Puzzle, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

include(FetchContent)

# Habilite a Recarga Dinâmica para compiladores MSVC, se houver suporte.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
  #TODO: Adicionar a lib libwinpthread-1.dll
endif()

project ("8Puzzle")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Inclui todos os diretórios de headers dentro de src/
include_directories(src)

# -----------------------------
# Código do projeto (Biblioteca)
# -----------------------------

# Coleta os arquivos fonte do projeto
file(GLOB_RECURSE PROJECT_SOURCES
        "src/8puzzle/*.cpp" "src/8puzzle/*.h"
        "src/platform/*.cpp" "src/platform/*.h"
        "src/platform/domain/*.cpp" "src/platform/domain/*.h"
        "src/platform/domain/state/*.cpp" "src/platform/domain/state/*.h"
)

# Cria uma biblioteca com o código principal
add_library(core_lib ${PROJECT_SOURCES})
target_include_directories(core_lib PUBLIC src)

# -------------------------
# FTXUI
# -------------------------
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# -------------------------
# GoogleTest
# -------------------------
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# -----------------------------
# Executável principal
# -----------------------------
add_executable(8Puzzle src/main.cpp)
target_link_libraries(8Puzzle
  PRIVATE
    core_lib
#    ftxui::component
#    ftxui::dom
#    ftxui::screen
)

# Ativa testes
enable_testing()

# Adiciona seus testes
file(GLOB_RECURSE TEST_SOURCES "test/*/*.cpp")
add_executable(RunTests ${TEST_SOURCES})
target_link_libraries(RunTests
  PRIVATE
    core_lib
    gtest
    gtest_main
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 8Puzzle PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.
