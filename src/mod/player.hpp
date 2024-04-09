#ifndef MOD_PLAYER_HPP
#define MOD_PLAYER_HPP
#include <Geode/Geode.hpp>
#include "./enum.hpp"
#include "./save.hpp"
using namespace geode::prelude;

namespace AsakiYuki {
    class Player {
        protected:
        static inline GameManager* gameManager = GameManager::sharedState();
        public:
        static void changeIcon(PlayerObject* player,
            ModTypes::Gamemode gameMode = ModTypes::Gamemode::CUBE,
            int iconID = -1) {
            if (iconID == -1) iconID = AsakiYuki::Save::getPlayerFrame(AsakiYuki::Player::getCurrentGamemode(player));
            switch (gameMode) {
                case ModTypes::Gamemode::CUBE:
                    player->updatePlayerFrame(iconID);
                    break;
                case ModTypes::Gamemode::SHIP:
                    player->updatePlayerShipFrame(iconID);
                    break;
                case ModTypes::Gamemode::BALL:
                    player->updatePlayerRollFrame(iconID);
                    break;
                case ModTypes::Gamemode::UFO:
                    player->updatePlayerBirdFrame(iconID);
                    break;
                case ModTypes::Gamemode::WAVE:
                    player->updatePlayerDartFrame(iconID);
                    break;
                case ModTypes::Gamemode::ROBOT:
                    player->updatePlayerRobotFrame(iconID);
                    break;
                case ModTypes::Gamemode::SPIDER:
                    player->updatePlayerSpiderFrame(iconID);
                    break;
                case ModTypes::Gamemode::SWING:
                    player->updatePlayerSwingFrame(iconID);
                    break;
                case ModTypes::Gamemode::JETPACK:
                    player->updatePlayerJetpackFrame(iconID);
                    break;
                default:
                    return;
            };
        }
        static void updateIcon(int iconID, ModTypes::Gamemode gameMode = ModTypes::Gamemode::CUBE, ModTypes::PlayerType type = ModTypes::PlayerType::BOTH_PLAYERS) {
            auto gameLayer = gameManager->getGameLayer();
            try {
                if (gameLayer == nullptr) throw std::runtime_error("GJGameBaseLayer is not found!");
                if ((type == ModTypes::PlayerType::FIRST_PLAYER) || (type == ModTypes::PlayerType::BOTH_PLAYERS)) {
                    changeIcon(gameLayer->m_player1, gameMode, iconID);
                    changeIcon(gameLayer->m_player1, getCurrentGamemode());
                }
                if ((type == ModTypes::PlayerType::SECOND_PLAYER) || (type == ModTypes::PlayerType::BOTH_PLAYERS)) {   
                    changeIcon(gameLayer->m_player2, gameMode, iconID);     
                    changeIcon(gameLayer->m_player2, getCurrentGamemode(true));
                }
            }
            catch(const std::exception& e) {
                log::info("{}", e.what());
            }
        }
        static bool isPlayer2(PlayerObject* player, bool reverse = false) {
            if (Player::getCurrentIngameLayer() != nullptr) return reverse ? (player != Player::getCurrentIngameLayer()->m_player2) : (player == Player::getCurrentIngameLayer()->m_player2);
            return false;
        }
        static void loadFrameByCurrentGamemode(PlayerObject* player, bool isReverse = false) {
            bool isPlayer2 = (isReverse) ? (!Player::isPlayer2(player)) : (Player::isPlayer2(player));
            Player::changeIcon(player, Player::getCurrentGamemode(player), Save::getPlayerFrame(Player::getCurrentGamemode(player), isPlayer2));
        }
        static void loadFrameColor(PlayerObject* player, bool isReverse = false) {
            bool isPlayer2 = Player::isPlayer2(player, isReverse);
            auto color1 = Color::playerColor(isPlayer2);
            player->setColor(color1);
            player->setSecondColor(Color::playerColor2(isPlayer2));
            player->m_glowColor = Color::playerGlowColor(isPlayer2);
            player->updateGlowColor();
            player->m_hasGlow = Save::getPlayerFrame(ModTypes::IconItemType::GLOW, isPlayer2);
        }
        static GJBaseGameLayer* getCurrentIngameLayer() {
            GJBaseGameLayer* layer = nullptr;
            if (PlayLayer::get() != nullptr) layer = PlayLayer::get();
            else if (LevelEditorLayer::get() != nullptr) layer = LevelEditorLayer::get();
            return layer;
        }
        static bool isPlayer(PlayerObject* player) {
            auto gameScreen = Player::getCurrentIngameLayer();
            return (gameScreen != nullptr) && ((player == gameScreen->m_player1) || (player == gameScreen->m_player2));
        }
        static void updateColor(ccColor3B firstColor, ccColor3B secondColor, bool hasGlow, ccColor3B glowColor, ModTypes::PlayerType type, bool sameDualColor = false) {
            auto gameLayer = gameManager->getGameLayer();
            try {
                if (gameLayer == nullptr) throw std::runtime_error("GJGameBaseLayer is not found!");
                if ((type == ModTypes::PlayerType::FIRST_PLAYER) || (type == ModTypes::PlayerType::BOTH_PLAYERS)) {
                    auto player = gameLayer->m_player1;
                    player->setColor(firstColor);
                    player->setSecondColor(secondColor);
                    player->m_glowColor = glowColor;
                    player->m_hasGlow = hasGlow;
                    player->updateGlowColor();
                    player->updatePlayerGlow();
                }
                if ((type == ModTypes::PlayerType::SECOND_PLAYER) || (type == ModTypes::PlayerType::BOTH_PLAYERS)) {
                    auto player = gameLayer->m_player2;
                    player->setColor((sameDualColor || type == ModTypes::PlayerType::SECOND_PLAYER) ? firstColor : secondColor);
                    player->setSecondColor((sameDualColor || type == ModTypes::PlayerType::SECOND_PLAYER) ? secondColor : firstColor);
                    player->m_glowColor = glowColor;
                    player->m_hasGlow = hasGlow;
                    player->updateGlowColor();
                    player->updatePlayerGlow();
                }
            }
            catch(const std::exception& e) {
                log::info("{}", e.what());
            }
        }

