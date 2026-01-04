#include "stdafx.h"
#include "GameState.h"
#include "Sword.h"

void GameState::initDeferredRender()
{
	this->renderTexture.create(
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height
	);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(
		sf::IntRect(
			0, 
			0, 
			this->stateData->gfxSettings->resolution.width, 
			this->stateData->gfxSettings->resolution.height
		)
	);
}

//fungsi inisialisasi
void GameState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width / 2),
			static_cast<float>(this->stateData->gfxSettings->resolution.height / 2)
		)
	);

	this->view.setCenter(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width) / 2.f,
			static_cast<float>(this->stateData->gfxSettings->resolution.height) / 2.f
		)
	);
}

void GameState::initKeybinds()
{
	std::ifstream ifs("Config/gamestate_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();
}

void GameState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

void GameState::initTextures()
{
	if (!this->textures["PLAYER_SHEET"].loadFromFile("Resources/Images/Sprites/Player/PLAYER_SHEET2.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_TEXTURE";
	}

	if(!this->textures["RAT1_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/rat1_60x64.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_RAT1_TEXTURE";
	}

	if (!this->textures["BIRD1_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/bird1_61x57.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_BIRD1_TEXTURE";
	}
	if (!this->textures["SPIDER_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/spider1_60x64.png")) 
    {
        throw "ERROR::GAME_STATE::GAGAL_LOAD_TEXTURE_SPIDER";
    }
    if (!this->textures["SCORPION_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/scorpion1_60x64.png")) 
    {
        throw "ERROR::GAME_STATE::GAGAL_LOAD_TEXTURE_SCORPION";
    }
}

void GameState::initPauseMenu()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	this->pmenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font);

	this->pmenu->addButton("QUIT", gui::p2pY(74.f, vm), gui::p2pX(13.f, vm), gui::p2pY(6.f, vm), gui::calcCharSize(vm), "Quit");
}

void GameState::initShaders()
{
	if (!this->core_shader.loadFromFile("vertex_shader.vert", "fragment_shader.frag"))
	{
		std::cout << "ERROR::GAMESTATE::COULD NOT LOAD SHADER." << "\n";
	}
}

void GameState::initKeyTime()
{
	this->keyTimeMax = 0.3f;
	this->keyTimer.restart();
}

void GameState::initDebugText()
{
	this->debugText.setFont(this->font);
	this->debugText.setFillColor(sf::Color::White);
	this->debugText.setCharacterSize(16);
	this->debugText.setPosition(15.f, this->window->getSize().y / 2.f);
}

void GameState::initPlayers()
{
	this->player = new Player(220, 220, this->textures["PLAYER_SHEET"]);
}

void GameState::initPlayerGUI()
{
	this->playerGUI = new PlayerGUI(this->player, this->stateData->gfxSettings->resolution);
}

void GameState::initEnemySystem()
{
	this->enemySystem = new EnemySystem(this->activeEnemies, this->textures, *this->player);
}

void GameState::initTileMap()
{
	this->tileMap = new TileMap("text.slmp"); 
}

void GameState::initSystems()
{
	this->tts = new TextTagSystem("Fonts/PixellettersFull.ttf");
}

//Constructors / Destructors
GameState::GameState(StateData* state_data)
	: State(state_data)
{
	this->initDeferredRender();
	this->initView();
	this->initKeybinds();
	this->initFonts();
	this->initTextures();
	this->initPauseMenu();
	this->initShaders();
	this->initKeyTime();
	this->initDebugText();

	this->initPlayers();
	this->initPlayerGUI();
	this->initEnemySystem();
	this->initTileMap();
	this->initSystems();

	this->showInventory = false;
}

GameState::~GameState()
{
	delete this->pmenu;
	delete this->player;
	delete this->playerGUI;
	delete this->enemySystem;
	delete this->tileMap;
	delete this->tts;

	for (size_t i = 0; i < this->activeEnemies.size(); i++)
	{
		delete this->activeEnemies[i];
	}
}

const bool GameState::getKeyTime()
{	
	if (this->keyTimer.getElapsedTime().asSeconds() >= this->keyTimeMax)
	{
		this->keyTimer.restart();
		return true;
	}

	return false;	
}

//Functions
void GameState::updateView(const float & dt)
{
	this->view.setCenter(
		std::floor(this->player->getPosition().x + (static_cast<float>(this->mousePosWindow.x) - static_cast<float>(this->stateData->gfxSettings->resolution.width / 2)) / 10.f),
		std::floor(this->player->getPosition().y + (static_cast<float>(this->mousePosWindow.y) - static_cast<float>(this->stateData->gfxSettings->resolution.height / 2)) / 10.f)
	);
	
	if (this->tileMap->getMaxSizeF().x >= this->view.getSize().x)
	{
		if (this->view.getCenter().x - this->view.getSize().x / 2.f < 0.f)
		{
			this->view.setCenter(0.f + this->view.getSize().x / 2.f, this->view.getCenter().y);
		}
		else if (this->view.getCenter().x + this->view.getSize().x / 2.f > this->tileMap->getMaxSizeF().x)
		{
			this->view.setCenter(this->tileMap->getMaxSizeF().x - this->view.getSize().x / 2.f, this->view.getCenter().y);
		}
	}

	if (this->tileMap->getMaxSizeF().y >= this->view.getSize().y)
	{
		if (this->view.getCenter().y - this->view.getSize().y / 2.f < 0.f)
		{
			this->view.setCenter(this->view.getCenter().x, 0.f + this->view.getSize().y / 2.f);
		}
		else if (this->view.getCenter().y + this->view.getSize().y / 2.f > this->tileMap->getMaxSizeF().y)
		{
			this->view.setCenter(this->view.getCenter().x, this->tileMap->getMaxSizeF().y - this->view.getSize().y / 2.f);
		}
	}

	this->viewGridPosition.x = static_cast<int>(this->view.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	this->viewGridPosition.y = static_cast<int>(this->view.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void GameState::updateInput(const float & dt)
{
    // Pause (esc)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime())
    {
        if (!this->paused)
            this->pauseState();
        else
            this->unpauseState();
    }

    // 2.inventory(i)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && this->getKeyTime())
    {
        this->showInventory = !this->showInventory;
    }

    //loot(e)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && this->getKeyTime())
    {
        for (int i = 0; i < this->activeItems.size(); i++)
        {
            if (this->player->getGlobalBounds().intersects(this->activeItems[i]->getGlobalBounds()))
            {
                Item* itemDiambil = this->activeItems[i];
                this->player->addItem(itemDiambil);

                //hapus
                this->activeItems.erase(this->activeItems.begin() + i);
                break;
            }
        }
    }
}

void GameState::updatePlayerInput(const float & dt)
{
	//Update player input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))
		this->player->move(-1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))
		this->player->move(1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))))
	{
		this->player->move(0.f, -1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))
	{
		this->player->move(0.f, 1.f, dt);
	}
}

void GameState::updatePlayerGUI(const float & dt)
{
	this->playerGUI->update(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("TOGGLE_PLAYER_TAB_CHARACTER"))) && this->getKeyTime())
	{
		this->playerGUI->toggleCharacterTab();
	}
}

