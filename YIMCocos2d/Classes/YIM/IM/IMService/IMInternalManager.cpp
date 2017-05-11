//
//  IMInternalManager.cpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/3.
//
//

#include "IMInternalManager.hpp"
#include <IM/IMClient.hpp>
#include <IM/Utils/Conv.hpp>
#include <Common/Const/Const.h>
#include <Common/Model/ChannelEvent.h>
#include <IM/Model/IMConnectEvent.h>
#include <IM/Model/IMMessage.hpp>




IMInternalManager* IMInternalManager::s_ins = NULL;

YIMManager* IMInternalManager::m_imp = NULL;
IMInternalManager* IMInternalManager::getInstance(){
    if( s_ins == NULL ){
        s_ins = new IMInternalManager;
        
        IMManager()->SetLoginCallback( s_ins );
        IMManager()->SetContactCallback( s_ins );
        IMManager()->SetMessageCallback( s_ins );
        IMManager()->SetChatRoomCallback( s_ins );
        IMManager()->SetDownloadCallback( s_ins );
        IMManager()->SetLocationCallback( s_ins );
    }
    return s_ins;
}

IMInternalManager::IMInternalManager(){
    
}

YIMManager* IMInternalManager::IMManager(){
    if( m_imp == NULL ){
        m_imp = YIMManager::CreateInstance();
    }
    
    return m_imp;
}


bool IMInternalManager::AddMessageCallback( XUINT64 reqID, const MessageCallbackObject& cb ){
    typename std::map< XUINT64, MessageCallbackObject>::iterator it = m_mapMessageCallback.find( reqID );
    if( it == m_mapMessageCallback.end() ){
        m_mapMessageCallback[ reqID ] = cb;
    }
    else{
        Log::e("message id is already in sending queue.");
        return false;
    }
    
    return true;
}


bool IMInternalManager::AddDownloadCallback( XUINT64 reqID, const DownloadCallback& cb ){
    return AddCallback( m_mapDownloadCallback, reqID, cb );
}

///IYIMLoginCallback
//登录回调
#include <iostream>
using namespace std;
void IMInternalManager::OnLogin(YIMErrorcode errorcode, const XString& userID)  {
    if( errorcode == YIMErrorcode_Success ){
        m_lastLoginUser.userID = userID;
        
        cout<<"登录成功："<<m_lastLoginUser.userID<<endl;
    }
    
    if( IMClient::getInstance()->connectListener != NULL ){
        IMConnectEvent evt( Conv::ErrorCodeConvert( errorcode),
                           errorcode == YIMErrorcode_Success ? ConnectEventType::CE_CONNECTED : ConnectEventType::CE_CONNECT_FAIL,
                           userID );
        
        IMClient::getInstance()->connectListener(evt);
    }
    
}
//登出回调
void IMInternalManager::OnLogout(YIMErrorcode errorcode){
    if( IMClient::getInstance()->connectListener != NULL ){
        IMConnectEvent evt( StatusCode::Success, ConnectEventType::CE_DISCONNECTED, __XT("") );
        IMClient::getInstance()->connectListener(evt);
    }
}
//被踢下线
void IMInternalManager::OnKickOff(){
    if( IMClient::getInstance()->connectListener != NULL ){
        IMConnectEvent evt( StatusCode::Success, ConnectEventType::CE_KICKED, __XT("") );
        IMClient::getInstance()->connectListener(evt);
    }
}


///IYIMChatRoomCallback
//加入频道回调
void IMInternalManager::OnJoinChatRoom(YIMErrorcode errorcode, const XString& chatRoomID){
    if( IMClient::getInstance()->channelEventListener != NULL ){
        ChannelEventType et = errorcode == YIMErrorcode_Success ? ChannelEventType::JOIN_SUCCESS : ChannelEventType::JOIN_FAIL ;
        ChannelEvent evt( Conv::ErrorCodeConvert( errorcode), et, chatRoomID );
        IMClient::getInstance()->channelEventListener( evt );
    }
}
//离开频道回调
void IMInternalManager::OnLeaveChatRoom(YIMErrorcode errorcode, const XString& chatRoomID) {
    if( IMClient::getInstance()->channelEventListener != NULL ){
        ChannelEventType et = errorcode == YIMErrorcode_Success ? ChannelEventType::LEAVE_SUCCESS : ChannelEventType::LEAVE_FAIL ;
        ChannelEvent evt( Conv::ErrorCodeConvert( errorcode), et, chatRoomID  );
        IMClient::getInstance()->channelEventListener( evt );
    }
}


