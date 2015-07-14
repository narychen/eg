#ifndef __EG_COMMON__
#define __EG_COMMON__

#include <sys/epoll.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>		// define int8_t ...
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <map>
#include <memory>
#include <set>

typedef unsigned char	uchar_t;

namespace eg { 
    
#define SERVER_HEARTBEAT_INTERVAL	5000
#define SERVER_TIMEOUT				30000
#define CLIENT_HEARTBEAT_INTERVAL	30000
#define CLIENT_TIMEOUT				120000
#define MOBILE_CLIENT_TIMEOUT       60000 * 5
#define READ_BUF_SIZE	2048

#define EG_PDU_HEADER_LEN		16
#define EG_PDU_VERSION			1

const int SOCKET_ERROR	= -1;
const int INVALID_SOCKET = -1;

}
#endif