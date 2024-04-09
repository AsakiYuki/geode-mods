#include <Geode/Geode.hpp>
#include <Geode/modify/CharacterColorPage.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../mod/AsakiYuki.hpp"
using namespace geode::prelude;
using namespace AsakiYuki;

bool isPlayer2ExitDual = false;
bool onSelectSecondPlayer = false;
bool isPlayer2 = false;
bool lastBool = false;

void updateAllDualFrame() {
    auto currentLayer = Player::getCurrentIngameLayer();
    if (currentLayer != nullptr) {
        auto player1 = currentLayer->m_player1;
        auto player2 = currentLayer->m_player2;
        player1->updatePlayerFrame(1);
        player2->updatePlayerFrame(1);
        Player::loadFrameByCurrentGamemode(player1, isPlayer2ExitDual);
        Player::loadFrameByCurrentGamemode(player2, isPlayer2ExitDual);
        isPlayer2 = isPlayer2ExitDual;
        player1->createRobot(1);
        player1->createSpider(1);
        isPlayer2 = !isPlayer2ExitDual;
        player2->createRobot(1);
        player2->createSpider(1);
        isPlayer2 = false;

        if (isPlayer2ExitDual != lastBool) {
            auto p1Streak = player1->m_regularTrail;
            auto p2Streak = player2->m_regularTrail;
            player1->m_regularTrail = p2Streak;
            player2->m_regularTrail = p1Streak;
        }
        lastBool = isPlayer2ExitDual;
    }
}

SimplePlayer* previewPlayer1 = nullptr;
SimplePlayer* previewPlayer2 = nullptr;

class $modify(SeparateDuelLayer, GJGarageLayer) {
    protected:
    int currentPage = 0;
    ModTypes::Gamemode m_gamemode = ModTypes::Gamemode::CUBE;
    IconType iconType = IconType::Cube;
    CCSprite* cursor1 = nullptr;
    CCSprite* cursor1_shipStreak = nullptr;
    CCSprite* cursor2 = Node::Sprite::createWithSpriteFrameName("GJ_select_001.png", 0.85f, {0.f, 0.f}, {0.5f, 0.5f});
    CCSprite* cursor2_shipStreak = Node::Sprite::createWithSpriteFrameName("GJ_select_001.png", 0.85f, {0.f, 0.f}, {0.5f, 0.5f});
    bool isPlayer2 = false;
    CCLabelBMFont* cursor1Label = Node::LabelBMFont::create(0.3f, "P1", {0, 200, 0});
    CCLabelBMFont* cursor2Label = Node::LabelBMFont::create(0.3f, "P2", {0, 125, 255});
    CCSprite* arrow_1;
    CCSprite* arrow_2;
    CCMenuItemToggler* defaultMiniIconToggle;
    public:
    void onDefaultMiniIconToggle(CCObject* sender) {
        Mod::get()->setSavedValue<bool>(CCString::createWithFormat("asakiyuki:default-mini-icon_%i", static_cast<int>(this->m_fields->isPlayer2))->m_sString, !static_cast<CCMenuItemToggler*>(sender)->isToggled());
    }
    void keyBackClicked() {
        previewPlayer1 = nullptr;
        previewPlayer2 = nullptr;
        GJGarageLayer::keyBackClicked();
    }
    void onShop(cocos2d::CCObject* sender) {
        previewPlayer1 = nullptr;
        previewPlayer2 = nullptr;
        GJGarageLayer::onShop(sender);
    };
    void onBack(cocos2d::CCObject* sender) {
        previewPlayer1 = nullptr;
        previewPlayer2 = nullptr;
        GJGarageLayer::onBack(sender);
    }
    void swapIcon(CCObject*) {
        if (int(this->m_fields->m_gamemode) > 8) return;
        int icon1 = Save::getPlayerFrame(this->m_fields->m_gamemode);
        int icon2 = Save::getPlayerFrame(this->m_fields->m_gamemode, true);
        Save::savePlayerFrame(icon2, this->m_fields->m_gamemode);
        Save::savePlayerFrame(icon1, this->m_fields->m_gamemode, true);
        previewPlayer1->updatePlayerFrame(icon2, this->m_fields->iconType);
        previewPlayer2->updatePlayerFrame(icon1, this->m_fields->iconType);

        bool on1Visible = this->m_fields->cursor1->isVisible();
        bool on2Visible = this->m_fields->cursor2->isVisible();
        this->m_fields->cursor1->setVisible(on2Visible);
        this->m_fields->cursor2->setVisible(on1Visible);

        auto on1Pos = this->m_fields->cursor1->getPosition();
        auto on2Pos = this->m_fields->cursor2->getPosition();
        this->m_fields->cursor1->setPosition(on2Pos);
        this->m_fields->cursor2->setPosition(on1Pos);
    }

