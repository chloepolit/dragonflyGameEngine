#pragma once

#include <fstream>
#include <string>
#include "Manager.h"

// Default log file name
const std::string LOGFILE_DEFAULT = "dragonfly.log";

#define LM df::LogManager::getInstance()

namespace df {

class LogManager : public Manager {
private:
    bool m_do_flush;         // True if flush to disk after every write
    std::ofstream m_p_f;     // Pointer to log file

    LogManager();                               // Private (singleton)
    LogManager(LogManager const &);             // No copy
    void operator=(LogManager const &);         // No assign

public:
    // Get the one and only instance of LogManager
    static LogManager &getInstance();

    // Start up LogManager: open log file
    // Return 0 if ok, else -1
    int startUp();

    // Shut down LogManager: close log file
    void shutDown();

    // Set flush (true = flush after every write, false = buffer)
    void setFlush(bool new_do_flush);

    // Write formatted string to log file.
    // printf-style format string, return 0 if ok, else -1.
    int writeLog(const char *fmt, ...) const;
};

} // end namespace df
