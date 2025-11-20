/**
 * @file open_space.h
 * @brief Сценарий "Открытое пространство"
 */

#ifndef OPEN_SPACE_H
#define OPEN_SPACE_H

#include "grid/grid.h"

namespace scenarios {

/**
 * @brief Создать сценарий "Открытое пространство"
 * @param grid Сетка для инициализации
 * @param start_x Начальная координата X (выходной параметр)
 * @param start_y Начальная координата Y (выходной параметр)
 * @param end_x Конечная координата X (выходной параметр)
 * @param end_y Конечная координата Y (выходной параметр)
 */
void createOpenSpace(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y);

} // namespace scenarios

#endif // OPEN_SPACE_H