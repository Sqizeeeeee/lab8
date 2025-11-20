/**
 * @file csv_writer.h
 * @brief Класс для записи результатов тестирования в CSV файлы
 * 
 * Предоставляет функциональность для сохранения метрик алгоритмов
 * в формате CSV для последующего анализа
 */

#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "utils/metrics_calculator.h"

#include <string>
#include <vector>
#include <fstream>
#include <memory>

/**
 * @struct AlgorithmResult
 * @brief Результаты работы одного алгоритма на одном тесте
 */
struct AlgorithmResult {
    std::string algorithm_name;          ///< Название алгоритма
    std::string test_scenario;           ///< Название тестового сценария
    PathMetrics metrics;                 ///< Вычисленные метрики
};

/**
 * @class CSVWriter
 * @brief Класс для записи результатов в CSV файлы
 */
class CSVWriter {
public:
    /**
     * @brief Конструктор писателя CSV
     * @param output_dir Директория для сохранения результатов
     */
    explicit CSVWriter(const std::string& output_dir = "results/csv");
    
    /**
     * @brief Деструктор (закрывает файлы)
     */
    ~CSVWriter();
    
    /**
     * @brief Сохранить результаты одного алгоритма
     * @param result Результат для сохранения
     */
    void writeResult(const AlgorithmResult& result);
    
    /**
     * @brief Сохранить группу результатов
     * @param results Вектор результатов
     */
    void writeResults(const std::vector<AlgorithmResult>& results);
    
    /**
     * @brief Создать сводную таблицу по всем алгоритмам
     * @param results Все результаты тестирования
     */
    void createSummaryTable(const std::vector<AlgorithmResult>& results);
    
    /**
     * @brief Установить директорию для выходных файлов
     * @param output_dir Путь к директории
     */
    void setOutputDirectory(const std::string& output_dir);

private:
    std::string output_dir_;                     ///< Директория для результатов
    std::ofstream detailed_file_;                ///< Файл с детальными результатами
    std::ofstream summary_file_;                 ///< Файл со сводной таблицей
    
    /**
     * @brief Открыть файлы для записи
     */
    void openFiles();
    
    /**
     * @brief Закрыть открытые файлы
     */
    void closeFiles();
    
    /**
     * @brief Записать заголовок CSV файла
     * @param file Файл для записи
     */
    void writeCSVHeader(std::ofstream& file);
    
    /**
     * @brief Записать одну строку с результатами
     * @param file Файл для записи
     * @param result Результат для записи
     */
    void writeCSVRow(std::ofstream& file, const AlgorithmResult& result);
    
    /**
     * @brief Создать имя файла с временной меткой
     * @param base_name Базовое имя файла
     * @return Полное имя файла
     */
    std::string createTimestampedFilename(const std::string& base_name);
};

#endif // CSV_WRITER_H