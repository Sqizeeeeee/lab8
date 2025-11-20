/**
 * @file metrics_calculator.cpp
 * @brief Реализация калькулятора метрик
 */

#include "utils/metrics_calculator.h"

#include <cmath>
#include <algorithm>
#include <limits>

namespace metrics {

PathMetrics calculateAllMetrics(const std::vector<Node*>& path,
                               const Grid& grid,
                               int start_x, int start_y,
                               int end_x, int end_y,
                               int nodes_expanded,
                               double execution_time) {
    PathMetrics metrics;
    
    // Базовые метрики
    metrics.path_length = calculatePathLength(path);
    metrics.nodes_expanded = nodes_expanded;
    metrics.execution_time = execution_time;
    metrics.success = !path.empty();
    
    // Метрики оптимальности
    double optimal_length = euclideanDistance(grid.getNode(start_x, start_y), 
                                             grid.getNode(end_x, end_y));
    metrics.optimality_coefficient = (optimal_length > 0) ? metrics.path_length / optimal_length : 1.0;
    metrics.path_deviation = (optimal_length > 0) ? 
                           (metrics.path_length - optimal_length) / optimal_length * 100.0 : 0.0;
    
    // Метрики гладкости
    metrics.smoothness = calculateSmoothness(path);
    metrics.total_turn_angle = metrics.smoothness; // Для простоты используем ту же метрику
    
    // Метрики эффективности поиска
    metrics.search_efficiency = (nodes_expanded > 0 && optimal_length > 0) ? 
                              optimal_length / nodes_expanded : 0.0;
    
    // Фактор ветвления (упрощенная версия)
    double solution_depth = path.size();
    metrics.branching_factor = (solution_depth > 0) ? 
                             std::pow(nodes_expanded, 1.0 / solution_depth) : 0.0;
    
    // Метрики расстояния до препятствий
    auto obstacle_distances = calculateObstacleDistances(path, grid);
    metrics.min_obstacle_distance = obstacle_distances.first;
    metrics.avg_obstacle_distance = obstacle_distances.second;
    
    // Метрика кривизны
    metrics.max_curvature = calculateMaxCurvature(path);
    
    return metrics;
}

double calculatePathLength(const std::vector<Node*>& path) {
    if (path.size() < 2) {
        return 0.0;
    }
    
    double length = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        length += euclideanDistance(*path[i-1], *path[i]);
    }
    return length;
}

double calculateSmoothness(const std::vector<Node*>& path) {
    if (path.size() < 3) {
        return 0.0;
    }
    
    double total_angle = 0.0;
    for (size_t i = 1; i < path.size() - 1; ++i) {
        double angle = calculateAngle(*path[i-1], *path[i], *path[i+1]);
        total_angle += std::abs(angle);
    }
    
    return total_angle;
}

std::pair<double, double> calculateObstacleDistances(const std::vector<Node*>& path,
                                                    const Grid& grid) {
    if (path.empty()) {
        return {0.0, 0.0};
    }
    
    double min_distance = std::numeric_limits<double>::max();
    double total_distance = 0.0;
    int count = 0;
    
    for (Node* node : path) {
        // Ищем ближайшее препятствие для этого узла пути
        double min_node_distance = std::numeric_limits<double>::max();
        
        // Проверяем окрестность вокруг узла пути
        int search_radius = 5; // Можно настроить в config
        for (int dy = -search_radius; dy <= search_radius; ++dy) {
            for (int dx = -search_radius; dx <= search_radius; ++dx) {
                int check_x = node->x + dx;
                int check_y = node->y + dy;
                
                if (grid.isValidCoordinate(check_x, check_y) && 
                    grid.isObstacle(check_x, check_y)) {
                    
                    double distance = std::sqrt(dx*dx + dy*dy);
                    if (distance < min_node_distance) {
                        min_node_distance = distance;
                    }
                }
            }
        }
        
        if (min_node_distance < std::numeric_limits<double>::max()) {
            min_distance = std::min(min_distance, min_node_distance);
            total_distance += min_node_distance;
            count++;
        }
    }
    
    double avg_distance = (count > 0) ? total_distance / count : 0.0;
    if (min_distance == std::numeric_limits<double>::max()) {
        min_distance = 0.0;
    }
    
    return {min_distance, avg_distance};
}

double calculateMaxCurvature(const std::vector<Node*>& path) {
    if (path.size() < 3) {
        return 0.0;
    }
    
    double max_curvature = 0.0;
    for (size_t i = 1; i < path.size() - 1; ++i) {
        const Node* prev = path[i-1];
        const Node* curr = path[i];
        const Node* next = path[i+1];
        
        // Векторы между точками
        double dx1 = curr->x - prev->x;
        double dy1 = curr->y - prev->y;
        double dx2 = next->x - curr->x;
        double dy2 = next->y - curr->y;
        
        // Длины векторов
        double len1 = std::sqrt(dx1*dx1 + dy1*dy1);
        double len2 = std::sqrt(dx2*dx2 + dy2*dy2);
        
        if (len1 > config::ANGLE_EPSILON && len2 > config::ANGLE_EPSILON) {
            // Нормализуем векторы
            dx1 /= len1; dy1 /= len1;
            dx2 /= len2; dy2 /= len2;
            
            // Кривизна как изменение направления
            double curvature = std::abs(dx1 * dy2 - dy1 * dx2);
            max_curvature = std::max(max_curvature, curvature);
        }
    }
    
    return max_curvature;
}

double euclideanDistance(const Node& a, const Node& b) {
    double dx = static_cast<double>(b.x - a.x);
    double dy = static_cast<double>(b.y - a.y);
    return std::sqrt(dx * dx + dy * dy);
}

double calculateAngle(const Node& a, const Node& b, const Node& c) {
    // Векторы BA и BC
    double ba_x = a.x - b.x;
    double ba_y = a.y - b.y;
    double bc_x = c.x - b.x;
    double bc_y = c.y - b.y;
    
    // Скалярное произведение
    double dot_product = ba_x * bc_x + ba_y * bc_y;
    
    // Модули векторов
    double mod_ba = std::sqrt(ba_x * ba_x + ba_y * ba_y);
    double mod_bc = std::sqrt(bc_x * bc_x + bc_y * bc_y);
    
    if (mod_ba < config::ANGLE_EPSILON || mod_bc < config::ANGLE_EPSILON) {
        return 0.0;
    }
    
    // Косинус угла
    double cos_angle = dot_product / (mod_ba * mod_bc);
    
    // Ограничиваем значение для арккосинуса
    cos_angle = std::max(-1.0, std::min(1.0, cos_angle));
    
    // Угол в радианах
    return std::acos(cos_angle);
}

} // namespace metrics