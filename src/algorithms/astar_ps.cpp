/**
 * @file astar_ps.cpp
 * @brief Реализация алгоритма A* с пост-сглаживанием
 */

#include "algorithms/astar_ps.h"

#include <stdexcept>
#include <cmath>

AStarPS::AStarPS(Grid& grid) 
    : grid_(grid), astar_(grid), original_path_length_(0.0), smoothed_path_length_(0.0) {}

std::vector<Node*> AStarPS::findPath(int start_x, int start_y, int end_x, int end_y) {
    // Сброс статистики
    resetStatistics();
    
    // Шаг 1: Находим путь с помощью базового A*
    std::vector<Node*> original_path = astar_.findPath(start_x, start_y, end_x, end_y);
    original_path_length_ = astar_.getPathLength();
    
    // Шаг 2: Сглаживаем путь
    std::vector<Node*> smoothed_path = smoothPath(original_path);
    smoothed_path_length_ = calculateSmoothedPathLength(smoothed_path);
    
    return smoothed_path;
}

std::vector<Node*> AStarPS::smoothPath(const std::vector<Node*>& original_path) {
    if (original_path.size() < 3) {
        return original_path;
    }
    
    std::vector<Node*> smoothed_path;
    smoothed_path.push_back(original_path[0]);
    
    size_t current_index = 0;
    
    while (current_index < original_path.size() - 1) {
        size_t farthest_visible_index = current_index + 1;
        
        for (size_t test_index = current_index + 2; test_index < original_path.size(); ++test_index) {
            Node* current_node = original_path[current_index];
            Node* test_node = original_path[test_index];
            
            // ПРАВИЛЬНЫЙ ВЫЗОВ: передаем grid_ первым параметром
            if (line_of_sight::hasLineOfSight(grid_, *current_node, *test_node)) {
                farthest_visible_index = test_index;
            } else {
                break;
            }
        }
        
        smoothed_path.push_back(original_path[farthest_visible_index]);
        current_index = farthest_visible_index;
    }
    
    if (smoothed_path.back() != original_path.back()) {
        smoothed_path.push_back(original_path.back());
    }
    
    return smoothed_path;
}

double AStarPS::calculateSmoothedPathLength(const std::vector<Node*>& path) const {
    if (path.size() < 2) {
        return 0.0;
    }
    
    double length = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        Node* from = path[i-1];
        Node* to = path[i];
        
        if (config::ALLOW_DIAGONAL_MOVEMENT) {
            // Евклидово расстояние для сглаженного пути
            double dx = static_cast<double>(to->x - from->x);
            double dy = static_cast<double>(to->y - from->y);
            length += std::sqrt(dx * dx + dy * dy);
        } else {
            // Манхэттенское расстояние
            length += std::abs(to->x - from->x) + std::abs(to->y - from->y);
        }
    }
    
    return length;
}

void AStarPS::resetStatistics() {
    astar_.resetStatistics();
    original_path_length_ = 0.0;
    smoothed_path_length_ = 0.0;
}