    void setupPage(int p0, IconType p1) {
        GJGarageLayer::setupPage(p0, p1);
        auto winsize = CCDirector::get()->getWinSize();
        p0 = (p0 == -1) ? (Save::getPlayerFrame(this->m_fields->m_gamemode) - 1) / 36 : p0;
        this->m_fields->currentPage = p0;
        int currentP2 = Save::getPlayerFrame(this->m_fields->m_gamemode, true) - 1;
        this->m_fields->cursor2->setVisible(p0 == int(currentP2 / 36));
        this->m_fields->cursor2->setPosition({
            (winsize.width / 2 - 165) + (30 * (currentP2 % 12)),
            (winsize.height / 2 - 35) - (30 * (int(currentP2 / 12) % 3))
        });
        if (p1 == IconType::Special) {
            this->m_fields->cursor2_shipStreak->setVisible(true);
            this->m_fields->cursor2_shipStreak->setPosition({
                (winsize.width / 2 - 165) + 30 * (Save::getPlayerFrame(ModTypes::Gamemode::SHIP_STREAK, true) - 1),
                winsize.height / 2 - 65
            });
        } else this->m_fields->cursor2_shipStreak->setVisible(false);
        if (this->m_fields->cursor1 != nullptr) {
            this->m_fields->cursor1->setZOrder(102);
            this->m_fields->cursor1_shipStreak->setZOrder(102);
            this->m_fields->cursor2->setZOrder(101);
            this->m_fields->cursor2_shipStreak->setZOrder(101);
        }
    }
    void playerSelected(CCObject* sender) {
        this->m_fields->isPlayer2 = sender->getTag() == 1;
        onSelectSecondPlayer = this->m_fields->isPlayer2;
        this->m_fields->arrow_1->setVisible(!this->m_fields->isPlayer2);
        this->m_fields->arrow_2->setVisible(this->m_fields->isPlayer2);
        this->m_fields->defaultMiniIconToggle->toggle(Mod::get()->getSavedValue<bool>(CCString::createWithFormat("asakiyuki:default-mini-icon_%i", static_cast<int>(this->m_fields->isPlayer2))->m_sString, false));
    }
    void onSelect(cocos2d::CCObject* sender) {
        if (!this->m_fields->isPlayer2) {
            GJGarageLayer::onSelect(sender);
            if (!GameManager::sharedState()->isIconUnlocked(sender->getTag(), this->m_fields->iconType)) return;
            if (static_cast<int>(this->m_fields->m_gamemode) <= 8)
                previewPlayer1->updatePlayerFrame(sender->getTag(), static_cast<IconType>(this->m_fields->m_gamemode));
        } else {
            auto winsize = CCDirector::get()->getWinSize();
            if (!GameManager::sharedState()->isIconUnlocked(sender->getTag(), this->m_fields->iconType)) return GJGarageLayer::onSelect(sender);
            this->m_fields->cursor2->setVisible(true);
            if (static_cast<int>(this->m_fields->m_gamemode) <= 8) {
                this->m_fields->cursor2->setPosition({
                    (winsize.width / 2 - 165) + 30 * ((sender->getTag() - 1) % 12),
                    (winsize.height / 2 - 35) - 30 * (int((sender->getTag() - 1) / 12) % 3)
                });
                previewPlayer2->updatePlayerFrame(sender->getTag(), static_cast<IconType>(this->m_fields->m_gamemode));
            }

            if (this->m_fields->m_gamemode == ModTypes::Gamemode::STREAK) {
                bool isShipFire = static_cast<CCMenuItemSpriteExtra*>(sender)->getPositionY() < -35.f;
                Save::savePlayerFrame(sender->getTag(), isShipFire ? ModTypes::Gamemode::SHIP_STREAK : ModTypes::Gamemode::STREAK, true);
                if (isShipFire) this->m_fields->cursor2->setPosition({
                        (winsize.width / 2 - 165) + 30 * (sender->getTag() - 1),
                        winsize.height / 2 - 65
                    });
                else this->m_fields->cursor2->setPosition({
                        (winsize.width / 2 - 165) + 30 * (sender->getTag() - 1),
                        winsize.height / 2 - 35
                    });
            } else {
                this->m_fields->cursor2->setPosition({
                    (winsize.width / 2 - 165) + 30 * ((sender->getTag() - 1) % 12),
                    (winsize.height / 2 - 35) - 30 * (int((sender->getTag() - 1) / 12) % 3)
                });
                Save::savePlayerFrame(sender->getTag(), static_cast<ModTypes::Gamemode>(this->m_fields->m_gamemode), true);
            }
        }
    }
    void selectTab(IconType p0) {
        this->m_fields->iconType = p0;
        if (static_cast<int>(p0) <= 8) {
            this->m_fields->m_gamemode = static_cast<ModTypes::Gamemode>(p0);
            previewPlayer1->updatePlayerFrame(Save::getPlayerFrame(static_cast<ModTypes::Gamemode>(p0)), p0);
            previewPlayer2->updatePlayerFrame(Save::getPlayerFrame(static_cast<ModTypes::Gamemode>(p0), true), p0);

            previewPlayer1->setColor(Color::playerColor());
            previewPlayer1->setSecondColor(Color::playerColor2());
            previewPlayer1->setGlowOutline(Color::playerGlowColor());
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW)) previewPlayer1->disableGlowOutline();

