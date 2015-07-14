#ifndef __EG_CLIENT__
#define __EG_CLIENT__

#include "EgCommon.h"
#include "EgIo.h"
#include "EgConn.h"
#include "EgUtil.h"
#include "HttpClient.h"
#include "json/json.h"

namespace eg {
    
class EgClientConn : public EgConn {
public:
    EgClientConn(int fd) : EgConn(fd) {}
    ~EgClientConn() {}
    virtual void HandleReadData() {}
};
    
struct ClientIoFactory : EgIoFactory{
    
    EgConn* NewConn(int fd) {
        EgConn* p = new EgClientConn(fd);
        return p;
    }
    
};

class Client
{
    std::string _login_url;
    std::string _msg_prior_ip;
    std::string _msg_backup_ip;
    uint16_t _msg_port;
    
    void _GetMsgServerUrl() {
    	string resp;
    	CHttpClient ht;
    
        CURLcode nRet = ht.Get("http://127.0.0.1:8080/msg_server", resp);
        if(nRet != CURLE_OK){
            loge("login falied. access url:%s error\n", _login_url.c_str());
            return;
        }
        
        Json::Reader reader;
        Json::Value value;
        if(!reader.parse(resp, value))
        {
            loge("login falied. parse response error:%s\n", resp.c_str());
            return;
        }
    
        uint32_t code = value["code"].asUInt();
        if(code != 0)
        {
            string strMsg = value["msg"].asString();
            loge("login falied. errorMsg:%s\n", strMsg.c_str());
            return;
        }
        _msg_prior_ip = value["priorIP"].asString();
        _msg_backup_ip = value["backupIp"].asString();
        _msg_port = value["port"].asUInt();
    
        logi("priorIP=%s, backIP=%s, port=%d", 
            _msg_prior_ip.c_str(), _msg_backup_ip.c_str(), _msg_port);
    }
    
public:
    Client() {}
    virtual ~Client() {}
    
    virtual void run() {
    	_GetMsgServerUrl();
    	EgIo::Instance()->Connect(_msg_prior_ip.c_str(), _msg_port, new ClientIoFactory());
    }

};

}
#endif