///IYIMLocationCallback
// 获取自己位置回调
void IMInternalManager::OnUpdateLocation(YIMErrorcode errorcode, std::shared_ptr<GeographyLocation> location){
    
}
// 获取附近目标回调
void IMInternalManager::OnGetNearbyObjects(YIMErrorcode errorcode, std::list< std::shared_ptr<RelativeLocation> >  neighbourList, unsigned int startDistance, unsigned int endDistance) {
    
}


///IYIMMessageCallback
void IMInternalManager::OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode) {
    MessageCallbackObject* callbackObj = NULL;
    std::map< XUINT64, MessageCallbackObject>::iterator it = m_mapMessageCallback.find( requestID );
    if( it != m_mapMessageCallback.end() ){
        callbackObj = &it->second;
        if( callbackObj != NULL && callbackObj->callback != NULL ){
            switch ( callbackObj->msgType ) {
                case MessageBodyType_TXT:
                {
                    TextMessage* pTextMsg = ((TextMessage*)callbackObj->message);
                    pTextMsg->sendTime = time(0);
                    if( errorcode == YIMErrorcode_Success ){
                        pTextMsg->sendStatus = SendStatus::Sended;
                    }
                    else{
                        pTextMsg->sendStatus = SendStatus::Fail;
                    }
                    
                    pTextMsg->isReceiveFromServer = false;
                    callbackObj->callback( Conv::ErrorCodeConvert( errorcode ), *pTextMsg );
                }
                    break;
                default:
                    break;
            }
        }
        
        delete callbackObj->message;
        m_mapMessageCallback.erase( it );
    }
}
//停止语音回调（调用StopAudioMessage停止语音之后，发送语音消息之前）
void IMInternalManager::OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime){
    OnSendAudioMessageStatusChange( requestID, errorcode, text, audioPath, audioTime, false  );
}
//发送语音消息回调
void IMInternalManager::OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime){
     OnSendAudioMessageStatusChange( requestID, errorcode, text, audioPath, audioTime, true  );
}

