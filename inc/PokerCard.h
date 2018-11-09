#ifndef POKER_CARD_H_20181107
#define POKER_CARD_H_20181107

#include <AADefine.h>

namespace ArmyAntServer{

class PokerCard{
public:
	enum class Color : int8{
		Any,  // 任意, 未知
		Spades, // 黑桃
		Heart, // 红桃
		Cube, // 草花
		Diamond, // 方片
		Joker, // 王牌
		Main, // 主
		Unmain, // 副
	};

public:
	PokerCard(int32 id);
	PokerCard(int8 value, Color color, int8 group = 1i8);
	PokerCard(const PokerCard&value);
	PokerCard&operator=(const PokerCard&value);
	~PokerCard();

public:
	int8 getValue()const;
	Color getColor()const;
	int8 getGroup()const;
	int32 getId()const;

public:
	static int8 getValue(int32 id);
	static Color getColor(int32 id);
	static int8 getGroup(int32 id);

private:
	int32 id;
};

}

#endif // POKER_CARD_H_20181107
