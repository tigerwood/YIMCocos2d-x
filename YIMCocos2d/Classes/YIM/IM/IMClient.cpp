//
//  IMClient.cpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/3.
//
//

#include "IMClient.hpp"
#include <IM/IMService/IMInternalManager.hpp>
#include <IM/Utils/Conv.hpp>
#include <IM/Model/IMConnectEvent.h>

#include <iostream>
using namespace std;


MessageCallbackObject::MessageCallbackObject( IMMessage* msg, YIMMessageBodyType msgtype, const MessageCallback& callback ){
    this->message = msg;
    this->msgType = msgtype;
    this->callback = callback;
}

MessageCallbackObject::MessageCallbackObject(){
    this->message = NULL;
    this->msgType = YIMMessageBodyType::MessageBodyType_Unknow;
    this->callback = NULL;
}

namespace  YouMe {
    
    IMClient* IMClient::s_ins = NULL;
    
    IMClient* IMClient::getInstance(){
        if( s_ins == NULL ){
            s_ins = new IMClient;
        }
        return s_ins;
    }
    
    IMClient::IMClient(){
        m_imMgr = IMInternalManager::getInstance();
        connectListener = std::bind( &IMClient::OnConnect, this, std::placeholders::_1  );
        channelEventListener = std::bind(&IMClient::OnChannelEvent, this, std::placeholders::_1 );
    }
    
    
    void IMClient::Initialize( const XCHAR* appKey, const XCHAR* appSecurity, const Config* config ){
        if( config != NULL ){
            m_imMgr->IMManager()->SetServerZone( config->serverZone );
        }
        
        m_imMgr->IMManager()->Init( appKey , appSecurity , "");
        //todo
        
        return;
    }
    
    void IMClient::Login( const XCHAR* userID,const XCHAR* token, const LoginCallback& callback  ){
        loginCallback = callback;
        
        auto code = m_imMgr->IMManager()->Login( userID, FAKE_PAPSSWORD.c_str(),  token );
        
        if( code != YIMErrorcode_Success && connectListener != NULL ){
            IMConnectEvent evt( Conv::ErrorCodeConvert(code) , CE_CONNECT_FAIL,userID);
            connectListener( evt );
        }
    }
    
    void IMClient::Logout(  const LogoutCallback& callback  ){
        logoutCallback = callback;
        
        auto code = m_imMgr->IMManager()->Logout();
        if( code != YIMErrorcode_Success && connectListener != NULL ){
            IMConnectEvent evt( Conv::ErrorCodeConvert(code) , CE_DISCONNECTED , __XT(""));
            connectListener( evt );
        }
    }
    
    void IMClient::SetKickOffListener( const KickOffCallback& callback ){
        kickoffCallback = callback;
    }
    
    void IMClient::SetDisconnectListener( const DisconnectCallback& callback ){
        disconnectCallback = callback;
    }
    
    void IMClient::SetReceiveMessageListener( const ReceiveMessageListener& callback ){
        receiveMessageListener = callback;
    }
    
    std::vector<IMChannel>& IMClient::GetCurrentChannels(){
        //todo: 什么时候加进来，什么时候移出去，需要决定
        return m_vecChannels;
    }
    
    ConnectStatus IMClient::GetCurrentStatus(){
        //todo
        return ConnectStatus::CONNECTED;
    }
    
    IMUser IMClient::GetCurrentUserID(){
        return m_imMgr->m_lastLoginUser;
    }
    
    void IMClient::JoinChannel( const XString& channel,  const JoinChannelCallback& callback ){
        joinChannelCallback = callback;
        
        auto code = m_imMgr->IMManager()->GetChatRoomManager()->JoinChatRoom( channel.c_str() );
        if( code != YIMErrorcode_Success && joinChannelCallback != NULL ){
            ChannelEvent evt( Conv::ErrorCodeConvert(code), JOIN_FAIL, channel);
            channelEventListener( evt  );
        }
    }

