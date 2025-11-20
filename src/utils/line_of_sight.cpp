/**
 * @file line_of_sight.cpp
 * @brief Реализация функций проверки прямой видимости
 */

#include "utils/line_of_sight.h"

#include <cmath>
#include <algorithm>

namespace line_of_sight {

bool hasLineOfSight(const Grid& grid, const Node& from, const Node& to) {
    return isPathClear(grid, from.x, from.y, to.x, to.y);
}

bool hasLineOfSightSupercover(const Grid& grid, const Node& from, const Node& to) {
    auto cells = getLineCells(from.x, from.y, to.x, to.y);
    
    for (const auto& [x, y] : cells) {
        if (!grid.isValidCoordinate(x, y) || grid.isObstacle(x, y)) {
            return false;
        }
    }
    return true;
}

std::vector<std::pair<int, int>> getLineCells(int x0, int y0, int x1, int y1) {
    std::vector<std::pair<int, int>> cells;
    
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int x = x0;
    int y = y0;
    int n = 1 + dx + dy;
    int x_inc = (x1 > x0) ? 1 : -1;
    int y_inc = (y1 > y0) ? 1 : -1;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;
    
    for (; n > 0; --n) {
        cells.emplace_back(x, y);
        
        if (error > 0) {
            x += x_inc;
            error -= dy;
        } else if (error < 0) {
            y += y_inc;
            error += dx;
        } else {
            // Диагональное движение - добавляем обе клетки для суперпокрытия
            x += x_inc;
            y += y_inc;
            error -= dy;
            error += dx;
            --n;
        }
    }
    
    return cells;
}

bool isPathClear(const Grid& grid, int x0, int y0, int x1, int y1) {
    // Используем алгоритм Брезенхема для проверки всех клеток на пути
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int x = x0;
    int y = y0;
    int n = 1 + dx + dy;
    int x_inc = (x1 > x0) ? 1 : -1;
    int y_inc = (y1 > y0) ? 1 : -1;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;
    
    for (; n > 0; --n) {
        // Пропускаем проверку начальной и конечной точек
        if (!(x == x0 && y == y0) && !(x == x1 && y == y1)) {
            if (!grid.isValidCoordinate(x, y) || grid.isObstacle(x, y)) {
                return false;
            }
        }
        
        if (error > 0) {
            x += x_inc;
            error -= dy;
        } else {
            y += y_inc;
            error += dx;
        }
    }
    
    return true;
}

} // namespace line_of_sight