void GameState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("QUIT"))
		this->endState();
}

void GameState::updateTileMap(const float & dt)
{
	this->tileMap->updateWorldBoundsCollision(this->player, dt); 
	this->tileMap->updateTileCollision(this->player, dt);
	this->tileMap->updateTiles(this->player, dt, *this->enemySystem);
}

void GameState::updatePlayer(const float & dt)
{
	this->player->update(dt, this->mousePosView, this->view);
}

void GameState::updateCombatAndEnemies(const float & dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->player->getWeapon()->getAttackTimer())
		this->player->setInitAttack(true);

	unsigned index = 0;
    for (auto *enemy : this->activeEnemies)
    {
        enemy->update(dt, this->mousePosView, this->view);
        this->tileMap->updateWorldBoundsCollision(enemy, dt);
        this->tileMap->updateTileCollision(enemy, dt);
        this->updateCombat(enemy, index, dt);

        // LOGIKA KEMATIAN
      if (enemy->isDead())
        {
            //nambah exp
            int expGain = enemy->getGainExp();
            this->player->gainEXP(expGain);
            this->tts->addTextTag(EXPERIENCE_TAG, 
                this->player->getPosition().x - 40.f, 
                this->player->getPosition().y - 30.f, 
                expGain, "+", "EXP");

            //drop item
            int dropChance = rand() % 100;
            int enemyType = enemy->getEnemyType(); 

            if (true) 
            {
                Item* drop = nullptr;
                std::string iconPath = "";

                switch (enemyType)
                {
                case 0: iconPath = "Resources/Images/Icons/Inventory/Food/Meat_Raw.png"; break;
                case 1: iconPath = "Resources/Images/Icons/Inventory/Ore/Gem_Blue.png"; break;
                case 2: iconPath = "Resources/Images/Icons/Inventory/Food/Egg.png"; break;
                case 3: iconPath = "Resources/Images/Icons/Inventory/Ore/Gem_Red.png"; break;
                default: iconPath = "Resources/Images/Icons/Inventory/Food/Meat_Raw.png"; break;
                }

                drop = new Sword(1, 10, 5, 1, 1, iconPath);
                
                // Set posisi drop
                drop->setPosition(enemy->getPosition().x, enemy->getPosition().y);
                
                drop->setScale(0.5f, 0.5f); 

                this->activeItems.push_back(drop);
            }

            //heal
            int healAmount = static_cast<int>(this->player->getAttributeComponent()->hpMax * 0.1f); // 10% Heal
            if (healAmount < 1) healAmount = 1;

            this->player->gainHP(healAmount);
            
            //tag Heal
            this->tts->addTextTag(HEAL_TAG, 
                this->player->getPosition().x + 30.f, 
                this->player->getPosition().y - 30.f, 
                healAmount, "+", "HP");

            //hapus
            this->enemySystem->removeEnemy(index);
            continue;
        }
        ++index;
    }

	this->player->setInitAttack(false);
}

