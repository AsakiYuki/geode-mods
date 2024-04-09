#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
using namespace geode::prelude;

class $modify(PlayerObject) {
    void flashPlayer(float p0, float p1, cocos2d::ccColor3B mainColor, cocos2d::ccColor3B secondColor) {
        auto mod = Mod::get();
        if (!(mod->getSavedValue<bool>("asakiyuki:no-player-effect", false) && mod->getSavedValue<bool>("asakiyuki:no-player-effect:3", true)))
            return PlayerObject::flashPlayer(p0, p1, mainColor, secondColor);
    }
    bool init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4) {
        PlayerObject::init(p0, p1, p2, p3, p4);
        auto mod = Mod::get();
        if (mod->getSavedValue<bool>("asakiyuki:no-player-fire", false)) {
            if (mod->getSavedValue<bool>("asakiyuki:no-player-fire:0", true))
                this->m_robotFire->setOpacity(0);
            if (mod->getSavedValue<bool>("asakiyuki:no-player-fire:1", true)) {
                this->m_swingFireTop->setOpacity(0);
                this->m_swingFireMiddle->setOpacity(0);
                this->m_swingFireBottom->setOpacity(0);
            }
        }
        return true;
    };
};