            previewPlayer2->setColor(Color::playerColor(true));
            previewPlayer2->setSecondColor(Color::playerColor2(true));
            previewPlayer2->setGlowOutline(Color::playerGlowColor(true));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, true)) previewPlayer2->disableGlowOutline();
        } else {
            switch (static_cast<int>(p0)) {
                case 99:
                    this->m_fields->m_gamemode = ModTypes::Gamemode::STREAK; break;
                case 98:
                    this->m_fields->m_gamemode = ModTypes::Gamemode::DEATH_EFFECT; break;
            }
        }
        GJGarageLayer::selectTab(p0);
    }
    bool init() {
        CC_SAFE_RELEASE_NULL(previewPlayer1);
        CC_SAFE_RELEASE_NULL(previewPlayer2);
        previewPlayer1 = Node::SimPlayer::create(
            ModTypes::Gamemode::CUBE,
            Save::getPlayerFrame(ModTypes::Gamemode::CUBE),
            {30.f, 30.f}, {0.5f, 0.5f}, 1.6f,
            Color::playerColor(),
            Color::playerColor2(),
            Color::playerGlowColor(),
            Save::getPlayerFrame(ModTypes::IconItemType::GLOW)
        );
        previewPlayer2 = Node::SimPlayer::create(
            ModTypes::Gamemode::CUBE,
            Save::getPlayerFrame(ModTypes::Gamemode::CUBE, true),
            {30.f, 30.f}, {0.5f, 0.5f}, 1.6f,
            Color::playerColor(true),
            Color::playerColor2(true),
            Color::playerGlowColor(true),
            Save::getPlayerFrame(ModTypes::IconItemType::GLOW, true)
        );
        GJGarageLayer::init();
        onSelectSecondPlayer = false;
        this->m_fields->cursor1 = static_cast<CCSprite*>(this->getChildByID("cursor-1"));
        this->m_fields->cursor1_shipStreak = static_cast<CCSprite*>(this->getChildByID("cursor-2"));
        this->m_fields->cursor1->setColor({0, 200, 0});
        this->m_fields->cursor1_shipStreak->setColor({0, 200, 0});
        this->m_fields->cursor1->addChild(this->m_fields->cursor1Label, 5);
        this->m_fields->cursor1_shipStreak->addChild(this->m_fields->cursor1Label, 5);
        this->m_fields->cursor1->setZOrder(102);
        this->m_fields->cursor1_shipStreak->setZOrder(102);
        this->m_fields->cursor1Label->setAnchorPoint({-0.3f, -0.3f});

        this->addChild(this->m_fields->cursor2, 101);
        this->addChild(this->m_fields->cursor2_shipStreak, 101);
        this->m_fields->cursor2->setColor({0, 125, 255});
        this->m_fields->cursor2_shipStreak->setColor({0, 125, 255});
        this->m_fields->cursor2->addChild(this->m_fields->cursor2Label, 5);
        this->m_fields->cursor2_shipStreak->addChild(this->m_fields->cursor2Label, 5);
        this->m_fields->cursor2Label->setAnchorPoint({1.2f, 1.3f});
        this->m_fields->cursor2Label->setPosition(this->m_fields->cursor2->getContentSize());

        auto winsize = CCDirector::sharedDirector()->getWinSize();
        auto menu = Node::Menu::create({0.f, 0.f}, winsize, this);

        Node::MenuItemSpriteExtra::create(
            previewPlayer1,
            this, menu_selector(SeparateDuelLayer::playerSelected),
            menu, { winsize.width / 2 - 50.f, winsize.height - 84.f}, { 0.5f, 0.5f },
            { 60.f, 60.f }, 0
        );
        Node::MenuItemSpriteExtra::create(
            previewPlayer2,
            this, menu_selector(SeparateDuelLayer::playerSelected),
            menu, { winsize.width / 2 + 50.f, winsize.height - 84.f}, { 0.5f, 0.5f },
            { 60.f, 60.f }, 1
        );
        previewPlayer1->setPosition({30.f, 30.f});
        previewPlayer2->setPosition({30.f, 30.f});
        this->getChildByID("player-icon")->setVisible(false);
        this->m_fields->arrow_1 = Node::Sprite::createWithSpriteFrameName("navArrowBtn_001.png",
            0.5f, {winsize.width / 2 - 90.f, winsize.height - 82.f}, {0.5f, 0.5f});
        this->m_fields->arrow_2 = Node::Sprite::createWithSpriteFrameName("navArrowBtn_001.png",
            0.5f, {winsize.width / 2 + 90.f, winsize.height - 82.f}, {0.5f, 0.5f}, true);
        this->m_fields->arrow_2->setVisible(false);

        this->m_fields->arrow_1->runAction(CCRepeatForever::create(CCSequence::create(
            CCEaseSineInOut::create(CCMoveTo::create(0.5f, {winsize.width / 2 - 90.f, winsize.height - 82.f})),
            CCEaseSineInOut::create(CCMoveTo::create(0.5f, {winsize.width / 2 - 95.f, winsize.height - 82.f})),
            nullptr
        )));
        this->m_fields->arrow_2->runAction(CCRepeatForever::create(CCSequence::create(
            CCEaseSineInOut::create(CCMoveTo::create(0.5f, {winsize.width / 2 + 90.f, winsize.height - 82.f})),
            CCEaseSineInOut::create(CCMoveTo::create(0.5f, {winsize.width / 2 + 95.f, winsize.height - 82.f})),
            nullptr
        )));

        this->m_fields->arrow_1->setColor({0, 200, 0});
        this->m_fields->arrow_2->setColor({0, 125, 255});

        this->addChild(this->m_fields->arrow_1);
        this->addChild(this->m_fields->arrow_2);

        Node::MenuItemSpriteExtra::create(Node::Sprite::createWithSpriteFrameName("AY_SwapBtn.png"_spr, 1.f), this, menu_selector(SeparateDuelLayer::swapIcon), menu, {winsize.width / 2, winsize.height - 82.f});
        
        auto toggle_unchecked = Node::Sprite::createWithSpriteFrameName("GJ_checkOff_001.png", 0.7f);
        auto toggle_checked = Node::Sprite::createWithSpriteFrameName("GJ_checkOn_001.png", 0.7f);
        
        this->m_fields->defaultMiniIconToggle = CCMenuItemToggler::create(toggle_unchecked, toggle_checked, this, menu_selector(SeparateDuelLayer::onDefaultMiniIconToggle));
        this->m_fields->defaultMiniIconToggle->setPosition({ 70.f, winsize.height - 95.f });
        this->m_fields->defaultMiniIconToggle->toggle(Mod::get()->getSavedValue<bool>("asakiyuki:default-mini-icon_0", false));
        menu->addChild(Node::LabelBMFont::create("Default Mini Icon", "bigFont.fnt", 0.32f, {
            this->m_fields->defaultMiniIconToggle->getPositionX() + 15.5f,
            this->m_fields->defaultMiniIconToggle->getPositionY()
        }, {
            0.f, 0.5f
        }));
        menu->addChild(this->m_fields->defaultMiniIconToggle);

        return true;
    }
};

class $modify(PlayLayer) {
    void resetLevel() {
        PlayLayer::resetLevel();
        if (this->m_isPracticeMode) return;
        isPlayer2ExitDual = false;
        lastBool = isPlayer2ExitDual;
        updateAllDualFrame();
    }
    void postUpdate(float p0) {
        PlayLayer::postUpdate(p0);
        Player::loadFrameColor(this->m_player1, isPlayer2ExitDual);
        Player::loadFrameColor(this->m_player2, isPlayer2ExitDual);
        if (Save::getCustomPlayer(ModTypes::CustomBool::CUSTOM_WAVE_TRAIL, isPlayer2ExitDual))
            this->m_player1->m_waveTrail->setColor(Color::playerWaveTrailColor(isPlayer2ExitDual));
        if (Save::getCustomPlayer(ModTypes::CustomBool::CUSTOM_WAVE_TRAIL, !isPlayer2ExitDual))
            this->m_player2->m_waveTrail->setColor(Color::playerWaveTrailColor(!isPlayer2ExitDual));
    };
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        auto backupColor1 = Save::getPlayerFrame(ModTypes::Gamemode::COLOR_1);
        auto backupColor2 = Save::getPlayerFrame(ModTypes::Gamemode::COLOR_2);
        if (backupColor1 == 15) Save::savePlayerFrame(12, ModTypes::Gamemode::COLOR_1);
        if (backupColor2 == 15) Save::savePlayerFrame(12, ModTypes::Gamemode::COLOR_2);
        PlayLayer::init(level, useReplay, dontCreateObjects);
        Save::savePlayerFrame(backupColor1, ModTypes::Gamemode::COLOR_1);
        Save::savePlayerFrame(backupColor2, ModTypes::Gamemode::COLOR_2);
        return true;
    }
};

class $modify(LevelEditorLayer) {
    void onPlaytest() {
        LevelEditorLayer::onPlaytest();
        isPlayer2ExitDual = false;
        lastBool = isPlayer2ExitDual;
        updateAllDualFrame();
    }
    void postUpdate(float p0) {
        LevelEditorLayer::postUpdate(p0);
        Player::loadFrameColor(this->m_player1, isPlayer2ExitDual);
        Player::loadFrameColor(this->m_player2, isPlayer2ExitDual);
    }
};

