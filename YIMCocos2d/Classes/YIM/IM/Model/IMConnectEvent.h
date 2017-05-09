//
//  IMConnectEvent.h
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/3.
//
//

#ifndef IMConnectEvent_h
#define IMConnectEvent_h

#include <YIM.h>
#include <Common/Const/Const.h>

namespace  YouMe {
    
    enum  ConnectEventType{
        CE_CONNECTED,
        CE_DISCONNECTED,
        CE_CONNECT_FAIL,
        CE_KICKED,
        CE_OFF_LINE //掉线
    };
    
    struct IMConnectEvent{
        StatusCode code;
        ConnectEventType type;
        XString userID;

        IMConnectEvent(StatusCode code,ConnectEventType type, const XString& userID);
    };
    
    
    
    class ConnectEvent{
    public:
        StatusCode code;
        XString userID;
    public:
        ConnectEvent( StatusCode code, const XString& userID );
    };
    
    class LoginEvent : public ConnectEvent{
    public:
        LoginEvent( StatusCode code, const XString& userID );
        
    };
    
    class LogoutEvent: public ConnectEvent{
    public:
        LogoutEvent( StatusCode code, const XString& userID );

        
    };
    
    class KickOffEvent: public ConnectEvent{
    public:
        KickOffEvent( StatusCode code, const XString& userID );

        
    };
    
    class DisconnectEvent: public ConnectEvent{
    public:
        DisconnectEvent( StatusCode code, const XString& userID );

        
    };
    
}

#endif /* IMConnectEvent_h */
