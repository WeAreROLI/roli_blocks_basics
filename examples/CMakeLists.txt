# To build examples, configure with `-D PATH_TO_JUCE=/path/to/juce`

cmake_minimum_required(VERSION 3.15)

project(ROLI_BLOCKS_BASICS VERSION 6.0.1)

add_subdirectory("${PATH_TO_JUCE}" juce)

juce_add_module("${CMAKE_CURRENT_SOURCE_DIR}/../../roli_blocks_basics")

juce_add_pip(BlocksDrawingDemo.h)
juce_add_pip(BlocksMonitorDemo.h)
juce_add_pip(BlocksSynthDemo.h)
