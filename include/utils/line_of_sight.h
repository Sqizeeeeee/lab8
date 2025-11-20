/**
 * @file line_of_sight.h
 * @brief Функции проверки прямой видимости между узлами
 * 
 * Содержит алгоритмы для определения, есть ли прямая видимость
 * между двумя точками на сетке без пересечения препятствий
 */

#ifndef LINE_OF_SIGHT_H
#define LINE_OF_SIGHT_H

#include "grid/grid.h"
#include "../../config.h"

#include <vector>
#include <cmath>

/**
 * @namespace line_of_sight
 * @brief Пространство имен для функций проверки прямой видимости
 */
namespace line_of_sight {

/**
 * @brief Проверяет прямую видимость между двумя узлами используя алгоритм Брезенхема
 * @param grid Ссылка на сетку
 * @param from Начальный узел
 * @param to Конечный узел
 * @return true если есть прямая видимость, false если есть препятствия
 */
bool hasLineOfSight(const Grid& grid, const Node& from, const Node& to);

/**
 * @brief Проверяет прямую видимость между двумя узлами используя алгоритм с суперпокрытием
 * @param grid Ссылка на сетку
 * @param from Начальный узел
 * @param to Конечный узел
 * @return true если есть прямая видимость, false если есть препятствия
 */
bool hasLineOfSightSupercover(const Grid& grid, const Node& from, const Node& to);

/**
 * @brief Получает все клетки, через которые проходит линия между двумя узлами
 * @param from Начальный узел
 * @param to Конечный узел
 * @return Вектор пар координат (x,y) всех пересекаемых клеток
 */
std::vector<std::pair<int, int>> getLineCells(int x0, int y0, int x1, int y1);

/**
 * @brief Проверяет, пересекает ли отрезок между двумя точками какие-либо препятствия
 * @param grid Ссылка на сетку
 * @param x0 Начальная координата X
 * @param y0 Начальная координата Y
 * @param x1 Конечная координата X
 * @param y1 Конечная координата Y
 * @return true если отрезок не пересекает препятствия, false в противном случае
 */
bool isPathClear(const Grid& grid, int x0, int y0, int x1, int y1);

} // namespace line_of_sight

#endif // LINE_OF_SIGHT_H