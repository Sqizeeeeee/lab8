/**
 * @file grid.cpp
 * @brief Реализация класса Grid
 */

#include "grid/grid.h"
#include "../../config.h"

#include <cmath>
#include <algorithm>

Grid::Grid(int width, int height) 
    : width_(width), height_(height) {
    initializeGrid();
}

void Grid::initializeGrid() {
    nodes_.resize(height_);
    for (int y = 0; y < height_; ++y) {
        nodes_[y].reserve(width_);
        for (int x = 0; x < width_; ++x) {
            nodes_[y].emplace_back(x, y, true);
        }
    }
}

Node& Grid::getNode(int x, int y) {
    if (!isValidCoordinate(x, y)) {
        throw std::out_of_range("Grid coordinates out of range");
    }
    return nodes_[y][x];
}

const Node& Grid::getNode(int x, int y) const {
    if (!isValidCoordinate(x, y)) {
        throw std::out_of_range("Grid coordinates out of range");
    }
    return nodes_[y][x];
}

bool Grid::isValidCoordinate(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

void Grid::setObstacle(int x, int y) {
    if (isValidCoordinate(x, y)) {
        nodes_[y][x].walkable = false;
    }
}

void Grid::clearObstacle(int x, int y) {
    if (isValidCoordinate(x, y)) {
        nodes_[y][x].walkable = true;
    }
}

bool Grid::isObstacle(int x, int y) const {
    if (!isValidCoordinate(x, y)) {
        return true; // Координаты вне сетки считаются препятствиями
    }
    return !nodes_[y][x].walkable;
}

void Grid::inflateObstacles(double agent_radius) {
    // Создаем временную копию текущего состояния препятствий
    std::vector<std::vector<bool>> original_obstacles(height_, std::vector<bool>(width_, false));
    
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            original_obstacles[y][x] = !nodes_[y][x].walkable;
        }
    }
    
    // Вычисляем радиус для инфляции в клетках
    int inflation_radius = static_cast<int>(std::ceil(agent_radius + config::SAFETY_MARGIN));
    
    // Применяем инфляцию к каждому исходному препятствию
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (original_obstacles[y][x]) {
                // "Раздуваем" препятствие
                for (int dy = -inflation_radius; dy <= inflation_radius; ++dy) {
                    for (int dx = -inflation_radius; dx <= inflation_radius; ++dx) {
                        int new_x = x + dx;
                        int new_y = y + dy;
                        
                        if (isValidCoordinate(new_x, new_y)) {
                            // Проверяем расстояние для круговой инфляции
                            double distance = std::sqrt(dx*dx + dy*dy);
                            if (distance <= agent_radius + config::SAFETY_MARGIN) {
                                nodes_[new_y][new_x].walkable = false;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Grid::resetSearchData() {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            nodes_[y][x].reset();
        }
    }
}

std::vector<Node*> Grid::getNeighbors(const Node& node) {
    std::vector<Node*> neighbors;
    neighbors.reserve(8); // Максимум 8 соседей
    
    // Возможные направления движения
    const std::vector<std::pair<int, int>> directions = {
        {0, -1},  // вверх
        {1, 0},   // вправо
        {0, 1},   // вниз
        {-1, 0},  // влево
    };
    
    const std::vector<std::pair<int, int>> diagonal_directions = {
        {1, -1},  // вверх-вправо
        {1, 1},   // вниз-вправо
        {-1, 1},  // вниз-влево
        {-1, -1}  // вверх-влево
    };
    
    // Добавляем ортогональных соседей
    for (const auto& [dx, dy] : directions) {
        int new_x = node.x + dx;
        int new_y = node.y + dy;
        
        if (isValidCoordinate(new_x, new_y) && nodes_[new_y][new_x].walkable) {
            neighbors.push_back(&nodes_[new_y][new_x]);
        }
    }
    
    // Добавляем диагональных соседей, если разрешено
    if (config::ALLOW_DIAGONAL_MOVEMENT) {
        for (const auto& [dx, dy] : diagonal_directions) {
            int new_x = node.x + dx;
            int new_y = node.y + dy;
            
            // Проверяем, что целевая клетка доступна
            // и соседние ортогональные клетки не блокируют диагональный проход
            if (isValidCoordinate(new_x, new_y) && nodes_[new_y][new_x].walkable) {
                bool can_move_diagonal = true;
                
                // Проверяем соседние клетки, чтобы избежать "срезания углов"
                if (config::AGENT_RADIUS > 0.5) {
                    can_move_diagonal = nodes_[node.y + dy][node.x].walkable && 
                                      nodes_[node.y][node.x + dx].walkable;
                }
                
                if (can_move_diagonal) {
                    neighbors.push_back(&nodes_[new_y][new_x]);
                }
            }
        }
    }
    
    return neighbors;
}