class $modify(PlayerObject) {
    bool isOnMini = false;
    bool init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4) {
        if (Player::getCurrentIngameLayer() != nullptr) {
            if (isPlayer2) {
                int bakStreak = Save::getPlayerFrame(ModTypes::Gamemode::STREAK);
                int bakShipStreak = Save::getPlayerFrame(ModTypes::Gamemode::SHIP_STREAK);
                Save::savePlayerFrame(Save::getPlayerFrame(ModTypes::Gamemode::STREAK, true), ModTypes::Gamemode::STREAK);
                Save::savePlayerFrame(Save::getPlayerFrame(ModTypes::Gamemode::SHIP_STREAK, true), ModTypes::Gamemode::SHIP_STREAK);
                PlayerObject::init(p0, p1, p2, p3, p4);
                Save::savePlayerFrame(bakStreak, ModTypes::Gamemode::STREAK);
                Save::savePlayerFrame(bakShipStreak, ModTypes::Gamemode::SHIP_STREAK);
            } else PlayerObject::init(p0, p1, p2, p3, p4);
            isPlayer2 = !isPlayer2;
        } else PlayerObject::init(p0, p1, p2, p3, p4);
        return true;
    }
    void updatePlayerFrame(int id) {
        if (Player::isPlayer(this)) {
            bool isPlayer2 = Player::isPlayer2(this, isPlayer2ExitDual);
            auto mod = Mod::get();

            if (mod->getSavedValue<bool>("asakiyuki:custom_dual_mod", false) && (mod->getSavedValue<int>("asakiyuki:dual_types", 0) == 2))
                return (this->m_fields->isOnMini && mod->getSavedValue<bool>(CCString::createWithFormat("asakiyuki:default-mini-icon_%i", static_cast<int>(isPlayer2))->m_sString, false)) ? PlayerObject::updatePlayerFrame(0) : Player::changeIcon(this, ModTypes::Gamemode::CUBE, Save::getPlayerFrame(ModTypes::Gamemode::CUBE, isPlayer2));
            else if (mod->getSavedValue<bool>("asakiyuki:default-mini-icon_0", false) && this->m_fields->isOnMini) return PlayerObject::updatePlayerFrame(0);
            else return Player::changeIcon(this, ModTypes::Gamemode::CUBE, Save::getPlayerFrame(ModTypes::Gamemode::CUBE, isPlayer2));
        } PlayerObject::updatePlayerFrame(id);
    }
    void updatePlayerShipFrame(int id) {
        if (Player::isPlayer(this)) 
            return Player::loadFrameByCurrentGamemode(this, isPlayer2ExitDual);
        PlayerObject::updatePlayerShipFrame(id);
    }
    void updatePlayerRollFrame(int id) {
        if (Player::isPlayer(this)) {
            bool isPlayer2 = Player::isPlayer2(this, isPlayer2ExitDual);
            auto mod = Mod::get();
            if (mod->getSavedValue<bool>("asakiyuki:custom_dual_mod", false) && (mod->getSavedValue<int>("asakiyuki:dual_types", 0) == 2))
                return (this->m_fields->isOnMini && mod->getSavedValue<bool>(CCString::createWithFormat("asakiyuki:default-mini-icon_%i", static_cast<int>(isPlayer2))->m_sString, false)) ? PlayerObject::updatePlayerRollFrame(0) : Player::loadFrameByCurrentGamemode(this, isPlayer2ExitDual);
            else if (mod->getSavedValue<bool>("asakiyuki:default-mini-icon_0", false) && this->m_fields->isOnMini) return PlayerObject::updatePlayerRollFrame(0);
            else return Player::loadFrameByCurrentGamemode(this, isPlayer2ExitDual);
        } PlayerObject::updatePlayerRollFrame(id);
    }
    void updatePlayerBirdFrame(int id) {
        if (Player::isPlayer(this)) 
            return Player::loadFrameByCurrentGamemode(this, isPlayer2ExitDual);
        PlayerObject::updatePlayerBirdFrame(id);
    }
    void updatePlayerDartFrame(int id) {
        if (Player::isPlayer(this)) 
            return Player::loadFrameByCurrentGamemode(this, isPlayer2ExitDual);
        PlayerObject::updatePlayerDartFrame(id);
    }
    void updatePlayerSwingFrame(int id) {
        if (Player::isPlayer(this)) 
            return Player::loadFrameByCurrentGamemode(this, isPlayer2ExitDual);
        PlayerObject::updatePlayerSwingFrame(id);
    }
    void updatePlayerJetpackFrame(int id) {
        if (Player::isPlayer(this)) 
            return Player::loadFrameByCurrentGamemode(this, isPlayer2ExitDual);
        PlayerObject::updatePlayerJetpackFrame(id);
    }
    void createRobot(int id) {
        if (Player::isPlayer(this)) 
            return PlayerObject::createRobot(Save::getPlayerFrame(ModTypes::Gamemode::ROBOT, Player::isPlayer2(this, isPlayer2ExitDual)));
        PlayerObject::createRobot(id);
    }
    void createSpider(int id) {
        if (Player::isPlayer(this)) 
            return PlayerObject::createSpider(Save::getPlayerFrame(ModTypes::Gamemode::SPIDER, Player::isPlayer2(this, isPlayer2ExitDual)));
        PlayerObject::createSpider(id);
    }
    void togglePlayerScale(bool p0, bool p1) {
        PlayerObject::togglePlayerScale(p0, p1);
        if (Player::isPlayer(this)) {
            this->m_fields->isOnMini = p0;
            this->updatePlayerRollFrame(0);
            this->updatePlayerFrame(0);
            Player::loadFrameByCurrentGamemode(this, isPlayer2ExitDual);
        }
    };
};

class $modify(GJBaseGameLayer) {
    void toggleDualMode(GameObject* p0, bool p1, PlayerObject* p2, bool p3) {
        if (Player::isPlayer2(p2, !isPlayer2ExitDual) && !p3) {
            GJBaseGameLayer::toggleDualMode(p0, p1, p2, p3);
            if (!Mod::get()->getSavedValue<bool>("asakiyuki:separate_swapper", false)) return;
            isPlayer2ExitDual = false;
        } else {
            GJBaseGameLayer::toggleDualMode(p0, p1, p2, p3);
            if (!Mod::get()->getSavedValue<bool>("asakiyuki:separate_swapper", false)) return;
            isPlayer2ExitDual = true;
        };
        updateAllDualFrame();
    };

    void playExitDualEffect(PlayerObject* p0) {
        if (Player::isPlayer2(p0, isPlayer2ExitDual)) {
            auto currentGamemode = Player::getCurrentGamemode(p0);
            int currentFrame = Save::getPlayerFrame(currentGamemode);
            Save::savePlayerFrame(Save::getPlayerFrame(currentGamemode, true), currentGamemode);
            GJBaseGameLayer::playExitDualEffect(p0);
            Save::savePlayerFrame(currentFrame, currentGamemode);
        } else {
            GJBaseGameLayer::playExitDualEffect(p0);
        };
    }
};

