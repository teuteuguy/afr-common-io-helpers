set(module_name "common-io-helpers")
message("Module: ${module_name}")

add_library(${module_name})

target_sources(
    ${module_name}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/common_io_helpers.c"
)

target_include_directories(
    ${module_name}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(
    ${module_name} 
    PRIVATE
        AFR::common
        AFR::common_io        
)
