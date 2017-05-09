//
//  IMConnectEvent.cpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/3.
//
//

#include <stdio.h>
#include "IMConnectEvent.h"
namespace  YouMe {


    
    IMConnectEvent::IMConnectEvent(StatusCode code,ConnectEventType type, const XString& userID){
        this->code = code;
        this->userID = userID;
        this->type = type;
    }
    
    
    ConnectEvent::ConnectEvent( StatusCode code, const XString& userID  ){
        this->code = code;
        this->userID = userID;
    }
    
    
    LoginEvent::LoginEvent( StatusCode code, const XString& userID ):ConnectEvent(code, userID ){
        
    }
    
    LogoutEvent::LogoutEvent( StatusCode code, const XString& userID ):ConnectEvent(code, userID ){
        
    }
    
    KickOffEvent::KickOffEvent( StatusCode code, const XString& userID ):ConnectEvent(code, userID ){
        
    }
    
    DisconnectEvent::DisconnectEvent( StatusCode code, const XString& userID ):ConnectEvent(code, userID ){
        
    }
    
    
    
    
    
    
    

}
