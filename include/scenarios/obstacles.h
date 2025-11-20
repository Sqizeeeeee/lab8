/**
 * @file obstacles.h
 * @brief Сценарий "Множество препятствий"
 */

#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "grid/grid.h"

namespace scenarios {

/**
 * @brief Создать сценарий "Множество препятствий"
 * @param grid Сетка для инициализации
 * @param start_x Начальная координата X (выходной параметр)
 * @param start_y Начальная координата Y (выходной параметр)
 * @param end_x Конечная координата X (выходной параметр)
 * @param end_y Конечная координата Y (выходной параметр)
 * @param obstacle_density Плотность препятствий в процентах
 */
void createManyObstacles(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y,
                        int obstacle_density = 30);

/**
 * @brief Создать случайные препятствия на сетке
 * @param grid Сетка для добавления препятствий
 * @param density Плотность препятствий в процентах (0-100)
 * @param seed Seed для генератора случайных чисел
 */
void createRandomObstacles(Grid& grid, int density, unsigned int seed = 42);


/**
 * @brief Проверить доступность пути между двумя точками
 */
bool isPathPossible(const Grid& grid, int start_x, int start_y, int end_x, int end_y);

} // namespace scenarios

#endif // OBSTACLES_H