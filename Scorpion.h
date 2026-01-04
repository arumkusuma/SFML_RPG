#ifndef SCORPION_H
#define SCORPION_H

#include "Enemy.h"

class Scorpion :
    public Enemy
{
private:
    sf::RectangleShape hpBar;
    sf::Font font;
    sf::Text hpText;
    AIFollow* follow;
    

    void initVariables();
    void initAnimations();
    void initGUI();
    void initAI();

public:
    Scorpion(float x, float y, sf::Texture& texture_sheet, EnemySpawnerTile& enemy_spawner_tile, Entity& player);
    virtual ~Scorpion();

    //Functions
    void generateAttributes(const unsigned level);

    void updateAnimation(const float& dt);
    void update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view);
    void render(sf::RenderTarget& target, sf::Shader* shader = NULL, const sf::Vector2f light_position = sf::Vector2f(), const bool show_hitbox = false);
};

#endif //!SCORPION_H