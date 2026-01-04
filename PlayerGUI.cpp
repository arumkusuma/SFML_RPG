#include "stdafx.h"
#include "PlayerGUI.h"

void PlayerGUI::initFont()
{
    this->font.loadFromFile("Fonts/Dosis-Light.ttf");
}

void PlayerGUI::initLevelBar()
{
	this->levelBarText.setFont(this->font);
    this->levelBarText.setCharacterSize(gui::calcCharSize(this->vm, 100)); 
    this->levelBarText.setFillColor(sf::Color(255, 215, 0, 255)); 
    this->levelBarText.setOutlineColor(sf::Color::Black);
    this->levelBarText.setOutlineThickness(1.5f); 

    float x = gui::p2pX(1.f, this->vm);
    
    float y = gui::p2pY(2.0f, this->vm); 

    this->levelBarText.setPosition(x, y);
    this->levelBarText.setString("Lvl 1");
}

void PlayerGUI::initEXPBar()
{
    //Load Texture 
    if (!this->expBarTexture.loadFromFile("Resources/Images/Gui/expbar.png"))
    {
        std::cout << "ERROR::PLAYERGUI::COULD NOT LOAD EXP BAR TEXTURE" << "\n";
    }

    //Setup Sprite
    this->expBarInner.setTexture(this->expBarTexture);
    this->expBarBack = this->expBarInner; 

    //modifikasi 
    this->expBarBack.setColor(sf::Color(50, 50, 50, 150)); 

    float x = gui::p2pX(1.f, this->vm);
    float y = gui::p2pY(5.6f, this->vm);
    
    float scale = 1.3f; 

    this->expBarBack.setPosition(x, y);
    this->expBarBack.setScale(scale, scale);

    this->expBarInner.setPosition(x, y);
    this->expBarInner.setScale(scale, scale);

    // Teks Angka
    this->expBarText.setFont(this->font);
    this->expBarText.setCharacterSize(gui::calcCharSize(this->vm, 120));
    this->expBarText.setFillColor(sf::Color::White);
    this->expBarText.setOutlineColor(sf::Color::Black);
    this->expBarText.setOutlineThickness(1.0f);
    
    this->expBarText.setPosition(x + 20.f, y + 10.f); 
}

void PlayerGUI::initHPBar()
{
    // Texture
    if (!this->hpBarTexture.loadFromFile("Resources/Images/Gui/hpbar.png"))
    {
        std::cout << "ERROR::PLAYERGUI::COULD NOT LOAD HP BAR TEXTURE" << "\n";
    }

    this->hpBarInner.setTexture(this->hpBarTexture);
    this->hpBarBack = this->hpBarInner; 

    this->hpBarBack.setColor(sf::Color(50, 50, 50, 150)); 
	this->hpBarInner.setColor(sf::Color::White);

    float x = gui::p2pX(1.f, this->vm);
    float y = gui::p2pY(8.3f, this->vm);
    
    float scale = 1.3f; 

    this->hpBarBack.setPosition(x, y);
    this->hpBarBack.setScale(scale, scale);

    this->hpBarInner.setPosition(x, y);
    this->hpBarInner.setScale(scale, scale);

    //Teks Angka
    this->hpBarText.setFont(this->font);
    this->hpBarText.setCharacterSize(gui::calcCharSize(this->vm, 120));
    this->hpBarText.setFillColor(sf::Color::White);
    this->hpBarText.setOutlineColor(sf::Color::Black);
    this->hpBarText.setOutlineThickness(1.0f);
    this->hpBarText.setPosition(x + 20.f, y + 10.f);
}

void PlayerGUI::initPlayerTabs(sf::VideoMode &vm, sf::Font &font, Player &player)
{
    this->playerTabs = new PlayerGUITabs(vm, font, player);
}

PlayerGUI::PlayerGUI(Player* player, sf::VideoMode& vm)
    : vm(vm)
{
    this->player = player;

    this->initFont();
    this->initLevelBar();
    this->initEXPBar(); 
    this->initHPBar();  
    this->initPlayerTabs(vm, font, *player);
    this->initInventory();
}

PlayerGUI::~PlayerGUI()
{
    delete this->playerTabs;
}

const bool PlayerGUI::getTabsOpen() const
{
    return this->playerTabs->tabsOpen();
}

void PlayerGUI::toggleCharacterTab()
{
    this->playerTabs->toggleTab(PLAYER_TABS::CHARACTER_TAB);
}

//Functions
void PlayerGUI::updateLevelBar()
{
    std::string levelStr = "Lvl " + std::to_string(this->player->getAttributeComponent()->level);
    
    this->levelBarText.setString(levelStr);
}

void PlayerGUI::updateEXPBar()
{
   int exp = this->player->getAttributeComponent()->exp;
    int expNext = this->player->getAttributeComponent()->expNext;
    float percent = static_cast<float>(exp) / static_cast<float>(expNext);

    this->expBarInner.setTextureRect(sf::IntRect(
        0, 0, 
        static_cast<int>(this->expBarTexture.getSize().x * percent), 
        this->expBarTexture.getSize().y
    ));

    std::stringstream ss;
    ss << exp << " / " << expNext;
    this->expBarText.setString(ss.str());

    sf::FloatRect textRect = this->expBarText.getLocalBounds();
    
    this->expBarText.setOrigin(
        textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f
    );

    float xPos = std::floor(this->expBarBack.getPosition().x + (this->expBarBack.getGlobalBounds().width / 2.f));
    float yPos = std::floor(this->expBarBack.getPosition().y + (this->expBarBack.getGlobalBounds().height / 2.f));

    this->expBarText.setPosition(xPos, yPos);
}

