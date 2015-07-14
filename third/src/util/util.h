#ifndef __UTIL_H__
#define __UTIL_H__

#define _CRT_SECURE_NO_DEPRECATE	// remove warning C4996, 
#include <cxxabi.h>
#include <execinfo.h>
#include <signal.h>
#include "ostype.h"
#include "UtilPdu.h"
#include "Lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slog_api.h"
#ifndef _WIN32
#include <strings.h>
#endif

#include <sys/stat.h>
#include <assert.h>


#ifdef _WIN32
#define	snprintf	sprintf_s
#else
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#endif

#define NOTUSED_ARG(v) ((void)v)		// used this to remove warning C4100, unreferenced parameter

/// yunfan modify end 
class CRefObject
{
public:
	CRefObject();
	virtual ~CRefObject();

	void SetLock(CLock* lock) { m_lock = lock; }
	void AddRef();
	void ReleaseRef();
private:
	int				m_refCount;
	CLock*	m_lock;
};


extern CSLog g_iminfo;
extern CSLog g_imdebug;
extern CSLog g_imerror;
extern CSLog g_imwarn;
extern CSLog g_imtrace;

// Add By ZhangYuanhao 2015-01-14 For log show the file name not the full path + filename
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#if defined(_WIN32) || defined(_WIN64)
#define log(fmt, ...)  g_iminfo.Info("<%s>\t<%d>\t<%s>,"fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define log(fmt, args...)  g_iminfo.Info("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define logd(fmt, args...) g_imdebug.Debug("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define logw(fmt, args...) g_imwarn.Warn("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define loge(fmt, args...) g_imerror.Error("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#define logt(fmt, args...) g_imtrace.Trace("<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##args)
#endif
//#define log(fmt, ...)  g_imlog.Info("<%s>\t<%d>\t<%s>,"+fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

uint64_t get_tick_count();
void util_sleep(uint32_t millisecond);


class CStrExplode
{
public:
	CStrExplode(char* str, char seperator);
	virtual ~CStrExplode();

	uint32_t GetItemCnt() { return m_item_cnt; }
	char* GetItem(uint32_t idx) { return m_item_list[idx]; }
private:
	uint32_t	m_item_cnt;
	char** 		m_item_list;
};

char* replaceStr(char* pSrc, char oldChar, char newChar);
string int2string(uint32_t user_id);
uint32_t string2int(const string& value);
void replace_mark(string& str, string& new_value, uint32_t& begin_pos);
void replace_mark(string& str, uint32_t new_value, uint32_t& begin_pos);

void writePid();
inline unsigned char toHex(const unsigned char &x);
inline unsigned char fromHex(const unsigned char &x);
string URLEncode(const string &sIn);
string URLDecode(const string &sIn);


int64_t get_file_size(const char *path);
const char*  memfind(const char *src_str,size_t src_len, const char *sub_str, size_t sub_len, bool flag = true);
void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63);

#endif
