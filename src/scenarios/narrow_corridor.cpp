/**
 * @file narrow_corridor.cpp
 * @brief Реализация сценария "Узкие коридоры"
 */

#include "scenarios/narrow_corridor.h"
#include "config.h"
#include <iostream>

namespace scenarios {

void createNarrowCorridors(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y,
                          int corridor_width) {
    int width = grid.getWidth();
    int height = grid.getHeight();
    
    std::cout << "Generating narrow corridors for grid " << width << "x" << height << std::endl;
    
    // 1. Сначала делаем ВСЕ клетки препятствиями
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid.setObstacle(x, y);
        }
    }
    
    int center_x = width / 2;
    int center_y = height / 2;
    
    // 2. Создаем вертикальный коридор (полностью проходимый)
    std::cout << "Creating vertical corridor at x=" << center_x 
              << " with width " << (corridor_width * 2 + 1) << std::endl;
    for (int y = 0; y < height; ++y) {
        for (int x = center_x - corridor_width; x <= center_x + corridor_width; ++x) {
            if (x >= 0 && x < width) {
                grid.clearObstacle(x, y);
            }
        }
    }
    
    // 3. Создаем горизонтальный коридор (полностью проходимый)
    std::cout << "Creating horizontal corridor at y=" << center_y 
              << " with width " << (corridor_width * 2 + 1) << std::endl;
    for (int x = 0; x < width; ++x) {
        for (int y = center_y - corridor_width; y <= center_y + corridor_width; ++y) {
            if (y >= 0 && y < height) {
                grid.clearObstacle(x, y);
            }
        }
    }
    
    // 4. Устанавливаем старт и финиш в ГАРАНТИРОВАННО проходимых местах
    start_x = 1;
    start_y = center_y;  // Старт в горизонтальном коридоре слева
    end_x = width - 2;
    end_y = center_y;    // Финиш в горизонтальном коридоре справа
    
    // Дополнительная проверка: если точки не проходимы, используем альтернативные
    if (!grid.getNode(start_x, start_y).walkable) {
        start_x = center_x;
        start_y = 1;
        std::cout << "Using alternative start position: (" << start_x << ", " << start_y << ")" << std::endl;
    }
    
    if (!grid.getNode(end_x, end_y).walkable) {
        end_x = center_x;
        end_y = height - 2;
        std::cout << "Using alternative end position: (" << end_x << ", " << end_y << ")" << std::endl;
    }
    
    // 5. Визуализация для отладки
    std::cout << "Final positions:" << std::endl;
    std::cout << "Start: (" << start_x << ", " << start_y << ") - Walkable: " 
              << grid.getNode(start_x, start_y).walkable << std::endl;
    std::cout << "End: (" << end_x << ", " << end_y << ") - Walkable: " 
              << grid.getNode(end_x, end_y).walkable << std::endl;
    
    // Простая текстуальная визуализация
    std::cout << "Grid preview (5x5 area around center):" << std::endl;
    for (int y = center_y - 2; y <= center_y + 2; ++y) {
        if (y >= 0 && y < height) {
            for (int x = center_x - 2; x <= center_x + 2; ++x) {
                if (x >= 0 && x < width) {
                    if (x == start_x && y == start_y) std::cout << "S";
                    else if (x == end_x && y == end_y) std::cout << "E";
                    else if (x == center_x && y == center_y) std::cout << "+";
                    else std::cout << (grid.getNode(x, y).walkable ? "." : "#");
                }
            }
            std::cout << std::endl;
        }
    }
}

} // namespace scenarios