CCSprite* pastelColorSprite = nullptr;
CCSprite* pastelColorSpriteReverse = nullptr;
CCSprite* chromaColorSprite = nullptr;
CCSprite* chromaColorSpriteReverse = nullptr;

SimplePlayer* preview_cube = nullptr;
SimplePlayer* preview_ship = nullptr;
SimplePlayer* preview_ball = nullptr;
SimplePlayer* preview_ufo = nullptr;
SimplePlayer* preview_wave = nullptr;
SimplePlayer* preview_robot = nullptr;
SimplePlayer* preview_spider = nullptr;
SimplePlayer* preview_swing = nullptr;

bool isPlayer2SelectorColor = false;

class ColorSpeed : public Popup<>, TextInputDelegate {
    public:
    static ColorSpeed* create() {
        auto pRet = new ColorSpeed();
        if (pRet && pRet->initAnchored(225.f, 75.f)) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
    void onOpen(CCObject*) {
        ColorSpeed::create()->show();
    }
    InputNode* cycleSpeedInput;

    virtual void textChanged(CCTextInputNode* input) {
        if (input->getString() == "") return;
        Mod::get()->setSavedValue<float>("asakiyuki:color_cycle_speed", std::stof(input->getString()));
    }
    protected:
    bool setup() {
        auto ps = this->m_mainLayer->getContentSize();
        this->setTitle("Color Cycle Speed", "bigFont.fnt", 0.55f, 15.f);
        this->cycleSpeedInput = InputNode::create(175.f, "Cycle Speed", "bigFont.fnt", "0123456789.", 6);
        this->cycleSpeedInput->getInput()->setLabelPlaceholderColor({ 200, 200, 200 });
        this->cycleSpeedInput->getInput()->setDelegate(this);
        this->cycleSpeedInput->setPosition({ ps.width / 2, ps.height / 2 - 7.5f });
        this->cycleSpeedInput->setString(
            CCString::createWithFormat("%.2f", Mod::get()->getSavedValue<float>("asakiyuki:color_cycle_speed", 1.f))->getCString()
        );
        this->m_mainLayer->addChild(this->cycleSpeedInput);
        return true;
    }
};

class $modify(AsaCharacterColorPage, CharacterColorPage) {
    bool isPlayer2 = false;
    int colorMode = 0;
    int shipID = 8;

    extension::CCScale9Sprite* text1 = nullptr;
    extension::CCScale9Sprite* text2 = nullptr;

    CCSprite* cursor1 = nullptr;
    CCSprite* cursor2;
    CCSprite* cursorGLow;
    CCSprite* cursorWave = Node::Sprite::createWithSpriteFrameName("GJ_select_001.png", 0.7f);
    
    CCMenuItemToggler* waveToggle = nullptr;
    CCLabelBMFont* waveLabel;

    void keyBackClicked() {
        CharacterColorPage::keyBackClicked();
        CC_SAFE_RELEASE_NULL(pastelColorSpriteReverse);
        CC_SAFE_RELEASE_NULL(pastelColorSprite);
        CC_SAFE_RELEASE_NULL(chromaColorSpriteReverse);
        CC_SAFE_RELEASE_NULL(chromaColorSprite);
        CC_SAFE_RELEASE_NULL(preview_cube);
        CC_SAFE_RELEASE_NULL(preview_ship);
        CC_SAFE_RELEASE_NULL(preview_ball);
        CC_SAFE_RELEASE_NULL(preview_ufo);
        CC_SAFE_RELEASE_NULL(preview_wave);
        CC_SAFE_RELEASE_NULL(preview_robot);
        CC_SAFE_RELEASE_NULL(preview_spider);
        CC_SAFE_RELEASE_NULL(preview_swing);
    }
    void onClose(cocos2d::CCObject* sender) {
        CharacterColorPage::onClose(sender);
        CC_SAFE_RELEASE_NULL(pastelColorSpriteReverse);
        CC_SAFE_RELEASE_NULL(pastelColorSprite);
        CC_SAFE_RELEASE_NULL(chromaColorSpriteReverse);
        CC_SAFE_RELEASE_NULL(chromaColorSprite);
        CC_SAFE_RELEASE_NULL(preview_cube);
        CC_SAFE_RELEASE_NULL(preview_ship);
        CC_SAFE_RELEASE_NULL(preview_ball);
        CC_SAFE_RELEASE_NULL(preview_ufo);
        CC_SAFE_RELEASE_NULL(preview_wave);
        CC_SAFE_RELEASE_NULL(preview_robot);
        CC_SAFE_RELEASE_NULL(preview_spider);
        CC_SAFE_RELEASE_NULL(preview_swing);
    }

    void onMode(CCObject* p0) {
        CharacterColorPage::onMode(p0);
        this->m_fields->text1->setVisible(p0->getTag() != 3);
        this->m_fields->text2->setVisible(p0->getTag() == 3);
        this->m_fields->waveToggle->setVisible(p0->getTag() == 3);
        this->m_fields->waveLabel->setVisible(p0->getTag() == 3);
        if (p0->getTag() == 3)
            this->m_fields->cursorWave->setColor({ 255, 255, 255 });
        else 
            this->m_fields->cursorWave->setColor({ 50, 50, 50 });
        this->m_fields->cursorWave->setZOrder((p0->getTag() == 3) ? 11 : 1);
    }

    void toggleGlow(CCObject* p0) {
        if (this->m_fields->isPlayer2) {
            Save::savePlayerFrame(static_cast<CCMenuItemToggler*>(p0)->isToggled(), ModTypes::IconItemType::GLOW, true);
            this->updateIconColors();
        } else CharacterColorPage::toggleGlow(p0);
    }

    void updateColorMode(int id) {
        CharacterColorPage::updateColorMode(id);
        if (Player::getCurrentIngameLayer() != nullptr) return;
        this->m_fields->colorMode = id;
        this->updateSelector();
    }
    
    void updateSelector() {
        if (this->m_fields->cursor1 == nullptr) return;
        auto c1 = this->m_buttonMenu->getChildByID(CCString::createWithFormat("%i", Save::getPlayerFrame(ModTypes::Gamemode::COLOR_1, this->m_fields->isPlayer2))->m_sString)->getPosition();
        auto c2 = this->m_buttonMenu->getChildByID(CCString::createWithFormat("%i", Save::getPlayerFrame(ModTypes::Gamemode::COLOR_2, this->m_fields->isPlayer2))->m_sString)->getPosition();
        auto c3 = this->m_buttonMenu->getChildByID(CCString::createWithFormat("%i", Save::getPlayerFrame(ModTypes::Gamemode::COLOR_GLOW, this->m_fields->isPlayer2))->m_sString)->getPosition();
        auto c4 = this->m_buttonMenu->getChildByID(CCString::createWithFormat("%i", Save::getCustomPlayer(ModTypes::Custom::WAVE_TRAIL, this->m_fields->isPlayer2))->m_sString)->getPosition();
        auto bmm = this->m_buttonMenu->getPosition();
        this->m_fields->cursor1->setPosition({bmm.x + c1.x, bmm.y + c1.y});
        this->m_fields->cursor2->setPosition({bmm.x + c2.x, bmm.y + c2.y});
        this->m_fields->cursorGLow->setPosition({bmm.x + c3.x, bmm.y + c3.y});
        this->m_fields->cursorWave->setPosition({bmm.x + c4.x, bmm.y + c4.y});
    }