    void IMClient::JoinMultiChannel( std::vector<XString>& channels,const JoinChannelCallback& callback){
        joinChannelCallback = callback;
        for( XString channel: channels ){
            auto code = m_imMgr->IMManager()->GetChatRoomManager()->JoinChatRoom( channel.c_str() );
            if( code != YIMErrorcode_Success && joinChannelCallback != NULL ){
                ChannelEvent evt( Conv::ErrorCodeConvert(code), JOIN_FAIL,  channel);
                channelEventListener( evt );
            }
        }
    }
//
    void IMClient::LeaveAllChannel( const LeaveChannelCallback& callback ){
        leaveChannelCallback = callback;

        //todo
    }
    
    void IMClient::LeaveChannel( const XString& channel, const LeaveChannelCallback& callback ){
        leaveChannelCallback = callback;

        auto code = m_imMgr->IMManager()->GetChatRoomManager()->LeaveChatRoom( channel.c_str() );
        if( code != YIMErrorcode_Success && leaveChannelCallback != NULL ){
            ChannelEvent evt( Conv::ErrorCodeConvert(code), LEAVE_FAIL,  channel);
            leaveChannelCallback( evt );
        }
    }
//
    void IMClient::SwitchToChannels( std::vector<XString>& channels, const LeaveChannelCallback& leaveCallback, const JoinChannelCallback& joinCallback ){
        LeaveAllChannel( leaveCallback );
        
        JoinMultiChannel( channels, joinCallback);
    }
//
//    void IMClient::ReConnect(){
//        //todo
//    }
//    
//    void IMClient::SetDebug( bool isDebug ){
//        if( isDebug ){
//            m_imMgr->IMManager()->SetMode( 2 );
//        }
//        else{
//            m_imMgr->IMManager()->SetMode( 0 );
//        }
//    }
//    
//    void IMClient::DownloadFile( XUINT64 requestID,  const XString& targetFilePath, const DownloadCallback& callback ){
//        auto code = m_imMgr->IMManager()->GetMessageManager()->DownloadFile( requestID, targetFilePath.c_str() );
//        bool ret = false;
//        if( code == YIMErrorcode_Success ){
//            ret = m_imMgr->AddDownloadCallback( requestID, callback );
//        }
//        
//        if( !ret && callback != NULL ) {
//            //todo
//            //callback( )
//            
//        }
//    }

    void IMClient::OnConnect( IMConnectEvent connectEvent ){
        switch ( connectEvent.type ) {
            case ConnectEventType::CE_CONNECTED:
                cout<<"onConnect:"<<m_imMgr->m_lastLoginUser.userID<<::std::endl;
                if( loginCallback != NULL )
                    loginCallback( LoginEvent( connectEvent.code, connectEvent.userID ) );
                break;
            case ConnectEventType::CE_CONNECT_FAIL:
                if( loginCallback != NULL )
                    loginCallback( LoginEvent( connectEvent.code, connectEvent.userID ) );
                break;
            case ConnectEventType::CE_OFF_LINE:
                if( disconnectCallback != NULL )
                    disconnectCallback( DisconnectEvent( connectEvent.code, connectEvent.userID ));
                break;
            case ConnectEventType::CE_DISCONNECTED:
                if( logoutCallback  != NULL )
                    logoutCallback( LogoutEvent( connectEvent.code, connectEvent.userID ) );
                break;
            case ConnectEventType::CE_KICKED:
                if( kickoffCallback  != NULL )
                    kickoffCallback( KickOffEvent( connectEvent.code, connectEvent.userID ) );
                break;
            default:
                break;
        }
    }
    
    
    void IMClient::OnChannelEvent( ChannelEvent channelEvent ){
        switch ( channelEvent.eventType ) {
            case ChannelEventType::JOIN_SUCCESS:
            case ChannelEventType::JOIN_FAIL:
                if( joinChannelCallback != NULL)
                    joinChannelCallback( channelEvent );
                break;
            case ChannelEventType::LEAVE_FAIL:
            case ChannelEventType::LEAVE_SUCCESS:
                if( leaveChannelCallback != NULL)
                    leaveChannelCallback( channelEvent );
                break;
            default:
                break;
        }
    }
    
