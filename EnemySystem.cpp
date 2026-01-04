#include "stdafx.h"
#include "EnemySystem.h"
#include "Spider.h"
#include "Scorpion.h"

EnemySystem::EnemySystem(std::vector<Enemy*>& activeEnemies,
	std::map<std::string, sf::Texture>& textures, Entity& player)
	: textures(textures), activeEnemies(activeEnemies), player(player)
{

}

EnemySystem::~EnemySystem()
{

}

void EnemySystem::createEnemy(const short type, const float xPos, const float yPos, EnemySpawnerTile& enemy_spawner_tile)
{
    switch (type)
    {
    case EnemyTypes::RAT: // Type 0
        this->activeEnemies.push_back(new Rat(xPos, yPos, this->textures["RAT1_SHEET"], enemy_spawner_tile, this->player));
        enemy_spawner_tile.increaseEnemyCounter();
        break;

    case EnemyTypes::BIRD1: // Type 1
        this->activeEnemies.push_back(new Bird1(xPos, yPos, this->textures["BIRD1_SHEET"], enemy_spawner_tile, this->player));
        enemy_spawner_tile.increaseEnemyCounter();
        break;

    case EnemyTypes::SPIDER: // Type 2 (Pastikan Enum SPIDER ada di EnemySpawnerTile.h / Enemy.h)
        // Pastikan texture SPIDER_SHEET sudah di-load di GameState!
        this->activeEnemies.push_back(new Spider(xPos, yPos, this->textures["SPIDER_SHEET"], enemy_spawner_tile, this->player));
        enemy_spawner_tile.increaseEnemyCounter();
        break;

    case EnemyTypes::SCORPION: // Type 3
        // Pastikan texture SCORPION_SHEET sudah di-load di GameState!
        this->activeEnemies.push_back(new Scorpion(xPos, yPos, this->textures["SCORPION_SHEET"], enemy_spawner_tile, this->player));
        enemy_spawner_tile.increaseEnemyCounter();
        break;

    default:
        std::cout << "ERROR::ENEMYSYSTEM::TYPE TIDAK DIKENAL (" << type << ")" << "\n";
        break;
    }
}

void EnemySystem::removeEnemy(const int index)
{
	this->activeEnemies[index]->getEnemySpawnerTile().decreaseEnemyCounter();
	delete this->activeEnemies[index];
	this->activeEnemies.erase(this->activeEnemies.begin() + index);
}

void EnemySystem::update(const float & dt)
{

}

void EnemySystem::render(sf::RenderTarget * target)
{

}
