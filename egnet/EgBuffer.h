#ifndef __EG_BUFFER__
#define __EG_BUFFER__

#include "EgCommon.h"
#include "EgUtil.h"

namespace eg {

class EgBuffer {
	char* _pbuf;
	int _size;
	int _offset;
	
public:
	EgBuffer() {
		_pbuf = NULL;
		_size = 0;
		_offset = 0;
	}
	
	virtual ~EgBuffer() {
		_size = 0;
		_offset = 0;
		if (_pbuf) {
			free(_pbuf);
			_pbuf = NULL;
		}
	}
	
	char* GetBuffer() { return _pbuf; }
	const char* GetBuffer() const { return _pbuf; }
	int GetSize() { return _size; }
	int GetOffset() { return _offset; }
	void IncOffset(int len) { _offset += len; }
	
	void AdjustSize(int len) {
		if ( len >= _size - _offset) {
			Extend(len);
		}
	}
	
	char* GetOffsetBuf() { 
		char* p = _pbuf + _offset;	
		return p; 
	}
	
	void Extend(int len) {
        _size = _offset + len;
        _size += _size >> 2;	// increase by 1/4 allocate size
        char* new_buf = (char*)realloc(_pbuf, _size);
        if(new_buf != NULL) {
            _pbuf = new_buf;
        } else {
        	throw egex("realloc failed");
        }
    }
    
   	int Write(const void* pbuf, int len) {
        if (_offset + len >= _size) {
            Extend(len);
        }
        if (pbuf) {
            memcpy(_pbuf + _offset, pbuf, len);
        }
        
        _offset += len;
        return len;
    }
    
    int Read(void* pbuf, int len) {
        if (len > _offset)
            len = _offset;
        if (pbuf)
            memcpy(pbuf, _pbuf, len);
        _offset -= len;
        memmove(_pbuf, _pbuf + len, _offset);
        return len;
    }
    
	void operator >> (int8_t& data) {
	    Read(&data, 1);
	}
	
	void operator >> (uint8_t& data) {
	    Read(&data, 1);
	}

	void operator >> (int16_t& data) {
	    unsigned char buf[2];
	    
	    Read(buf, 2);
	    
	    data = (buf[0] << 8) | buf[1];
	}
	
	void operator >> (uint16_t& data) {
	    unsigned char buf[2];
	    
	    Read(buf, 2);
	    
	    data = (buf[0] << 8) | buf[1];
	}

	void operator >> (int32_t& data) {
	    unsigned char buf[4];
	    
	    Read(buf, 4);
	    
	    data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	}
	
	void operator >> (uint32_t& data) {
	    unsigned char buf[4];
	    
	    Read(buf, 4);
	    
	    data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	}
	
	void operator >> (std::string& s) {
		uint32_t len;
		*this >> len;
		s = std::string(_pbuf, len);
		Read(NULL, len);
	}

	void operator << (int8_t data) {
	    Write(&data, 1);
	}
	
	void operator << (uint8_t data)	{
	    Write(&data, 1);
	}
	
	void operator << (int16_t data) {
	    uchar_t buf[2];
	    buf[0] = static_cast<uchar_t>(data >> 8);
	    buf[1] = static_cast<uchar_t>(data & 0xFF);
	    Write(buf, 2);
	}
	
	void operator << (uint16_t data) {
	    uchar_t buf[2];
	    buf[0] = static_cast<uchar_t>(data >> 8);
	    buf[1] = static_cast<uchar_t>(data & 0xFF);
	    Write(buf, 2);
	}
	
	void operator << (int32_t data) {
	    uchar_t buf[4];
	    buf[0] = static_cast<uchar_t>(data >> 24);
	    buf[1] = static_cast<uchar_t>((data >> 16) & 0xFF);
	    buf[2] = static_cast<uchar_t>((data >> 8) & 0xFF);
	    buf[3] = static_cast<uchar_t>(data & 0xFF);
	    Write(buf, 4);
	}
	
	void operator << (uint32_t data) {
	    uchar_t buf[4];
	    buf[0] = static_cast<uchar_t>(data >> 24);
	    buf[1] = static_cast<uchar_t>((data >> 16) & 0xFF);
	    buf[2] = static_cast<uchar_t>((data >> 8) & 0xFF);
	    buf[3] = static_cast<uchar_t>(data & 0xFF);
	    Write(buf, 4);
	}
	
	void operator << (std::string s) {
		*this << (uint32_t)s.length();
    	Write(s.c_str(), s.length());
	}
	
};

}
#endif