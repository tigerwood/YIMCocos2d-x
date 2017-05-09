//
//  ChannelEvent.cpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/5.
//
//

#include "ChannelEvent.h"

namespace  YouMe {
    
    ChannelEvent::ChannelEvent( StatusCode code, ChannelEventType eventType, const XString& channelID ){
        this->code = code;
        this->eventType = eventType;
        this->channelID = channelID;
    }
}
