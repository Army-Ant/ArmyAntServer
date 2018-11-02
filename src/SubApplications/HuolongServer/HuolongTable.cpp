#include <HuolongTable.h>
#include <HuolongUserDataManager.h>

namespace ArmyAntServer{

HuolongTable::HuolongTable(HuolongUserDataManager& dataMgr, const int32 room_number, const char* const creator_id, const ArmyAntMessage::SubApps::HuolongGameType type) :dataMgr(dataMgr), room_number(room_number), creator_uid(creator_id), type(type),self_player(), next_player(), friend_player(), back_player(), status(ArmyAntMessage::SubApps::HuolongStatus::Idle){

}

HuolongTable::~HuolongTable(){

}

ArmyAntMessage::SubApps::ST_HuolongRoomInfo * HuolongTable::getTableInfoMsg(){
	auto ret = new ArmyAntMessage::SubApps::ST_HuolongRoomInfo();
	ret->set_room_number(room_number);
	ret->set_creator_uid(creator_uid);
	ret->set_type(type);
	ret->set_allocated_self_player(dataMgr.getPlayerInfoMsg(self_player));
	ret->set_allocated_back_player(dataMgr.getPlayerInfoMsg(back_player));
	ret->set_allocated_friend_player(dataMgr.getPlayerInfoMsg(friend_player));
	ret->set_allocated_next_player(dataMgr.getPlayerInfoMsg(next_player));
	ret->set_status(status);
	return ret;
}

int32 HuolongTable::getRoomNumber() const{
	return room_number;
}

std::string HuolongTable::getCreatorUid() const{
	return creator_uid;
}

const ArmyAntMessage::SubApps::HuolongGameType HuolongTable::getType() const{
	return type;
}

std::string HuolongTable::getSelfPlayer() const{
	return self_player;
}

std::string HuolongTable::getNextPlayer() const{
	return next_player;
}

std::string HuolongTable::getFriendPlayer() const{
	return friend_player;
}

std::string HuolongTable::getBackPlayer() const{
	return back_player;
}

ArmyAntMessage::SubApps::HuolongStatus HuolongTable::getStatus() const{
	return status;
}

bool HuolongTable::setSelfPlayer(std::string value){
	self_player = value;
	return true;
}

bool HuolongTable::setNextPlayer(std::string value){
	next_player = value;
	return true;
}

bool HuolongTable::setFriendPlayer(std::string value){
	friend_player = value;
	return true;
}

bool HuolongTable::setBackPlayer(std::string value){
	back_player = value;
	return true;
}

bool HuolongTable::setStatus(ArmyAntMessage::SubApps::HuolongStatus value){
	return false;
}

}
