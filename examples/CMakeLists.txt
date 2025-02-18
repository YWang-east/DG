file(GLOB_RECURSE DG_EXAMPLES *.cpp)

# generate executable for each example
foreach(example_file ${DG_EXAMPLES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(${example_name} ${example_file})
    target_link_libraries(${example_name} DG)
    target_compile_definitions(${example_name} PUBLIC EXAMPLE_NAME="${example_name}")
endforeach()