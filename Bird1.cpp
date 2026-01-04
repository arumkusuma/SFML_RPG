#include "stdafx.h"
#include "Bird1.h"

void Bird1::initVariables()
{
    this->damageTimerMax = 1000; // delay
    this->despawnTimerMax = 1000;
    this->gainExp = 20; // +EXP lebih besar dari tikus
}

void Bird1::initAnimations()
{
    this->animationComponent->addAnimation("IDLE", 25.f, 0, 0, 2, 0, 61, 57);
    this->animationComponent->addAnimation("WALK_DOWN", 11.f, 0, 0, 2, 0, 61, 57);
    this->animationComponent->addAnimation("WALK_LEFT", 11.f, 0, 1, 2, 1, 61, 57);
    this->animationComponent->addAnimation("WALK_RIGHT", 11.f, 0, 2, 2, 2, 61, 57);
    this->animationComponent->addAnimation("WALK_UP", 11.f, 0, 3, 2, 3, 61, 57);
}

void Bird1::initAI()
{

}

//FUNGSI STATISTIK
void Bird1::generateAttributes(const unsigned level)
{
    this->attributeComponent->updateStats(true);
    
    // Balancing Burung
    this->attributeComponent->hpMax = 8 + (level * 1); 
    this->attributeComponent->hp = this->attributeComponent->hpMax;
}

void Bird1::initGUI()
{
  if (!this->font.loadFromFile("Resources/Fonts/Dosis-Light.ttf")) 
    {
        std::cout << "ERROR::BIRD1::INITGUI::GAGAL_LOAD_FONT" << "\n";
    }

    this->hpBar.setFillColor(sf::Color::Red);
    this->hpBar.setSize(sf::Vector2f(80.f, 15.f));
    this->hpBar.setPosition(this->sprite.getPosition());

    // Teks HP
    this->hpText.setFont(this->font);
    this->hpText.setCharacterSize(13);
    this->hpText.setFillColor(sf::Color::Black);
    this->hpText.setOutlineThickness(1.f);
}

//Constructors / Destructors
Bird1::Bird1(float x, float y, sf::Texture& texture_sheet, EnemySpawnerTile& enemy_spawner_tile, Entity& player)
    : Enemy(enemy_spawner_tile)
{
    this->initVariables();
    this->initGUI();
    this->type = 2;

    this->createHitboxComponent(this->sprite, 15.f, 15.f, 40.f, 40.f);
    this->createMovementComponent(250.f, 350.f, 50.f);
    this->createAnimationComponent(texture_sheet);
    this->createAttributeComponent(1);

    this->generateAttributes(this->attributeComponent->level);

    this->setPosition(x, y);
    this->initAnimations();

    this->follow = new AIFollow(*this, player);
}

Bird1::~Bird1()
{
    delete this->follow;
}

void Bird1::updateAnimation(const float& dt)
{
    if (this->movementComponent->getState(IDLE))
    {
        this->animationComponent->play("IDLE", dt);
    }
    else if (this->movementComponent->getState(MOVING_LEFT))
    {
        this->animationComponent->play("WALK_LEFT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
    }
    else if (this->movementComponent->getState(MOVING_RIGHT))
    {
        this->animationComponent->play("WALK_RIGHT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
    }
    else if (this->movementComponent->getState(MOVING_UP))
    {
        this->animationComponent->play("WALK_UP", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
    }
    else if (this->movementComponent->getState(MOVING_DOWN))
    {
        this->animationComponent->play("WALK_DOWN", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
    }

    if (this->damageTimer.getElapsedTime().asMilliseconds() <= this->damageTimerMax)
    {
        this->sprite.setColor(sf::Color::Red);
    }
    else
        this->sprite.setColor(sf::Color::White);
}

void Bird1::update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view)
{
    Enemy::update(dt, mouse_pos_view, view);

    this->movementComponent->update(dt);

    float hpPercent = static_cast<float>(this->attributeComponent->hp) / this->attributeComponent->hpMax;
    if (hpPercent < 0.f) hpPercent = 0.f;
    if (hpPercent > 1.f) hpPercent = 1.f;

    //Ukuran Bar Merah
    this->hpBar.setSize(sf::Vector2f(80.f * hpPercent, 15.f));
    this->hpBar.setPosition(this->sprite.getPosition().x - 10.f, this->sprite.getPosition().y - 25.f);

    // angka Teks
    std::stringstream ss;
    ss << this->attributeComponent->hp << " / " << this->attributeComponent->hpMax;
    this->hpText.setString(ss.str());

    this->hpText.setOrigin(this->hpText.getGlobalBounds().width / 2.f, this->hpText.getGlobalBounds().height / 2.f);
    
    this->hpText.setPosition(
        this->hpBar.getPosition().x + 40.f,
        this->hpBar.getPosition().y + 5.f 
    );

    this->updateAnimation(dt);
    this->hitboxComponent->update();
    this->follow->update(dt);
}

void Bird1::render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox)
{
  if (shader)
    {
        shader->setUniform("hasTexture", true);
        shader->setUniform("lightPos", light_position);
        target.draw(this->sprite, shader);
    }
    else
    {
        target.draw(this->sprite);
    }

    target.draw(this->hpBar);
    target.draw(this->hpText); 
    if (show_hitbox)
        this->hitboxComponent->render(target);
}