    void onPlayerColor(CCObject* sender) {
        if (this->m_fields->colorMode != 3) {
            if (this->m_fields->isPlayer2) {
                if (!GameManager::get()->isColorUnlocked(sender->getTag(), static_cast<UnlockType>((this->m_fields->colorMode == 0) ? 2 : 3))) return CharacterColorPage::onPlayerColor(sender); 
                auto bmm = this->m_buttonMenu->getPosition();
                auto clm = static_cast<CCMenuItemSpriteExtra*>(sender)->getPosition();
                Save::savePlayerFrame(sender->getTag(), static_cast<ModTypes::Gamemode>(100 + this->m_fields->colorMode), true);
                this->updateIconColors();
                this->updateSelector();
            } else CharacterColorPage::onPlayerColor(sender);
        } else {
            if (!GameManager::get()->isColorUnlocked(sender->getTag(), static_cast<UnlockType>((this->m_fields->colorMode == 0) ? 2 : 3))) return CharacterColorPage::onPlayerColor(sender); 
            Save::saveCustomPlayer(sender->getTag(), ModTypes::Custom::WAVE_TRAIL, this->m_fields->isPlayer2);
            this->updateSelector();
        }
    }

    void toggleShip(cocos2d::CCObject* p0) {
        this->m_fields->shipID = p0->getTag();
        CharacterColorPage::toggleShip(p0);
        preview_ship->updatePlayerFrame(Save::getPlayerFrame((this->m_fields->shipID == 8) ? ModTypes::Gamemode::SHIP : ModTypes::Gamemode::JETPACK, this->m_fields->isPlayer2 ),(this->m_fields->shipID == 8) ? IconType::Ship : IconType::Jetpack);
    };

    void changePlayerAction(CCObject* sender) {
        this->m_fields->isPlayer2 = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
        isPlayer2SelectorColor = this->m_fields->isPlayer2;
        this->m_fields->waveToggle->toggle(Save::getCustomPlayer(ModTypes::CustomBool::CUSTOM_WAVE_TRAIL, this->m_fields->isPlayer2));
        preview_cube->updatePlayerFrame(Save::getPlayerFrame(ModTypes::Gamemode::CUBE, this->m_fields->isPlayer2), IconType::Cube);
        static_cast<CCMenuItemToggler*>(this->m_buttonMenu->getChildByID("glow-toggler"))->toggle(!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, this->m_fields->isPlayer2));
        preview_ship->updatePlayerFrame(Save::getPlayerFrame((this->m_fields->shipID == 8) ? ModTypes::Gamemode::SHIP : ModTypes::Gamemode::JETPACK, this->m_fields->isPlayer2 ),(this->m_fields->shipID == 8) ? IconType::Ship : IconType::Jetpack);

        preview_ball->updatePlayerFrame(Save::getPlayerFrame(ModTypes::Gamemode::BALL, this->m_fields->isPlayer2), IconType::Ball);
        preview_ufo->updatePlayerFrame(Save::getPlayerFrame(ModTypes::Gamemode::UFO, this->m_fields->isPlayer2), IconType::Ufo);
        preview_wave->updatePlayerFrame(Save::getPlayerFrame(ModTypes::Gamemode::WAVE, this->m_fields->isPlayer2), IconType::Wave);
        preview_robot->updatePlayerFrame(Save::getPlayerFrame(ModTypes::Gamemode::ROBOT, this->m_fields->isPlayer2), IconType::Robot);
        preview_spider->updatePlayerFrame(Save::getPlayerFrame(ModTypes::Gamemode::SPIDER, this->m_fields->isPlayer2), IconType::Spider);
        preview_swing->updatePlayerFrame(Save::getPlayerFrame(ModTypes::Gamemode::SWING, this->m_fields->isPlayer2), IconType::Swing);
        this->updateIconColors();
        this->updateSelector();
    }

    void toggleWaveTrail(CCObject* sender) {
        Save::saveCustomPlayer(!static_cast<CCMenuItemToggler*>(sender)->isToggled(), ModTypes::CustomBool::CUSTOM_WAVE_TRAIL, this->m_fields->isPlayer2);
    }

