//
//  IMMessage.cpp
//  YIMCocos2d
//
//  Created by 杜红 on 2017/5/5.
//
//

#include "IMMessage.hpp"
#include "IMInternalManager.hpp"
#include <IM/IMClient.hpp>
#include <IM/Utils/Log.hpp>


IMMessage::IMMessage( YIMMessageBodyType msgType,  const XString& sender, const XString& receiver, ChatType chatType, bool isFromServer  ){
    this->senderID = sender;
    this->messageType = msgType;
    this->receiverID = receiver;
    this->chatType = chatType;
    this->isReceiveFromServer = isFromServer;
    
    
    this->requestID = 0;
    this->sendTime = 0;
    
    if(isFromServer){
        this->sendStatus = SendStatus::Sended;
    }else{
        this->sendStatus = SendStatus::NotStartSend;
    }

}

AudioMessage::AudioMessage( const XString& sender, const XString& receiver, ChatType chatType, const XString& extraParam, bool isFromServer )
:IMMessage( YIMMessageBodyType::MessageBodyType_Voice, sender, receiver, chatType,  isFromServer )
{
    this->extraParam = extraParam;
   
    this->isRecorgnizeText = false;
    this->downloadStatus = MessageDownloadStatus::NOTDOWNLOAD;
    this->audioFilePath = __XT("");
    this->recognizedText = __XT("");
    this->audioDuration = 0;
}

void AudioMessage::Play( const PlayCompletionCallback& playCompletionCallback ){
    if( !audioFilePath.empty() ){
        IMInternalManager::IMManager()->StartPlayAudio( audioFilePath.c_str() );
    }
    
    //todo:Add callback
}

void AudioMessage::PlayInQueue(){
    //todo:
}

//void AudioMessage::Download( const DownloadCallback& downloadCallback ){
//    
//}

void AudioMessage::Download( const XString& targetPath, const DownloadAudioCallback& downloadCallback ){
    if( !isReceiveFromServer ){
        Log::e("只能下载从服务器收到的语音消息，自己发送的语音消息不需要下载。");
        return;
    }
    
    if( downloadStatus == MessageDownloadStatus::DOWNLOADED ){
        if( downloadCallback != NULL ){
            downloadCallback( StatusCode::Success, *this );
            return ;
        }
    }

    downloadStatus = MessageDownloadStatus::DOWNLOADING;
    
    IMClient::getInstance()->DownloadFile( requestID, targetPath, [=](StatusCode code, const XString& filePath){
        if( code == StatusCode::Success ){
            downloadStatus = MessageDownloadStatus::DOWNLOADED;
        }
        else{
            downloadStatus = MessageDownloadStatus::DOWNLOAD_FAIL ;
        }
        
        audioFilePath = filePath;
        
        if( downloadCallback != NULL ){
            downloadCallback( code, *this );
        }
    });
    
}

//XString AudioMessage::GetUniqAudioPath(){
//    
//}

TextMessage::TextMessage( const XString& sender, const XString& receiver, ChatType chatType, const XString& content, bool isFromServer )
:IMMessage( YIMMessageBodyType::MessageBodyType_TXT, sender, receiver, chatType,  isFromServer ){
    this->content = content;
}
