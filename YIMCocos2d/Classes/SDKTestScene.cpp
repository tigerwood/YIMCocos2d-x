#include "SDKTestScene.h"
#include "SimpleAudioEngine.h"

#include <IM/IMClient.hpp>
#include <iostream>
using namespace std;

USING_NS_CC;

using namespace YouMe;

string g_roomName;
string g_userID;



Scene* SDKTest::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SDKTest::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SDKTest::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto curTime = time(0);
    int userID = curTime % 1000;
    stringstream ss;
    ss<<"u"<<userID;
    g_userID = ss.str();
    g_roomName = "12345";

    
    Config  config;
    config.serverZone = ServerZone::ServerZone_China;
    IMClient::getInstance()->Initialize("YOUME5BE427937AF216E88E0F84C0EF148BD29B691556",
                                        "y1sepDnrmgatu/G8rx1nIKglCclvuA5tAvC0vXwlfZKOvPZfaUYOTkfAdUUtbziW8Z4HrsgpJtmV/RqhacllbXD3abvuXIBlrknqP+Bith9OHazsC1X96b3Inii6J7Und0/KaGf3xEzWx/t1E1SbdrbmBJ01D1mwn50O/9V0820BAAE=",
                                        &config);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    loadui();
    
    //loaduimsg();
    
    loadLocation();
    
    IMClient::getInstance()->SetKickOffListener( [](const KickOffEvent& evt ){
        cout<<"踢线回调:"<<endl;
        cout<<evt.code<<endl;
        cout<<evt.userID<<endl;

    });
    
    IMClient::getInstance()->SetDisconnectListener( [](const DisconnectEvent& evt ){
        cout<<"掉线回调:"<<endl;
        cout<<evt.code<<endl;
        cout<<evt.userID<<endl;
        
    });
    
    IMClient::getInstance()->SetReceiveMessageListener( []( std::shared_ptr<IMMessage> msg ){
        switch ( msg->messageType) {
            case MessageBodyType_TXT:
            {
                TextMessage& textMsg = (TextMessage&)msg;
                
                cout<<"接收文本消息回调"<<textMsg.content<<endl;
                cout<<textMsg.senderID<<","<<textMsg.receiverID<<","<<textMsg.sendTime<<","<<textMsg.sendStatus<<","<<textMsg.chatType<<","<<textMsg.requestID<<endl;
               
            }
                break;
            case MessageBodyType_Voice:
            {
                AudioMessage& audioMsg = (AudioMessage&)msg;
                cout<<"接受语音消息回调"<<endl;
                cout<<"时长:"<<audioMsg.audioDuration<<endl;
                cout<<"Sending:"<<audioMsg.sendStatus<<endl;
                cout<<"附加信息:"<<audioMsg.extraParam<<endl;
                cout<<"识别文本:"<<audioMsg.recognizedText<<endl;
            }
                break;
            default:
                break;
        }
    });
    
    return true;
}

void SDKTest::onBtnLogin( Ref* pSender ){
    g_userID = idInput->getText();
    
    IMClient::getInstance()->Login( g_userID.c_str(), "", [](const LoginEvent&  evt ){
        cout<<"登录回调:"<<endl;
        cout<<evt.code<<endl;
        cout<<evt.userID<<endl;
        
        IMUser user = IMClient::getInstance()->GetCurrentUserID();
        string str = user.userID;
        cout<<"当前用户ID:"<<IMClient::getInstance()->GetCurrentUserID().userID.c_str() <<endl;
    });
    
}
void SDKTest::onBtnLogout( Ref* pSender ){
    IMClient::getInstance()->Logout( [](const LogoutEvent&  evt ){
        cout<<"登出回调:"<<endl;
        cout<<evt.code<<endl;
        cout<<evt.userID<<endl;
        
        string user = IMClient::getInstance()->GetCurrentUserID().userID;
        cout<<"当前用户ID:"<<user<<endl;
    });
}
void SDKTest::onBtnEnter( Ref* pSender ){
    g_roomName = roomInput->getText();
    
    IMClient::getInstance()->JoinChannel( g_roomName.c_str(),  [](const ChannelEvent& evt ){
        cout<<"进入房间回调"<<endl;
        cout<<evt.code<<endl;
        cout<<evt.channelID<<endl;
        cout<<evt.eventType<<endl;
    });
    
}
void SDKTest::onBtnLeave( Ref* pSender ){
    g_roomName = roomInput->getText();
    
    IMClient::getInstance()->LeaveChannel( g_roomName.c_str(),  [](const ChannelEvent& evt ){
        cout<<"离开房间回调"<<endl;
        cout<<evt.code<<endl;
        cout<<evt.channelID<<endl;
        cout<<evt.eventType<<endl;
        
        
    });
}

