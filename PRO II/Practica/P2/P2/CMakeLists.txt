#PLEASE, DO NOT CHANGE THIS FILE
cmake_minimum_required(VERSION 3.5)
project(P2 C)
set(CMAKE_C_STANDARD 99)

include(config.cmake)

set(CMAKE_VERBOSE_MAKEFILE true)
set(warnings -Wall -Wextra)


set(dependencies_participant participant_list.h participant_list.c)
add_executable(test_participant_list types.h test_participant_list.c ${dependencies_participant})
target_compile_options(test_participant_list PRIVATE ${warnings})


set(dependencies_jury jury_list.h jury_list.c)
add_executable(test_jury_list types.h test_jury_list.c ${dependencies_jury})
target_compile_options(test_jury_list PRIVATE ${warnings})


add_definitions(-DINPUT_FILE="${input}")
add_executable(main main.c types.h ${dependencies_jury} ${dependencies_participant})
target_compile_options(main PRIVATE ${warnings})


set_target_properties(main test_participant_list test_jury_list PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)


