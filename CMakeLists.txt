cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



add_subdirectory(externals/shthreads)
add_subdirectory(externals/shvulkan)


#||||       ||||
#||||PROJECT||||
#||||       ||||
project(raining-threads C)



#||||       ||||
#||||LIBRARY||||
#||||       ||||
function(build_raining_threads)
add_library(raining-threads raining-threads/src/rainingThreads.c)
target_include_directories(raining-threads PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/raining-threads/include
)
target_link_libraries(raining-threads PUBLIC shthreads shvulkan)
set_target_properties(raining-threads PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



#||||        ||||
#||||EXAMPLES||||
#||||        ||||
function(build_raining_threads_examples)
add_executable(raining-threads-cpu-example examples/src/cpu-threads.c)
add_executable(raining-threads-gpu-compute examples/src/gpu-compute.c)

target_link_libraries(raining-threads-cpu-example PUBLIC raining-threads)
target_link_libraries(raining-threads-gpu-compute PUBLIC raining-threads)

set_target_properties(raining-threads-cpu-example raining-threads-gpu-compute PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()

#||||           ||||
#||||OS SPECIFIC||||
#||||           ||||
if (WIN32)
else()
endif(WIN32)
if(UNIX)
else()
endif(UNIX)

#||||     ||||
#||||FLAGS||||
#||||     ||||
option(RAINING_THREADS_BUILD_EXAMPLES CACHE OFF)



#||||    ||||
#||||MAIN||||
#||||    ||||
function(main)
build_raining_threads()
if (RAINING_THREADS_BUILD_EXAMPLES)
build_raining_threads_examples()
endif(RAINING_THREADS_BUILD_EXAMPLES)
endfunction()



#||||          ||||
#||||CMAKE MAIN||||
#||||          ||||
main()