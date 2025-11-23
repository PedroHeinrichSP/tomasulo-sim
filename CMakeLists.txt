cmake_minimum_required(VERSION 3.16)
project(TomasuloSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SIMULATOR_SOURCES
    src/ui/simulator/common_data_bus.cpp
    src/ui/simulator/instruction.cpp
    src/ui/simulator/register_file.cpp
    src/ui/simulator/reorder_buffer.cpp
    src/ui/simulator/reservation_station.cpp
    src/ui/simulator/simulator_utils.cpp
    src/ui/simulator/tomasulo_core.cpp
)

add_executable(TomasuloSim
    src/main.cpp
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.ui
    src/ui/resources.qrc
    
    ${SIMULATOR_SOURCES}
)

target_include_directories(TomasuloSim PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/simulator"
)
target_link_libraries(TomasuloSim PRIVATE Qt6::Widgets)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/arithmetic.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/fp_ops.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/load_store_sequence.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/branch_example.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


add_executable(run_unit_tests
    tests/test_instruction.cpp
    tests/test_instruction_extra.cpp
    tests/test_register_file.cpp
    tests/test_reorder_buffer.cpp
    tests/test_common_data_bus.cpp
    tests/test_reservation_station.cpp
    tests/test_tomasulo_core.cpp
    tests/test_step_by_step.cpp
    tests/test_branch_logic.cpp
    ${SIMULATOR_SOURCES}
)

target_include_directories(run_unit_tests PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/simulator"
)

target_link_libraries(run_unit_tests PRIVATE
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_unit_tests)