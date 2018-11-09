#ifndef IHUOLONG_BATTLE_PLAYER_H_20181107
#define IHUOLONG_BATTLE_PLAYER_H_20181107

#include <AADefine.h>

namespace ArmyAntServer{
class HuolongTable;

class IHuolongBattlePlayer{
public:
	enum class Type{
		Unknown,
		LocalHuman,
		LocalRobot,
		ServerRobot,
	};

public:
	IHuolongBattlePlayer();
	virtual ~IHuolongBattlePlayer();

public:
	virtual Type getType()const = 0;

public:
	virtual bool init(const HuolongTable&table) = 0;

private:
	AA_FORBID_COPY_CTOR(IHuolongBattlePlayer);
	AA_FORBID_ASSGN_OPR(IHuolongBattlePlayer);
};

}

#endif // IHUOLONG_BATTLE_PLAYER_H_20181107
