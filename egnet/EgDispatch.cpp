#include "EgDispatch.h"
#include "EgConn.h"

namespace eg {

using namespace std;

EgDispatch::EgDispatch() 
{
	_epfd = epoll_create(1024);
	if (_epfd == -1) {
		throw egex("epoll_create failed");
	}
	_thread_pool_size = 2;
	_thread_pool = new EgThreadPool(2);
}

~EgDispatch::EgDispatch()
{
	delete _thread_pool;
}

void EgDispatch::AddEvent(int fd) 
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLPRI | EPOLLERR | EPOLLHUP;
	ev.data.fd = fd;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) != 0) {
		loge("epoll_ctl add event failed, errno=%d", errno);
	}
}

void EgDispatch::RemoveEvent(int fd) 
{
	if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) != 0) {
		loge("epoll_ctl remove event failed, errno=%d", errno);
	}
}

void EgDispatch::AddTask(function<void()> task)
{
	_task_list.push_back(task);
}

void EgDispatch::_CheckTask()
{
	while (_task_list.empty()) {
		auto task = _task_list.front();
		task();
		_task_list.pop_front();
	}
}

void EgDispatch::AddRoutine(function<void()> routine)
{
	_routine_list.push_back(routine);
}

void EgDispatch::_CheckRoutine()
{
	for (auto& routine : _routine_list) {
		routine();
	}
}

void EgDispatch::SetTimeout(uint64_t millisecond, function<void()> callback)
{
	EgTimer* timer = new EgTimer(millisecond, callback);
	_timeout_list.push_back(timer); 
}

void EgDispatch::_CheckTimout()
{
	for (auto it = _timeout_list.begin(); it != _timeout_list.end();) {
		if (gettick() >= (*it)->GetNextTick()) {
			(*it)->OnTimer();
			it = _timeout_list.erase(it)
		} else {
			++it;
		}
	}
}

void EgDispatch::SetInterval(uint64_t millisecond, function<void()> callback)
{
	EgTimer* timer = new EgTimer(millisecond, callback);
	_interval_list.push_back(timer);
}

void EgDispatch::_CheckInterval()
{
	for (auto& e : _interval_list) {
		if (gettick() >= e->GetNextTick()) {
			e->GotoNextTick();
			e->OnTimer();
		}
	}
}

void AddThreadTask(function<(void()>) task)
{
	_thread_pool.AddTask(task);
}

void EgDispatch::Instance()
{
	static EgDispatch dispatch;
	return &dispatch;
}

void EgDispatch::StartLoop()
{
	struct epoll_event events[1024];
	int nfds = 0;

    if(_loop_running)
        return;
    _loop_running = true;
    
    _StartTaskLoop();
    
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
		
		_CheckTask();
		_CheckRoutine();
		_CheckTimout();
		_CheckInterval();
	}
}

void EgDispatch::Connect(string ip, uint16_t port, function<void(sp_EgSock)> callback)
{
	sp_EgSock sock = new EgSock();
	sock->Connect(ip.c_str(), port, callback);
	_sock_map[fd] = sock
}
	
void EgDispatch::Listen(string ip, uint16_t port, function<void(sp_EgSock)> callback)
{
	
}

}