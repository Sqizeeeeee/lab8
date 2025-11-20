/**
 * @file maze.h
 * @brief Сценарий "Сложный лабиринт"
 */

#ifndef MAZE_H
#define MAZE_H

#include "grid/grid.h"

namespace scenarios {

/**
 * @brief Создать сценарий "Сложный лабиринт"
 * @param grid Сетка для инициализации
 * @param start_x Начальная координата X (выходной параметр)
 * @param start_y Начальная координата Y (выходной параметр)
 * @param end_x Конечная координата X (выходной параметр)
 * @param end_y Конечная координата Y (выходной параметр)
 */
void createComplexMaze(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y);

} // namespace scenarios

#endif // MAZE_H