void GameState::updateCombat(Enemy* enemy, const int index, const float & dt)
{
	if (this->player->getInitAttack())
    {
        // jarak pemain ke musuh
      if (this->player->getDistance(*enemy) < this->player->getWeapon()->getRange() + 20.f 
            && enemy->getDamageTimerDone())
        {
            int dmg = static_cast<int>(this->player->getDamage());
            enemy->loseHP(dmg);
            enemy->resetDamageTimer();
            this->tts->addTextTag(DEFAULT_TAG, enemy->getPosition().x, enemy->getPosition().y, dmg, "", "");    
        }
    }

	//Cek damage musuh
	if (enemy->getGlobalBounds().intersects(this->player->getGlobalBounds()) && this->player->getDamageTimer())
	{
		int dmg = enemy->getAttributeComp()->damageMax;
		this->player->loseHP(dmg);
		this->tts->addTextTag(NEGATIVE_TAG, player->getPosition().x - 30.f, player->getPosition().y, dmg, "-", "HP");
	}
}

void GameState::updateDebugText(const float& dt)
{
	std::stringstream ss;

	ss << "Mouse Pos View: " << this->mousePosView.x << " " << this->mousePosView.y << "\n"
	<< "Active Enemies: " << this->activeEnemies.size() << "\n";

	this->debugText.setString(ss.str());
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	this->renderTexture.clear();
	this->renderTexture.setView(this->view);

	this->tileMap->render(
		this->renderTexture, 
		this->viewGridPosition, 
		&this->core_shader,
		this->player->getCenter(),
		false
	);

	for (auto *enemy : this->activeEnemies)
	{
		enemy->render(this->renderTexture, &this->core_shader, this->player->getCenter(), true);
	}

	for (auto *item : this->activeItems)
	{
    	item->render(this->renderTexture);
	}

	this->player->render(this->renderTexture, &this->core_shader, this->player->getCenter(), true);

	this->tileMap->renderDeferred(this->renderTexture, &this->core_shader, this->player->getCenter());

	this->tts->render(this->renderTexture);

	//Render GUI
	this->renderTexture.setView(this->renderTexture.getDefaultView());
	this->playerGUI->render(this->renderTexture);

	if (this->showInventory)
    {
        this->playerGUI->renderInventory(this->renderTexture);
    }

	if (this->paused) 
	{
	
	}
	this->renderTexture.draw(this->debugText);

	//FINAL RENDER
	this->renderTexture.display();
	target->draw(this->renderSprite);
}

void GameState::update(const float& dt)
{
    this->updateMousePositions(&this->view);
    this->updateKeytime(dt);
    this->updateInput(dt);

    this->updateDebugText(dt);
    
    if (!this->paused) //Unpaused update
    {
        this->updateView(dt);
        this->updatePlayerInput(dt);
        this->updateTileMap(dt);
        this->updatePlayer(dt);
        this->updatePlayerGUI(dt);
        this->updateCombatAndEnemies(dt); 
        this->tts->update(dt);

        if (this->player->getAttributeComponent()->isDead())
        {
            std::cout << "GAME OVER! PLAYER MATI." << "\n";
            this->endState(); 
        }
    }
    else 
    {
        this->pmenu->update(this->mousePosWindow);
        this->updatePauseMenuButtons();
    }
}