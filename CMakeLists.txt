cmake_minimum_required(VERSION 3.10)

project(TransportTask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ������� �������� � ���������� (������������� ��� MSVC � MSBuild)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/windows-1251>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/windows-1251>")

#####################
# �������� ����������
#####################

add_library(MenuWork SHARED
    ${PROJECT_SOURCE_DIR}/lib/MenuWork/MenuWork.cpp
)

target_include_directories(MenuWork
    PUBLIC
        ${PROJECT_SOURCE_DIR}/lib/MenuWork
)

set_target_properties(MenuWork PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

##########################
# �������� ����� ���������
##########################

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/transportTask.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        MenuWork
)