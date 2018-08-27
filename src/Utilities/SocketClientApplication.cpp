#include <SocketClientApplication.h>

namespace ArmyAntServer{

static bool onClientConnected(bool isSucceed, void*pThis){
	auto self = static_cast<SocketClientApplication*>(pThis);

	if(self->eventCallback == nullptr){
		return false;
	}
	if(isSucceed)
		self->eventCallback(SocketClientApplication::EventType::Connected, "Server connected successful");
	else
		self->eventCallback(SocketClientApplication::EventType::ErrorReport, "Server connected failed");
	return false;
}

static bool onClientLostServer(void*pThis){
	auto self = static_cast<SocketClientApplication*>(pThis);
	if(self->eventCallback == nullptr){
		return true;
	}
	self->eventCallback(SocketClientApplication::EventType::LostServer, "Server disconnected");
	return true;
}

static void onClientReceived(uint8*data, mac_uint datalen, void*pThis){
	auto self = static_cast<SocketClientApplication*>(pThis);
	if(self->receiveCallback != nullptr){
		self->rwMutex.lock();
		while(datalen > 0){
			mac_uint currCopyLen = datalen;
			// ������buffer����, ����д��һ����
			if(self->receivingBufferEnd + datalen > self->bufferLength){
				currCopyLen = self->bufferLength - self->receivingBufferEnd - 1;
			}
			// д�뵽buffer
			datalen -= currCopyLen;
			memcpy(self->receivingBuffer + self->receivingBufferEnd, data, currCopyLen);
			self->receivingBufferEnd += currCopyLen;
			// ����Ӧ��ͷ
			if(self->receivingBufferEnd > sizeof(MessageBaseHead)){
				MessageBaseHead tmpHead;
				memcpy(&tmpHead, self->receivingBuffer, sizeof(MessageBaseHead));
				// ������չͷ
				if(self->receivingBufferEnd >= sizeof(MessageBaseHead) + tmpHead.extendLength){
					int64 appId = 0;
					int32 contentLength = 0;
					int32 contentCode = 0;
					switch(tmpHead.extendVersion){
						case 1:
						{
							ArmyAntMessage::System::SocketExtendNormal_V0_0_0_1 extend;
							extend.ParseFromArray(self->receivingBuffer + sizeof(MessageBaseHead), tmpHead.extendLength);
							appId = extend.app_id();
							contentLength = extend.content_length();
							contentCode = extend.message_code();
							break;
						}
					}
					// ��ȡ����, ���ͻص�
					uint32 usedLength = sizeof(MessageBaseHead) + tmpHead.extendLength + contentLength;
					if(self->receivingBufferEnd >= usedLength){
						self->receiveCallback(tmpHead, appId, contentLength, contentCode,
											  self->receivingBuffer + sizeof(MessageBaseHead) + tmpHead.extendLength);
						// ��֮����������, ��ת�Ƶ���㴦, �Ѵ��������Ϣ��buffer�Ƴ�
						if(self->receivingBufferEnd > usedLength){
							memcpy(self->receivingBuffer, self->receivingBuffer + usedLength, self->receivingBufferEnd - usedLength);
						}
						self->receivingBufferEnd -= usedLength;
						data = data + currCopyLen;
						continue; // ѭ��Ӧ��������ȷ�������˳�
					}
				}
			}
			// buffer������, ȴδ������һ��������Э���, �����������
			if(datalen > 0){
				self->eventCallback(SocketClientApplication::EventType::ErrorReport, ArmyAnt::String("Receiving buffer full !"));
				break;
			}
		}
		self->rwMutex.unlock();
	}
}

static bool onClientSendResponse(mac_uint sendedSize, uint32 retriedTimes, int32, void*sendedData, uint64 len, void* pThis){
	auto self = static_cast<SocketClientApplication*>(pThis);
	if(self->eventCallback != nullptr){
		if(sendedSize > 0){
			int32 sendingIndex = 0;
			auto sendedList = self->waitingResponseSended;
			for(auto i = sendedList.begin(); i != sendedList.end(); ++i){
				if(!memcmp(sendedData, i->second, sendedSize))
					sendingIndex = i->first;
			}
			self->eventCallback(SocketClientApplication::EventType::SendingResponse, ArmyAnt::String("Sending responsed, sending index: ") + int64(sendingIndex));
			if(sendingIndex > 0){
				auto i = sendedList.find(sendingIndex);
				ArmyAnt::Fragment::AA_SAFE_DELALL(i->second);
				sendedList.erase(i);
			}
		} else
			self->eventCallback(SocketClientApplication::EventType::ErrorReport, ArmyAnt::String("Sending failed"));
	}
	return false;
}

static void onClientErrorReport(ArmyAnt::SocketException err, const ArmyAnt::IPAddr&addr, uint16 port, ArmyAnt::String functionName, void*pThis){
	auto self = static_cast<SocketClientApplication*>(pThis);
	if(self->eventCallback != nullptr){
		self->eventCallback(SocketClientApplication::EventType::ErrorReport, ArmyAnt::String("Found error, code:") + int64(err.code) + ", message: " + err.message);
	}
}

/***********************************************************/

SocketClientApplication::SocketClientApplication()
	:tcpSocket(), eventCallback(nullptr), receiveCallback(nullptr), rwMutex(), bufferLength(0)
{}
SocketClientApplication::~SocketClientApplication(){}

bool SocketClientApplication::setEventCallback(SocketClientApplication::EventCallback cb){
	eventCallback = cb;
	return true;
}

bool SocketClientApplication::setReceiveCallback(SocketClientApplication::ReceiveCallback cb){
	receiveCallback = cb;
	return true;
}

ArmyAnt::TCPClient&SocketClientApplication::getSocket(){
	return tcpSocket;
}

bool SocketClientApplication::connect(ArmyAnt::IPAddr & ip, uint16 port, uint16 localPort, bool isAsync, uint32 maxBufferLength){
	if(tcpSocket.isConnection())
		return false;
	if(receiveCallback == nullptr || eventCallback == nullptr)
		return false;

	tcpSocket.setLostServerCallBack(onClientLostServer, this);
	tcpSocket.setGettingCallBack(onClientReceived, this);
	tcpSocket.setSendingResponseCallBack(onClientSendResponse, this);
	tcpSocket.setErrorReportCallBack(onClientErrorReport, this);

	bufferLength = maxBufferLength;
	// tcpSocket.setMaxIOBufferLen(maxBufferLength);
	tcpSocket.setServerAddr(ip);
	tcpSocket.setServerPort(port);
	return tcpSocket.connectServer(localPort, isAsync, onClientConnected, this);
}

bool SocketClientApplication::disconnect(){
	if(!tcpSocket.isConnection())
		return true;
	return tcpSocket.disconnectServer(20000);
}

int32 SocketClientApplication::send(int32 serials, MessageType type, int32 extendVersion, google::protobuf::Message&extend, void*content){
	if(!tcpSocket.isConnection())
		return false;

	int64 index = 0;
	int32 extendLength = 0;
	int64 contentLength = 0;
	switch(extendVersion){
		case 1:
		{
			ArmyAntMessage::System::SocketExtendNormal_V0_0_0_1*headExtend = static_cast<ArmyAntMessage::System::SocketExtendNormal_V0_0_0_1*>(&extend);
			extendLength = headExtend->ByteSize();
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
	extend.SerializePartialToArray(sendingBuffer + sizeof(head), extend.ByteSize());
	memcpy(sendingBuffer + sizeof(head) + extendLength, content, contentLength);
	if(counter == AA_INT64_MAX - 1)
		counter = 0;
	rwMutex.lock();
	waitingResponseSended.insert(std::make_pair(++counter, sendingBuffer));
	rwMutex.unlock();
	return tcpSocket.send(sendingBuffer, totalLength) == 0 ? 0 : counter;
}

}
