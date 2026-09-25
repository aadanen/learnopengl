cmake_minimum_required(VERSION 3.12)

project(learnopengl)

set(GLAD_DIR "lib/GLAD")
set(GLFW_DIR "lib/glfw-3.5.1")
set(IMGUI_DIR "lib/imgui")

add_executable(learnopengl)
add_subdirectory(${GLFW_DIR})
add_subdirectory(${IMGUI_DIR})

target_include_directories(learnopengl 
  PUBLIC
    include
  PUBLIC
    ${GLAD_DIR}/include
  PUBLIC
    ${GLFW_DIR}/include
  PUBLIC
    ${IMGUI_DIR}/
  PUBLIC
    ${IMGUI_DIR}/backends
)

target_sources(learnopengl
  PRIVATE
    src/main.cpp
    src/color.cpp
    ${GLAD_DIR}/src/glad.c
    ${IMGUI_SRC}
)

target_link_libraries(learnopengl PUBLIC glfw)



