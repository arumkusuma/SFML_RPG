#include "stdafx.h" 
#include "Sword.h"
#include <cmath> 

Sword::Sword(unsigned level, unsigned damageMin, unsigned damageMax, unsigned range,
    unsigned value, 
    std::string texture_file)
    : Weapon(level, damageMin, damageMax, range, value, texture_file)
{
    this->weapon_sprite.setOrigin(
        0.f, 
        this->weapon_sprite.getGlobalBounds().height
    );
}

Sword::~Sword()
{

}

Sword* Sword::clone()
{
    return new Sword(*this);
}

void Sword::generate(const unsigned levelMin, const unsigned levelMax)
{
    // mengko diisi logika RNG stats tergantung level
}

void Sword::update(const sf::Vector2f& mouse_pos_view, const sf::Vector2f center)
{   
    float dX = mouse_pos_view.x - center.x;
    float dY = mouse_pos_view.y - center.y;
    const float PI = 3.14159265f;
    float deg = atan2(dY, dX) * 180.f / PI;

    // LOGIKA ANIMASI 
    float offsetDistance = 3.f; 

    if (this->attackTimer.getElapsedTime().asMilliseconds() < this->attackTimerMax / 2) 
    {
        offsetDistance = 20.f; 
    }
    
    // POSISI
    float rad = deg * (PI / 180.f); 
    float posX = center.x + std::cos(rad) * offsetDistance;
    float posY = center.y + std::sin(rad) * offsetDistance;
    
    this->weapon_sprite.setPosition(posX, posY);

    this->weapon_sprite.setRotation(deg + 90.f);
}

void Sword::render(sf::RenderTarget& target, sf::Shader* shader)
{
    if (shader)
        target.draw(this->weapon_sprite, shader);
    else
        target.draw(this->weapon_sprite);
}

const sf::FloatRect Sword::getGlobalBounds() const
{
    return this->weapon_sprite.getGlobalBounds();
}
