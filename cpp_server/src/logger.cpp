#include <iostream>
#include <string.h>
#include <stdarg.h>
#include <new>
#include <cstdlib>

#include "logger.h"

Logger* Logger::m_ = 0;

pthread_mutex_t Logger::lock_ = PTHREAD_MUTEX_INITIALIZER;

inline void Logger::lock()
{
    pthread_mutex_lock(&lock_);
}

inline void Logger::unlock()
{
    pthread_mutex_unlock(&lock_);
}

Logger::Logger(): configured_(false)
{
    gettimeofday(&initialTime_, NULL);
}

void Logger::configure(const std::string& outputFile,
                       const loggerConf configuration,
                       const int fileVerbosityLevel,
                       const int screenVerbosityLevel)
{
    Logger::lock();

    fileVerbosityLevel_   = fileVerbosityLevel;
    screenVerbosityLevel_ = screenVerbosityLevel;

    // Close the old stream, if needed
    if (configuration_ & file_on)
        out_.close();

    char curtime[4096];
    struct timeval currentTime;
    struct tm lt;

    gettimeofday(&currentTime, NULL);
    strftime(curtime, sizeof(curtime) - 1,
             "_%Y-%m-%d_%H-%M-%S", localtime_r(&currentTime.tv_sec, &lt));
    logFile_ = outputFile + curtime + ".log";

    if (configuration & file_on)
        out_.open(logFile_.c_str(), std::ios::app);

    configuration_ = configuration;
    configured_ = true;

    Logger::unlock();
}

Logger::~Logger()
{
    if(configuration_ & file_on)
        out_.close();
}

Logger& Logger::getInstance()
{
    Logger::lock();
    if(m_ == 0)
        m_ = new Logger;
    Logger::unlock();
    return *m_;
}

void Logger::deleteInstance()
{
    Logger::lock();
    if(m_)
        delete m_;
    m_ = 0;
    Logger::unlock();
}

void Logger::print(const unsigned int verbosityLevel,
                   const std::string& file,
                   const int line,
                   const std::string& func,
                   const char * fmt,
                   ...)
{
    va_list argp;

    char message[4096];

    va_start(argp, fmt);
    vsnprintf(message, sizeof(message) - 1, fmt, argp);
    va_end(argp);

    if(!configured_) 
    {
        std::cerr << "ERROR: Logger not configured! " << std::endl
                  << message << std::endl;
        return;
    }

    Logger::lock();

    char curtime[4096];
    struct timeval currentTime;
    struct tm lt;

    std::string reduceFile = file;
    size_t pos = reduceFile.find("../../src/");

    if (pos == 0)
    {
        reduceFile.erase(reduceFile.begin(), reduceFile.begin() + 
                         strlen("../../src/"));
    }

    gettimeofday(&currentTime, NULL);
    strftime(curtime, sizeof(curtime) - 1,
             "[%Y-%m-%d %H:%M:%S", localtime_r(&currentTime.tv_sec, &lt));
    sprintf(curtime + strlen(curtime), ".%03ld] ", currentTime.tv_usec / 1000);

    std::stringstream output;
    output <<
        "\x1b[39m" <<
        priorityText[verbosityLevel] << " " <<
        curtime << 
        // "[" << reduceFile << ":" << line << "]" <<
        "[" << func << "] : " <<
        "\x1b[" << priorityColor[verbosityLevel] << "m" <<
        message << 
        "\x1b[39m" <<
        std::endl;


    if ((configuration_ & file_on) && (verbosityLevel >= fileVerbosityLevel_))
        out_ << output.str();

    if ((configuration_ & screen_on) && (verbosityLevel >= screenVerbosityLevel_))
        std::cerr << output.str();

    Logger::unlock();
}

