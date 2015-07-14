#ifndef __EG_PDU__
#define __EG_PDU__

#include "EgCommon.h"
#include "EgBuffer.h"

namespace eg {

class EgPdu {
	EgBuffer& _buf;
	struct Header{
		uint32_t length;
		uint16_t version;
		uint16_t flag;
		uint16_t service_id;
		uint16_t command_id;
		uint16_t seq_num;
		uint16_t reversed;
	} _header;
	
public:
	EgPdu(EgBuffer& buf) : _buf(buf) {}
	virtual ~EgPdu() {}
	
	void Handle() {
		if (_buf.GetOffset() < EG_PDU_HEADER_LEN)
			return;	
		_buf >> _header.length;
		_buf >> _header.version;
		_buf >> _header.flag;
		_buf >> _header.service_id;
		_buf >> _header.command_id;
		_buf >> _header.seq_num;
		_buf >> _header.reversed;
		
	}
	
	void Parse() {}
};

}

#endif