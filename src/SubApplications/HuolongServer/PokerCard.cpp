#include <PokerCard.h>

namespace ArmyAntServer{

PokerCard::PokerCard(int32 id) : id(id){}

PokerCard::PokerCard(int8 value, Color color, int8 group) : id(value + int8(color)*100 + group * 1000){}

PokerCard::PokerCard(const PokerCard & value) : id(value.id){}

PokerCard & PokerCard::operator=(const PokerCard & value){
	id = value.id;
	return *this;
}

PokerCard::~PokerCard(){}

int8 PokerCard::getValue() const{
	return getValue(id);
}

PokerCard::Color PokerCard::getColor() const{
	return getColor(id);
}

int8 PokerCard::getGroup() const{
	return getGroup(id);
}

int32 PokerCard::getId() const{
	return id;
}

int8 PokerCard::getValue(int32 id){
	return int8(id % 100);
}

PokerCard::Color PokerCard::getColor(int32 id){
	return Color(id / 100 % 10);
}

int8 PokerCard::getGroup(int32 id){
	return int8(id / 1000);
}




}
