#ifndef __EG_CONN__
#define __EG_CONN__

#include "EgBuffer.h"

namespace eg {

class EgConn {
	
protected:

	uint16_t _peer_port;
	std::string _peer_ip;
	int _sock;
	
	EgBuffer _inbuf;
	uint32_t _recv_bytes;
	uint64_t _last_recv_tick;

public:
	EgConn(int fd) { _sock = fd; }
	virtual ~EgConn() {}
	
	virtual void OnWrite() {}
	virtual void OnClose() {
        logi("Close connection ip=%s port=%d sock=%d", _peer_ip.c_str(), _peer_port, _sock);
	}
	virtual void OnConfirm() {
		logi("Connect ip=%s port=%d sock=%d successful", _peer_ip.c_str(), _peer_port, _sock);
	}
	
	virtual void HandleReadData() = 0;
	
	virtual void OnRead() {
		_Read();
		HandleReadData();
	}
	
	void _Read() {
		for (;;) {
			_inbuf.AdjustSize(READ_BUF_SIZE);
			int ret = recv(_sock, _inbuf.GetOffsetBuf(), READ_BUF_SIZE, 0);
			if (ret <= 0)
				break;
			// if (ret == 0) {
			// 	logi("recv = 0 sock = %d, socket closed", _sock);
			// }
			
			_recv_bytes += ret;
			_inbuf.IncOffset(ret); 
			_last_recv_tick = eg::gettick();
		}
	}
	
	void SetSocket(int sock) { _sock = sock; }
	int GetSocket() { return _sock; }
	
	void SetPeerIp(const char* ip) { _peer_ip = ip; }
	void SetPeerPort(uint16_t port) { _peer_port = port; }
	
	std::string& GetPeerIp() { return _peer_ip; }
	uint16_t GetPeerPort() { return _peer_port; }

};

}

#endif