#include "EgSock.h"

namespace eg {

using namespace std;

EgSock::EgSock()
{
	_state = EG_SOCK_STATE_NONE;
	_event = EG_SOCK_EVENT_NONE;
}

void EgSock::Bind(int fd, const char* ip, uint16_t port)
{
	sockaddr_in serv_addr;
	SetAddr(ip, port, &serv_addr);
	if (::bind(fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		loge("bind failed, err_code=%d", errno);
		close(fd);
		throw egex("bind failed, err_code=%d", errno);
	}
}

int EgSock::Listen(const char* ip, uint16_t port, function<void(sp_EgSock)> callback)
{
	_fd = CreateSocket();
	SetReuseAddr(_fd);
	SetNonblock(_fd);
	Bind(_fd, ip, port);

	if (listen(_fd, 64) == SOCKET_ERROR) {
		loge("listen failed, err_code=%d", errno);
		close(_fd);
		throw egex("listen failed, err_code=%d", errno);
	}

	_state = EG_SOCK_LISTENING;
	_callback = callback;
	return _fd;
}

int EgSock::Connect(const char* ip, uint16_t port, function<void(sp_EgSock)> callback) 
{
	_fd = CreateSocket();
	SetNonblock(fd);
	SetNodelay(fd);

	_peer_ip = ip;
	_port = port;
	_state = EG_SOCK_CONNECTING;
	_callback = callback;

	sockaddr_in serv_addr;
	SetAddr(ip, port,  &serv_addr);
	if (connect(_fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		if (!((errno == EINPROGRESS) || (errno == EWOULDBLOCK))) {
			loge("connect failed, err_code=%d", errno);
			close(_fd);
			throw egex("connect failed, errno=%d", errno);
		}
	} else {
		_state = EG_SOCK_CONNECTED;
		callback(shared_from_this());
	}
	return _fd;
}

void EgSock::_OnWrite()
{
	if (_state == EG_SOCK_CONNECTING) {
		_state = EG_SOCK_CONNECTED;
		_event = EG_SOCK_EVENT_CONFIRM;
		_callback(shared_from_this());
	} else {
		_event = EG_SOCK_EVENT_WRITE;
		_callback(shared_from_this());
	}
}

void EgSock::_OnClose() {
	_state = EG_SOCK_STATE_CLOSING;
	_event = EG_SOCK_EVENT_CLOSE;
	_callback(shared_from_this());
	close(fd);
}

void EgSock::_OnRead() 
{
	if (_state == EG_SOCK_STATE_LISTENING) {
		OnAccept();
	} else {
		uint64_t avail;
		if ((ioctl(fd, FIONREAD, &avail) == SOCKET_ERROR) || (avail == 0)) {
		    _event = EG_SOCK_EVENT_CLOSE;
			OnClose(fd);
		} else {
			_event = EG_SOCK_EVENT_READ;
			_callback(shared_from_this());
		}
	}
}

void EgSock::_OnAccept()
{
	int sessionFd = 0;
	sockaddr_in peer_addr;
	socklen_t addr_len = sizeof(sockaddr_in);
	char ip_str[64];
	while((sessionFd = accept(fd, (sockaddr*)&peer_addr, &addr_len)) != INVALID_SOCKET) {
		EgSock* sock = new EgSock();
		uint32_t ip = ntohl(peer_addr.sin_addr.s_addr);
		uint16_t port = ntohs(peer_addr.sin_port);
		snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d", ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
		sock->SetPeerIp(ip_str);
		sock->SetPeerPort(port);
		sock->SetFd(sessionFd);
		sock->SetCallback(_callback);
		SetNodelay(sessionFd);
		SetNonblock(sessionFd);
		EgDispatch::Instance()->AddEvent(sessionFd);
	}
}

void EgIo::SetAddr(const char* ip, const uint16_t port, sockaddr_in* pAddr) 
{
	memset(pAddr, 0, sizeof(sockaddr_in));
	pAddr->sin_family = AF_INET;
	pAddr->sin_port = htons(port);
	pAddr->sin_addr.s_addr = inet_addr(ip);
	if (pAddr->sin_addr.s_addr == INADDR_NONE) {
		hostent* host = gethostbyname(ip);
		if (host == NULL) {
			throw egex("gethostbyname failed, ip=%s", ip);
		}
		pAddr->sin_addr.s_addr = *(uint32_t*)host->h_addr;
	}
}

int EgIo::CreateSocket() 
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		loge("socket failed, err_code=%d", errno);
		throw egex("socket create failed, errno=%d", errno);
	}
	return sock;
}

void EgIo::SetNonblock(int fd)
{
	if (SOCKET_ERROR == fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL))) {
		loge("set nonblock failed, err_code=%d", errno);
		throw egex("set nonblock failed, err_code=%d", errno);
	}
}

void EgIo::SetReuseAddr(int fd) 
{
	int reuse = 1;
	if(SOCKET_ERROR == setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse))){
		loge("set reuse socket failed, err_code=%d", errno);
		throw egex("set reuse socket failed, err_code=%d", errno);
	}
}


void EgIo::SetNodelay(int fd)
{
	int nodelay = 1;
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay)) == SOCKET_ERROR) {
		loge("set nodelay failed, err_code=%d", errno);
		throw egex("set nodelay failed, errno=%d", errno);
	}
}


}