# src/resource_core/CMakeLists.txt

add_library(resource_core STATIC
        ResourceError.cpp
        ../include/ResourceError.hpp
        ../include/FileHandle.hpp
        ../include/ResourceManager.hpp
        ResourceManager.cpp
        FileHandle.cpp
)

target_link_libraries(resource_core PRIVATE project_warnings)

if (TARGET project_sanitizers)
        target_link_libraries(resource_core PRIVATE project_sanitizers)
endif ()

target_include_directories(resource_core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}   # ← текущая папка (где .h файл)
)