
#include "EgUtil.h"

namespace eg {
    
using namespace std;

void split(string s, vector<string>& ret)
{
	string element;

	for (auto& it : s){
		if (it == ' ' || it == '\t') {
			if (!element.empty()) {
				ret.push_back(element);
				element.clear();
			}
		} else {
			element += it;
		}
	}
	// put the last one
	if (!element.empty())
		ret.push_back(element);
}

void split(string s, vector<string>& ret, char sep)
{
	string element;

	for (auto& it : s) {
		if (it == sep) {
			if (!element.empty()) {
				ret.push_back(element);
				element.clear();
			}
		} else {
			element += it;
		}
	}
	// put the last one
	if (!element.empty())
		ret.push_back(element);
}

uint64_t gettick()
{
	struct timeval tval;
	uint64_t ret_tick;

	gettimeofday(&tval, NULL);

	ret_tick = tval.tv_sec * 1000L + tval.tv_usec / 1000L;
	return ret_tick;
}

void sleep(uint32_t millisecond)
{
	usleep(millisecond * 1000);
}

}