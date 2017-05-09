//
//  IMUser.cpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/5.
//
//

#include "IMUser.hpp"


namespace YouMe {
    
    IMUser::IMUser(){
        this->userID = "";
    }
    IMUser::IMUser( const XString&   userID ){
        this->userID = userID ;
    }
}
