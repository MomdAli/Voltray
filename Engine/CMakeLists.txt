file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(Voltray ${ENGINE_SRC})

target_link_libraries(Voltray PRIVATE glfw)

target_include_directories(Voltray PRIVATE
    ${CMAKE_SOURCE_DIR}/Vendor/glad/include
    ${glfw_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Engine
)

target_sources(Voltray PRIVATE
    ${CMAKE_SOURCE_DIR}/Vendor/glad/src/gl.c
)
