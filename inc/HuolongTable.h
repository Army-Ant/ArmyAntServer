#ifndef HUOLONG_TABLE_H_20181101
#define HUOLONG_TABLE_H_20181101

#include <AADefine.h>
#include <ArmyAntMessage/SubApps/Huolong.pb.h>

namespace ArmyAntServer{
class HuolongUserDataManager;

class HuolongTable{
public:
	HuolongTable(HuolongUserDataManager& dataMgr, const int32 room_number, const char* const creator_id, const ArmyAntMessage::SubApps::HuolongGameType type);
	~HuolongTable();

public:
	ArmyAntMessage::SubApps::ST_HuolongRoomInfo* getTableInfoMsg();
	int32 getRoomNumber()const;
	std::string getCreatorUid()const;
	const ArmyAntMessage::SubApps::HuolongGameType getType()const;
	std::string getSelfPlayer()const;
	std::string getNextPlayer()const;
	std::string getFriendPlayer()const;
	std::string getBackPlayer()const;
	ArmyAntMessage::SubApps::HuolongStatus getStatus()const;
	bool setSelfPlayer(std::string value = "");
	bool setNextPlayer(std::string value = "");
	bool setFriendPlayer(std::string value = "");
	bool setBackPlayer(std::string value = "");
	bool setStatus(ArmyAntMessage::SubApps::HuolongStatus value);

private:
	HuolongUserDataManager& dataMgr;

private:
	const int32 room_number;
	const std::string creator_uid;
	const ArmyAntMessage::SubApps::HuolongGameType type;
	std::string self_player;
	std::string next_player;
	std::string friend_player;
	std::string back_player;
	ArmyAntMessage::SubApps::HuolongStatus status;

private:
	AA_FORBID_ASSGN_OPR(HuolongTable);
	AA_FORBID_COPY_CTOR(HuolongTable);
};

}


#endif // HUOLONG_TABLE_H_20181101
