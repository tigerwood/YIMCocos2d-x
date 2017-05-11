#ifndef __SDKTEST_SCENE_H__
#define __SDKTEST_SCENE_H__

#include "cocos2d.h"


#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include <vector>

#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

#include "cocos-ext.h"
using namespace cocos2d::extension;

using namespace std;
class EditBox;

USING_NS_CC;

class SDKTest : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void loadui();
    
    void loaduimsg();
    void loadLocation();
    
    void onBtnLogin( Ref* pSender );
    void onBtnLogout( Ref* pSender );
    void onBtnEnter( Ref* pSender );
    void onBtnLeave( Ref* pSender );
    
    void onBtnSendText( Ref* pSender);
    void onBtnStartAudio( Ref* pSender );
    void onBtnStopAudio( Ref* pSender );
    
    void onBtnUpdateLocation( Ref* pSender );
    void onBtnGetNearby( Ref* pSender  );




    
    cocos2d::Label* labelTips;

    cocos2d::extension::EditBox *  idInput;
    cocos2d::extension::EditBox *  roomInput;



    
    // implement the "static create()" method manually
    CREATE_FUNC(SDKTest);
};

#endif // __SDKTEST_SCENE_H__
