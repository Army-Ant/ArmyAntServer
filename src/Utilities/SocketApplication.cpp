#include <SocketApplication.h>
#include "../../external/ArmyAntLib/inc/AAIStream.h"
#include <boost/endian/detail/endian_load.hpp>

namespace ArmyAntServer{

bool SocketApplication::onServerConnected(uint32 index, void*pThis){
	auto self = static_cast<SocketApplication*>(pThis);

	if(self->eventCallback == nullptr){
		return false;
	}

	if(self->clients.find(index)!=self->clients.end()){
		self->eventCallback(SocketApplication::EventType::ErrorReport, index, "New client connected into , but the same IP and port connection has found ! Please check the code");
		return false;
	} else{
		self->connectMutex.lock();
		self->clients.insert(std::make_pair(index, new ClientInformation(index, ClientStatus::Conversation)));
		self->clients.find(index)->second->setMaxBufferLength(self->bufferLength);
		self->connectMutex.unlock();
		self->eventCallback(SocketApplication::EventType::Connected, index, "New client connected");
		return true;
	}
}

void SocketApplication::onServerDisonnected(uint32 index, void*pThis){
	auto self = static_cast<SocketApplication*>(pThis);
	if(self->eventCallback == nullptr){
		return;
	}
	
	auto findedClient = self->clients.find(index);
	if(findedClient == self->clients.end()){
		self->eventCallback(SocketApplication::EventType::ErrorReport, index, "Detected a client disconnected, but cannot find it in list");
	} else{
		self->eventCallback(SocketApplication::EventType::Disconnected, index, "Client disconnected");
	}
	delete findedClient->second;
	self->clients.erase(findedClient);
}

void SocketApplication::onServerReceived(uint32 index, const void*data, mac_uint datalen, void*pThis){
	auto self = static_cast<SocketApplication*>(pThis);
	auto&clientBuffer = *self->clients.find(index)->second;
	if(self->receiveCallback != nullptr){
		clientBuffer.rwMutex.lock();
		while(datalen > 0){
			mac_uint currCopyLen = datalen;
			// 若超出buffer长度, 则先写入一部分
			if(clientBuffer.receivingBufferEnd + datalen > self->bufferLength){
				currCopyLen = self->bufferLength - clientBuffer.receivingBufferEnd - 1;
			}
			// 写入到buffer
			datalen -= currCopyLen;
			memcpy(clientBuffer.receivingBuffer + clientBuffer.receivingBufferEnd, data, currCopyLen);
			clientBuffer.receivingBufferEnd += currCopyLen;
			// 解析应用头
			if(clientBuffer.receivingBufferEnd > sizeof(MessageBaseHead)){
				MessageBaseHead tmpHead;
                if(ArmyAnt::IStream::IsLittleEnding()){
                    memcpy(&tmpHead, clientBuffer.receivingBuffer, sizeof(MessageBaseHead));
                } else{
                    tmpHead.serials = boost::endian::endian_load<int32, sizeof(int32), boost::endian::order::big>(clientBuffer.receivingBuffer);
                    tmpHead.type = boost::endian::endian_load<MessageType, sizeof(MessageType), boost::endian::order::big>(clientBuffer.receivingBuffer + 4);
                    tmpHead.extendVersion = boost::endian::endian_load<int32, sizeof(int32), boost::endian::order::big>(clientBuffer.receivingBuffer + 8);
                    tmpHead.extendLength = boost::endian::endian_load<int32, sizeof(int32), boost::endian::order::big>(clientBuffer.receivingBuffer + 12);
                }
				// 解析扩展头
				if(clientBuffer.receivingBufferEnd >= sizeof(MessageBaseHead) + tmpHead.extendLength){
					int64 appId = 0;
					int32 contentLength = 0;
					int32 messageCode = 0;
					int32 conversationCode = 0;
					int32 conversationStepIndex = 0;
					ArmyAntMessage::System::ConversationStepType conversationStepType;
					switch(tmpHead.extendVersion){
						case 1:
						{
							ArmyAntMessage::System::SocketExtendNormal_V0_0_0_1 extend;
							extend.ParseFromArray(clientBuffer.receivingBuffer + sizeof(MessageBaseHead), tmpHead.extendLength);
							appId = extend.app_id();
							contentLength = extend.content_length();
							messageCode = extend.message_code();
							conversationCode = extend.conversation_code();
							conversationStepIndex = extend.conversation_step_index();
							conversationStepType = extend.conversation_step_type();
							break;
						}
					}
					// 获取内容, 发送回调
					uint32 usedLength = sizeof(MessageBaseHead) + tmpHead.extendLength + contentLength;
					if(clientBuffer.receivingBufferEnd >= usedLength){
						self->receiveCallback(index, tmpHead, appId, contentLength, messageCode, conversationCode, conversationStepIndex, conversationStepType, clientBuffer.receivingBuffer + sizeof(MessageBaseHead) + tmpHead.extendLength);
						// 若之后仍有内容, 则转移到起点处, 已处理过的信息从buffer移除
						if(clientBuffer.receivingBufferEnd > usedLength){
							memcpy(clientBuffer.receivingBuffer, clientBuffer.receivingBuffer + usedLength, clientBuffer.receivingBufferEnd - usedLength);
						}
						clientBuffer.receivingBufferEnd -= usedLength;
						data = reinterpret_cast<const uint8*>(data) + currCopyLen;
						continue; // 循环应在这里正确继续和退出
					}
				}
			}
			// buffer填满了, 却未解析出一个完整的协议包, 这是有问题的
			if(datalen > 0){
				self->eventCallback(SocketApplication::EventType::ErrorReport,
									index,
									ArmyAnt::String("Receiving buffer full !"));
				break;
			}
		}
		clientBuffer.rwMutex.unlock();
	}
}

bool SocketApplication::onServerSendResponse(mac_uint sendedSize, uint32 retriedTimes, uint32 index, const void*sendedData, uint64 len, void* pThis){
	auto self = static_cast<SocketApplication*>(pThis);
	if(self->eventCallback != nullptr){
		if(sendedSize > 0){
			int32 sendingIndex = 0;
			auto cl = self->clients.find(index);
			cl->second->rwMutex.lock();
			auto&sendedList = cl->second->waitingResponseSended;
			for(auto i = sendedList.begin(); i != sendedList.end(); ++i){
				if(!memcmp(sendedData, i->second, sendedSize))
					sendingIndex = i->first;
			}
			self->eventCallback(SocketApplication::EventType::SendingResponse,
								index,
								ArmyAnt::String("Sending responsed, sending index: ") + int64(sendingIndex));
			if(sendingIndex > 0){
				auto i = sendedList.find(sendingIndex);
				ArmyAnt::Fragment::AA_SAFE_DELALL(i->second);
				sendedList.erase(i);
			}
			cl->second->rwMutex.unlock();
		}else
			self->eventCallback(SocketApplication::EventType::ErrorReport,
								index,
								ArmyAnt::String("Sending failed") );
	}
	return false;
}

void SocketApplication::onServerErrorReport(const ArmyAnt::SocketException& err, const ArmyAnt::IPAddr&addr, uint16 port, ArmyAnt::String functionName, void*pThis){
	auto self = static_cast<SocketApplication*>(pThis);
	uint32 index = self->tcpSocket->getIndexByAddrPort(addr, port);
	if(self->eventCallback != nullptr){
                ArmyAnt::String msg =  "Found error, code:";
                msg += int64(err.code);
                msg += ", message:";
                msg += err.message;
		self->eventCallback(SocketApplication::EventType::ErrorReport, 	index, 	msg);
	}
}

/***********************************************************/

ClientInformation::ClientInformation(const int32 index, ClientStatus status)noexcept
	:index(index), status(status),
	counter(1), waitingResponseSended(), receivingBuffer(nullptr), receivingBufferEnd(0), rwMutex(){
	MutexHelper::setMutexLogTag(rwMutex, "sendingThreadMutex");
}

ClientInformation::ClientInformation(ClientInformation&&moved)noexcept
	: index(moved.index), status(moved.status),
	counter(moved.counter), waitingResponseSended(), receivingBuffer(nullptr), receivingBufferEnd(0), rwMutex(){
	MutexHelper::setMutexLogTag(rwMutex, "sendingThreadMutex");
}

ClientInformation::~ClientInformation(){
	ArmyAnt::Fragment::AA_SAFE_DELALL(receivingBuffer);
	rwMutex.lock();
	while(!waitingResponseSended.empty()){
		ArmyAnt::Fragment::AA_SAFE_DELALL(waitingResponseSended.begin()->second);
		waitingResponseSended.erase(waitingResponseSended.begin());
	}
	rwMutex.unlock();
}

void ClientInformation::setMaxBufferLength(uint32 bufferLength){
	ArmyAnt::Fragment::AA_SAFE_DELALL(receivingBuffer);
	receivingBuffer = new uint8[bufferLength];
	memset(receivingBuffer, 0, bufferLength);
}


SocketApplication::SocketApplication(bool isWebSocket)
	:tcpSocket(isWebSocket ? new ArmyAnt::TCPWebSocketServer(AA_INT32_MAX - 100) : new ArmyAnt::TCPServer(AA_INT32_MAX - 100)), eventCallback(nullptr), receiveCallback(nullptr), clients(), connectMutex(), bufferLength(0){}

SocketApplication::~SocketApplication(){
	delete tcpSocket;
}

bool SocketApplication::setEventCallback(SocketApplication::EventCallback cb){
	eventCallback = cb;
	return true;
}

bool SocketApplication::setReceiveCallback(SocketApplication::ReceiveCallback cb){
	receiveCallback = cb;
	return true;
}

ArmyAnt::TCPServer*SocketApplication::getSocket(){
	return tcpSocket;
}

const std::map<uint32, ClientInformation*>&SocketApplication::getClientList()const{
	return clients;
}

bool SocketApplication::start(uint16 port, uint32 maxBufferLength, bool isIpv6){
	if(tcpSocket->isStarting())
		return false;
	if(receiveCallback == nullptr || eventCallback == nullptr)
		return false;

	tcpSocket->setConnectCallBack(onServerConnected, this);
	tcpSocket->setDisconnectCallBack(onServerDisonnected, this);
	tcpSocket->setGettingCallBack(onServerReceived, this);
	tcpSocket->setSendingResponseCallBack(onServerSendResponse, this);
	tcpSocket->setErrorReportCallBack(onServerErrorReport, this);

	bufferLength = maxBufferLength;
	return tcpSocket->start(port, isIpv6);
}

bool SocketApplication::stop(){
	if(!tcpSocket->isStarting())
		return true;
	return tcpSocket->stop(20000);
}

int32 SocketApplication::send(uint32 clientId, int32 serials, MessageType type, int32 extendVersion, google::protobuf::Message&extend, void*content){
	if(!tcpSocket->isStarting())
		return false;

	auto client = clients.find(clientId);
	if(client == clients.end())
		return false;

	int32 extendLength = 0;
	int64 contentLength = 0;
	switch(extendVersion){
		case 1:
		{
			ArmyAntMessage::System::SocketExtendNormal_V0_0_0_1*headExtend = static_cast<ArmyAntMessage::System::SocketExtendNormal_V0_0_0_1*>(&extend);
			extendLength = headExtend->ByteSizeLong();
			contentLength = headExtend->content_length();
			break;
		}
		default:
			return false;
	}

	MessageBaseHead head{
		serials,
		type,
		extendVersion,
		extendLength,
	};
	int64 totalLength = sizeof(head) + head.extendLength + contentLength;
	uint8* sendingBuffer = new uint8[totalLength];
	memcpy(sendingBuffer, &head, sizeof(head));
	extend.SerializePartialToArray(sendingBuffer + sizeof(head), extend.ByteSizeLong());
	memcpy(sendingBuffer + sizeof(head) + extendLength, content, contentLength);
	if(client->second->counter == AA_INT64_MAX - 1)
		client->second->counter = 0;
	client->second->rwMutex.lock();
	client->second->waitingResponseSended.insert(std::make_pair(++client->second->counter, sendingBuffer));
	client->second->rwMutex.unlock();
	return tcpSocket->send(clientId, sendingBuffer, totalLength) == 0 ? 0 : client->second->counter;
}

} // namespace ArmyAntServer