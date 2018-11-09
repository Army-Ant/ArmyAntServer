#ifndef HUOLONG_BATTLE_PLAYER___SERVER_ROBOT_H_20181107
#define HUOLONG_BATTLE_PLAYER___SERVER_ROBOT_H_20181107

#include <IHuolongBattlePlayer.h>

namespace ArmyAntServer{

class HuolongBattlePlayer_ServerRobot : public IHuolongBattlePlayer{
public:
	HuolongBattlePlayer_ServerRobot();
	~HuolongBattlePlayer_ServerRobot();

public:


private:
	AA_FORBID_COPY_CTOR(HuolongBattlePlayer_ServerRobot);
	AA_FORBID_ASSGN_OPR(HuolongBattlePlayer_ServerRobot);
};

}

#endif // HUOLONG_BATTLE_PLAYER___SERVER_ROBOT_H_20181107
