Cmake_minimum_required(VERSION 3.27,0rc3)
project(ARVORE VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(ARVORE ../Src/Main.c ../Lib/Funcoes.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)