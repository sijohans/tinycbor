cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Sanitizers)

include_directories(../src)


set(SOURCES
    ../src/cborerrorstrings.c
    ../src/cborencoder.c
    ../src/cborencoder_close_container_checked.c
    ../src/cborparser.c
    ../src/cborpretty.c
    ../src/cborparser_dup_string.c
    ../src/cborvalidation.c
)

add_library(tinycbor ${SOURCES})
add_library(main_entry main_entry.c)

if(USE_SANITIZERS)
    add_sanitizers(tinycbor)
    add_sanitizers(main_entry)
else(USE_SANITIZERS)
endif(USE_SANITIZERS)


macro(add_fuzz arg)
    add_executable(${arg} ${arg}.c)
    if(USE_SANITIZERS)
        add_sanitizers(${arg})
    else(USE_SANITIZERS)
    endif(USE_SANITIZERS)
    target_link_libraries(${arg} ${ARGN})

endmacro(add_fuzz)

add_fuzz(fuzz_tinycbor tinycbor main_entry)
add_fuzz(fuzz_validate tinycbor main_entry)
target_link_options(fuzz_validate PRIVATE -lm)


#add_custom_command(kalle ${PROJECT_NAME} POST_BUILD COMMAND afl-fuzz)