#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../mod/AsakiYuki.hpp"
using namespace geode::prelude;
using namespace AsakiYuki;

class $modify(HUDPlayLayer, PlayLayer) {
    CCMenuItemToggler* practiceToggleBTN;
    void togglePracticeMode(bool p0) {
        PlayLayer::togglePracticeMode(p0);
        this->m_fields->practiceToggleBTN->toggle(p0);
    }
    void onHudParticleMode(CCObject*) {
        this->togglePracticeMode(!this->m_isPracticeMode);
    }
    void onResetLevel(CCObject*) {
        this->resetLevel();
    }
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        PlayLayer::init(level, useReplay, dontCreateObjects);
        auto pauseBtnMenu = static_cast<CCMenu*>(this->m_uiLayer->getChildByID("pause-button-menu"));
        auto pauseBtn = pauseBtnMenu->getChildByID("pause-button");

        auto resetLevel = Node::Sprite::createWithSpriteFrameName("AY_ResetLeveBtn.png"_spr);
        resetLevel->setOpacity(75);
        Node::MenuItemSpriteExtra::create(resetLevel, this, menu_selector(HUDPlayLayer::onResetLevel), pauseBtnMenu,
        { pauseBtn->getPositionX() - 27.f, 0.f });
        auto practiceOff = CCSprite::createWithSpriteFrameName("AY_PracticleBtn.png"_spr);
        practiceOff->setOpacity(75);
        auto practiceOn = CCSprite::createWithSpriteFrameName("AY_DisablePracticleBtn.png"_spr);
        practiceOn->setOpacity(75);
        this->m_fields->practiceToggleBTN = CCMenuItemToggler::create(practiceOff, practiceOn, this, menu_selector(HUDPlayLayer::onHudParticleMode));
        this->m_fields->practiceToggleBTN->setClickable(false);
        pauseBtnMenu->addChild(this->m_fields->practiceToggleBTN);
        this->m_fields->practiceToggleBTN->setPositionX(pauseBtn->getPositionX() - 54.f);
        return true;
    }
};