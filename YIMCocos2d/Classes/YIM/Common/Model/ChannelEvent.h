//
//  ChannelEvent.h
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/5.
//
//

#ifndef ChannelEvent_h
#define ChannelEvent_h

#include <YIM.h>
#include "../Const/Const.h"

namespace  YouMe {
    enum ChannelEventType{
        JOIN_SUCCESS,
        LEAVE_SUCCESS,
        JOIN_FAIL,
        LEAVE_FAIL,
    };
    
    class ChannelEvent{
    public:
        StatusCode code;
        ChannelEventType eventType;
        XString channelID;
        
        ChannelEvent( StatusCode code, ChannelEventType eventType, const XString&  channelID );
    };
    
    
}


#endif /* ChannelEvent_h */
