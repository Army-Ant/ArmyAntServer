#ifndef HUOLONG_BATTLE_PLAYER___LOCAL_HUMAN_H_20181107
#define HUOLONG_BATTLE_PLAYER___LOCAL_HUMAN_H_20181107

#include <IHuolongBattlePlayer.h>

namespace ArmyAntServer{

class HuolongBattlePlayer_LocalHuman : public IHuolongBattlePlayer{
public:
	HuolongBattlePlayer_LocalHuman();
	~HuolongBattlePlayer_LocalHuman();

public:


private:
	AA_FORBID_COPY_CTOR(HuolongBattlePlayer_LocalHuman);
	AA_FORBID_ASSGN_OPR(HuolongBattlePlayer_LocalHuman);
};

}

#endif // HUOLONG_BATTLE_PLAYER___LOCAL_HUMAN_H_20181107
