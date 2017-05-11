//
//  IMClient.hpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/3.
//
//

#ifndef IMClient_hpp
#define IMClient_hpp

#include <stdio.h>
#include <YIM.h>
#include <string>
#include "IMConnectEvent.h"
#include <Common/Const/Const.h>
#include <Common/Model/ChannelEvent.h>
#include <IM/Model/IMChannel.hpp>
#include <IM/Model/IMUser.hpp>
#include <IM/Model/IMMessage.hpp>
#include <memory>

typedef std::function< void( StatusCode, std::shared_ptr<IMMessage> ) > MessageCallback;
typedef std::function< void(StatusCode, XUINT64, const XString& ) > DownloadCallback;


class MessageCallbackObject{
public:
    MessageCallback callback;
    std::shared_ptr<IMMessage> message;
    YIMMessageBodyType msgType;
    
    MessageCallbackObject();
    MessageCallbackObject( std::shared_ptr<IMMessage> msg, YIMMessageBodyType msgtype, const MessageCallback& callback );
};


class IMInternalManager;
namespace YouMe{
    struct Config{
        ServerZone serverZone;
    };
    
    
    
    class IMClient{
    public:
        static IMClient* getInstance();
    private:
        IMClient();
        
    public:
        typedef std::function< void (const LoginEvent&) > LoginCallback;
        typedef std::function< void (const LogoutEvent&) > LogoutCallback;
        typedef std::function< void (const KickOffEvent&) > KickOffCallback;
        typedef std::function< void (const DisconnectEvent&) > DisconnectCallback;
        
        typedef std::function< void (const ChannelEvent&) > JoinChannelCallback;
        typedef std::function< void (const ChannelEvent&) > LeaveChannelCallback;

        typedef std::function< void (const IMConnectEvent&) > ConnectListener;
        typedef std::function< void (const ChannelEvent&) > ChannelEventListener;
        typedef std::function< void (std::shared_ptr<IMMessage>) > ReceiveMessageListener;
        
        typedef std::function< void (StatusCode, std::shared_ptr<GeographyLocation> )>  UpdateLocationCallback;
        typedef std::function< void (StatusCode, std::list< std::shared_ptr<RelativeLocation> > neighbourList,  unsigned int startDistance, unsigned int endDistance )> GetNearbyObjectsCallback;
        
    public:
        LoginCallback loginCallback;
        LogoutCallback logoutCallback;
        KickOffCallback kickoffCallback;
        DisconnectCallback disconnectCallback;
        
        JoinChannelCallback joinChannelCallback;
        LeaveChannelCallback leaveChannelCallback;

        ConnectListener connectListener;
        ChannelEventListener channelEventListener;
        ReceiveMessageListener receiveMessageListener;

        
        std::vector<IMChannel> m_vecChannels;
        
        void SetUpdateInterval(unsigned int interval);
        void GetCurrentLocation( const UpdateLocationCallback& cb  );
        void GetNearbyObjects( int count, const XCHAR* serverAreaID, DistrictLevel districtlevel , bool resetStartDistance ,  const GetNearbyObjectsCallback& cb  );
        
        UpdateLocationCallback updateLocationCallback;
        GetNearbyObjectsCallback  getNearbyObjectsCallback;
        
        
    public:
        void Initialize( const XCHAR* appKey, const XCHAR* appSecurity, const Config* config );

        
        void Login( const XCHAR* userID, const XCHAR* token, const LoginCallback& callback  );
        
        void Logout(  const LogoutCallback& callback  );
        
        
        void SetKickOffListener( const KickOffCallback& callback );
        void SetDisconnectListener( const DisconnectCallback& callback );
        
        void SetReceiveMessageListener( const ReceiveMessageListener& callback );
        
        std::vector<IMChannel>& GetCurrentChannels();
        ConnectStatus GetCurrentStatus();
        IMUser GetCurrentUserID();

