#ifndef __EG_IO__
#define __EG_IO__

#include "EgCommon.h"
#include "EgUtil.h"
#include "EgConn.h"
#include "EgBuffer.h"
#include "EgThread.h"
#include "EgSock.h"

namespace eg {

enum {
	FD_INFO_CONNECTING = 1,
	FD_INFO_LISTENING,
	FD_INFO_CONNECTED,
	FD_INFO_CLOSED
};

template<class F>
class EgTimer {
	uint64_t _interval;
	uint64_t _next_tick;
	F _callback;
public:
	EgTimer(uint64_t interval, F cb) : _interval(interval), _callback(cb) {
		_next_tick = gettick() + interval;
	} 
	
	virtual ~EgTimer() {}
	virtual void OnTimer() {
		_callback();
	}
	uint64_t GetNextTick() { return _next_tick; }
	void GotoNextTick() { _next_tick += _interval; }
};

class EgIo {

	bool _loop_running;
	
	int _epfd;

	std::list<std::function<void()>> _task_list;
	std::list<std::function<void()>> _routine_list;
	std::list<std::function<void()>> _timeout_list;
	std::list<std::function<void()>> _interval_list;

	std::map<int, EgSock*> _sock_map;

public:
	EgIo();
	virtual ~EgIo() {}

	void AddEvent(int fd);
	void RemoveEvent(int fd);

	void StartLoop();
	
	void AddRoutine(std::function<void()>);
	void _CheckRoutine();
	
	void AddTask(std::function<void()>);
	void _CheckTask();
	
	void SetTimeout(uint64_t millisecond, std::function<void()> callback);
	void _CheckTimout();
	
	void SetInterval(uint64_t millisecond, std::function<void()> callback);
	void _CheckInterval();
	
	static EgIo* Instance() {
		static EgIo egio;
		return &egio;
	}

};

}
#endif