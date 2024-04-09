/* #include <Geode/Geode.hpp>
#include <Geode/modify/CharacterColorPage.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../mod/AsakiYuki.hpp"
using namespace geode::prelude;

SimplePlayer* cube = nullptr;
SimplePlayer* ship = nullptr;
SimplePlayer* ball = nullptr;
SimplePlayer* ufo = nullptr;
SimplePlayer* wave = nullptr;
SimplePlayer* robot = nullptr;
SimplePlayer* spider = nullptr;
SimplePlayer* swing = nullptr;

class IconsPagePopup : public Popup<> {
    public:
    static inline int openIconID = 0; 
    static IconsPagePopup* create() {
        auto ret = new IconsPagePopup();
        if (ret && ret->initAnchored(400.f, 200.f, "GJ_square05.png")) {
            ret->autorelease();
            return ret;
        }
        return nullptr;
    }
    void open(CCObject* sender) {
        IconsPagePopup::openIconID = sender->getTag();
        IconsPagePopup::create()->show();
    }
    void changeIcon(CCObject* sender) {
        SimplePlayer* p[] = {cube, ship, ball, ufo, wave, robot, spider, swing};
        p[IconsPagePopup::openIconID - 1]->updatePlayerFrame(sender->getTag(), static_cast<IconType>(IconsPagePopup::openIconID - 1));
        this->player1->updatePlayerFrame(sender->getTag(), static_cast<IconType>(IconsPagePopup::openIconID - 1));
        this->player2->updatePlayerFrame(sender->getTag(), static_cast<IconType>(IconsPagePopup::openIconID - 1));
        AsakiYuki::Save::savePlayerFrame(sender->getTag(), static_cast<AsakiYuki::ModTypes::Gamemode>(IconsPagePopup::openIconID - 1));
        AsakiYuki::Player::updateIcon(sender->getTag(), static_cast<AsakiYuki::ModTypes::Gamemode>(IconsPagePopup::openIconID - 1), AsakiYuki::ModTypes::PlayerType::BOTH_PLAYERS);
    }
    protected:
    ScrollLayer* scroll;
    SimplePlayer* player1;
    SimplePlayer* player2;
    bool setup() {
        int iconCount[] = {484, 169, 118, 149, 96, 68, 69, 43, 5};
        auto iconTyp = static_cast<AsakiYuki::ModTypes::Gamemode>(IconsPagePopup::openIconID - 1);
        auto gameManager = GameManager::sharedState();
        auto pnl = this->m_mainLayer->getContentSize();
        player1 = AsakiYuki::Node::SimPlayer::create(
            iconTyp, AsakiYuki::Save::getPlayerFrame(iconTyp),
            {pnl.width / 2 - 35.f, pnl.height - 32.f}, {0.5f, 0.5f}, 1.f
        );
        player2 = AsakiYuki::Node::SimPlayer::create(
            iconTyp, AsakiYuki::Save::getPlayerFrame(iconTyp),
            {pnl.width / 2 + 35.f, pnl.height - 32.f}, {0.5f, 0.5f}, 1.f,
            AsakiYuki::Color::playerColor2(),
            AsakiYuki::Color::playerColor(),
            AsakiYuki::Color::playerGlowColor()
        );

        this->m_mainLayer->addChild(player1, 1);
        this->m_mainLayer->addChild(player2, 1);
 
        scroll = ScrollLayer::create({360.f, 120.f});
        scroll->setPosition(15.f, 15.f);
        scroll->setAnchorPoint({0.f, 0.f});
        scroll->ignoreAnchorPointForPosition(false);
        auto scrollBar = Scrollbar::create(this->scroll);
        scrollBar->setPosition({scroll->getContentSize().width + 25.f, scroll->getContentSize().height / 2 + 15.f});
        this->m_mainLayer->addChild(scrollBar, 1);
        this->m_mainLayer->addChild(scroll, 1);
        auto ss = scroll->getContentSize();
        auto menu = AsakiYuki::Node::Menu::create({ss.width / 2, 0.f}, ss, scroll->m_contentLayer);
        menu->setAnchorPoint({0.5f, 0.f});
        auto scrollBG = extension::CCScale9Sprite::create("square02_small.png");
        scrollBG->setContentSize({ss.width, ss.height});
        scrollBG->setPosition({this->m_mainLayer->getContentSize().width / 2 - 5.f, ss.height / 2 + 15.f });
        scrollBG->setOpacity(128);
        scroll->m_contentLayer->setTouchEnabled(true);
        this->m_mainLayer->addChild(scrollBG, 0);
        auto iconType = static_cast<IconType>(IconsPagePopup::openIconID - 1);
        for (int index = 1; index <= iconCount[IconsPagePopup::openIconID - 1]; index++) {
            if (!gameManager->isIconUnlocked(index, iconType)) continue;
            auto playerPreview = AsakiYuki::Node::SimPlayer::create(
                static_cast<AsakiYuki::ModTypes::Gamemode>(IconsPagePopup::openIconID - 1),
                index, {15.f, 15.f}, {0.5f, 0.5f},
                (IconsPagePopup::openIconID == 2 || IconsPagePopup::openIconID == 8) ? 0.65f : 0.75f
            );
            auto btn = AsakiYuki::Node::MenuItemSpriteExtra::create(playerPreview, this, menu_selector(IconsPagePopup::changeIcon), menu);
            playerPreview->setPosition({15.f, 15.f});
            btn->setContentSize({30.f, 30.f});
            btn->setTag(index);
        }
        menu->setLayout(RowLayout::create()->setAutoScale(true)->setGrowCrossAxis(true)->setCrossAxisOverflow(true)->setGap(0.f));
        const bool isLonger = ss.height <= menu->getContentHeight();
        scroll->m_contentLayer->setContentHeight(isLonger ? menu->getContentHeight() : ss.height);
        if (!isLonger) menu->setPosition({ss.width / 2, ss.height - menu->getContentHeight()});
        scroll->moveToTop();
        handleTouchPriority(this, true);
        return true;
    };
};

class $modify(CharacterColorPage) {
    public:
    void updateIconColors() {
        CharacterColorPage::updateIconColors();
        auto gameManager = GameManager::sharedState();
        auto gameLayer = gameManager->getGameLayer();
        if (gameLayer != nullptr) {
            using namespace AsakiYuki;
            Player::updateColor(
                Color::playerColor(),
                Color::playerColor2(),
                gameManager->getPlayerGlow(),
                Color::playerGlowColor(),
                ModTypes::PlayerType::BOTH_PLAYERS
            );

            if (cube == nullptr) return;
            cube->setColor(Color::playerColor());
            cube->setSecondColor(Color::playerColor2());
            ship->setColor(Color::playerColor());
            ship->setSecondColor(Color::playerColor2());
            ball->setColor(Color::playerColor());
            ball->setSecondColor(Color::playerColor2());
            ufo->setColor(Color::playerColor());
            ufo->setSecondColor(Color::playerColor2());
            wave->setColor(Color::playerColor());
            wave->setSecondColor(Color::playerColor2());
            robot->setColor(Color::playerColor());
            robot->setSecondColor(Color::playerColor2());
            spider->setColor(Color::playerColor());
            spider->setSecondColor(Color::playerColor2());
            swing->setColor(Color::playerColor());
            swing->setSecondColor(Color::playerColor2());
            if (gameManager->getPlayerGlow()) {
                cube->setGlowOutline(Color::playerGlowColor());
                ship->setGlowOutline(Color::playerGlowColor());
                ball->setGlowOutline(Color::playerGlowColor());
                ufo->setGlowOutline(Color::playerGlowColor());
                wave->setGlowOutline(Color::playerGlowColor());
                robot->setGlowOutline(Color::playerGlowColor());
                spider->setGlowOutline(Color::playerGlowColor());
                swing->setGlowOutline(Color::playerGlowColor());
            }
            else {
                cube->disableGlowOutline();
                ship->disableGlowOutline();
                ball->disableGlowOutline();
                ufo->disableGlowOutline();
                wave->disableGlowOutline();
                robot->disableGlowOutline();
                spider->disableGlowOutline();
                swing->disableGlowOutline();
            }
        }
    }
    bool init() {
        auto gameManager = GameManager::sharedState();
        auto gameLayer = gameManager->getGameLayer();
        if (gameLayer != nullptr) {
            CC_SAFE_RELEASE_NULL(cube);
            CC_SAFE_RELEASE_NULL(ship);
            CC_SAFE_RELEASE_NULL(ball);
            CC_SAFE_RELEASE_NULL(ufo);
            CC_SAFE_RELEASE_NULL(wave);
            CC_SAFE_RELEASE_NULL(robot);
            CC_SAFE_RELEASE_NULL(spider);
            CC_SAFE_RELEASE_NULL(swing);
        }
        if (!CharacterColorPage::init()) return false;
        if (gameLayer != nullptr) {
            using namespace AsakiYuki;
            auto cs = this->m_mainLayer->getContentSize();
            auto menu = Node::Menu::create(
                {cs.width / 2, cs.height - 72.5f},
                {420.f, 45.f},
                this->m_mainLayer
            );
            {
                this->m_mainLayer->getChildByID("player")->setVisible(false);
                this->m_buttonMenu->getChildByID("ship-jetpack")->setVisible(false);
                this->m_mainLayer->getChildByID("ball")->setVisible(false);
                this->m_mainLayer->getChildByID("ufo")->setVisible(false);
                this->m_mainLayer->getChildByID("wave")->setVisible(false);
                this->m_mainLayer->getChildByID("robot")->setVisible(false);
                this->m_mainLayer->getChildByID("spider")->setVisible(false);
                this->m_mainLayer->getChildByID("swing")->setVisible(false);
            };
            {
                cube = Node::SimPlayer::create(1.15f, ModTypes::CUBE);
                ship = Node::SimPlayer::create(1.15f, gameLayer->m_levelSettings->m_platformerMode ? ModTypes::JETPACK : ModTypes::SHIP);
                ball = Node::SimPlayer::create(1.15f, ModTypes::BALL);
                ufo = Node::SimPlayer::create(1.15f, ModTypes::UFO);
                wave = Node::SimPlayer::create(1.15f, ModTypes::WAVE);
                robot = Node::SimPlayer::create(1.15f, ModTypes::ROBOT);
                spider = Node::SimPlayer::create(1.15f, ModTypes::SPIDER);
                swing = Node::SimPlayer::create(1.15f, ModTypes::SWING);

                Node::MenuItemSpriteExtra::create(cube, this, menu_selector(CharacterColorPage::toggleShip), menu, {0.f, 0.f}, {0.5f, 0.5f}, {30.f, 30.f}, 1);
                Node::MenuItemSpriteExtra::create(ship, this, menu_selector(CharacterColorPage::toggleShip), menu, {0.f, 0.f}, {0.5f, 0.5f}, {30.f, 30.f}, gameLayer->m_levelSettings->m_platformerMode ? 9 : 2);
                Node::MenuItemSpriteExtra::create(ball, this, menu_selector(CharacterColorPage::toggleShip), menu, {0.f, 0.f}, {0.5f, 0.5f}, {30.f, 30.f}, 3);
                Node::MenuItemSpriteExtra::create(ufo, this, menu_selector(CharacterColorPage::toggleShip), menu, {0.f, 0.f}, {0.5f, 0.5f}, {30.f, 30.f}, 4);
                Node::MenuItemSpriteExtra::create(wave, this, menu_selector(CharacterColorPage::toggleShip), menu, {0.f, 0.f}, {0.5f, 0.5f}, {30.f, 30.f}, 5);
                Node::MenuItemSpriteExtra::create(robot, this, menu_selector(CharacterColorPage::toggleShip), menu, {0.f, 0.f}, {0.5f, 0.5f}, {30.f, 30.f}, 6);
                Node::MenuItemSpriteExtra::create(spider, this, menu_selector(CharacterColorPage::toggleShip), menu, {0.f, 0.f}, {0.5f, 0.5f}, {30.f, 30.f}, 7);
                Node::MenuItemSpriteExtra::create(swing, this, menu_selector(CharacterColorPage::toggleShip), menu, {0.f, 0.f}, {0.5f, 0.5f}, {30.f, 30.f}, 8);

                cube->setPosition({15.f, 15.f});
                ship->setPosition({15.f, 15.f});
                ball->setPosition({15.f, 15.f});
                ufo->setPosition({15.f, 15.f});
                wave->setPosition({15.f, 15.f});
                robot->setPosition({15.f, 15.f});
                spider->setPosition({15.f, 15.f});
                swing->setPosition({15.f, 15.f});

                menu->setLayout(RowLayout::create()->setAutoScale(true)->setGrowCrossAxis(true)->setGap(20.f));
            };
        }
        return true;
    }
    void toggleShip(CCObject* p0) {
        auto gameManager = GameManager::sharedState();
        auto gameLayer = gameManager->getGameLayer();
        if (gameLayer != nullptr) {
            IconsPagePopup::openIconID = p0->getTag();
            IconsPagePopup::create()->show();
        } else CharacterColorPage::toggleShip(p0);
    }
};

class $modify(IngameLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        auto gameManager = GameManager::sharedState();
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto menu = AsakiYuki::Node::Menu::create(
            {0.f, 0.f},
            winSize,
            this
        );

        AsakiYuki::Node::MenuItemSpriteExtra::create(
            AsakiYuki::Node::Sprite::createWithSpriteFrameName("GJ_garageBtn_001.png", 0.5f),
            this,
            menu_selector(IngameLayer::openCharacterColorPage),
            menu,
            {40.f, winSize.height - 40.f}
        );
    }
    void openCharacterColorPage(CCObject*) {
        reinterpret_cast<FLAlertLayer*>(CharacterColorPage::create())->show();
    }
}; */