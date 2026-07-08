# Configuração dos testes (mesmo padrão da suíte do cengine)

# -------------------------------------------------------------
# 1. GoogleTest via FetchContent
# -------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

# -------------------------------------------------------------
# 2. Executável de testes — linka a lib do jogo
# -------------------------------------------------------------
add_executable(8puzzle_tests
    "${CMAKE_CURRENT_SOURCE_DIR}/8puzzle/game/BoardTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/8puzzle/game/service/RecordServiceTest.cpp"
)

target_link_libraries(8puzzle_tests
  PRIVATE
  8puzzle_lib
  gtest_main
)

# -------------------------------------------------------------
# 3. Registrar no CTest
# -------------------------------------------------------------
include(GoogleTest)

gtest_discover_tests(8puzzle_tests)
