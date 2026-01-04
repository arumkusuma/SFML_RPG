#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum ItemTypes {IT_DEFAULT = 0, IT_RANGEDWEAPON, IT_MELEEWEAPON, IT_ARMOR};
enum ItemRarities {COMMON = 0, UNCOMMON, RARE, EPIC, LEGENDARY, MYTHIC};

class Item
{
private:
    void initVariables();

protected:
    //visual
    sf::Sprite sprite;
    sf::Texture texture;

    //Variables
    short unsigned type;
    unsigned level;
    unsigned value;

public:
    Item(unsigned level, unsigned value);
    virtual ~Item();

    //Accessors
    const short unsigned& getType() const { return this->type;}
    const unsigned& getLevel() const { return this->level;}
    const unsigned& getValue() const { return this->value;}

    const sf::Vector2f& getPosition() const;
    virtual void setPosition(const float x, const float y);
    
    // Hitbox 
    virtual const sf::FloatRect getGlobalBounds() const;
    virtual void setScale(const float x, const float y);

    //Function
    virtual Item* clone() = 0;
    
    virtual void update(const float& dt, const sf::Vector2f& mouse_pos_view);
    virtual void render(sf::RenderTarget& target, sf::Shader* shader = nullptr);
};

#endif