    bool init() {
        CharacterColorPage::init();
        if (Player::getCurrentIngameLayer() == nullptr) {
            {    
                CC_SAFE_RELEASE_NULL(pastelColorSpriteReverse);
                CC_SAFE_RELEASE_NULL(pastelColorSprite);
                CC_SAFE_RELEASE_NULL(chromaColorSpriteReverse);
                CC_SAFE_RELEASE_NULL(chromaColorSprite);
                CC_SAFE_RELEASE_NULL(preview_cube);
                CC_SAFE_RELEASE_NULL(preview_ship);
                CC_SAFE_RELEASE_NULL(preview_ball);
                CC_SAFE_RELEASE_NULL(preview_ufo);
                CC_SAFE_RELEASE_NULL(preview_wave);
                CC_SAFE_RELEASE_NULL(preview_robot);
                CC_SAFE_RELEASE_NULL(preview_spider);
                CC_SAFE_RELEASE_NULL(preview_swing);
            }
            {
                Node::MenuItemSpriteExtra::create(Node::Sprite::createWithSpriteFrameName("GJ_optionsBtn_001.png", 0.73), this, menu_selector(ColorSpeed::onOpen),
                this->m_buttonMenu, {40.f, 0.f});
            }
            auto winsize = CCDirector::get()->getWinSize(); 
            auto player1Btn = Node::Sprite::createWithSpriteFrameName("geode.loader/baseCircle_Small_Blue.png", 0.85f);
            {
                pastelColorSprite = Node::Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png", 0.65f, { 0.f, 0.f }, { 0.5f, 0.5f });
                pastelColorSpriteReverse = Node::Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png", 0.65f, { 0.f, 0.f }, { 0.5f, 0.5f });
                chromaColorSprite = Node::Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png", 0.65f, { 0.f, 0.f }, { 0.5f, 0.5f });
                chromaColorSpriteReverse = Node::Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png", 0.65f, { 0.f, 0.f }, { 0.5f, 0.5f });

                auto pastelText = Node::LabelBMFont::create("P", "bigFont.fnt", 0.5f, { 12.5f, 12.5f });
                pastelText->setOpacity(175);
                auto chromaText = Node::LabelBMFont::create("C", "bigFont.fnt", 0.5f, { 12.5f, 12.5f });
                chromaText->setOpacity(175);

                auto pastelBtn = Node::MenuItemSpriteExtra::create(
                    pastelColorSprite, this, menu_selector(CharacterColorPage::onPlayerColor), this->m_buttonMenu,
                    { this->m_buttonMenu->getChildByID("51")->getPositionX() - 45.f,
                      this->m_buttonMenu->getChildByID("51")->getPositionY() }, { 0.5f, 0.5f }, {23.075f, 23.075f}, 
                1001);
                pastelBtn->setID("1001");

                auto pastelReverseBtn = Node::MenuItemSpriteExtra::create(
                    pastelColorSpriteReverse, this, menu_selector(CharacterColorPage::onPlayerColor), this->m_buttonMenu,
                    { pastelBtn->getPositionX(), pastelBtn->getPositionY() - 24.f }, { 0.5f, 0.5f }, {23.075f, 23.075f}, 
                1002);
                pastelReverseBtn->setID("1002");

                auto chromaBtn = Node::MenuItemSpriteExtra::create(
                    chromaColorSprite, this, menu_selector(CharacterColorPage::onPlayerColor), this->m_buttonMenu,
                    { pastelReverseBtn->getPositionX(), pastelReverseBtn->getPositionY() - 24.f }, { 0.5f, 0.5f }, {23.075f, 23.075f}, 
                1003);
                chromaBtn->setID("1003");

                auto chromaReverseBtn = Node::MenuItemSpriteExtra::create(
                    chromaColorSpriteReverse, this, menu_selector(CharacterColorPage::onPlayerColor), this->m_buttonMenu,
                    { chromaBtn->getPositionX(), chromaBtn->getPositionY() - 24.f }, { 0.5f, 0.5f }, {23.075f, 23.075f}, 
                1004);
                chromaReverseBtn->setID("1004");

                pastelBtn->addChild(pastelText, 1);
                pastelReverseBtn->addChild(pastelText, 1);
                chromaBtn->addChild(chromaText, 1);
                chromaReverseBtn->addChild(chromaText, 1);

                auto rgbColorBG = extension::CCScale9Sprite::create("GJ_square05.png");
                rgbColorBG->setContentSize({ 55.f, 120.f });
                rgbColorBG->setPosition({
                    this->m_buttonMenu->getPositionX() + pastelBtn->getPositionX() + 5.f,
                    this->m_buttonMenu->getPositionY() + (pastelBtn->getPositionY() - 36.f)
                });
                rgbColorBG->setColor({ 150, 150, 150 });
                this->m_mainLayer->addChild(rgbColorBG, -2);
            }
            {
                player1Btn->addChild(Node::LabelBMFont::create("P1", "bigFont.fnt", 0.5f, {20.f, 20.5f}));
                auto player2Btn = Node::Sprite::createWithSpriteFrameName("geode.loader/baseCircle_Small_Blue.png", 0.85f);
                player2Btn->addChild(Node::LabelBMFont::create("P2", "bigFont.fnt", 0.5f, {20.f, 20.5f}));

                auto toggle = CCMenuItemToggler::create(player1Btn, player2Btn, this, menu_selector(AsaCharacterColorPage::changePlayerAction));
                toggle->setPosition({-10.f, -52.5f});
                this->m_buttonMenu->addChild(toggle, 5);
                this->m_buttonMenu->addChild(this->m_fields->cursorWave, 1);
                this->m_fields->cursorWave->setColor({ 50, 50, 50 });
                this->m_mainLayer->addChild(this->m_fields->cursorWave);

                this->m_fields->cursor1 = static_cast<CCSprite*>(this->m_mainLayer->getChildByID("cursor-col1"));
                this->m_fields->cursor2 = static_cast<CCSprite*>(this->m_mainLayer->getChildByID("cursor-col2"));
                this->m_fields->cursorGLow = static_cast<CCSprite*>(this->m_mainLayer->getChildByID("cursor-glow"));

                preview_cube = static_cast<SimplePlayer*>(this->m_mainLayer->getChildByID("cube-icon"));
                preview_ship = static_cast<SimplePlayer*>(this->m_buttonMenu->getChildByID("ship-button")->getChildren()->objectAtIndex(0));
                preview_ball = static_cast<SimplePlayer*>(this->m_mainLayer->getChildByID("ball-icon"));
                preview_ufo = static_cast<SimplePlayer*>(this->m_mainLayer->getChildByID("ufo-icon"));
                preview_wave = static_cast<SimplePlayer*>(this->m_mainLayer->getChildByID("wave-icon"));
                preview_robot = static_cast<SimplePlayer*>(this->m_mainLayer->getChildByID("robot-icon"));
                preview_spider = static_cast<SimplePlayer*>(this->m_mainLayer->getChildByID("spider-icon"));
                preview_swing = static_cast<SimplePlayer*>(this->m_mainLayer->getChildByID("swing-icon"));

                {
                    auto buttonSprite = extension::CCScale9Sprite::create("GJ_button_04.png");
                    buttonSprite->setContentSize({ 48.6f, 20.f });
                    buttonSprite->addChild(Node::LabelBMFont::create("Wave", "bigFont.fnt", 0.4f, { 24.3f, 12.f }));
                    this->m_fields->text1 = buttonSprite;
                    auto buttonSprite2 = extension::CCScale9Sprite::create("GJ_button_01.png");
                    buttonSprite2->setContentSize({ 48.6f, 20.f });
                    buttonSprite2->addChild(Node::LabelBMFont::create("Wave", "bigFont.fnt", 0.4f, { 24.3f, 12.f }));
                    buttonSprite2->setVisible(false);
                    this->m_fields->text2 = buttonSprite2;
                    auto node = CCNode::create();
                    node->addChild(buttonSprite);
                    node->addChild(buttonSprite2);
                    node->setTag(1);
                    node->setScale(0.85f);
                    node->setContentSize({ 48.6f, 20.f });
                    buttonSprite->setPosition({ 24.3f, 10.f });
                    buttonSprite2->setPosition({ 24.3f, 10.f });

                    auto btnColor1 = this->m_buttonMenu->getChildByID("col1-button");
                    auto btn = Node::MenuItemSpriteExtra::create(node, this, menu_selector(CharacterColorPage::onMode), this->m_buttonMenu);
                    btn->setContentSize({ 48.6f, 20.f });
                    btn->setTag(3);
                    btn->setPosition({ btnColor1->getPositionX() - 50.f, btnColor1->getPositionY() });
                    node->setPosition({ 24.3f, 10.f });

                    auto tggOff = Node::Sprite::createWithSpriteFrameName("GJ_checkOff_001.png", 0.6f);
                    auto tggOn = Node::Sprite::createWithSpriteFrameName("GJ_checkOn_001.png", 0.6f);

                    this->m_fields->waveToggle = CCMenuItemToggler::create(tggOff, tggOn, this, menu_selector(AsaCharacterColorPage::toggleWaveTrail));
                    this->m_buttonMenu->addChild(this->m_fields->waveToggle);
                    this->m_fields->waveToggle->setPosition(this->m_buttonMenu->getChildByID("glow-toggler")->getPosition());
                    this->m_fields->waveToggle->setVisible(false);
                    this->m_fields->waveLabel = CCLabelBMFont::create("Wave Trail", "bigFont.fnt");
                    this->m_fields->waveLabel->setScale(0.35f);
                    this->m_fields->waveLabel->setVisible(false);
                    this->m_fields->waveLabel->setAnchorPoint({ 0.f, 0.5f });
                    this->m_fields->waveLabel->setPosition(this->m_mainLayer->getChildByID("glow-label")->getPosition());
                    this->m_mainLayer->addChild(this->m_fields->waveLabel, 10);
                }
                this->m_fields->isPlayer2 = onSelectSecondPlayer;
                toggle->toggle(!onSelectSecondPlayer);
                this->changePlayerAction(static_cast<CCObject*>(toggle));
                toggle->toggle(onSelectSecondPlayer);
            }
        }
        return true;
    }
};