void PlayerGUI::updateHPBar()
{
   	int hp = this->player->getAttributeComponent()->hp;
    int hpMax = this->player->getAttributeComponent()->hpMax;
    float percent = static_cast<float>(hp) / static_cast<float>(hpMax);

    this->hpBarInner.setTextureRect(sf::IntRect(
        0, 0, 
        static_cast<int>(this->hpBarTexture.getSize().x * percent), 
        this->hpBarTexture.getSize().y
    ));

    std::stringstream ss;
    ss << hp << " / " << hpMax;
    this->hpBarText.setString(ss.str());

    sf::FloatRect textRect = this->hpBarText.getLocalBounds();
    
    this->hpBarText.setOrigin(
        textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f
    );

    float xPos = std::floor(this->hpBarBack.getPosition().x + (this->hpBarBack.getGlobalBounds().width / 2.f));
    float yPos = std::floor(this->hpBarBack.getPosition().y + (this->hpBarBack.getGlobalBounds().height / 2.f));

    this->hpBarText.setPosition(xPos, yPos);
}

void PlayerGUI::updatePlayerTabs()
{
    this->playerTabs->update();
}

void PlayerGUI::update(const float & dt)
{
    this->updateLevelBar();
    this->updateEXPBar();
    this->updateHPBar();
    this->updatePlayerTabs();
}


void PlayerGUI::renderLevelBar(sf::RenderTarget & target)
{
    // target.draw(this->levelBarBack);
    target.draw(this->levelBarText);
}

void PlayerGUI::renderEXPBar(sf::RenderTarget & target)
{
    target.draw(this->expBarBack);  
    target.draw(this->expBarInner); 
    target.draw(this->expBarText); 
}

void PlayerGUI::renderHPBar(sf::RenderTarget & target)
{
    target.draw(this->hpBarBack);   
    target.draw(this->hpBarInner);  
    target.draw(this->hpBarText);   
}

void PlayerGUI::renderPlayerTabs(sf::RenderTarget& target)
{
    this->playerTabs->render(target);
}

void PlayerGUI::render(sf::RenderTarget & target)
{
  this->renderEXPBar(target);
    this->renderHPBar(target);

    this->renderLevelBar(target); 

    this->renderPlayerTabs(target);
}

void PlayerGUI::initInventory()
{
    if (!this->borderTexture.loadFromFile("Resources/Images/GUI/BorderTemplate.png")) 
    {
        std::cout << "ERROR::PLAYERGUI::GAGAL_LOAD_BORDER_TEMPLATE" << "\n";
    }

    this->borderSprite.setTexture(this->borderTexture);
    this->borderSprite.setScale(1.5f, 1.5f); 

    //HITUNG UKURAN BACKGROUND OTOMATIS
    int cols = 5; 
    int capacity = this->player->getInventory()->maxSize();
    int rows = (capacity / cols) + 1; 

    float boxW = this->borderSprite.getGlobalBounds().width;
    float boxH = this->borderSprite.getGlobalBounds().height;
    float spacing = 10.f; 

    // (UkuranKotak * Jumlah) + (Spasi * Jumlah) + PaddingTepi
    float bgWidth = (cols * (boxW + spacing)) + 40.f; 
    float bgHeight = (rows * (boxH + spacing)) + 40.f;

    this->inventoryBackground.setSize(sf::Vector2f(bgWidth, bgHeight));
    this->inventoryBackground.setFillColor(sf::Color(20, 20, 20, 180));
    
    // di tengah layar
    this->inventoryBackground.setPosition(
        this->vm.width / 2.f - bgWidth / 2.f, 
        this->vm.height / 2.f - bgHeight / 2.f
    );
}

void PlayerGUI::renderInventory(sf::RenderTarget & target)
{
    //BG inventoreei
    target.draw(this->inventoryBackground);

    Inventory* inv = this->player->getInventory();

    float startX = this->inventoryBackground.getPosition().x + 20.f;
    float startY = this->inventoryBackground.getPosition().y + 20.f;
    
    // Ukuran kotak
    float boxW = this->borderSprite.getGlobalBounds().width;
    float boxH = this->borderSprite.getGlobalBounds().height;
    float spacing = 10.f; // Jarak kotak
    
    int cols = 5; 

    for (unsigned i = 0; i < inv->maxSize(); i++)
    {
        // baris dan kolom
        int gridX = i % cols;          
        int gridY = i / cols;           

        float x = startX + (gridX * (boxW + spacing));
        float y = startY + (gridY * (boxH + spacing));

        //gambar slotkosong
        this->borderSprite.setPosition(x, y);
        target.draw(this->borderSprite);

        //Gambar Item
        Item* item = inv->getItem(i);
        if (item != nullptr)
        {
            sf::Vector2f oldPos = item->getPosition();
            
            //Scaling

            item->setScale(1.f, 1.f);
            
            sf::FloatRect bounds = item->getGlobalBounds();
            
            // 70% dari kotak
            float targetSizeX = boxW * 0.7f;
            float targetSizeY = boxH * 0.7f;

            float scaleX = targetSizeX / bounds.width;
            float scaleY = targetSizeY / bounds.height;

            //scale kecil
            float finalScale = (scaleX < scaleY) ? scaleX : scaleY;
            item->setScale(finalScale, finalScale);

            sf::FloatRect newBounds = item->getGlobalBounds();
            float itemX = x + (boxW / 2.f) - (newBounds.width / 2.f);
            float itemY = y + (boxH / 2.f) - (newBounds.height / 2.f);

            item->setPosition(itemX, itemY);
            
            // Render
            item->render(target);

            // Kembalikan ke ukuran asli
            item->setPosition(oldPos.x, oldPos.y);
            item->setScale(0.5f, 0.5f); 
        }
    }
}