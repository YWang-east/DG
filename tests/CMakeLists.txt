file(GLOB_RECURSE DG_TESTS *.cpp)

# generate executable for each tests
foreach(test_file ${DG_TESTS})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} PUBLIC DG)
endforeach()