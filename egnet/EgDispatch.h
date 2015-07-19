#ifndef __EG_IO__
#define __EG_IO__

#include <functional>
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

class EgDispatch {

	bool _loop_running;
	
	int _epfd;
	int _thread_pool_size;
	EgThreadPool _thread_pool;

	std::list<std::function<void()>> _task_list;
	std::list<std::function<void()>> _routine_list;
	std::list<std::function<void()>> _timeout_list;
	std::list<std::function<void()>> _interval_list;

	std::map<int, sp_EgSock> _sock_map;

public:
	EgDispatch();
	virtual ~EgDispatch() {}

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
	
	void AddThreadTask(std::function<void()> callback);
	void SetThreadPoolSize(int size) { _thread_pool_size = size; }
	
	void Connect(string ip, uint16_t port, std::function<void()> callback);
	void Listen(string ip, uint16_t port, std::function<void()> callback);
	
	static EgDispatch* Instance();

};

}
#endif