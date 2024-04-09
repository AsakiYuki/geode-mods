#ifndef GD_SAVE
#define GD_SAVE
#include <Geode/Geode.hpp>
#include "./enum.hpp"
using namespace geode::prelude;
    namespace AsakiYuki {
        class Save {
            protected:
            static inline GameManager* gameManager = GameManager::sharedState();
            static inline GameStatsManager* gameStatsManager = GameStatsManager::sharedState();
            public:
            static void savePlayerFrame(int frameID, ModTypes::Gamemode type = ModTypes::Gamemode::CUBE, bool secondPlayer = false) {
                auto mod = Mod::get();
                GJBaseGameLayer* ingameLayer = (PlayLayer::get() != nullptr) ? PlayLayer::get() : static_cast<GJBaseGameLayer*>(LevelEditorLayer::get());
                bool dualMode = mod->getSavedValue<bool>("asakiyuki:custom_dual_mod", false);
                int dualType = mod->getSavedValue<int>("asakiyuki:dual_types", 0);
                if ((secondPlayer && (ingameLayer == nullptr)) || (secondPlayer && dualMode && (dualType == 2))) {
                    Mod::get()->setSavedValue<int>( CCString::createWithFormat("secondPlayerSave_%i", static_cast<int>(type))->m_sString, frameID );
                } else {
                    switch (type) {
                        case ModTypes::Gamemode::CUBE:
                            gameManager->setPlayerFrame(frameID); break;
                        case ModTypes::Gamemode::SHIP:
                            gameManager->setPlayerShip(frameID); break;
                        case ModTypes::Gamemode::BALL:
                            gameManager->setPlayerBall(frameID); break;
                        case ModTypes::Gamemode::UFO:
                            gameManager->setPlayerBird(frameID); break;
                        case ModTypes::Gamemode::WAVE:
                            gameManager->setPlayerDart(frameID); break;
                        case ModTypes::Gamemode::ROBOT:
                            gameManager->setPlayerRobot(frameID); break;
                        case ModTypes::Gamemode::SPIDER:
                            gameManager->setPlayerSpider(frameID); break;
                        case ModTypes::Gamemode::SWING:
                            gameManager->setPlayerSwing(frameID); break;
                        case ModTypes::Gamemode::JETPACK:
                            gameManager->setPlayerJetpack(frameID); break;
                        case ModTypes::Gamemode::DEATH_EFFECT:
                            gameManager->setPlayerDeathEffect(frameID); break;
                        case ModTypes::Gamemode::STREAK:
                            gameManager->setPlayerStreak(frameID); break;
                        case ModTypes::Gamemode::SHIP_STREAK:
                            gameManager->setPlayerShipStreak(frameID); break;
                        case ModTypes::Gamemode::COLOR_1:
                            gameManager->setPlayerColor(frameID); break;
                        case ModTypes::Gamemode::COLOR_2:
                            gameManager->setPlayerColor2(frameID); break;
                        case ModTypes::Gamemode::COLOR_GLOW:
                            gameManager->setPlayerColor3(frameID); break;
                        default:
                            return;
                    };
                }
            }
            static void savePlayerFrame(bool itemEnabled, ModTypes::IconItemType type = ModTypes::IconItemType::GLOW, bool secondPlayer = false) {
                if (secondPlayer) {
                    Mod::get()->setSavedValue<bool>(CCString::createWithFormat("secondPlayerItemSave_%i", static_cast<int>(type))->m_sString, itemEnabled);
                } else {
                    switch (type) {
                        case (ModTypes::IconItemType::GLOW):
                            gameManager->setPlayerGlow(itemEnabled); break;
                        case (ModTypes::IconItemType::EXPLORE_EFFECT):
                            gameManager->setGameVariable("0153", itemEnabled); break;
                        case (ModTypes::IconItemType::ROBOT_ANIMATE):
                            gameStatsManager->toggleEnableItem(UnlockType::GJItem, 18, itemEnabled); break;
                        case (ModTypes::IconItemType::ROBOT_ANIMATE_2):
                            gameStatsManager->toggleEnableItem(UnlockType::GJItem, 19, itemEnabled); break;
                        case (ModTypes::IconItemType::SPIDER_ANIMATE):
                            gameStatsManager->toggleEnableItem(UnlockType::GJItem, 20, itemEnabled); break;
                        default:
                            return;
                    }
                }
            }
            static int getPlayerFrame(ModTypes::Gamemode type = ModTypes::Gamemode::CUBE, bool secondPlayer = false) {
                auto mod = Mod::get();
                GJBaseGameLayer* ingameLayer = (PlayLayer::get() != nullptr) ? PlayLayer::get() : static_cast<GJBaseGameLayer*>(LevelEditorLayer::get());
                bool dualMode = mod->getSavedValue<bool>("asakiyuki:custom_dual_mod", false);
                int dualType = mod->getSavedValue<int>("asakiyuki:dual_types", 0);
                if ((secondPlayer && (ingameLayer == nullptr)) || (secondPlayer && dualMode && (dualType == 2))) {
                    return Mod::get()->getSavedValue<int>(CCString::createWithFormat("secondPlayerSave_%i", static_cast<int>(type))->m_sString, 1);
                } else {
                    switch (type) {
                        case ModTypes::Gamemode::CUBE:
                            return gameManager->getPlayerFrame(); break;
                        case ModTypes::Gamemode::SHIP:
                            return gameManager->getPlayerShip(); break;
                        case ModTypes::Gamemode::BALL:
                            return gameManager->getPlayerBall(); break;
                        case ModTypes::Gamemode::UFO:
                            return gameManager->getPlayerBird(); break;
                        case ModTypes::Gamemode::WAVE:
                            return gameManager->getPlayerDart(); break;
                        case ModTypes::Gamemode::ROBOT:
                            return gameManager->getPlayerRobot(); break;
                        case ModTypes::Gamemode::SPIDER:
                            return gameManager->getPlayerSpider(); break;
                        case ModTypes::Gamemode::SWING:
                            return gameManager->getPlayerSwing(); break;
                        case ModTypes::Gamemode::JETPACK:
                            return gameManager->getPlayerJetpack(); break;
                        case ModTypes::Gamemode::DEATH_EFFECT:
                            return gameManager->getPlayerDeathEffect(); break;
                        case ModTypes::Gamemode::STREAK:
                            return gameManager->getPlayerStreak(); break;
                        case ModTypes::Gamemode::SHIP_STREAK:
                            return gameManager->getPlayerShipFire(); break; 
                        case ModTypes::Gamemode::COLOR_1:
                            return (gameManager->getPlayerColor() == 0) ? 1 : gameManager->getPlayerColor(); break;
                        case ModTypes::Gamemode::COLOR_2:
                            return (gameManager->getPlayerColor2() == 0) ? 4 : gameManager->getPlayerColor2(); break;
                        case ModTypes::Gamemode::COLOR_GLOW:
                            return (gameManager->getPlayerGlowColor() == 0) ? 1 : gameManager->getPlayerGlowColor(); break;
                        default:
                            return 0;
                    };
                }
                return 0;
            }
            static bool getPlayerFrame(ModTypes::IconItemType type = ModTypes::IconItemType::GLOW, bool secondPlayer = false) {
                auto mod = Mod::get();
                GJBaseGameLayer* ingameLayer = (PlayLayer::get() != nullptr) ? PlayLayer::get() : static_cast<GJBaseGameLayer*>(LevelEditorLayer::get());
                bool dualMode = mod->getSavedValue<bool>("asakiyuki:custom_dual_mod", false);
                int dualType = mod->getSavedValue<int>("asakiyuki:dual_types", 0);
                if ((secondPlayer && (ingameLayer == nullptr)) || (secondPlayer && dualMode && (dualType == 2))) {
                    return Mod::get()->getSavedValue<bool>(CCString::createWithFormat("secondPlayerItemSave_%i", static_cast<int>(type))->m_sString, false );
                } else {
                    switch (type) {
                        case ModTypes::IconItemType::GLOW:
                            return gameManager->getPlayerGlow(); break;
                        case ModTypes::IconItemType::EXPLORE_EFFECT:
                            return gameManager->getGameVariableDefault("0153", false); break;
                        case ModTypes::IconItemType::ROBOT_ANIMATE:
                            return gameStatsManager->isItemEnabled(UnlockType::GJItem, 18); break;
                        case ModTypes::IconItemType::ROBOT_ANIMATE_2:
                            return gameStatsManager->isItemEnabled(UnlockType::GJItem, 19); break;
                        case ModTypes::IconItemType::SPIDER_ANIMATE:
                            return gameStatsManager->isItemEnabled(UnlockType::GJItem, 20); break;
                        default:
                            return false;
                    };
                }
                return false;
            }
            static void hackIconsManager(bool isLoaded = false) {
                auto mod = Mod::get();
                if (mod->getSavedValue<bool>("asakiyuki:icon-bypass")) {
                    ModTypes::Gamemode gamemodeList[15] = {
                        ModTypes::Gamemode::CUBE,
                        ModTypes::Gamemode::SHIP,
                        ModTypes::Gamemode::BALL,
                        ModTypes::Gamemode::UFO,
                        ModTypes::Gamemode::WAVE,
                        ModTypes::Gamemode::ROBOT,
                        ModTypes::Gamemode::SPIDER,
                        ModTypes::Gamemode::SWING,
                        ModTypes::Gamemode::JETPACK,
                        ModTypes::Gamemode::DEATH_EFFECT,
                        ModTypes::Gamemode::STREAK,
                        ModTypes::Gamemode::SHIP_STREAK,
                        ModTypes::Gamemode::COLOR_1,
                        ModTypes::Gamemode::COLOR_2,
                        ModTypes::Gamemode::COLOR_GLOW
                    };
                    ModTypes::IconItemType itemList[5] = {
                        ModTypes::IconItemType::GLOW,
                        ModTypes::IconItemType::EXPLORE_EFFECT,
                        ModTypes::IconItemType::ROBOT_ANIMATE,
                        ModTypes::IconItemType::ROBOT_ANIMATE_2,
                        ModTypes::IconItemType::SPIDER_ANIMATE
                    };
                    auto mod = Mod::get();
                    if (isLoaded) {
                        for (ModTypes::Gamemode type : gamemodeList)
                            Save::savePlayerFrame(mod->getSavedValue<int>(CCString::createWithFormat("icon-bypass-%i", static_cast<int>(type))->m_sString, 1), type);
                        for (ModTypes::IconItemType type : itemList)
                            Save::savePlayerFrame(mod->getSavedValue<bool>(CCString::createWithFormat("item-bypass-%i", static_cast<int>(type))->m_sString, false), type);
                    } else {
                        for (ModTypes::Gamemode type : gamemodeList)
                            mod->setSavedValue<int>(CCString::createWithFormat("icon-bypass-%i", static_cast<int>(type))->m_sString, Save::getPlayerFrame(type));
                        for (ModTypes::IconItemType type : itemList)
                            mod->setSavedValue<bool>(CCString::createWithFormat("item-bypass-%i", static_cast<int>(type))->m_sString, Save::getPlayerFrame(type));
                    }
                }
            }
        
            static void saveCustomPlayer(int itemID, ModTypes::Custom type, bool secondPlayer = false) {
                Mod::get()->setSavedValue<int>(
                    CCString::createWithFormat("asakiyuki:custom_%i:%i", 
                        static_cast<int>(type),
                        secondPlayer + 1
                    )->m_sString, itemID
                );
            }
            static int getCustomPlayer(ModTypes::Custom type, bool secondPlayer = false) {
                auto mod = Mod::get();
                GJBaseGameLayer* ingameLayer = (PlayLayer::get() != nullptr) ? PlayLayer::get() : static_cast<GJBaseGameLayer*>(LevelEditorLayer::get());
                bool dualMode = mod->getSavedValue<bool>("asakiyuki:custom_dual_mod", false);
                int dualType = mod->getSavedValue<int>("asakiyuki:dual_types", 0);     

                return Mod::get()->getSavedValue(
                    CCString::createWithFormat("asakiyuki:custom_%i:%i", 
                        static_cast<int>(type),
                        ((secondPlayer && (ingameLayer == nullptr)) || (secondPlayer && dualMode && (dualType == 2))) + 1
                    )->m_sString, 1
                );
            }

            static void saveCustomPlayer(bool boolean, ModTypes::CustomBool type, bool secondPlayer) {
                Mod::get()->setSavedValue<bool>(
                    CCString::createWithFormat("asakiyuki:custom_bool_%i:%i", 
                        static_cast<int>(type),
                        secondPlayer + 1
                        )->m_sString, boolean
                );
            }
            static bool getCustomPlayer(ModTypes::CustomBool type, bool secondPlayer) {
                auto mod = Mod::get();
                GJBaseGameLayer* ingameLayer = (PlayLayer::get() != nullptr) ? PlayLayer::get() : static_cast<GJBaseGameLayer*>(LevelEditorLayer::get());
                bool dualMode = mod->getSavedValue<bool>("asakiyuki:custom_dual_mod", false);
                int dualType = mod->getSavedValue<int>("asakiyuki:dual_types", 0);     

                return Mod::get()->getSavedValue(
                    CCString::createWithFormat("asakiyuki:custom_bool_%i:%i", 
                        static_cast<int>(type),
                        ((secondPlayer && (ingameLayer == nullptr)) || (secondPlayer && dualMode && (dualType == 2))) + 1
                    )->m_sString, false
                );
            }
        };
    }
#endif