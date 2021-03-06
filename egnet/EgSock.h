#ifndef __EG_SOCK__
#define __EG_SOCK__

#include "EgCommon.h"

namespace eg {

typedef enum {
	EG_SOCK_STATE_CONNECTING = 1,
	EG_SOCK_STATE_LISTENING,
	EG_SOCK_STATE_CONNECTED,
	EG_SOCK_STATE_CLOSING,
	EG_SOCK_STATE_NONE
} sock_state_t;

typedef enum {
	EG_SOCK_EVENT_READ = 1,
	EG_SOCK_EVENT_WRITE,
	EG_SOCK_EVENT_CLOSE,
	EG_SOCK_EVENT_CONFIRM,
	EG_SOCK_EVENT_NONE
} sock_event_t;

class EgSock : public enable_shared_from_this<EgSock> {
	sock_state_t _state;
	sock_event_t _event;
	int _fd;
	std::function<void(EgSock*)> _callback;
	string _peer_ip;
	uint16_t _peer_port;
	void _OnWrite();
	void _OnClose();
	void _OnRead();
	void _OnAccept();
public:
	void SetPeerIp(const char* ip) { _peer_ip = ip; }
	string& GetPeerIp() { return _peer_ip; }
	void SetPeerPort(uint16_t port) { _peer_port = port; }
	uint16_t GetPeerPort() { return _peer_port; }
	void SetFd(int fd) { _fd = fd; }
	int GetFd() { return _fd;}

	int Connect(const char* ip, uint16_t port, std::function<void(EgSock*)> cb);
	int Listen(const char* ip, uint16_t port, std::function<void(EgSock*)> cb);

	static void SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr);
	static int  CreateSocket();
	static void SetNonblock(int fd);
	static void SetReuseAddr(int fd);
	static void SetNodelay(int fd);
	static void Bind(int fd, const char* ip, uint16_t port);
	
};

typedef shared_ptr<EgSock> sp_EgSock;

}
#endif