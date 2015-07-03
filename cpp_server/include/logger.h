#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <ostream>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <pthread.h>

const int DBG_DEBUG    = 0;
const int DBG_INFO     = 1;
const int DBG_WARN     = 2;
const int DBG_ERROR    = 3;
const int DBG_CRITICAL = 4;

const char* const priorityText[] = { "DEBUG", "INFO ", "WARN", "ERROR", "CRIT " };
const int priorityColor[] = { 36, 32, 33, 31, 35 };

#define LOGGER_CONF(outputFile, \
                    configuration, \
                    fileVerbosityLevel, \
                    screenVerbosityLevel) { \
    Logger::getInstance().configure(outputFile, \
                                    configuration, \
                                    fileVerbosityLevel, \
                                    screenVerbosityLevel); \
}

#define DEBUG(priority, msg, args...) \
    do { \
        std::ostringstream __debug_stream__; \
        __debug_stream__ << msg; \
        Logger::getInstance().print(priority, __FILE__, __LINE__, __FUNCTION__,\
                                    __debug_stream__.str().c_str(), ##args); \
    } while (0) 

class Logger
{
    private:
        enum loggerConf_ 
        {
            L_nofile_    =    1 << 0,
            L_file_      =    1 << 1,
            L_noscreen_  =    1 << 2,
            L_screen_    =    1 << 3
        };

        static Logger* m_;
        static pthread_mutex_t lock_;

        bool configured_;
        std::string logFile_;
        loggerConf_ configuration_;
        std::ofstream out_;
        struct timeval initialTime_;
        unsigned int fileVerbosityLevel_;
        unsigned int screenVerbosityLevel_;

        Logger();
        ~Logger();

        inline static void lock();
        inline static void unlock();

    public:

        typedef loggerConf_ loggerConf;
        static const loggerConf file_on = L_nofile_;
        static const loggerConf file_off = L_file_;
        static const loggerConf screen_on = L_noscreen_;
        static const loggerConf screen_off = L_screen_;

        static Logger& getInstance();
        static void deleteInstance();

        void print(const unsigned int verbosityLevel,
                   const std::string& sourceFile,
                   const int codeLine,
                   const std::string& sourceFunc,
                   const char* fmt,
                   ...);

        void configure(const std::string& outputFile,
                       const loggerConf configuration,
                       const int fileVerbosityLevel,
                       const int screenVerbosityLevel);
};

inline Logger::loggerConf operator| (Logger::loggerConf __a, 
                                     Logger::loggerConf __b)
{
    return Logger::loggerConf(static_cast<int>(__a) |
                              static_cast<int>(__b));
}

inline Logger::loggerConf operator& (Logger::loggerConf __a,
                                     Logger::loggerConf __b)
{
    return Logger::loggerConf(static_cast<int>(__a) &
                              static_cast<int>(__b)); 
}

#endif /* LOGGER_H */

