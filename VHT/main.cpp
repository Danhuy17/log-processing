#include "LogProcessor.h"

int main(int argc, char* argv[]) {
    std::string filePath;
    bool countLevels = false;
    std::vector<std::string> listLevels;
    bool uptime = false;
    bool isError = false;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            filePath = argv[++i];
        }
        else if (std::strcmp(argv[i], "--count-levels") == 0) {
            countLevels = true;
        }
        else if (std::strcmp(argv[i], "--list") == 0 && i + 1 < argc) {
            listLevels.push_back(argv[++i]);
        }
        else if (std::strcmp(argv[i], "--uptime") == 0) {
            uptime = true;
        }
        else if (std::strcmp(argv[i], "--help") == 0) {
            displayHelp();
            return 0;
        }
        else {
            std::cerr << "Error: Unknown argument: " << argv[i] << std::endl;
            isError = true;
            break;
        }
    }

    // Check if there is any syntax error
    if (isError) {
        displayHelp();
    }

    // Check if the file path is provided
    if (filePath.empty()) {
        std::cerr << "Error: Log file path is required." << std::endl;
        return 1;
    }

    std::vector<LogEntry> logEntries = readLogFile(filePath); // Read log entries from the specified file

    // Check if log entries were successfully read
    if (logEntries.empty()) {
        return 1;
    }

    if (countLevels) {
        countLogLevels(logEntries);
    }

    for (const auto& level : listLevels) {
        listMessagesByLevel(logEntries, level);
    }

    if (uptime) {
        calculateUptime(logEntries);
    }

    return 0;
}
