/**
 * @file test_scenarios.h
 * @brief Тестовые сценарии для оценки алгоритмов поиска пути
 */

#ifndef TEST_SCENARIOS_H
#define TEST_SCENARIOS_H

#include "grid/grid.h"

#include <string>
#include <vector>

/**
 * @struct TestScenario
 * @brief Структура для хранения тестового сценария
 */
struct TestScenario {
    std::string name;                   ///< Название сценария
    Grid grid;                          ///< Сетка с препятствиями
    int start_x;                        ///< Стартовая координата X
    int start_y;                        ///< Стартовая координата Y  
    int end_x;                          ///< Конечная координата X
    int end_y;                          ///< Конечная координата Y
    
    TestScenario(const std::string& scenario_name, int width, int height)
        : name(scenario_name), grid(width, height), 
          start_x(0), start_y(0), end_x(width-1), end_y(height-1) {}
};

/**
 * @namespace scenarios
 * @brief Пространство имен для тестовых сценариев
 */
namespace scenarios {

/**
 * @brief Создать все тестовые сценарии
 * @return Вектор тестовых сценариев
 */
std::vector<TestScenario> createAllScenarios();

/**
 * @brief Сохранить сценарий в файл
 * @param scenario Сценарий для сохранения
 * @param filename Имя файла
 */
void saveScenario(const TestScenario& scenario, const std::string& filename);

/**
 * @brief Загрузить сценарий из файла
 * @param filename Имя файла
 * @return Загруженный сценарий
 */
TestScenario loadScenario(const std::string& filename);

/**
 * @brief Сохранить все сценарии в директорию
 * @param scenarios Вектор сценариев
 * @param directory Директория для сохранения
 */
void saveAllScenarios(const std::vector<TestScenario>& scenarios, 
                     const std::string& directory = "test_scenarios");

/**
 * @brief Загрузить все сценарии из директории
 * @param directory Директория с сценариями
 * @return Вектор загруженных сценариев
 */
std::vector<TestScenario> loadAllScenarios(const std::string& directory = "test_scenarios");

/**
 * @brief Экспортировать сценарий в формат для Python визуализации
 * @param scenario Сценарий для экспорта
 * @param filename Имя файла
 */
void exportForPython(const TestScenario& scenario, const std::string& filename);

/**
 * @brief Инициализировать и сохранить все сценарии один раз
 * @param force_recreate Принудительно пересоздать сценарии
 */
void initializeScenarios(bool force_recreate = false);

/**
 * @brief Загрузить все сценарии (создать если не существуют)
 * @return Вектор загруженных сценариев
 */
std::vector<TestScenario> getScenarios();

// Вспомогательные функции
bool createDirectory(const std::string& path);
bool directoryExists(const std::string& path);

// Функции создания конкретных сценариев
void createOpenSpace(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y);
void createComplexMaze(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y);
void createManyObstacles(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y, 
                        int obstacle_density = 30);
void createNarrowCorridors(Grid& grid, int& start_x, int& start_y, int& end_x, int& end_y,
                          int corridor_width = 3);
void createRandomObstacles(Grid& grid, int density, unsigned int seed = 42);
bool isPathPossible(const Grid& grid, int start_x, int start_y, int end_x, int end_y);
std::string getScenarioName(int index);

} // namespace scenarios

#endif // TEST_SCENARIOS_H