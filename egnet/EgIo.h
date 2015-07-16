#ifndef __EG_IO__
#define __EG_IO__

#include "EgCommon.h"
#include "EgUtil.h"
#include "EgConn.h"
#include "EgBuffer.h"
#include "EgAsyncTask.h"

namespace eg {

struct EgIoFactory {
	virtual EgConn* NewConn(int fd) = 0;
};

enum {
	FD_INFO_CONNECTING = 1,
	FD_INFO_LISTENING,
	FD_INFO_CONNECTED,
	FD_INFO_CLOSED
};


struct EgFd;
typedef void (*void_f_egfd)(EgFd*);


template <class T1>
class EgIoLoop {

	bool _loop_running;
	
	int _epfd;
	
	std::map<int, EgConn*> _conn_map;
	std::map<int, EgIoFactory*> _io_factory_map;
	std::set<int> _listen_socks;
	std::set<int> _connecting_socks;
	std::set<int> _connected_socks;
	
	std::list<EgTask*> _task_list;
	std::list<EgTask*> _routine_list;
	std::list<EgTimer*> _timeout_list;
	std::list<EgTimer*> _interval_list;
	
	std:map<int, EgIoFd<void_f_int>*> _sock_fd_map;
	
	std::map<int, EgSock*> _sock_map;

public:
	EgIo();
	virtual ~EgIo() {}
	
	// void Connect(const char* ip, uint16_t port, EgIoFactory* iofac);
	// void Listen(const char* ip, uint16_t port, EgIoFactory* conn);


	void AddEvent(int fd);
	void RemoveEvent(int fd);

	void StartLoop();
	
	void AddTimer(EgTimer* timer) { _timer_list.push_back(timer); }
	void _CheckTimer();
	
	void AddRoutine(EgTask* routine) { _routine_list.push_back(routine); }
	void _CheckRoutine();
	
	void AddTask(EgTask* task) { _task_list.push_back(task); }
	
	static EgIo* Instance() {
		static EgIo egio;
		return &egio;
	}

};

}
#endif