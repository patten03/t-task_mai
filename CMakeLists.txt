cmake_minimum_required(VERSION 3.10)

project(TransportTask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Решение проблемы с кодировкой (исключительно для MSVC и MSBuild)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/windows-1251>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/windows-1251>")

##########################
# Создание самой программы
##########################

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/transportTask.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)