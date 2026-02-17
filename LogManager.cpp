#include "LogManager.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>

namespace df {

LogManager::LogManager() : m_do_flush(false) {
    setType("LogManager");
}

LogManager &LogManager::getInstance() {
    static LogManager instance;
    return instance;
}

int LogManager::startUp() {
    m_p_f.open(LOGFILE_DEFAULT, std::ofstream::out);
    if (!m_p_f.is_open()) {
        return -1;
    }
    Manager::startUp();
    return 0;
}

void LogManager::shutDown() {
    if (m_p_f.is_open()) {
        m_p_f.close();
    }
    Manager::shutDown();
}

void LogManager::setFlush(bool new_do_flush) {
    m_do_flush = new_do_flush;
}

int LogManager::writeLog(const char *fmt, ...) const {
    if (!m_p_f.is_open()) {
        return -1;
    }

    // Get timestamp
    time_t t = time(nullptr);
    char time_buf[20];
    struct tm *tm_info = localtime(&t);
    strftime(time_buf, sizeof(time_buf), "%H:%M:%S", tm_info);

    // Write timestamp
    const_cast<std::ofstream &>(m_p_f) << "[" << time_buf << "] ";

    // Format and write the message
    va_list args;
    va_start(args, fmt);
    char buf[1024];
    int ret = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (ret < 0) {
        return -1;
    }

    const_cast<std::ofstream &>(m_p_f) << buf << "\n";

    if (m_do_flush) {
        const_cast<std::ofstream &>(m_p_f).flush();
    }

    return ret;
}

} // end namespace df
