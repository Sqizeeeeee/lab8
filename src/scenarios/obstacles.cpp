/**
 * @file obstacles.cpp
 * @brief Реализация сценария "Множество препятствий"
 */

#include "scenarios/obstacles.h"
#include "config.h"

#include <random>

namespace scenarios {

void createManyObstacles(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y,
                        int obstacle_density) {
    // Очищаем сетку
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            grid.clearObstacle(x, y);
        }
    }
    
    // Создаем случайные препятствия
    createRandomObstacles(grid, obstacle_density);
    
    // Устанавливаем старт и финиш в безопасных местах
    start_x = 2;
    start_y = 2;
    end_x = grid.getWidth() - 3;
    end_y = grid.getHeight() - 3;
    
    // Гарантируем, что старт и финиш доступны
    grid.clearObstacle(start_x, start_y);
    grid.clearObstacle(end_x, end_y);
    
    // Убеждаемся, что есть путь (если нет - перегенерируем)
    int attempts = 0;
    while (attempts < 10) {
        if (isPathPossible(grid, start_x, start_y, end_x, end_y)) {
            break;
        }
        // Перегенерируем препятствия с другим seed
        createRandomObstacles(grid, obstacle_density, 42 + attempts);
        attempts++;
    }
}

void createRandomObstacles(Grid& grid, int density, unsigned int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 99);
    
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            // Пропускаем границы, чтобы был проход
            if (x == 0 || x == grid.getWidth() - 1 || y == 0 || y == grid.getHeight() - 1) {
                continue;
            }
            
            if (dist(rng) < density) {
                grid.setObstacle(x, y);
            }
        }
    }
}

} // namespace scenarios