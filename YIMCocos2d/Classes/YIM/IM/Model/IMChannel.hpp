//
//  IMChannel.hpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/5.
//
//

#ifndef IMChannel_hpp
#define IMChannel_hpp

#include <stdio.h>
#include <YIM.h>

namespace  YouMe {
    
    struct IMChannel {
        XString channelID;
        
        IMChannel( const XString& channelID );
    };
}

#endif /* IMChannel_hpp */
