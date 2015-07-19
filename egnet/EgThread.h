#ifndef __EG_ASYNC_TASK__
#define __EG_ASYNC_TASK__

#include <functional>
#include "EgCommon.h"

namespace eg {

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
		pthread_mutexattr_destroy(&_mutexattr);
		pthread_mutex_destroy(&_mutex);
		pthread_cond_destroy(&_cond);
	}
	void Lock() { pthread_mutex_lock(&_mutex); }
	void Unlock() { pthread_mutex_unlock(&_mutex); }
	void Wait() { pthread_cond_wait(&_cond, &_mutex); }
	void Signal() { pthread_cond_signal(&_cond); }
};

class EgWorkerThread
{
	uint64_t	    _task_cnt;
	pthread_t		_thread_id;
	EgThreadNotify	_thread_notify;
	list<function<void()>>	_task_list;    
	
public:
    EgWorkerThread() {
    	(void)pthread_create(&_thread_id, nullptr, StartRoutine, this);
    }
    ~EgWorkerThread();
    
	static void* StartRoutine(void* arg) {
	    EgWorkerThread* pThread = (EgWorkerThread*)arg;
    	pThread->Execute();
    	return nullptr;
	}
	
	bool IsEmpty() {
		_thread_notify.Lock();
		bool e =  _task_list.empty();
		_thread_notify.Unlock();
		return e;
	}
	
	void Execute() {
	    while (true) {
    		_thread_notify.Lock();
        	// put wait in while cause there can be spurious wake up (due to signal/ENITR)
    		while (_task_list.empty()) {
    			_thread_notify.Wait();
    		}
    		function<void()> task = _task_list.front();
    		_task_list.pop_front();
    		_thread_notify.Unlock();
    		task();
    		_task_cnt++;
        }
	}
	
	void PushTask(function<void()> task) {
	    _thread_notify.Lock();
    	_task_list.push_back(task);
    	_thread_notify.Signal();
    	_thread_notify.Unlock();
	}

};


class EgThreadPool {
	uint32_t _worker_size;
	vector<EgWorkerThread*>	_worker_list;
public:
	EgThreadPool(uint32_t worker_size) {
	    _worker_size = worker_size;
    	_worker_list = vector<EgWorkerThread*>(worker_size, new EgWorkerThread());
	}
	
	~EgThreadPool(){
	    for (auto& i : _worker_list)	delete i;
            _worker_list.clear();
	}

	void AddTask(function<void()> task) {
        /*
        * select a random thread to push task
        * we can also select a thread that has less task to do
        * but that will scan the whole thread list and use thread lock to get each task size
        * just do it !!!
        */
        for (auto& t : _worker_list) {
        	if (t->IsEmpty()) {
        		t->PushTask(task);
        		return;
        	}
        }
        if (_worker_size > 100) {
	        int i = random() % _worker_size;
	        _worker_list[i]->PushTask(task);
        } else {
        	EgWorkerThread* worker = new EgWorkerThread();
        	worker->PushTask(task);
        	_worker_list.push_back(worker);
        }
	}
};

}
#endif