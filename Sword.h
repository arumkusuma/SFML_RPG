#ifndef SWORD_H
#define SWORD_H

#include "Weapon.h"

class Sword :
    public Weapon
{
private:
    // Tidak ada variabel khusus untuk saat ini

public:
    Sword(unsigned level, unsigned damageMin, unsigned damageMax, unsigned range,
        unsigned value, 
        std::string texture_file);
    
    virtual ~Sword();

    // Fungsi Wajib dari Parent (Item & Weapon)
    virtual Sword* clone();
    virtual void generate(const unsigned levelMin, const unsigned levelMax);

    virtual void update(const sf::Vector2f& mouse_pos_view, const sf::Vector2f center);
	virtual void render(sf::RenderTarget& target, sf::Shader* shader = nullptr);
    virtual const sf::FloatRect getGlobalBounds() const;
};

#endif