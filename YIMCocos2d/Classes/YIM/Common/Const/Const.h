//
//  Const.h
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/5.
//
//

#ifndef Const_h
#define Const_h

namespace YouMe
{
    enum LogLevel
    {
        NONE = 0,
        FATAL  = 1,
        ERROR = 10,
        WARNING = 20,
        INFO = 40,
        ALL = 50
    };
    
    enum ConnectStatus
    {
        DISCONNECTED = 0,
        CONNECTED  = 1,
        CONNECTING = 2,
        RECONNECTING = 3
    };
    
    enum StatusCode{
        // =======================原始ERRORCODE保留========================
        Success = 0,
        EngineNotInit = 1,
        NotLogin = 2,
        ParamInvalid = 3,
        TimeOut = 4,
        StatusError = 5,
        SDKInvalid = 6,
        AlreadyLogin = 7,
        ServerError = 8,
        NetError = 9,
        LoginSessionError = 10,
        NotStartUp = 11,
        FileNotExist = 12,
        SendFileError = 13,
        UploadFailed = 14,
        UsernamePasswordError = 15,
        UserStatusError = 16,
        MessageTooLong = 17,	//消息太长
        ReceiverTooLong = 18,	//接收方ID过长(检查房间名)
        InvalidChatType = 19,	//无效聊天类型(私聊、聊天室)
        InvalidReceiver = 20,	//无效用户ID（私聊接受者为数字格式ID）
        UnknowError = 21,		//常见于发送房间消息，房间号并不存在。修正的方法是往自己joinRoom的房间id发消息，就可以发送成功。
        InvalidAppkey = 22,			//无效APPKEY
        ForbiddenSpeak = 23,			//被禁言
        CreateFileFailed = 24,     //无法创建文件
        UnsupportFormat = 25,			//不支持的文件格式
        ReceiverEmpty = 26,			//接收方为空
        RoomIDTooLong = 27,			//房间名太长
        ContentInvalid = 28,			//聊天内容严重非法
        NoLocationAuthrize = 29,		//未打开定位权限
        NoAudioDevice = 30,			//无音频设备
        AudioDriver = 31,				//音频驱动问题
        DeviceStatusInvalid = 32,		//设备状态错误
        ResolveFileError = 33,			//文件解析错误
        ReadWriteFileError = 34,		//文件读写错误
        NoLangCode = 35,				//语言编码错误
        TranslateUnable = 36,			//翻译接口不可用
        
        //服务器的错误码
        ALREADYFRIENDS = 1000,
        LoginInvalid = 1001,
        
        //语音部分错误码
        PTT_Start = 2000,
        PTT_Fail = 2001,
        PTT_DownloadFail = 2002,
        PTT_GetUploadTokenFail = 2003,
        PTT_UploadFail = 2004,
        PTT_NotSpeech = 2005,
        PTT_DeviceStatusError = 2006,		//音频设备状态错误
        PTT_IsSpeeching = 2007,			//已开启语音
        PTT_FileNotExist = 2008,
        PTT_ReachMaxDuration = 2009,		//达到语音最大时长限制
        PTT_SpeechTooShort = 2010,			//语音时长太短
        PTT_StartAudioRecordFailed = 2011,	//启动语音失败
        PTT_SpeechTimeout = 2012,			//音频输入超时
        PTT_IsPlaying = 2013,				//正在播放
        PTT_NotStartPlay = 2014,			//未开始播放
        PTT_CancelPlay = 2015,				//主动取消播放
        PTT_NotStartRecord = 2016,			//未开始语音
        Fail = 10000,
        
        // =======================END 原始ERRORCODE保留========================
        
        // ==========================状态码扩充================================
        START_DOWNLOAD_FAIL = 20001
        // =========================END状态码扩充==============================
    };
    
}

#endif /* Const_h */
