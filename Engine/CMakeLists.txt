set(ENGINE_SRC
    Entry.cpp
)

add_executable(Voltray ${ENGINE_SRC})

target_link_libraries(Voltray PRIVATE glfw)

target_include_directories(Voltray PRIVATE
    ${CMAKE_SOURCE_DIR}/Vendor/glad/include
    ${glfw_SOURCE_DIR}/include
)

target_sources(Voltray PRIVATE
    ${CMAKE_SOURCE_DIR}/Vendor/glad/src/gl.c
)
