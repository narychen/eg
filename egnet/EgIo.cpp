#include "EgIo.h"
#include "EgConn.h"

namespace eg {

EgIo::EgIo() 
{
	_epfd = epoll_create(1024);
	if (_epfd == -1) {
		throw egex("epoll_create failed");
	}
}

void EgIo::AddEvent(int fd) 
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLPRI | EPOLLERR | EPOLLHUP;
	ev.data.fd = fd;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) != 0) {
		loge("epoll_ctl add event failed, errno=%d", errno);
	}
}

void EgIo::RemoveEvent(int fd) 
{
	if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) != 0) {
		loge("epoll_ctl remove event failed, errno=%d", errno);
	}
}

void EgIo::OnWrite(int fd)
{
	auto it1 = _connecting_socks.find(fd);
	if (it1 != _connecting_socks.end()) {
		_connecting_socks.erase(it1);
		auto it2 = _io_factory_map.find(fd);
		if(it2 != _io_factory_map.end()) {
			_connected_socks.insert(fd);
			EgIoFactory* iofac = it2->second;
			EgConn* conn = iofac->NewConn(fd);
			_conn_map[fd] = conn;
			conn->OnConfirm();
		}
	} else {
		auto it3 = _conn_map.find(fd);
		if(it3 != _conn_map.end()) {
			it3->second->OnWrite();
		}
	}
}

void EgIo::OnClose(int fd) {
	EgConn* conn;
	auto it1 = _conn_map.find(fd);
	if (it1 != _conn_map.end()) {
		conn = it1->second;
		conn->OnClose();
		_conn_map.erase(it1);
		auto it2 = _connected_socks.find(fd);
		if (it2 != _connected_socks.end()) {
			_connected_socks.erase(it2);
		}
	}
	// cuz listen socket is much fewer than connection socket, put it behind
	auto it3 = _listen_socks.find(fd);
	if (it3 != _listen_socks.end()) {
		_listen_socks.erase(fd);
		auto it4 = _io_factory_map.find(fd);
		if (it4 != _io_factory_map.end()) {
			_io_factory_map.erase(it4);
		}
	}
	
	close(fd);
	
}

void EgIo::OnRead(int fd) 
{
	auto it = _listen_socks.find(fd);
	if (it != _listen_socks.end()) {
		OnAccept(fd);
	} else {
		uint64_t avail;
		if ((ioctl(fd, FIONREAD, &avail) == SOCKET_ERROR) || (avail == 0)) {
			OnClose(fd);
		} else {
			auto it2 = _conn_map.find(fd);
			if (it2 != _conn_map.end()) {
				auto conn = it2->second;
				conn->OnRead();
			}
		}
	}
}

void EgIo::OnAccept(int fd)
{
	int sessionFd = 0;
	sockaddr_in peer_addr;
	socklen_t addr_len = sizeof(sockaddr_in);
	char ip_str[64];
	while((sessionFd = accept(fd, (sockaddr*)&peer_addr, &addr_len)) != INVALID_SOCKET) {
		uint32_t ip = ntohl(peer_addr.sin_addr.s_addr);
		uint16_t port = ntohs(peer_addr.sin_port);
		snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d", ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
		
		auto it = _io_factory_map.find(fd);
		if (it != _io_factory_map.end()) {
			auto conn = it->second->NewConn(sessionFd);
			conn->SetPeerIp(ip_str);
			conn->SetPeerPort(port);
			conn->SetSocket(sessionFd);
			
			SetNodelay(sessionFd);
			SetNonblock(sessionFd);
			_conn_map[sessionFd] = conn;
			AddEvent(sessionFd);
		}

	}
}

void EgIo::StartLoop()
{
	struct epoll_event events[1024];
	int nfds = 0;

    if(_loop_running)
        return;
    _loop_running = true;
    
	while (_loop_running) {
		nfds = epoll_wait(_epfd, events, 1024, 100); //default wait timeout 100
		for (int i = 0; i < nfds; i++) {
			int ev_fd = events[i].data.fd;
			
            #ifdef EPOLLRDHUP
            if (events[i].events & EPOLLRDHUP) {
                logt("On Peer Close, socket=%d", ev_fd);
                OnClose(ev_fd);
            }
            #endif

			if (events[i].events & EPOLLIN) {
				logt("OnRead, socket=%d", ev_fd);
				OnRead(ev_fd);
			}

			if (events[i].events & EPOLLOUT) {
				logt("OnWrite, socket=%d", ev_fd);
				OnWrite(ev_fd);
			}

			if (events[i].events & (EPOLLPRI | EPOLLERR | EPOLLHUP)) {
				logt("OnClose, socket=%d", ev_fd);
				OnClose(ev_fd);
			}
		}
		
		_CheckTimer();
// _CheckLoop();
	}
}

void EgIo::_CheckTimer()
{
	for (auto& e : _timer_list) {
		if (eg::gettick() >= e->GetNextTick()) {
			e->GotoNextTick();
			e->OnTimer();
		}
	}
}

void EgIo::Bind(int fd, const char* ip, uint16_t port) {
	sockaddr_in serv_addr;
	SetAddr(ip, port, &serv_addr);
	if (::bind(fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		loge("bind failed, err_code=%d", errno);
		close(fd);
		throw egex("bind failed, err_code=%d", errno);
	}
}

void EgIo::Listen(const char* ip, uint16_t port, EgIoFactory* iofac)
{
	auto _sock = CreateSocket();
	SetReuseAddr(_sock);
	SetNonblock(_sock);
	Bind(_sock, ip, port);

	if (listen(_sock, 64) == SOCKET_ERROR) {
		loge("listen failed, err_code=%d", errno);
		close(_sock);
		throw egex("listen failed, err_code=%d", errno);
	}
	
	_listen_socks.insert(_sock);
	_io_factory_map[_sock] = iofac;
	AddEvent(_sock);
}

void EgIo::Connect(const char* ip, uint16_t port, EgIoFactory* iofac) 
{
	int fd = CreateSocket();
	SetNonblock(fd);
	SetNodelay(fd);
	
	EgConn* conn = iofac->NewConn(fd);
	conn->SetPeerIp(ip);
	conn->SetPeerPort(port);
	conn->SetSocket(fd);

	sockaddr_in serv_addr;
	SetAddr(ip, port,  &serv_addr);
	if (connect(fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		if (!((errno == EINPROGRESS) || (errno == EWOULDBLOCK))) {
			loge("connect failed, err_code=%d", errno);
			close(fd);
			throw egex("connect failed, errno=%d", errno);
		}
	} else {
		_conn_map[fd] = conn;
		_connected_socks.insert(fd);
		conn->OnConfirm();
	}
	_io_factory_map[fd] = iofac;
	_connecting_socks.insert(fd);
	AddEvent(fd);
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