        static ModTypes::Gamemode getCurrentGamemode(bool isSecondPlayer = false) {
            auto gameLayer = gameManager->getGameLayer();
            try {
                if (gameLayer == nullptr) throw std::runtime_error("GJGameBaseLayer is not found!");
                auto player = isSecondPlayer ? gameLayer->m_player2 : gameLayer->m_player1;
                if (player->m_isShip) return gameLayer->m_levelSettings->m_platformerMode ? ModTypes::Gamemode::JETPACK : ModTypes::Gamemode::SHIP;
                else if (player->m_isBall) return ModTypes::Gamemode::BALL;
                else if (player->m_isBird) return ModTypes::Gamemode::UFO;
                else if (player->m_isDart) return ModTypes::Gamemode::WAVE;
                else if (player->m_isRobot) return ModTypes::Gamemode::ROBOT;
                else if (player->m_isSpider) return ModTypes::Gamemode::SPIDER;
                else if (player->m_isSwing) return ModTypes::Gamemode::SWING;
                else return ModTypes::Gamemode::CUBE;
            }
            catch(const std::exception& e) {
                log::info("{}", e.what());
                return ModTypes::Gamemode::CUBE;
            }
        }

        static ModTypes::Gamemode getCurrentGamemode(PlayerObject* player) {
            auto gameLayer = gameManager->getGameLayer();
            try {
                if (gameLayer == nullptr) throw std::runtime_error("GJGameBaseLayer is not found!");
                if (player->m_isShip) return gameLayer->m_levelSettings->m_platformerMode ? ModTypes::Gamemode::JETPACK : ModTypes::Gamemode::SHIP;
                else if (player->m_isBall) return ModTypes::Gamemode::BALL;
                else if (player->m_isBird) return ModTypes::Gamemode::UFO;
                else if (player->m_isDart) return ModTypes::Gamemode::WAVE;
                else if (player->m_isRobot) return ModTypes::Gamemode::ROBOT;
                else if (player->m_isSpider) return ModTypes::Gamemode::SPIDER;
                else if (player->m_isSwing) return ModTypes::Gamemode::SWING;
                else return ModTypes::Gamemode::CUBE;
            }
            catch(const std::exception& e) {
                log::info("{}", e.what());
                return ModTypes::Gamemode::CUBE;
            }
        }
    
        static void setShipColor(PlayerObject* player, ccColor3B color) {
            static_cast<CCSprite*>(player->getChildByID("main-layer")->getChildByID("ship-frame"))->setColor(color);
        }
        static void setShipSecondColor(PlayerObject* player, ccColor3B color) {
            static_cast<CCSprite*>(player->getChildByID("main-layer")->getChildByID("ship-frame")->getChildByID("secondary-frame"))->setColor(color);
        }
        static void setShipGlowColor(PlayerObject* player, ccColor3B color) {
            static_cast<CCSprite*>(player->getChildByID("main-layer")->getChildByID("glow-frame")->getChildByID("ship-glow"))->setColor(color);
        }
        static void setShipGlow(PlayerObject* player, bool isVisible) {
            static_cast<CCSprite*>(player->getChildByID("main-layer")->getChildByID("glow-frame")->getChildByID("ship-glow"))->setVisible(isVisible);
        };
        static void setShipColorFrame(PlayerObject* player, ccColor3B color = gameManager->colorForIdx(0), ccColor3B secondColor = gameManager->colorForIdx(3), ccColor3B glowColor = gameManager->colorForIdx(3), bool isHasGlow = false) {
            auto mainLayer = player->getChildByID("main-layer");
            auto shipFrame = static_cast<CCSprite*>(mainLayer->getChildByID("ship-frame"));
            auto shipGlow = static_cast<CCSprite*>(mainLayer->getChildByID("glow-frame")->getChildByID("ship-glow"));
            shipGlow->setVisible(isHasGlow);
            shipGlow->setColor(glowColor);
            shipFrame->setColor(color);
            static_cast<CCSprite*>(shipFrame->getChildByID("secondary-frame"))->setColor(secondColor);
        }
    };
};
#endif