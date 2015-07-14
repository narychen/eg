#ifndef __EG_IM_CONN__
#define __EG_IM_CONN__

#include "EgCommon.h"
#include "EgConn.h"

namespace eg {

class EgImConn : public EgConn {
    virtual void HandleReadData();
};

}

#endif