void SDKTest::onBtnSendText( Ref* pSender){
    IMClient::getInstance()->SendTextMessage( g_userID.c_str(), ChatType::PrivateChat, "I'm Pinky!oo!", [](StatusCode code, std::shared_ptr<IMMessage> msg ){
        if( msg != NULL ){
            TextMessage& textMsg = (TextMessage&)(*msg.get());
            
            cout<<"发送文本消息回调"<<code<<":"<<textMsg.content<<endl;
            cout<<textMsg.senderID<<","<<textMsg.receiverID<<","<<textMsg.sendTime<<","<<textMsg.sendStatus<<","<<textMsg.chatType<<","<<textMsg.requestID<<endl;
        }
    });
}

void SDKTest::onBtnStartAudio( Ref* pSender ){
    IMClient::getInstance()->StartRecordAudio( g_userID.c_str(), ChatType::PrivateChat, "Test Start Audio",  true , [](StatusCode code, std::shared_ptr<IMMessage> msg ){
        if( msg != NULL ){
            AudioMessage& audioMsg = (AudioMessage&)(*msg.get());
            
            cout<<"发送语音消息"<<code<<":"<<endl;
            cout<<"时长:"<<audioMsg.audioDuration<<endl;
            cout<<"Sending:"<<audioMsg.sendStatus<<endl;
            cout<<"附加信息:"<<audioMsg.extraParam<<endl;
            cout<<"识别文本:"<<audioMsg.recognizedText<<endl;
            cout<<"本地路径:"<<audioMsg.audioFilePath<<endl;
        }
       
    });
    
}
void SDKTest::onBtnStopAudio( Ref* pSender ){
    IMClient::getInstance()->StopRecordAndSendAudio();
    
}

void SDKTest::loadui(){
    idInput = cocos2d::extension::EditBox::create( Size( 200, 80 ), "chat_bottom_textfield.png");
    idInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
    idInput->setAnchorPoint( Vec2(0,0) );
    idInput->setFontColor( Color3B::BLACK );
    idInput->setFontSize( 50  );
    idInput->setPosition( Vec2( 0, 680 ) );
    addChild( idInput );
    idInput->setText( g_userID.c_str() );
    
    roomInput = cocos2d::extension::EditBox::create( Size(  200, 80 ), "chat_bottom_textfield.png");
    roomInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
    roomInput->setAnchorPoint( Vec2(0,0) );
    roomInput->setFontColor( Color3B::BLACK );
    roomInput->setFontSize( 50  );
    roomInput->setPosition( Vec2(  250, 680 ) );
    addChild( roomInput );
    roomInput->setText( g_roomName.c_str() );
    
    auto btnLogin = Button::create();
    btnLogin->setTitleFontSize( 80 );
    btnLogin->setTitleText("登录");
    btnLogin->setPosition( Vec2( 500,  680  ) );
    btnLogin->setAnchorPoint( Vec2(0, 0) );
    btnLogin->addClickEventListener( CC_CALLBACK_1( SDKTest::onBtnLogin, this  ) );
    addChild( btnLogin );
    
    auto btnLogout = Button::create();
    btnLogout->setTitleFontSize( 80 );
    btnLogout->setTitleText("登出");
    btnLogout->setPosition( Vec2( 700,  680  ) );
    btnLogout->setAnchorPoint( Vec2(0, 0) );
    btnLogout->addClickEventListener( CC_CALLBACK_1( SDKTest::onBtnLogout, this  ) );
    addChild( btnLogout );

    auto btnEnterRoom = Button::create();
    btnEnterRoom->setTitleFontSize( 80 );
    btnEnterRoom->setTitleText("进入");
    btnEnterRoom->setPosition( Vec2( 500,  580  ) );
    btnEnterRoom->setAnchorPoint( Vec2(0, 0) );
    btnEnterRoom->addClickEventListener( CC_CALLBACK_1( SDKTest::onBtnEnter, this  ) );
    addChild( btnEnterRoom );
    
    auto btnLeaveRoom = Button::create();
    btnLeaveRoom->setTitleFontSize( 80 );
    btnLeaveRoom->setTitleText("退出");
    btnLeaveRoom->setPosition( Vec2( 700,  580  ) );
    btnLeaveRoom->setAnchorPoint( Vec2(0, 0) );
    btnLeaveRoom->addClickEventListener( CC_CALLBACK_1( SDKTest::onBtnLeave, this  ) );
    addChild( btnLeaveRoom );


    
    labelTips = Label::createWithSystemFont("", "Arial", 24);
    labelTips->setPosition( 0 , 0 );
    labelTips->setAnchorPoint( Vec2(0,0)) ;
    labelTips->setHorizontalAlignment( TextHAlignment::LEFT  );
    labelTips->setVerticalAlignment( TextVAlignment::TOP );
    labelTips->setWidth( 1000 );
    addChild( labelTips );

}

