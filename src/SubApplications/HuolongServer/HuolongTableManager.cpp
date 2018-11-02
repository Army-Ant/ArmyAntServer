#include <HuolongTable.h>
#include <HuolongTableManager.h>
#include <HuolongServer.h>
#include <ArmyAntMessage/SubApps/Huolong.pb.h>

#include <AAMath.h>

namespace ArmyAntServer{

HuolongTableManager::~HuolongTableManager(){
	mutex.lock();
	auto table = tables.begin();
	while(table != tables.end()){
		delete table->second;
		table = tables.erase(table);
	}
	mutex.unlock();
}

int32 HuolongTableManager::createTable(ArmyAntMessage::SubApps::HuolongGameType type, std::string creatorId){
	int32 roomNum = getRandomTableNumber();
	while(isTableExist(roomNum)){
		++roomNum;
	}
	auto table = new HuolongTable(hserver.getUserDataManager(), roomNum, creatorId.c_str(), type);
	mutex.lock();
	tables.insert(std::make_pair(roomNum, table));
	mutex.unlock();
	return roomNum;
}
int32 HuolongTableManager::enterTable(int32 tableId, std::string userId){
	mutex.lock();
	if(!isTableExist(tableId)){
		mutex.unlock();
		return -1;  //  牌桌不存在
	}
	HuolongTable* t = tables.find(tableId)->second;
	mutex.unlock();
	if(t->getSelfPlayer() == "")
		t->setSelfPlayer(userId);
	else if(t->getNextPlayer() == "")
		t->setNextPlayer(userId);
	else if(t->getFriendPlayer() == "")
		t->setFriendPlayer(userId);
	else if(t->getBackPlayer() == "")
		t->setBackPlayer(userId);
	else
		return -2;  //  桌内人数已满
	// 通知桌内其他人
	ArmyAntMessage::SubApps::SM2C_HuolongNoticeRoomInfo msg;
	msg.set_allocated_room_info(t->getTableInfoMsg());
	if(t->getSelfPlayer() != "" && t->getSelfPlayer() != userId){
		auto user = hserver.getUserById(t->getSelfPlayer());
		if(user != nullptr){
			hserver.sendMsgToUser(t->getSelfPlayer(), ArmyAntMessage::System::ConversationStepType::NoticeOnly, &msg);
		}
	}
	if(t->getNextPlayer() != "" && t->getNextPlayer() != userId){
		auto user = hserver.getUserById(t->getNextPlayer());
		if(user != nullptr){
			hserver.sendMsgToUser(t->getNextPlayer(), ArmyAntMessage::System::ConversationStepType::NoticeOnly, &msg);
		}
	}
	if(t->getFriendPlayer() != "" && t->getFriendPlayer() != userId){
		auto user = hserver.getUserById(t->getFriendPlayer());
		if(user != nullptr){
			hserver.sendMsgToUser(t->getFriendPlayer(), ArmyAntMessage::System::ConversationStepType::NoticeOnly, &msg);
		}
	}
	if(t->getBackPlayer() != "" && t->getBackPlayer() != userId){
		auto user = hserver.getUserById(t->getBackPlayer());
		if(user != nullptr){
			hserver.sendMsgToUser(t->getBackPlayer(), ArmyAntMessage::System::ConversationStepType::NoticeOnly, &msg);
		}
	}
	return 0;
}

bool HuolongTableManager::isTableExist(int32 id) const{
	return tables.find(id) != tables.end();
}
HuolongTable & HuolongTableManager::getTableById(int32 id){
	HuolongTable* ret = nullptr;
	mutex.lock();
	auto t = tables.find(id);
	ret = t->second;
	mutex.unlock();
	return *ret;
}

int32 HuolongTableManager::getRandomTableNumber(){
	auto ret = ArmyAnt::Math::getRandomInt32Number(0, 99990000);
	return ret;
}

HuolongTableManager::HuolongTableManager(HuolongServer & hserver):hserver(hserver), mutex(), tables(){

}

}
