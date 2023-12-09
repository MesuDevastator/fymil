cmake_minimum_required(VERSION 3.27)
project(fymil)

set(CMAKE_CXX_STANDARD 23)

find_package(log4cxx REQUIRED)
find_package(XercesC REQUIRED)

add_executable(fymil main.cpp fymil/program.hpp fymil/elements/print.hpp fymil/elements/command.hpp fymil/utilities/string_conversions.hpp fymil/element_manager.hpp fymil/context.hpp fymil/elements/element.hpp fymil/elements/string.hpp fymil/elements/parser.hpp fymil/types/type.hpp fymil/types/string.hpp fymil/types/undefined.hpp fymil/types/boolean.hpp fymil/utilities/transcode.hpp)
target_link_libraries(fymil PUBLIC log4cxx ${XercesC_LIBRARIES})