class $modify(CCScheduler) {
    void update(float delta) {
        CCScheduler::update(delta);
        if (pastelColorSprite != nullptr) pastelColorSprite->setColor(Color::getColor(ModTypes::ColorType::PASTEL, Mod::get()->getSavedValue<float>("asakiyuki:color_cycle_speed", 1.f)));
        if (pastelColorSpriteReverse != nullptr) pastelColorSpriteReverse->setColor(Color::getColor(ModTypes::ColorType::PASTEL, Mod::get()->getSavedValue<float>("asakiyuki:color_cycle_speed", 1.f), true));
        if (chromaColorSprite != nullptr) chromaColorSprite->setColor(Color::getColor(ModTypes::ColorType::RAINBOW, Mod::get()->getSavedValue<float>("asakiyuki:color_cycle_speed", 1.f)));
        if (chromaColorSpriteReverse != nullptr) chromaColorSpriteReverse->setColor(Color::getColor(ModTypes::ColorType::RAINBOW, Mod::get()->getSavedValue<float>("asakiyuki:color_cycle_speed", 1.f), true));

        if (previewPlayer1 != nullptr) {
            previewPlayer1->setColor(Color::playerColor());
            previewPlayer1->setSecondColor(Color::playerColor2());
            previewPlayer1->setGlowOutline(Color::playerGlowColor());
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW)) previewPlayer1->disableGlowOutline();
        }
        if (previewPlayer2 != nullptr) {
            previewPlayer2->setColor(Color::playerColor(true));
            previewPlayer2->setSecondColor(Color::playerColor2(true));
            previewPlayer2->setGlowOutline(Color::playerGlowColor(true));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, true)) previewPlayer2->disableGlowOutline();
        }

        if (preview_cube != nullptr) {
            preview_cube->setColor(Color::playerColor(isPlayer2SelectorColor));
            preview_cube->setSecondColor(Color::playerColor2(isPlayer2SelectorColor));
            preview_cube->setGlowOutline(Color::playerGlowColor(isPlayer2SelectorColor));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, isPlayer2SelectorColor)) preview_cube->disableGlowOutline();
        }
        if (preview_ship != nullptr) {
            preview_ship->setColor(Color::playerColor(isPlayer2SelectorColor));
            preview_ship->setSecondColor(Color::playerColor2(isPlayer2SelectorColor));
            preview_ship->setGlowOutline(Color::playerGlowColor(isPlayer2SelectorColor));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, isPlayer2SelectorColor)) preview_ship->disableGlowOutline();
        }
        if (preview_ball != nullptr) {
            preview_ball->setColor(Color::playerColor(isPlayer2SelectorColor));
            preview_ball->setSecondColor(Color::playerColor2(isPlayer2SelectorColor));
            preview_ball->setGlowOutline(Color::playerGlowColor(isPlayer2SelectorColor));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, isPlayer2SelectorColor)) preview_ball->disableGlowOutline();
        }
        if (preview_ufo != nullptr) {
            preview_ufo->setColor(Color::playerColor(isPlayer2SelectorColor));
            preview_ufo->setSecondColor(Color::playerColor2(isPlayer2SelectorColor));
            preview_ufo->setGlowOutline(Color::playerGlowColor(isPlayer2SelectorColor));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, isPlayer2SelectorColor)) preview_ufo->disableGlowOutline();
        }
        if (preview_wave != nullptr) {
            preview_wave->setColor(Color::playerColor(isPlayer2SelectorColor));
            preview_wave->setSecondColor(Color::playerColor2(isPlayer2SelectorColor));
            preview_wave->setGlowOutline(Color::playerGlowColor(isPlayer2SelectorColor));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, isPlayer2SelectorColor)) preview_wave->disableGlowOutline();
        }
        if (preview_robot != nullptr) {
            preview_robot->setColor(Color::playerColor(isPlayer2SelectorColor));
            preview_robot->setSecondColor(Color::playerColor2(isPlayer2SelectorColor));
            preview_robot->setGlowOutline(Color::playerGlowColor(isPlayer2SelectorColor));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, isPlayer2SelectorColor)) preview_robot->disableGlowOutline();
        }
        if (preview_spider != nullptr) {
            preview_spider->setColor(Color::playerColor(isPlayer2SelectorColor));
            preview_spider->setSecondColor(Color::playerColor2(isPlayer2SelectorColor));
            preview_spider->setGlowOutline(Color::playerGlowColor(isPlayer2SelectorColor));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, isPlayer2SelectorColor)) preview_spider->disableGlowOutline();
        }
        if (preview_swing != nullptr) {
            preview_swing->setColor(Color::playerColor(isPlayer2SelectorColor));
            preview_swing->setSecondColor(Color::playerColor2(isPlayer2SelectorColor));
            preview_swing->setGlowOutline(Color::playerGlowColor(isPlayer2SelectorColor));
            if (!Save::getPlayerFrame(ModTypes::IconItemType::GLOW, isPlayer2SelectorColor)) preview_swing->disableGlowOutline();
        }
    };
};