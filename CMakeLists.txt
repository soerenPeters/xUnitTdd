cmake_minimum_required(VERSION 2.6)

project(xUnit)

add_library(xUnit STATIC src/TestSuite.h src/TestCase.h src/TestResult.cpp src/TestResult.h)

add_executable(xUnit.Test tests/main.cpp tests/TestCaseTest.h tests/TestCaseSpy.h)
target_link_libraries(xUnit.Test xUnit)

add_definitions("-std=c++11")