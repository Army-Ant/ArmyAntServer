#ifndef HUOLONG_TABLE_MANAGER_H_20181101
#define HUOLONG_TABLE_MANAGER_H_20181101

#include <map>
#include <AADefine.h>
#include <ArmyAntMessage/SubApps/Huolong.pb.h>
#include <DebugMutex.h>

namespace ArmyAntServer{
class HuolongServer;
class HuolongTable;

class HuolongTableManager{
public:
	~HuolongTableManager();

public:
	int32 createTable(ArmyAntMessage::SubApps::HuolongGameType type, std::string creatorId);
	int32 enterTable(int32 tableId, std::string userId);
	bool isTableExist(int32 id) const;
	HuolongTable& getTableById(int32 id);

private:
	int32 getRandomTableNumber();

private:
	HuolongServer& hserver;
	Mutex mutex;
	std::map<int32, HuolongTable*> tables;

private:
	AA_FORBID_ASSGN_OPR(HuolongTableManager);
	AA_FORBID_COPY_CTOR(HuolongTableManager);
	HuolongTableManager(HuolongServer& hserver);
	friend class HuolongServer;
};

}


#endif // HUOLONG_TABLE_MANAGER_H_20181101
