#include "stdafx.h"
#include "AttributeComponent.h"

AttributeComponent::AttributeComponent(int level)
{
	this->level = level;
	this->exp = 0;
	this->expNext = (50 * pow(this->level, 3) - 150 * pow(this->level, 2) + (uint64_t)400 * this->level) / 3;
	this->attributePoints = 2;

	this->vitality = 1;
	this->strength = 1;
	this->dexterity = 1;
	this->agility = 1;
	this->intelligence = 1;

	this->updateLevel();
	this->updateStats(true);
}

AttributeComponent::~AttributeComponent()
{
	
}

//Functions
std::string AttributeComponent::debugPrint() const
{
	std::stringstream ss;

	ss << "Level: " << this->level << "\n"
		<< "Exp: " << this->exp << "\n"
		<< "Exp Next: " << this->expNext << "\n"
		<< "Attp: " << this->attributePoints << "\n";

	return ss.str();
}

void AttributeComponent::loseHP(const int hp_loss)
{
	this->hp -= hp_loss;
    if (this->hp < 0)
        this->hp = 0;
}

void AttributeComponent::gainHP(const int hp)
{
	this->hp += hp;

	if (this->hp > this->hpMax)
		this->hp = this->hpMax;
}

void AttributeComponent::loseEXP(const int exp)
{
	this->exp -= exp;

	if (this->exp < 0)
		this->exp = 0;
}

void AttributeComponent::gainExp(const int exp)
{
	this->exp += exp;

	this->updateLevel();
}

const bool AttributeComponent::isDead() const
{
	return this->hp <= 0;
}

void AttributeComponent::updateStats(const bool reset)
{
    this->hpMax = 20 + (this->vitality * 15) + (this->strength * 5) + (this->intelligence * 1);

    this->damageMin = this->strength * 2 + this->strength / 2;
    this->damageMax = this->strength * 2 + this->strength;
    
    this->accuracy  = this->dexterity * 4 + this->dexterity / 2;
    this->defence   = this->agility * 2 + this->agility / 4;
    this->luck      = this->intelligence * 2;

    if (reset)
    {
        this->hp = this->hpMax;
    }
}

void AttributeComponent::updateLevel()
{
	while (this->exp >= this->expNext)
	{
		++this->level;
		this->exp -= this->expNext;
		this->expNext = (50 * pow(this->level, 3) - 150 * pow(this->level, 2) + (uint64_t)400 * this->level) / 3;
		++this->attributePoints;
	}
}

void AttributeComponent::update()
{
	this->updateLevel();
}



