/**
 * @file narrow_corridor.h
 * @brief Сценарий "Узкие коридоры"
 */

#ifndef NARROW_CORRIDOR_H
#define NARROW_CORRIDOR_H

#include "grid/grid.h"

namespace scenarios {

/**
 * @brief Создать сценарий "Узкие коридоры"
 * @param grid Сетка для инициализации
 * @param start_x Начальная координата X (выходной параметр)
 * @param start_y Начальная координата Y (выходной параметр)
 * @param end_x Конечная координата X (выходной параметр)
 * @param end_y Конечная координата Y (выходной параметр)
 * @param corridor_width Ширина коридора в клетках
 */
void createNarrowCorridors(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y,
                          int corridor_width = 3);

} // namespace scenarios

#endif // NARROW_CORRIDOR_H