        void JoinChannel( const XString& channel,  const JoinChannelCallback& callback );
        void JoinMultiChannel( std::vector<XString>& channels,const JoinChannelCallback& callback);
        void LeaveAllChannel( const LeaveChannelCallback& callback );
        void LeaveChannel( const XString& channel, const LeaveChannelCallback& callback );
        void SwitchToChannels( std::vector<XString>& channels, const LeaveChannelCallback& leaveCallback, const JoinChannelCallback& joinCallback );
//
        void ReConnect();
        
//        void SetDebug( bool isDebug );
//        
//

        TextMessage SendTextMessage( const XString& receiverID,  ChatType chatType, const XString& msgContent, const MessageCallback& sendCallback );
        
        AudioMessage StartRecordAudio(const XString& reciverID, ChatType chatType,const XString&  extraMsg, bool recognizeText, const MessageCallback& sendCallback );
        bool StopRecordAndSendAudio();

        void DownloadFile( XUINT64 requestID,  const XString& targetFilePath, const DownloadCallback& callback );

        void OnConnect( IMConnectEvent connectEvent );
        void OnChannelEvent( ChannelEvent channelEvent );
        
    private:
        AudioMessage* lastRecordAudioMessage;
        
    public:
        // 获取当前地理位置
//        virtual StatusCode GetCurrentLocation() ;
//        // 获取附近的目标	count:获取数量（一次最大200） serverAreaID：区服	districtlevel：行政区划等级		resetStartDistance：是否重置查找起始距离
//        virtual StatusCode GetNearbyObjects(int count, const XCHAR* serverAreaID, DistrictLevel districtlevel = DISTRICT_UNKNOW, bool resetStartDistance = false) ;
//        // 设置位置更新间隔(单位：分钟)
//        virtual void SetUpdateInterval(unsigned int interval) ;
//        
//        //发送文本消息
//        virtual StatusCode SendTextMessage(const XCHAR* receiverID, YIMChatType chatType, const XCHAR* text, XUINT64* requestID);
//        //群发文本消息 receivers:接收方ID JSON数组 "["xx","xx","xx"]"
//        virtual StatusCode MultiSendTextMessage(const char* receivers, const XCHAR* text);
//        //发送自定义消息
//        virtual StatusCode SendCustomMessage(const XCHAR* receiverID, YIMChatType chatType, const char* content, unsigned int size, XUINT64* requestID) ;
//        //发送文件
//        virtual StatusCode SendFile(const XCHAR* receiverID, YIMChatType chatType, const XCHAR* filePath, XUINT64* requestID, const XCHAR* extraParam, YIMFileType fileType = FileType_Other) ;        //发送语音消息（语音转文字）
//        virtual StatusCode SendAudioMessage(const XCHAR* receiverID, YIMChatType chatType, XUINT64* requestID) ;
//        //发送语音消息（语音不转文字）
//        virtual StatusCode SendOnlyAudioMessage(const XCHAR* receiverID, YIMChatType chatType, XUINT64* requestID) ;
//        //停止语音
//        virtual StatusCode StopAudioMessage(const XCHAR* extraParam);
//        //取消语音
//        virtual StatusCode CancleAudioMessage();
//        //送礼物
//        //extraParam:附加参数 格式为json {"nickname":"","server_area":"","location":"","score":"","level":"","vip_level":"","extra":""}
//        virtual StatusCode SendGift(const XCHAR* anchorID, const XCHAR* channel, int giftId, int giftCount, const char* extraParam, XUINT64* requestID);
//        //下载语音
//        virtual StatusCode DownloadFile(XUINT64 messageID, const XCHAR* savePath);
//        //下载语音文件
//        virtual StatusCode DownloadFile(const XCHAR* downloadURL, const XCHAR* savePath);
//        
//        //查询消息记录(direction 查询方向 0：向前查找	1：向后查找)
//        virtual StatusCode QueryHistoryMessage(const XCHAR* targetID, YIMChatType chatType, XUINT64 startMessageID = 0, int count = 30, int direction = 0) ;
//        /*清理消息记录
//         YIMChatType:私聊消息、房间消息
//         time：删除指定时间之前的消息*/
//        virtual StatusCode DeleteHistoryMessage(YIMChatType chatType = ChatType_Unknow, XUINT64 time = 0) ;
//        //删除指定messageID对应消息
//        virtual StatusCode DeleteHistoryMessageByID(XUINT64 messageID);
//        //查询房间历史消息(房间最近N条聊天记录)
//        virtual StatusCode QueryRoomHistoryMessageFromServer(const XCHAR* roomID);
//        
//        //开始语音（不通过游密发送该语音消息，由调用方发送，调用StopAudioSpeech完成语音及上传后会回调OnStopAudioSpeechStatus）
//        virtual StatusCode StartAudioSpeech(XUINT64* requestID, bool translate = true) ;
//        //停止语音（不通过游密发送该语音消息，由调用方发送，完成语音及上传后会回调OnStopAudioSpeechStatus）
//        virtual StatusCode StopAudioSpeech();
//        //转换AMR格式到WAV格式
//        virtual StatusCode ConvertAMRToWav(const XCHAR* amrFilePath, const XCHAR* wavFielPath = NULL) ;
//        
//        //是否自动接收消息(true:自动接收(默认)	false:不自动接收消息，有新消息达到时，SDK会发出OnReceiveMessageNotify回调，调用方需要调用GetMessage获取新消息)
//        //targets:房间ID JSON数组 "["xx","xx","xx"]"
//        virtual StatusCode SetReceiveMessageSwitch(const XCHAR* targets, bool receive);
//        //获取新消息（只有SetReceiveMessageSwitch设置为不自动接收消息，才需要在收到OnReceiveMessageNotify回调时调用该函数）
//        //targets:房间ID JSON数组 "["xx","xx","xx"]"
//        virtual StatusCode GetNewMessage(const XCHAR* targets);
//        // 是否保存房间消息记录（默认不保存）roomIDs:房间ID JSON数组 "["xx","xx","xx"]"
//        virtual StatusCode SetRoomHistoryMessageSwitch(const XCHAR* roomIDs, bool save);
//        
//        // 文本翻译
//        virtual StatusCode TranslateText(unsigned int* requestID, const XCHAR* text, LanguageCode destLangCode, LanguageCode srcLangCode = LANG_AUTO);
//        
//        //设置语音缓存目录
//        void SetAudioCacheDir(const XCHAR* audioCacheDir);
//        
//        //获取最近联系人(最大100条)
//        virtual StatusCode GetRecentContacts();
//        
//        //设置用户信息 格式为json {"nickname":"","server_area_id":"","server_area":"","location_id":"","location":"","level":"","vip_level":"","platform_id":"","platform":""} (前七个必填，可以添加其他字段)
//        virtual StatusCode SetUserInfo(const XCHAR* userInfo);
//        //查询用户信息
//        virtual StatusCode GetUserInfo(const XCHAR* userID);
//        //查询用户在线状态
//        virtual StatusCode QueryUserStatus(const XCHAR* userID);
//        
//        //关键词过滤
//        XString FilterKeyword(const XCHAR* text, int* level);
//        
//        //程序切到后台运行
//        virtual void OnPause();
//        //程序切到前台运行
//        virtual void OnResume() ;
//        
//        //设置播放语音音量(volume:0.0-1.0)
//        void SetVolume(float volume);
//        //播放语音
//        StatusCode StartPlayAudio(const XCHAR* path);
//        //停止语音播放
//        StatusCode StopPlayAudio();
//        //查询播放状态
//        bool IsPlaying();
//        
        //获取语音缓存目录
//        XString GetAudioCachePath();
//        //清理语音缓存目录(注意清空语音缓存目录后历史记录中会无法读取到音频文件，调用清理历史记录接口也会自动删除对应的音频缓存文件)
//        bool ClearAudioCachePath();
//
        
    private:
        static IMClient* s_ins;
        IMInternalManager*  m_imMgr;
        
        const XString FAKE_PAPSSWORD = __XT("123456");
        
        
        
    };
    
}

#endif /* IMClient_hpp */
