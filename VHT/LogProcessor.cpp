#include "LogProcessor.h"

// Function: parse a timestamp string into a time_t value
time_t parseTimestamp(const std::string& timestampStr) {
    std::tm timeStruct = {};
    std::istringstream ss(timestampStr);

    ss >> std::get_time(&timeStruct, "%Y-%m-%d %H:%M:%S");

    // If parsing fails, output an error message and return -1
    if (ss.fail()) {
        std::cerr << "Error: Failed to parse timestamp: " << timestampStr << std::endl;
        return -1;
    }

    // If parsed, convert the parsed time structure to time_t
    return mktime(&timeStruct);
}

// Function: read the input log files
std::vector<LogEntry> readLogFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<LogEntry> logEntries;
    std::string line;

    // Check if the file path is empty
    if (filePath.empty()) {
        std::cerr << "Error: Log file path is required." << std::endl;
        return {};
    }

    // Check if the file is opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return {};
    }

    // Read each line of the file
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        LogEntry entry;
        std::string _timestampStr, _logLevel, _message;

        // Find positions of spaces in the line 
        size_t firstSpace = line.find(' ');
        size_t secondSpace = line.find(' ', firstSpace + 1);
        size_t thirdSpace = line.find(' ', secondSpace + 1);

        if (firstSpace != std::string::npos && secondSpace != std::string::npos && thirdSpace != std::string::npos) {
            // Separate timestamp, log level, and message from space
            _timestampStr = line.substr(0, secondSpace);
            _logLevel = line.substr(secondSpace + 1, thirdSpace - secondSpace - 1);
            _message = line.substr(thirdSpace + 1);

            entry.timestamp = parseTimestamp(_timestampStr); // Convert the _timestamp string to time_t format
            entry.logLevel = _logLevel;
            entry.message = _message;

            logEntries.push_back(entry);
        }
    }

    return logEntries;
}

// Function: Count the number of log levels
void countLogLevels(const std::vector<LogEntry>& logEntries) {
    int infoCount = 0, warningCount = 0, errorCount = 0;

    // Loop through the log entries, if found then increment the variable
    for (const auto& entry : logEntries) {
        if (entry.logLevel == "INFO") infoCount++;
        else if (entry.logLevel == "WARNING") warningCount++;
        else if (entry.logLevel == "ERROR") errorCount++;
    }

    // Print the log summary
    std::cout << "Log Summary:" << std::endl;
    std::cout << "INFO: " << infoCount << " messages" << std::endl;
    std::cout << "WARNING: " << warningCount << " messages" << std::endl;
    std::cout << "ERROR: " << errorCount << " messages" << std::endl;
    std::cout << std::endl;
}

// Function: List log messages by level
void listMessagesByLevel(const std::vector<LogEntry>& logEntries, const std::string& level) {
    std::string formattedLevel = level;
    std::transform(formattedLevel.begin(), formattedLevel.end(), formattedLevel.begin(), ::toupper);

    std::set<std::string> validLevels = { "INFO", "WARNING", "ERROR" };
    if (validLevels.find(formattedLevel) == validLevels.end()) {
        std::cerr << "Error: Invalid log level: " << level << std::endl;
        return;
    }

    std::cout << formattedLevel << " messages:" << std::endl;

    for (const auto& entry : logEntries) {
        if (entry.logLevel == formattedLevel) {
            std::tm timeInfo;
            localtime_s(&timeInfo, &entry.timestamp); // Convert timestamp to local time
            std::cout << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S")
                << " " << entry.logLevel << " " << entry.message << std::endl;
        }
    }
    std::cout << std::endl;
}

// Function: Calculate system uptime 
void calculateUptime(const std::vector<LogEntry>& logEntries) {
    if (logEntries.empty()) {
        std::cerr << "Error: No log entries to calculate uptime." << std::endl;
        return;
    }

    time_t startTime = 0;
    time_t endTime = 0;

    // Find the system startup time
    for (const auto& entry : logEntries) {
        if (entry.message.find("System startup") != std::string::npos) {
            startTime = entry.timestamp;
            break;
        }
    }
    // Find the system shutdown time
    for (const auto& entry : logEntries) {
        if (entry.message.find("System shutdown") != std::string::npos) {
            endTime = entry.timestamp;
            break;
        }
    }

    if (startTime == 0) {
        std::cerr << "Error: No startup log found." << std::endl;
        return;
    }

    long uptimeSeconds = static_cast<long>(difftime(endTime, startTime)); // Calculate uptime

    std::cout << "System uptime: "
        << std::setfill('0') << std::setw(2) << static_cast<int>(uptimeSeconds / 3600) << ":"
        << std::setfill('0') << std::setw(2) << static_cast<int>((uptimeSeconds % 3600) / 60) << ":"
        << std::setfill('0') << std::setw(2) << static_cast<int>(uptimeSeconds % 60) << std::endl;
    std::cout << std::endl;
}

// Function: display instruction
void displayHelp() {
    std::cout << "Usage: ./log_processor --file <file_path> [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --count-levels      Count log messages by log level." << std::endl;
    std::cout << "  --list <level>      List all messages with the specified log level." << std::endl;
    std::cout << "  --uptime            Calculate total system uptime." << std::endl;
    std::cout << "  --help              Display usage instructions." << std::endl;
    std::cout << std::endl;
}
