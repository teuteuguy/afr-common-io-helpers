set(module_name "common-io-helpers")

afr_module()

message("Module: ${module_name}")

set(inc_dir "${CMAKE_CURRENT_LIST_DIR}/include")
set(src_dir "${CMAKE_CURRENT_LIST_DIR}/src")

afr_module_sources(
    ${module_name} INTERFACE
    "${src_dir}/common_io_helpers.c"
    "${src_dir}/common_io_helpers.h"
)

afr_module_include_dirs(
    ${module_name} PUBLIC
    "${inc_dir}"
)

afr_module_dependencies(
    ${module_name} INTERFACE
        AFR::common
        AFR::common_io
)

add_library(${module_name} INTERFACE)
add_library(sensor::${module_name} ALIAS ${module_name})

