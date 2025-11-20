/**
 * @file maze.cpp
 * @brief Реализация сценария "Сложный лабиринт"
 */

#include "scenarios/maze.h"
#include "config.h"

#include <vector>
#include <random>
#include <algorithm>

namespace scenarios {

void createComplexMaze(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y) {
    int width = grid.getWidth();
    int height = grid.getHeight();
    
    // Очищаем сетку и создаем полную стену
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid.setObstacle(x, y);
        }
    }
    
    // Алгоритм генерации лабиринта (упрощенный)
    std::vector<std::pair<int, int>> stack;
    
    // Начальная точка для генерации
    int start_gen_x = 1;
    int start_gen_y = 1;
    grid.clearObstacle(start_gen_x, start_gen_y);
    stack.push_back({start_gen_x, start_gen_y});
    
    // Направления: вверх, вправо, вниз, влево
    std::vector<std::pair<int, int>> directions = {{0, -2}, {2, 0}, {0, 2}, {-2, 0}};
    
    std::random_device rd;
    std::mt19937 rng(rd());
    
    while (!stack.empty()) {
        auto current = stack.back();
        int x = current.first;
        int y = current.second;
        
        // Получаем непосещенных соседей
        std::vector<std::pair<int, int>> neighbors;
        
        for (const auto& [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && 
                grid.isObstacle(nx, ny)) {
                neighbors.push_back({nx, ny});
            }
        }
        
        if (!neighbors.empty()) {
            // Выбираем случайного соседа
            std::shuffle(neighbors.begin(), neighbors.end(), rng);
            auto [nx, ny] = neighbors[0];
            
            // Убираем стену между текущей клеткой и соседом
            int wall_x = x + (nx - x) / 2;
            int wall_y = y + (ny - y) / 2;
            grid.clearObstacle(wall_x, wall_y);
            grid.clearObstacle(nx, ny);
            
            stack.push_back({nx, ny});
        } else {
            stack.pop_back();
        }
    }
    
    // Устанавливаем старт и финиш
    start_x = 1;
    start_y = 1;
    end_x = width - 2;
    end_y = height - 2;
    
    // Гарантируем, что старт и финиш доступны
    grid.clearObstacle(start_x, start_y);
    grid.clearObstacle(end_x, end_y);
}

} // namespace scenarios