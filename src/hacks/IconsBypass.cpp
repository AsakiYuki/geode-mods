#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/CharacterColorPage.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../mod/AsakiYuki.hpp"
using namespace geode::prelude;
using namespace AsakiYuki;

class $modify(GameManager) {
    bool isIconUnlocked(int id, IconType type) {
        if (Mod::get()->getSavedValue<bool>("asakiyuki:icon-bypass", false)) return true;
        return GameManager::isIconUnlocked(id, type);
    }

    bool isColorUnlocked(int id, UnlockType type) {
        if (Mod::get()->getSavedValue<bool>("asakiyuki:icon-bypass", false)) return true;
        return GameManager::isColorUnlocked(id, type);
    }
};

class $modify(GJGarageLayer) {
    void onSelect(CCObject* sender) {
        Save::hackIconsManager();
        GJGarageLayer::onSelect(sender); 
    }
    void keyBackClicked() {
        Save::hackIconsManager();
        GJGarageLayer::keyBackClicked();
    }
    void onBack(CCObject* sender) {
        Save::hackIconsManager();
        GJGarageLayer::onBack(sender);
    }
};

class $modify(CharacterColorPage) {
    void toggleGlow(CCObject* p0) {
        CharacterColorPage::toggleGlow(p0);
        Save::hackIconsManager();
    }
    void onPlayerColor(CCObject* sender) {
        CharacterColorPage::onPlayerColor(sender);
        Save::hackIconsManager();
    }
};

class $modify(MenuLayer) {
    bool init() {
        MenuLayer::init();
        Save::hackIconsManager(true);
        return true;
    }
};