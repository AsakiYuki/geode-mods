#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/CCDrawNode.hpp>
#include "../mod/AsakiYuki.hpp"
using namespace geode::prelude;
using namespace AsakiYuki;

class $modify(CCDrawNode) {
    bool drawPolygon(CCPoint *p0, unsigned int p1, const ccColor4F &p2, float p3, const ccColor4F &p4) {
        if (Player::getCurrentIngameLayer() != nullptr) {
            if (Mod::get()->getSavedValue<bool>("asakiyuki:solid-wave-trail", false)) {
                if (p2.r == 1.f && p2.g == 1.f && p2.b == 1.f && p2.a != 1.f) return true;
                this->setBlendFunc(CCSprite::create()->getBlendFunc());
                this->setZOrder(-1);
            }
        }
        return CCDrawNode::drawPolygon(p0, p1, p2, p3, p4);
    };
};

class $modify(PlayerObject) {
    void spawnPortalCircle(cocos2d::ccColor3B p0, float p1) {
        auto mod = Mod::get();
        if (!(mod->getSavedValue<bool>("asakiyuki:no-player-effect", false) && mod->getSavedValue<bool>("asakiyuki:no-player-effect:0", true)))
            PlayerObject::spawnPortalCircle(p0, p1);
    }
    void playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to) {
        auto mod = Mod::get();
        if (!(mod->getSavedValue<bool>("asakiyuki:no-player-effect", false) && mod->getSavedValue<bool>("asakiyuki:no-player-effect:1", true)))
            PlayerObject::playSpiderDashEffect(from, to);
    }
    void playBumpEffect(int p0, GameObject* p1) {
        auto mod = Mod::get();
        if (!(mod->getSavedValue<bool>("asakiyuki:no-player-effect", false) && mod->getSavedValue<bool>("asakiyuki:no-player-effect:5", true)))
            PlayerObject::playBumpEffect(p0, p1);
    }
};

class $modify(CCNode) {
    CCAction* runAction(CCAction* p0) {
        auto mod = Mod::get();
        if (mod->getSavedValue<bool>("asakiyuki:no-player-effect", false) && mod->getSavedValue<bool>("asakiyuki:no-player-effect:2", true)) 
            if (PlayLayer::get() && ((PlayLayer::get()->m_player1 == as<CCNode*>(this)) || (PlayLayer::get()->m_player2 == as<CCNode*>(this))) && p0->getTag() == 11) return p0;
        return CCNode::runAction(p0);
    }
};