#ifndef __EG_UTIL__
#define __EG_UTIL__

#include "EgCommon.h"
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

#define logi(fmt, args...) eg::EgLog::Logi("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define logd(fmt, args...) eg::EgLog::Logd("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define logw(fmt, args...) eg::EgLog::Logw("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define loge(fmt, args...) eg::EgLog::Loge("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define logt(fmt, args...) eg::EgLog::Logt("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)

#define egex(fmt, args...)  eg::EgException("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)

namespace eg {

using namespace std;

void split(string s, vector<string>& ret);
void split(string s, vector<string>& ret, char sep);
uint64_t gettick();
void sleep(uint32_t millisecond);

class EgException : public std::exception
{
	std::string _why;
public:
	EgException(const char* why, ...) {
	    va_list args;
	    va_start(args, why);
	    char szBuffer[4096];
	    vsnprintf(szBuffer, sizeof(szBuffer), why, args);
	    va_end(args);
	    _why = szBuffer;
	}
	virtual const char* what() const throw() {
		return _why.c_str();
	}
};

using namespace log4cxx;

#define MAX_LOG_LENGTH   1024 * 10
#define WATCH_DELAY_TIME     10 * 1000

class EgLog 
{
    LoggerPtr _logger;
public:
    EgLog(const char* module_name) {
        PropertyConfigurator::configureAndWatch("log4cxx.properties", WATCH_DELAY_TIME);
        _logger = Logger::getLogger(module_name);
    }
    
    virtual ~EgLog() {}
    
    void Trace(const char* loginfo) { _logger->trace(loginfo); }
    void Debug(const char* loginfo) { _logger->debug(loginfo); }
    void Info(const char* loginfo) { _logger->info(loginfo); }
    void Warn(const char* loginfo) { _logger->warn(loginfo); }
    void Error(const char* loginfo) { _logger->error(loginfo); }
    void Fatal(const char* loginfo) { _logger->fatal(loginfo); }
    
    static void Logi(const char *format, ...) {
        static EgLog s_info("INFO");
        va_list args;
        va_start(args, format);
        char szBuffer[MAX_LOG_LENGTH];
        vsnprintf(szBuffer, sizeof(szBuffer), format, args);
        va_end(args);
        s_info.Info(szBuffer);
    }
    
    static void Loge(const char *format, ...) {
        static EgLog s_error("ERROR");
        va_list args;
        va_start(args, format);
        char szBuffer[MAX_LOG_LENGTH];
        vsnprintf(szBuffer, sizeof(szBuffer), format, args);
        va_end(args);
        s_error.Error(szBuffer);
    }
    
    static void Logd(const char *format, ...) {
        static EgLog s_debug("DEBUG");
        va_list args;
        va_start(args, format);
        char szBuffer[MAX_LOG_LENGTH];
        vsnprintf(szBuffer, sizeof(szBuffer), format, args);
        va_end(args);
        s_debug.Debug(szBuffer);
    }    

    static void Logw(const char *format, ...) {
        static EgLog s_warn("WARN");
        va_list args;
        va_start(args, format);
        char szBuffer[MAX_LOG_LENGTH];
        vsnprintf(szBuffer, sizeof(szBuffer), format, args);
        va_end(args);
        s_warn.Warn(szBuffer);
    } 

    static void Logt(const char *format, ...) {
        static EgLog s_trace("TRACE");
        va_list args;
        va_start(args, format);
        char szBuffer[MAX_LOG_LENGTH];
        vsnprintf(szBuffer, sizeof(szBuffer), format, args);
        va_end(args);
        s_trace.Trace(szBuffer);
    }     
};


class EgTimer {
	uint64_t _interval;
	uint64_t _next_tick;
public:
	EgTimer(uint64_t interval) : _interval(interval) {
		_next_tick = eg::gettick() + interval;
	}
	virtual ~EgTimer() {}
	virtual void OnTimer() = 0;
	uint64_t GetNextTick() { return _next_tick; }
	void GotoNextTick() { _next_tick += _interval; }
};

}

#endif