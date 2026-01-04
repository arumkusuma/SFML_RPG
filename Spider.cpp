#include "stdafx.h"
#include "Spider.h"

void Spider::initVariables()
{
    this->damageTimerMax = 1000;
    this->despawnTimerMax = 1000;
    this->gainExp = 30; 
}

void Spider::initAnimations()
{
    this->animationComponent->addAnimation("IDLE", 25.f, 0, 0, 3, 0, 60, 64);
    this->animationComponent->addAnimation("WALK_DOWN", 11.f, 0, 1, 3, 1, 60, 64);
    this->animationComponent->addAnimation("WALK_LEFT", 11.f, 0, 2, 3, 2, 60, 64);
    this->animationComponent->addAnimation("WALK_RIGHT", 11.f, 0, 3, 3, 3, 60, 64);
    this->animationComponent->addAnimation("WALK_UP", 11.f, 0, 4, 3, 4, 60, 64);
}

void Spider::initAI()
{
}

void Spider::initGUI()
{
    if (!this->font.loadFromFile("Resources/Fonts/Dosis-Light.ttf")) 
    {
        std::cout << "ERROR::SPIDER::INITGUI::GAGAL_LOAD_FONT" << "\n";
    }

    // Setup Bar Merah
    this->hpBar.setFillColor(sf::Color::Red);
    this->hpBar.setSize(sf::Vector2f(80.f, 15.f)); 
    this->hpBar.setPosition(this->sprite.getPosition());

    // Setup Teks HP
    this->hpText.setFont(this->font);
    this->hpText.setCharacterSize(13);
    this->hpText.setFillColor(sf::Color::Black);
    this->hpText.setStyle(sf::Text::Bold); 
}

void Spider::generateAttributes(const unsigned level)
{
    this->attributeComponent->updateStats(true);

    this->attributeComponent->hpMax = 15 + (level * 3);
    this->attributeComponent->hp = this->attributeComponent->hpMax;

    // Damage
    this->attributeComponent->damageMin = 2 + (level * 1);
    this->attributeComponent->damageMax = 4 + (level * 1);
}

Spider::Spider(float x, float y, sf::Texture& texture_sheet, EnemySpawnerTile& enemy_spawner_tile, Entity& player)
    : Enemy(enemy_spawner_tile)
{
    this->initVariables();
    this->initGUI();
    this->type = 1;

    this->createHitboxComponent(this->sprite, 10.f, 20.f, 40.f, 40.f);
    
    this->createMovementComponent(50.f, 300.f, 100.f); 
    
    this->createAnimationComponent(texture_sheet);
    this->createAttributeComponent(1);

    this->generateAttributes(this->attributeComponent->level);

    this->setPosition(x, y);
    this->initAnimations();

    this->follow = new AIFollow(*this, player);
}

Spider::~Spider()
{
    delete this->follow;
}

void Spider::updateAnimation(const float& dt)
{
    if (this->movementComponent->getState(IDLE))
        this->animationComponent->play("IDLE", dt);
    else if (this->movementComponent->getState(MOVING_LEFT))
        this->animationComponent->play("WALK_LEFT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
    else if (this->movementComponent->getState(MOVING_RIGHT))
        this->animationComponent->play("WALK_RIGHT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
    else if (this->movementComponent->getState(MOVING_UP))
        this->animationComponent->play("WALK_UP", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
    else if (this->movementComponent->getState(MOVING_DOWN))
        this->animationComponent->play("WALK_DOWN", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());

    if (this->damageTimer.getElapsedTime().asMilliseconds() <= this->damageTimerMax)
        this->sprite.setColor(sf::Color::Red);
    else
        this->sprite.setColor(sf::Color::White);
}

void Spider::update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view)
{
     Enemy::update(dt, mouse_pos_view, view);
    this->movementComponent->update(dt);

    //HP BAR & TEXT
    
    //Persentase
    float hpPercent = static_cast<float>(this->attributeComponent->hp) / this->attributeComponent->hpMax;
    if (hpPercent < 0.f) hpPercent = 0.f;
    if (hpPercent > 1.f) hpPercent = 1.f;

    this->hpBar.setSize(sf::Vector2f(80.f * hpPercent, 13.f));
    this->hpBar.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y - 10.f);

    // Angka Teks
    std::stringstream ss;
    ss << this->attributeComponent->hp << " / " << this->attributeComponent->hpMax;
    this->hpText.setString(ss.str());

    this->hpText.setOrigin(
        this->hpText.getGlobalBounds().width / 2.f,
        this->hpText.getGlobalBounds().height / 2.f);
    
    this->hpText.setPosition(
        this->hpBar.getPosition().x + 40.f,
        this->hpBar.getPosition().y + 5.f 
    );

    this->updateAnimation(dt);
    this->hitboxComponent->update();
    this->follow->update(dt);
}

void Spider::render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox)
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