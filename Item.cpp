#include "stdafx.h"
#include "Item.h"

void Item::initVariables()
{
   
}

Item::Item(unsigned level, unsigned value)
{
    this->initVariables();

    this->level = level;
    this->value = value;
    this->type = ItemTypes::IT_DEFAULT;
}

Item::~Item()
{

}

//POSISI & VISUAL

const sf::Vector2f& Item::getPosition() const
{
    return this->sprite.getPosition();
}

void Item::setPosition(const float x, const float y)
{
    this->sprite.setPosition(x, y);
}

const sf::FloatRect Item::getGlobalBounds() const
{
    return this->sprite.getGlobalBounds();
}

void Item::update(const float& dt, const sf::Vector2f& mouse_pos_view)
{
    // Nanti bisa tambahin animasi melayang
}

void Item::render(sf::RenderTarget& target, sf::Shader* shader)
{
    if (shader)
        target.draw(this->sprite, shader);
    else
        target.draw(this->sprite);
}

void Item::setScale(const float x, const float y)
{
    this->sprite.setScale(x, y);
}