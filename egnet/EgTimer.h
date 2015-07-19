#ifndef __EG_TIMER__
#define __EG_TIMER__

#include "EgCommon.h"
namespace eg {

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

}
#endif