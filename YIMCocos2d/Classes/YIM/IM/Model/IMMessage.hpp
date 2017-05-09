//
//  IMMessage.hpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/5.
//
//

#ifndef IMMessage_hpp
#define IMMessage_hpp

#include <stdio.h>
#include <YIM.h>
#include <Common/Const/Const.h>

using namespace YouMe;

enum ChatType{
    Unknow = 0,
    PrivateChat = 1,
    RoomChat = 2,
};

enum SendStatus{
    NotStartSend = 0,
    Sending = 1,
    Sended = 2,
    Fail = 3,
};

enum MessageDownloadStatus{
    NOTDOWNLOAD = 0 ,
    DOWNLOADING = 1,
    DOWNLOADED = 2,
    DOWNLOAD_FAIL = 3
};

class IMMessage{
public:
    YIMMessageBodyType  messageType;
    XString senderID;
    XString receiverID;
    ChatType chatType;
    bool isReceiveFromServer;
    SendStatus sendStatus;
    unsigned int  sendTime;
    
    /// 会话期间的消息唯一id
    XUINT64 requestID;
    
    IMMessage(  YIMMessageBodyType msgType, const XString& sender, const XString& receiver, ChatType chatType, bool isFromServer  );
};

class AudioMessage : public  IMMessage {
public:
    MessageDownloadStatus  downloadStatus;
    bool isRecorgnizeText;
    XString audioFilePath;
    XString recognizedText;
    XString extraParam;
    int audioDuration;
    
    AudioMessage( const XString& sender, const XString& receiver, ChatType chatType, const XString& extraParam, bool isFromServer );
    
    typedef  std::function< void( StatusCode , const XString& )> PlayCompletionCallback;
    void Play( const PlayCompletionCallback& playCompletionCallback );
    
    void PlayInQueue();
    
    typedef std::function< void (StatusCode, const AudioMessage& )>  DownloadAudioCallback;

//    void Download( const DownloadCallback& downloadCallback );
//    
    void Download( const XString& targetPath, const DownloadAudioCallback& downloadCallback );
    
//    XString GetUniqAudioPath();
    
};

class TextMessage :  public IMMessage{
public:
    XString content;
    
    TextMessage( const XString& sender, const XString& receiver, ChatType chatType, const XString& content, bool isFromServer );
    
};

#endif /* IMMessage_hpp */