    TextMessage IMClient::SendTextMessage( const XString& receiverID,  ChatType chatType, const XString& msgContent, const MessageCallback& sendCallback ){
        XUINT64 reqID = 0 ;
        
        YIMErrorcode code = YIMErrorcode_Success;
        code = m_imMgr->IMManager()->GetMessageManager()->SendTextMessage( receiverID.c_str(), YIMChatType(chatType), msgContent.c_str(), &reqID );
        auto msg = new TextMessage( GetCurrentUserID().userID, receiverID, chatType, msgContent, false );
        if( code == YIMErrorcode_Success ){
            msg->sendStatus = SendStatus::Sending;
            msg->requestID = reqID;
            
            MessageCallbackObject  callbackObj( (IMMessage*)msg, MessageBodyType_TXT, sendCallback );
            
            m_imMgr->AddMessageCallback( reqID,  callbackObj );
        }
        else{
            msg->sendStatus = SendStatus::Fail;
            if( sendCallback != NULL ){
                sendCallback( Conv::ErrorCodeConvert( code ), *msg );
            }
        }
        
        return *msg;
    }
//
//    
//    ///////////////////////////////////////////////////////////////////////////////////////////////
//
//    
//    StatusCode IMClient::GetCurrentLocation() {
//        
//    }
//
//    
//    StatusCode IMClient::GetNearbyObjects(int count, const XCHAR* serverAreaID, DistrictLevel districtlevel , bool resetStartDistance ) {
//        
//    }
//
//    
//    void IMClient::SetUpdateInterval(unsigned int interval) {
//        m_imMgr->IMManager()->GetLocationManager()->SetUpdateInterval( interval );
//    }
//    
//
//    
//    StatusCode IMClient::SendTextMessage(const XCHAR* receiverID, YIMChatType chatType, const XCHAR* text, XUINT64* requestID){
//        
//    }
//   
//    
//    StatusCode IMClient::MultiSendTextMessage(const char* receivers, const XCHAR* text){
//        
//    }
//
//    
//    StatusCode IMClient::SendCustomMessage(const XCHAR* receiverID, YIMChatType chatType, const char* content, unsigned int size, XUINT64* requestID) {
//        
//    }
//
//    
//    StatusCode IMClient::SendFile(const XCHAR* receiverID, YIMChatType chatType, const XCHAR* filePath, XUINT64* requestID, const XCHAR* extraParam, YIMFileType fileType ) {
//        
//    }
//    
//    StatusCode IMClient::SendAudioMessage(const XCHAR* receiverID, YIMChatType chatType, XUINT64* requestID) {
//        
//    }
//   
//    StatusCode IMClient::SendOnlyAudioMessage(const XCHAR* receiverID, YIMChatType chatType, XUINT64* requestID) {
//        
//    }
//
//    StatusCode IMClient::StopAudioMessage(const XCHAR* extraParam){
//        
//    }
//    
//    StatusCode IMClient::CancleAudioMessage(){
//        
//    }
//  
//    StatusCode IMClient::SendGift(const XCHAR* anchorID, const XCHAR* channel, int giftId, int giftCount, const char* extraParam, XUINT64* requestID){
//        
//    }
//
//    StatusCode IMClient::DownloadFile(XUINT64 messageID, const XCHAR* savePath){
//        
//    }
//
//    StatusCode IMClient::DownloadFile(const XCHAR* downloadURL, const XCHAR* savePath){
//        
//    }
//    
//
//    StatusCode IMClient::QueryHistoryMessage(const XCHAR* targetID, YIMChatType chatType, XUINT64 startMessageID , int count , int direction) {
//        
//    }
//
//    StatusCode IMClient::DeleteHistoryMessage(YIMChatType chatType , XUINT64 time ){
//        auto code = m_imMgr->IMManager()->GetMessageManager()->DeleteHistoryMessage( chatType, time );
//        return Conv::ErrorCodeConvert( code );
//    }
//
//    StatusCode IMClient::DeleteHistoryMessageByID(XUINT64 messageID){
//        auto code =  m_imMgr->IMManager()->GetMessageManager()->DeleteHistoryMessageByID( messageID );
//        return Conv::ErrorCodeConvert( code );
//    }
//
//    StatusCode IMClient::QueryRoomHistoryMessageFromServer(const XCHAR* roomID){
//        
//    }
//    
//
//    StatusCode IMClient::StartAudioSpeech(XUINT64* requestID, bool translate) {
//        
//    }
//
//    StatusCode IMClient::StopAudioSpeech(){
//        
//    }
//
//    StatusCode IMClient::ConvertAMRToWav(const XCHAR* amrFilePath, const XCHAR* wavFielPath ) {
//        auto code =  m_imMgr->IMManager()->GetMessageManager()->ConvertAMRToWav( amrFilePath, wavFielPath );
//        return Conv::ErrorCodeConvert( code );
//    }
//    
//
//    StatusCode IMClient::SetReceiveMessageSwitch(const XCHAR* targets, bool receive){
//        auto code =  m_imMgr->IMManager()->GetMessageManager()->SetReceiveMessageSwitch( targets, receive );
//        return Conv::ErrorCodeConvert( code );
//    }
//
//
//    StatusCode IMClient::GetNewMessage(const XCHAR* targets){
//        auto code =  m_imMgr->IMManager()->GetMessageManager()->GetNewMessage( targets );
//        return Conv::ErrorCodeConvert( code );
//    }
//
//    StatusCode IMClient::SetRoomHistoryMessageSwitch(const XCHAR* roomIDs, bool save){
//        auto code =  m_imMgr->IMManager()->GetMessageManager()->SetRoomHistoryMessageSwitch( roomIDs, save );
//        return Conv::ErrorCodeConvert( code );
//    }
//    
//    StatusCode IMClient::TranslateText(unsigned int* requestID, const XCHAR* text, LanguageCode destLangCode, LanguageCode srcLangCode ){
//        
//    }
//    
//
//    void IMClient::SetAudioCacheDir(const XCHAR* audioCacheDir){
//        m_imMgr->IMManager()->SetAudioCacheDir( audioCacheDir );
//    }
//    
//
//    StatusCode IMClient::GetRecentContacts(){
//        
//    }
//    
//   
//    StatusCode IMClient::SetUserInfo(const XCHAR* userInfo){
//        auto code = m_imMgr->IMManager()->SetUserInfo( userInfo );
//        return Conv::ErrorCodeConvert( code );
//    }
//
//    StatusCode IMClient::GetUserInfo(const XCHAR* userID){
//        
//    }
//
//    StatusCode IMClient::QueryUserStatus(const XCHAR* userID){
//        
//    }
//
//    XString IMClient::FilterKeyword(const XCHAR* text, int* level){
//        return m_imMgr->IMManager()->FilterKeyword( text , level );
//    }
//    
//
//    void IMClient::OnPause(){
//        m_imMgr->IMManager()->OnPause();
//    }
//
//    void IMClient::OnResume() {
//        m_imMgr->IMManager()->OnResume();
//    }
//    
//
//    void IMClient::SetVolume(float volume){
//        m_imMgr->IMManager()->SetVolume( volume );
//    }
//
//    StatusCode IMClient::StartPlayAudio(const XCHAR* path){
//        
//    }
//
//    StatusCode IMClient::StopPlayAudio(){
//        
//    }
//
//    bool IMClient::IsPlaying(){
//        return m_imMgr->IMManager()->IsPlaying();
//    }
//    
//
//    XString IMClient::GetAudioCachePath(){
//        return m_imMgr->IMManager()->GetAudioCachePath();
//    }
//    
//    bool IMClient::ClearAudioCachePath(){
//        return m_imMgr->IMManager()->ClearAudioCachePath();
//    }






}
