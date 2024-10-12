#pragma once
#ifndef LOGPROCESSOR_H
#define LOGPROCESSOR_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <set>

// Structure for the input log
struct LogEntry {
    time_t timestamp;
    std::string logLevel;
    std::string message;
};

// Function declarations
time_t parseTimestamp(const std::string& timestampStr);
std::vector<LogEntry> readLogFile(const std::string& filePath);
void countLogLevels(const std::vector<LogEntry>& logEntries);
void listMessagesByLevel(const std::vector<LogEntry>& logEntries, const std::string& level);
void calculateUptime(const std::vector<LogEntry>& logEntries);
void displayHelp();

#endif // LOGPROCESSOR_H