void SDKTest::loaduimsg(){
    auto btnText = Button::create();
    btnText->setTitleFontSize( 80 );
    btnText->setTitleText("发送文本");
    btnText->setPosition( Vec2( 100,  500  ) );
    btnText->setAnchorPoint( Vec2(0, 0) );
    btnText->addClickEventListener( CC_CALLBACK_1( SDKTest::onBtnSendText, this  ) );
    addChild( btnText );
    
    auto btnStartAudio = Button::create();
    btnStartAudio->setTitleFontSize( 80 );
    btnStartAudio->setTitleText("开始语音");
    btnStartAudio->setPosition( Vec2( 400,  500  ) );
    btnStartAudio->setAnchorPoint( Vec2(0, 0) );
    btnStartAudio->addClickEventListener( CC_CALLBACK_1( SDKTest::onBtnStartAudio, this  ) );
    addChild( btnStartAudio );
    
    auto btnStopAudio = Button::create();
    btnStopAudio->setTitleFontSize( 80 );
    btnStopAudio->setTitleText("停止语音");
    btnStopAudio->setPosition( Vec2( 800,  500  ) );
    btnStopAudio->setAnchorPoint( Vec2(0, 0) );
    btnStopAudio->addClickEventListener( CC_CALLBACK_1( SDKTest::onBtnStopAudio, this  ) );
    addChild( btnStopAudio );

    
}

void SDKTest::onBtnUpdateLocation( Ref* pSender ){
    IMClient::getInstance()->SetUpdateInterval( 30 );
    
    IMClient::getInstance()->GetCurrentLocation( [](StatusCode code , std::shared_ptr<GeographyLocation> location ){
        cout<<"更新位置回调:err:"<<code<<endl;
        if( code == StatusCode::Success ){
            cout<<"经纬度："<<location->GetLatitude()<<","<<location->GetLongitude()<<endl;
            cout<<"行政区："<<location->GetCountry()<<":"<<location->GetProvince()<<","<<location->GetCity()<<endl;
            cout<<location->GetDistrictCounty()<<","<<location->GetStreet()<<endl;
            cout<<"编码:"<<location->GetDistrictCode()<<endl;
        }
     });
}

void SDKTest::onBtnGetNearby( Ref* pSender  ){
    IMClient::getInstance()->GetNearbyObjects( 10 , "", DISTRICT_UNKNOW, false,
                    [](StatusCode code , std::list< std::shared_ptr<RelativeLocation> > neighbourList,  unsigned int startDistance, unsigned int endDistance){
                        cout<<"获取附近的人回调:err:"<<code<<",count:"<<neighbourList.size()<<endl;
                        cout<<"start:"<<startDistance<<",end:"<<endDistance<<endl;
                        
                        auto it = neighbourList.begin();
                        for(; it!= neighbourList.end(); ++it ){
                            auto location = *it;
                            cout<<"用户ID："<<location->GetUserID()<<"........................."<<endl;
                            cout<<"经纬度："<<location->GetLatitude()<<","<<location->GetLongitude()<<endl;
                            cout<<"行政区："<<location->GetCountry()<<":"<<location->GetProvince()<<","<<location->GetCity()<<endl;
                            cout<<location->GetDistrictCounty()<<","<<location->GetStreet()<<endl;
                            cout<<"距离:"<<location->GetDistance()<<endl;

                        }
                        
    });
    
}

void SDKTest::loadLocation(){
    auto node = Node::create();
    node->setPosition(0 , 0 );
    this->addChild( node );
    
    
    auto btnUpdate = Button::create();
    btnUpdate->setTitleFontSize( 80 );
    btnUpdate->setTitleText("更新位置");
    btnUpdate->setPosition( Vec2( 100,  400  ) );
    btnUpdate->setAnchorPoint( Vec2(0, 0) );
    btnUpdate->addClickEventListener( CC_CALLBACK_1( SDKTest::onBtnUpdateLocation, this  ) );
    node->addChild( btnUpdate );
    
    auto btnGetNearby = Button::create();
    btnGetNearby->setTitleFontSize( 80 );
    btnGetNearby->setTitleText("附近的人");
    btnGetNearby->setPosition( Vec2( 400,  400  ) );
    btnGetNearby->setAnchorPoint( Vec2(0, 0) );
    btnGetNearby->addClickEventListener( CC_CALLBACK_1( SDKTest::onBtnGetNearby, this  ) );
    node->addChild( btnGetNearby );
}



