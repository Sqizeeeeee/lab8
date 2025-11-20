/**
 * @file main.cpp
 * @brief Основной файл для запуска всех тестов алгоритмов поиска пути
 */

#include <iostream>
#include <chrono>
#include <vector>
#include <memory>

#include "config.h"
#include "scenarios/test_scenarios.h"
#include "algorithms/astar.h"
#include "algorithms/astar_ps.h"
#include "algorithms/thetastar.h"
#include "utils/metrics_calculator.h"
#include "utils/csv_writer.h"

/**
 * @brief Запустить один тест для алгоритма
 */
template<typename AlgorithmType>
AlgorithmResult runTest(AlgorithmType& algorithm, const TestScenario& scenario, 
                       const std::string& algorithm_name) {
    AlgorithmResult result;
    result.algorithm_name = algorithm_name;
    result.test_scenario = scenario.name;
    
    try {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Запускаем поиск пути
        auto path = algorithm.findPath(scenario.start_x, scenario.start_y,
                                      scenario.end_x, scenario.end_y);
        
        auto end_time = std::chrono::high_resolution_clock::now();
        double execution_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        
        // Вычисляем метрики
        result.metrics = metrics::calculateAllMetrics(
            path, scenario.grid, scenario.start_x, scenario.start_y,
            scenario.end_x, scenario.end_y, algorithm.getNodesExpanded(),
            execution_time
        );
        
        std::cout << "✓ " << algorithm_name << " on " << scenario.name 
                  << " - Path length: " << result.metrics.path_length
                  << ", Time: " << execution_time << "ms" << std::endl;
                  
    } catch (const std::exception& e) {
        std::cout << "✗ " << algorithm_name << " on " << scenario.name 
                  << " - FAILED: " << e.what() << std::endl;
        result.metrics.success = false;
    }
    
    return result;
}

/**
 * @brief Запустить все тесты для одного сценария
 * @param scenario Тестовый сценарий
 * @param csv_writer Писатель CSV
 * @return Вектор результатов тестов
 */
std::vector<AlgorithmResult> runScenarioTests(const TestScenario& scenario, CSVWriter& csv_writer) {
    std::cout << "\n=== Testing scenario: " << scenario.name << " ===" << std::endl;
    
    // Создаем копии сетки для каждого алгоритма
    Grid grid_astar = scenario.grid;
    Grid grid_astar_ps = scenario.grid;
    Grid grid_thetastar = scenario.grid;
    
    // Инфляция препятствий
    grid_astar.inflateObstacles(config::AGENT_RADIUS);
    grid_astar_ps.inflateObstacles(config::AGENT_RADIUS);
    grid_thetastar.inflateObstacles(config::AGENT_RADIUS);
    
    // Создаем алгоритмы
    AStar astar(grid_astar);
    AStarPS astar_ps(grid_astar_ps);
    ThetaStar thetastar(grid_thetastar);
    
    // Запускаем тесты
    std::vector<AlgorithmResult> results;
    
    for (int run = 0; run < config::NUM_TEST_RUNS; ++run) {
        if (run > 0) {
            // Сбрасываем статистику для повторных запусков
            astar.resetStatistics();
            astar_ps.resetStatistics();
            thetastar.resetStatistics();
        }
        
        results.push_back(runTest(astar, scenario, "AStar"));
        results.push_back(runTest(astar_ps, scenario, "AStarPS"));
        results.push_back(runTest(thetastar, scenario, "ThetaStar"));
    }
    
    // Сохраняем результаты
    csv_writer.writeResults(results);
    
    return results;  // ВОЗВРАЩАЕМ РЕЗУЛЬТАТЫ
}

/**
 * @brief Основная функция
 */
int main() {
    std::cout << "=== Pathfinding Algorithms Benchmark ===" << std::endl;
    std::cout << "Grid size: " << config::GRID_WIDTH << "x" << config::GRID_HEIGHT << std::endl;
    std::cout << "Agent radius: " << config::AGENT_RADIUS << std::endl;
    std::cout << "Test runs per scenario: " << config::NUM_TEST_RUNS << std::endl;
    
    try {
        // Инициализируем сценарии
        scenarios::initializeScenarios(true);
        auto test_scenarios = scenarios::getScenarios();
        
        CSVWriter csv_writer("results/csv");

        // Запускаем тесты для всех сценариев и собираем результаты
        std::vector<AlgorithmResult> all_results;
        for (const auto& scenario : test_scenarios) {
            auto scenario_results = runScenarioTests(scenario, csv_writer);
            all_results.insert(all_results.end(), scenario_results.begin(), scenario_results.end());
        }

        // Создаем сводную таблицу
        csv_writer.createSummaryTable(all_results);

        std::cout << "\n=== All tests completed ===" << std::endl;
        std::cout << "Results saved to results/csv/" << std::endl;
        std::cout << "Python visualization files saved to python_visualization/" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}