void IMInternalManager::OnSendAudioMessageStatusChange(XUINT64 requestID, YIMErrorcode errorcode, const XString& text, const XString& audioPath, unsigned int audioTime, bool isFinish){
    MessageCallbackObject* callbackObj = NULL;
    std::map< XUINT64, MessageCallbackObject>::iterator it = m_mapMessageCallback.find( requestID );
    if( it != m_mapMessageCallback.end() ){
        callbackObj = &(it->second);
        
        if( callbackObj != NULL && callbackObj->callback != NULL && callbackObj->msgType == MessageBodyType_Voice ){
            AudioMessage* pVoiceMsg = ( AudioMessage*) callbackObj->message;
            
            pVoiceMsg->recognizedText = text;
            pVoiceMsg->audioFilePath = audioPath;
            pVoiceMsg->audioDuration = audioTime;
            
            if( !isFinish ){
                pVoiceMsg->sendTime = time(0);
            }
            
            if( errorcode == YIMErrorcode_Success ){
                pVoiceMsg->sendStatus = isFinish? SendStatus::Sended : SendStatus::Sending;
                if( isFinish ){
                    pVoiceMsg->downloadStatus = MessageDownloadStatus::DOWNLOADED;
                }
            }
            else{
                pVoiceMsg->sendStatus = SendStatus::Fail;
            }
            
            pVoiceMsg->isReceiveFromServer = false;
            
            callbackObj->callback( Conv::ErrorCodeConvert( errorcode ), *pVoiceMsg );
        }
       
        //todo:两个消息，走同一个接口，不太好吧。
        //消息发送结束了，或者已经出错了，就删掉回调函数了
        if( isFinish == true || errorcode != YIMErrorcode_Success ){
            m_mapMessageCallback.erase( it );
        }
        
    }
    
}
//收到消息
void IMInternalManager::OnRecvMessage(std::shared_ptr<IYIMMessage> message) {
    if( IMClient::getInstance()->receiveMessageListener != NULL && message != NULL ){
        IMMessage* msg = NULL;
        
        switch ( message->GetMessageBody()->GetMessageType() ) {
            case MessageBodyType_TXT:
            {
                IYIMMessageBodyText* pTextBody = (IYIMMessageBodyText*)message->GetMessageBody();
                if( pTextBody ){
                    TextMessage* textmsg = new TextMessage( message->GetSenderID(),
                                          message->GetReceiveID(),
                                          (ChatType)message->GetChatType(),
                                          pTextBody->GetMessageContent(),
                                          true );
                    textmsg->sendTime = message->GetCreateTime();
                    textmsg->sendStatus = SendStatus::Sended;
                    
                    msg = textmsg;
                }
            }
                break;
            case MessageBodyType_Voice:
            {
                IYIMMessageBodyAudio* pVoiceBody = (IYIMMessageBodyAudio*)message->GetMessageBody();
                if( pVoiceBody ){
                    AudioMessage* audiomsg = new AudioMessage( message->GetSenderID(),
                                          message->GetReceiveID(),
                                          (ChatType)message->GetChatType(),
                                          pVoiceBody->GetExtraParam(),
                                          true );
                    
                    audiomsg->recognizedText = pVoiceBody->GetText();
                    audiomsg->audioDuration = pVoiceBody->GetAudioTime();
                    audiomsg->sendTime = message->GetCreateTime();
                    audiomsg->sendStatus = SendStatus::Sended;
                    
                    msg = audiomsg;
                }
            }
                break;
            default:
                //todo
                //Log::e("unknown message type:"+message.MessageType.ToString());
                break;
        }
        
        if( msg != NULL ){
            msg->requestID = message->GetMessageID();
            
            IMClient::getInstance()->receiveMessageListener( *msg );
            
            delete  msg;
        }
    }
}
//获取消息历史纪录回调
void IMInternalManager::OnQueryHistoryMessage(YIMErrorcode errorcode, const XString& targetID, int remain, std::list<std::shared_ptr<IYIMMessage> > messageList){
    
}
//获取房间历史纪录回调
void IMInternalManager::OnQueryRoomHistoryMessage(YIMErrorcode errorcode, std::list<std::shared_ptr<IYIMMessage> > messageList){
    
}
//语音上传后回调
void IMInternalManager::OnStopAudioSpeechStatus(YIMErrorcode errorcode, std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo){
    
}

//新消息通知（只有SetReceiveMessageSwitch设置为不自动接收消息，才会收到该回调）
void IMInternalManager::OnReceiveMessageNotify(YIMChatType chatType, const XString& targetID){
    
}

//文本翻译完成回调
void IMInternalManager::OnTranslateTextComplete(YIMErrorcode errorcode, unsigned int requestID, const XString& text, LanguageCode srcLangCode, LanguageCode destLangCode){
    
}


///IYIMDownloadCallback
void IMInternalManager::OnDownload(XUINT64 messageID, YIMErrorcode errorcode, const XString& savePath){
//    auto it = m_mapDownloadCallback.find( messageID );
//    if( it != m_mapDownloadCallback.end() ){
//        auto callback = it->second;
//        if( callback != NULL ){
//            callback( Conv::ErrorCodeConvert( errorcode ), savePath );
//        }
//        
//        m_mapDownloadCallback.erase( it );
//    }
}

///IYIMContactCallback
//获取最近联系人回调
void IMInternalManager::OnGetRecentContacts(YIMErrorcode errorcode, std::list<XString>& contactList) {
    
}
//获取用户信息回调(用户信息为JSON格式)
void IMInternalManager::OnGetUserInfo(YIMErrorcode errorcode, const XString& userInfo){
    
}
//查询用户状态回调
void IMInternalManager::OnQueryUserStatus(YIMErrorcode errorcode, const XString& userID, YIMUserStatus status){
    
}


///IYIMAudioPlayCallback
void IMInternalManager::OnPlayCompletion(YIMErrorcode errorcode, const XString& path){
    
}


