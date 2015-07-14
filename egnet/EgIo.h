#ifndef __EG_IO__
#define __EG_IO__

#include "EgCommon.h"
#include "EgUtil.h"
#include "EgConn.h"
#include "EgBuffer.h"

namespace eg {

struct EgIoFactory {
	virtual EgConn* NewConn(int fd) = 0;
};

class EgIo {

	bool _loop_running;
	
	int _epfd;
	
	std::list<EgTimer*> _timer_list;
	
	std::map<int, EgConn*> _conn_map;
	std::map<int, EgIoFactory*> _io_factory_map;
	std::set<int> _listen_socks;
	std::set<int> _connecting_socks;
	std::set<int> _connected_socks;


public:
	EgIo();
	virtual ~EgIo() {}
	
	void Connect(const char* ip, uint16_t port, EgIoFactory* iofac);
	void Listen(const char* ip, uint16_t port, EgIoFactory* conn);
	
	void AddEvent(int fd);
	void RemoveEvent(int fd);
	void OnWrite(int fd);
	void OnClose(int fd);
	void OnRead(int fd);
	void OnAccept(int fd);
	void StartLoop();
	
	void AddTimer(EgTimer* timer) { _timer_list.push_back(timer); }
	void _CheckTimer();
	
	static EgIo* Instance() {
		static EgIo egio;
		return &egio;
	}

	static void SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr);
	static int  CreateSocket();
	static void SetNonblock(int fd);
	static void SetReuseAddr(int fd);
	static void SetNodelay(int fd);
	static void Bind(int fd, const char* ip, uint16_t port);
	
};

}
#endif