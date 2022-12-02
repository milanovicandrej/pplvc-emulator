cmake_minimum_required(VERSION 3.0.0)
project(PPLVC-Emulator VERSION 0.1.0)

include(CTest)
enable_testing()

set (source_dir "${PROJECT_SOURCE_DIR}/src/")


add_executable(PPLVC-Emulator "${source_dir}/pplvc-emulator.cpp")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
