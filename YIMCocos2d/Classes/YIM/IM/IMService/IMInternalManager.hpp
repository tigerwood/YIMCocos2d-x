//
//  IMInternalManager.hpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/3.
//
//

#ifndef IMInternalManager_hpp
#define IMInternalManager_hpp

#include <stdio.h>
#include <YIM.h>
#include "IMConnectEvent.h"
#include <map>
#include <IM/Model/IMUser.hpp>
#include <IM/Utils/Log.hpp>
#include <IM/Model/IMMessage.hpp>
#include <IM/IMClient.hpp>

using namespace YouMe;



    
class IMInternalManager: public IYIMLoginCallback,
public IYIMChatRoomCallback,
public IYIMLocationCallback,
public IYIMMessageCallback,
public IYIMDownloadCallback,
public IYIMContactCallback,
public IYIMAudioPlayCallback
{
public:
    
    static IMInternalManager* getInstance();
    IMInternalManager();
    
    static IMInternalManager* s_ins;
    
    static YIMManager* IMManager();
    static YIMManager* m_imp;
    
    IMUser m_lastLoginUser;
    
public:
    //function define
    
    
    std::map< XUINT64, MessageCallbackObject> m_mapMessageCallback;
    std::map< XUINT64, DownloadCallback> m_mapDownloadCallback;
    
    
    bool AddMessageCallback( XUINT64 reqID, const MessageCallbackObject& cb );
    bool AddDownloadCallback( XUINT64 reqID, const DownloadCallback& cb );
    
    template< typename Key, typename TCallback >
    bool AddCallback(  std::map<Key, TCallback>& m_mapCallback, Key  keyvalue, const TCallback& callback ){
        typename std::map< Key, TCallback>::iterator it = m_mapCallback.find( keyvalue );
        if( it != m_mapCallback.end() ){
            m_mapCallback[ keyvalue ] = callback;
        }
        else{
            Log::e("message id is already in sending queue.");
            return false;
        }
        
        return true;
    }
    
public:
    
    ///IYIMLoginCallback
    //登录回调
    virtual void OnLogin(YIMErrorcode errorcode, const XString&  userID) override ;
    //登出回调
    virtual void OnLogout(YIMErrorcode errorcode)override;
    //被踢下线
    virtual void OnKickOff()override;
    
    
    ///IYIMChatRoomCallback
    //加入频道回调
    virtual void OnJoinChatRoom(YIMErrorcode errorcode, const XString&  chatRoomID)override;
    //离开频道回调
    virtual void OnLeaveChatRoom(YIMErrorcode errorcode, const XString&  chatRoomID) override;
    
    
    ///IYIMLocationCallback
    // 获取自己位置回调
    virtual void OnUpdateLocation(YIMErrorcode errorcode, std::shared_ptr<GeographyLocation> location) override ;
    // 获取附近目标回调
    virtual void OnGetNearbyObjects(YIMErrorcode errorcode, std::list< std::shared_ptr<RelativeLocation> >  neighbourList, unsigned int startDistance, unsigned int endDistance) override;
    
    
    ///IYIMMessageCallback
    virtual void OnSendMessageStatus(XUINT64 requestID, YIMErrorcode errorcode) override;
    //停止语音回调（调用StopAudioMessage停止语音之后，发送语音消息之前）
    virtual void OnStartSendAudioMessage(XUINT64 requestID, YIMErrorcode errorcode, const XString&  text, const XString&  audioPath, unsigned int audioTime)override;
    //发送语音消息回调
    virtual void OnSendAudioMessageStatus(XUINT64 requestID, YIMErrorcode errorcode, const XString&  text, const XString&  audioPath, unsigned int audioTime)override;
    
    void OnSendAudioMessageStatusChange(XUINT64 requestID, YIMErrorcode errorcode, const XString&  text, const XString&  audioPath, unsigned int audioTime, bool isFinish);
    
    
    //收到消息
    virtual void OnRecvMessage( std::shared_ptr<IYIMMessage> message) override;
    //获取消息历史纪录回调
    virtual void OnQueryHistoryMessage(YIMErrorcode errorcode, const XString&  targetID, int remain, std::list<std::shared_ptr<IYIMMessage> >  messageList)override;
    //获取房间历史纪录回调
    virtual void OnQueryRoomHistoryMessage(YIMErrorcode errorcode, std::list<std::shared_ptr<IYIMMessage> >  messageList)override;
    //语音上传后回调
    virtual void OnStopAudioSpeechStatus(YIMErrorcode errorcode, std::shared_ptr<IAudioSpeechInfo> audioSpeechInfo)override;
    
    //新消息通知（只有SetReceiveMessageSwitch设置为不自动接收消息，才会收到该回调）
    virtual void OnReceiveMessageNotify(YIMChatType chatType, const XString&  targetID)override;
    
    //文本翻译完成回调
    virtual void OnTranslateTextComplete(YIMErrorcode errorcode, unsigned int requestID, const XString&  text, LanguageCode srcLangCode, LanguageCode destLangCode)override;
    
    
    ///IYIMDownloadCallback
    virtual void OnDownload(XUINT64 messageID, YIMErrorcode errorcode, const XString&  savePath)override;
    
    ///IYIMContactCallback
    //获取最近联系人回调
    virtual void OnGetRecentContacts(YIMErrorcode errorcode, std::list<XString>& contactList) override;
    //获取用户信息回调(用户信息为JSON格式)
    virtual void OnGetUserInfo(YIMErrorcode errorcode, const XString&  userInfo)override;
    //查询用户状态回调
    virtual void OnQueryUserStatus(YIMErrorcode errorcode, const XString&  userID, YIMUserStatus status)override;
    
    
    ///IYIMAudioPlayCallback
    virtual void OnPlayCompletion(YIMErrorcode errorcode, const XString&  path)override;
};


#endif /* IMInternalManager_hpp */
