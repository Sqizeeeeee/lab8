cmake_minimum_required(VERSION 3.16)
project(PathfindingBenchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройки компилятора
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
endif()

# Включаем только корневую директорию include
include_directories(include)
include_directories(.)

# Все исходные файлы
set(SOURCES
    main.cpp
    config.h
    src/grid/grid.cpp
    src/grid/obstacle_inflator.cpp
    src/algorithms/astar.cpp
    src/algorithms/astar_ps.cpp
    src/algorithms/thetastar.cpp
    src/utils/metrics_calculator.cpp
    src/utils/csv_writer.cpp
    src/utils/line_of_sight.cpp
    src/scenarios/test_scenarios.cpp
    src/scenarios/open_space.cpp
    src/scenarios/maze.cpp
    src/scenarios/obstacles.cpp
    src/scenarios/narrow_corridor.cpp
)

# Один исполняемый файл
add_executable(pathfinding_benchmark ${SOURCES})

# Создание директорий
add_custom_command(TARGET pathfinding_benchmark POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory results/csv
    COMMAND ${CMAKE_COMMAND} -E make_directory results/plots
    COMMAND ${CMAKE_COMMAND} -E make_directory test_scenarios
    COMMAND ${CMAKE_COMMAND} -E make_directory python_visualization
    COMMENT "Creating output directories"
)