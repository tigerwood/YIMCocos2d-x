//
//  IMUser.hpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/5.
//
//

#ifndef IMUser_hpp
#define IMUser_hpp

#include <stdio.h>
#include <YIM.h>

namespace YouMe {
    struct IMUser{
        XString userID;
        
        IMUser( const XString&  userID );
        IMUser();
    };
}

#endif /* IMUser_hpp */
