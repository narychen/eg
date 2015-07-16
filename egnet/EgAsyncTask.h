#ifndef __EG_ASYNC_TASK__
#define __EG_ASYNC_TASK__

#include "EgCommon.h"

namespace eg {

struct EgTask {
    virtual void operator() () = 0;
};

class EgTimer {
	uint64_t _interval;
	uint64_t _next_tick;
	EgTask* _task;
public:
	EgTimer(uint64_t interval, EgTask* task) : _interval(interval), _task(task) {
		_next_tick = gettick() + interval;
	} 
	
	virtual ~EgTimer() {}
	virtual void OnTimer() {
		(*_task)();
	}
	uint64_t GetNextTick() { return _next_tick; }
	void GotoNextTick() { _next_tick += _interval; }
};

class EgThreadNotify
{
	pthread_mutex_t 	_mutex;
	pthread_mutexattr_t	_mutexattr;
	pthread_cond_t 		_cond;
public:
	EgThreadNotify() {
		pthread_mutexattr_init(&_mutexattr);
		pthread_mutexattr_settype(&_mutexattr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&_mutex, &_mutexattr);
		pthread_cond_init(&_cond, nullptr);
	}
	~EgThreadNotify() {
		pthread_mutexattr_destroy(&m_mutexattr);
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
	}
	void Lock() { pthread_mutex_lock(&_mutex); }
	void Unlock() { pthread_mutex_unlock(&_mutex); }
	void Wait() { pthread_cond_wait(&_cond, &_mutex); }
	void Signal() { pthread_cond_signal(&_cond); }
};

class EgWorkerThead
{
	uint64_t	    _task_cnt;
	pthread_t		_thread_id;
	EgThreadNotify	_thread_notify;
	list<EgTask*>	_task_list;    
	
public:
    EgWorkerThead() {
    	(void)pthread_create(&_thread_id, nullptr, StartRoutine, this);
    }
    ~EgWorkerThead();
    
	static void* StartRoutine(void* arg) {
	    EgWorkerThead* pThread = (EgWorkerThead*)arg;
    	pThread->Execute();
    	return nullptr;
	}

	
	void Execute() {
	    while (true) {
    		_thread_notify.Lock();
        	// put wait in while cause there can be spurious wake up (due to signal/ENITR)
    		while (_task_list.empty()) {
    			_thread_notify.Wait();
    		}
    		EgTask* pTask = _task_list.front();
    		_task_list.pop_front();
    		_thread_notify.Unlock();
    		(*pTask)();
    		delete pTask;
    		_task_cnt++;
        }
	}
	
	void PushTask(EgTask* pTask) {
	    _thread_notify.Lock();
    	_task_list.push_back(pTask);
    	_thread_notify.Signal();
    	_thread_notify.Unlock();
	}

};


class EgThreadPool {
	uint32_t 		    _worker_size;
	EgWorkerThread* 	_worker_list;
public:
	EgThreadPool(uint32_t worker_size) {
	    _worker_size = worker_size;
    	_worker_list = new EgWorkerThread [_worker_size];
	}
	
	~EgThreadPool(){
	    if(_worker_list)
            delete [] _worker_list;
	}

	void AddTask(EgTask* pTask) {
        /*
        * select a random thread to push task
        * we can also select a thread that has less task to do
        * but that will scan the whole thread list and use thread lock to get each task size
        */
        uint32_t thread_idx = random() % _worker_size;
        _worker_list[thread_idx].PushTask(pTask);
	}
};

}
#endif