cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



add_subdirectory(externals/shthreads)



#||||       ||||
#||||PROJECT||||
#||||       ||||
project(raining-jobs C)



#||||       ||||
#||||LIBRARY||||
#||||       ||||
function(build_raining_jobs)
add_library(raining-jobs raining-jobs/src/rainingJobs.c)
target_include_directories(raining-jobs PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/raining-jobs/include
)
target_link_libraries(raining-jobs PUBLIC shthreads)
set_target_properties(raining-jobs PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



#||||        ||||
#||||EXAMPLES||||
#||||        ||||
function(build_raining_jobs_examples)
add_executable(raining-example examples/src/raining-example.c)

target_link_libraries(raining-example PUBLIC raining-jobs)

set_target_properties(raining-example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()

#||||           ||||
#||||OS SPECIFIC||||
#||||           ||||
if (WIN32)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif(WIN32)

#||||     ||||
#||||FLAGS||||
#||||     ||||
option(RAINING_JOBS_BUILD_EXAMPLES CACHE OFF)



#||||    ||||
#||||MAIN||||
#||||    ||||
function(main)
build_raining_jobs()
if (RAINING_JOBS_BUILD_EXAMPLES)
build_raining_jobs_examples()
endif(RAINING_JOBS_BUILD_EXAMPLES)
endfunction()



#||||          ||||
#||||CMAKE MAIN||||
#||||          ||||
main()