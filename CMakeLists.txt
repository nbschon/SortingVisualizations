cmake_minimum_required(VERSION 3.20)
project(SortingVisualization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "../CS124/SortingVisualization")

find_package(SDL2 REQUIRED)

find_package(wxWidgets REQUIRED core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

include_directories(SortingVisualization ${SDL2_INCLUDE_DIRS})

add_executable(SortingVisualization src/main.cpp src/Utils.cpp src/Utils.h src/List.cpp src/List.h src/ListElement.cpp src/ListElement.h)
target_link_libraries(SortingVisualization ${SDL2_LIBRARIES} ${wxWidgets_LIBRARIES})