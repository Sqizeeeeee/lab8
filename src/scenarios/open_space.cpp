/**
 * @file open_space.cpp
 * @brief Реализация сценария "Открытое пространство"
 */

#include "scenarios/open_space.h"
#include "config.h"

namespace scenarios {

void createOpenSpace(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y) {
    // Очищаем сетку от всех препятствий
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            grid.clearObstacle(x, y);
        }
    }
    
    // Устанавливаем старт и финиш в противоположных углах
    start_x = 5;
    start_y = 5;
    end_x = grid.getWidth() - 6;
    end_y = grid.getHeight() - 6;
    
    // Гарантируем, что старт и финиш доступны
    grid.clearObstacle(start_x, start_y);
    grid.clearObstacle(end_x, end_y);
}

} // namespace scenarios