#include <AALog.h>

#include <EventManager.h>
#include <HuolongConstants.h>
#include <HuolongServer.h>
#include <HuolongUser.h>
#include <HuolongUserDataManager.h>
#include <HuolongTable.h>
#include <ArmyAntMessage/SubApps/Huolong.pb.h>

#define NETWORK_CALLBACK(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)

namespace ArmyAntServer{

static const char* const LOGGER_TAG = "HuolongServer_User";

HuolongUser::HuolongUser(HuolongServer& hserver, LoginType type, std::string id, std::string auth, int32 clientId, HuolongUserDataManager&dataMgr)
	: hserver(hserver), type(type), dataMgr(dataMgr), clientId(clientId), userId(dataMgr.getUserId(type, id, auth)){
	hserver.addUserListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongCreateTableRequest>(), clientId, NETWORK_CALLBACK(HuolongUser::onCreateTable));
	hserver.addUserListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongEnterTableRequest>(), clientId, NETWORK_CALLBACK(HuolongUser::onEnterTable));
}

HuolongUser::HuolongUser(HuolongServer& hserver, std::string autoLoginAuth, int32 clientId, HuolongUserDataManager & dataMgr)
	: hserver(hserver), type(LoginType::Auto), dataMgr(dataMgr), clientId(clientId), userId(dataMgr.getUserIdByAuth(autoLoginAuth.c_str())){
	hserver.removeUserListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongCreateTableRequest>(), clientId);
	hserver.removeUserListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongEnterTableRequest>(), clientId);
}

HuolongUser::~HuolongUser(){

}

std::string HuolongUser::getUserId() const{
	return userId;
}

int32 HuolongUser::getClientId() const{
	return clientId;
}

std::string HuolongUser::generateAutoLoginAuth(){
	return dataMgr.generateAutoLoginAuth(userId);
}

bool HuolongUser::setUserPasswordMd5(std::string passwordMd5){
	return dataMgr.setUserPasswordMd5(userId, passwordMd5);
}

std::string HuolongUser::getUserPasswordMd5(){
	return dataMgr.getUserPasswordMd5(userId);
}

bool HuolongUser::setUserNickname(std::string nickname){
	return dataMgr.setUserNickname(userId, nickname);
}

std::string HuolongUser::getUserNickname(){
	return dataMgr.getUserNickname(userId);
}

bool HuolongUser::setUserAvatar(std::string avatarUrl){
	return dataMgr.setUserAvatar(userId, avatarUrl);
}

std::string HuolongUser::getUserAvatar(){
	return dataMgr.getUserAvatar(userId);
}

ArmyAntMessage::SubApps::ST_HuolongPlayerInfo * HuolongUser::getPlayerInfoMsg(){
	return dataMgr.getPlayerInfoMsg(userId);
}

void HuolongUser::onCreateTable(int32, int32 convCode, int32, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_HuolongCreateTableRequest msg;
	if(!msg.ParseFromArray(message, length)){
		hserver.getLogger().pushLog("Parse send protocol failure !", ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto type = msg.type();
	// response
	ArmyAntMessage::SubApps::SM2C_HuolongCreateTableResponse response;
	if(dataMgr.getUserTable(userId) >= 0){
		response.set_result(Huolong::Constants::USER_HAS_BEEN_IN_BATTLE);
		response.set_message("The user has been in a table");
	} else{
		auto tableId = hserver.getTableManager().createTable(type, userId);
		if(tableId < 0){
			response.set_result(tableId);
			response.set_message("Create table failed");
		} else{
			dataMgr.setUserEnterTable(userId, tableId);    // Warning : 慎重确认这一步可能返回false的情况
			response.set_result(0);
			response.set_message("Successful");
			hserver.getTableManager().enterTable(tableId, userId);
			auto& table = hserver.getTableManager().getTableById(tableId);
			auto info = table.getTableInfoMsg();
			response.set_allocated_room_info(info);
		}
	}
	hserver.sendMsgToUser(userId, convCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

void HuolongUser::onEnterTable(int32, int32 convCode, int32, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_HuolongEnterTableRequest msg;
	if(!msg.ParseFromArray(message, length)){
		hserver.getLogger().pushLog("Parse send protocol failure !", ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto number = msg.room_number();
	// response
	ArmyAntMessage::SubApps::SM2C_HuolongEnterTableResponse response;
	if(dataMgr.getUserTable(userId) >= 0){
		response.set_result(Huolong::Constants::USER_HAS_BEEN_IN_BATTLE);
		response.set_message("The user has been in a table");
	} else{
		auto ret = hserver.getTableManager().enterTable(number, userId);
		if(ret != Huolong::Constants::ENTER_TABLE_RESULT_SUCCESS){
			response.set_result(ret);
			response.set_message("Enter table failed");
		} else{
			dataMgr.setUserEnterTable(userId, number);    // Warning : 慎重确认这一步可能返回false的情况
			response.set_result(0);
			response.set_message("Successful");
			auto info = hserver.getTableManager().getTableById(number).getTableInfoMsg();
			response.set_allocated_room_info(info);
		}
	}
	hserver.sendMsgToUser(userId, convCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

}

#undef NETWORK_CALLBACK
