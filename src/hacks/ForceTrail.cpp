#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>
using namespace geode::prelude;

class $modify(CCMotionStreak) {
    void update(float dt) {
        auto mod = Mod::get();
        if (mod->getSavedValue<bool>("asakiyuki:force_trail", false) && (PlayLayer::get() != nullptr)) {
            switch (mod->getSavedValue<int>("asakiyuki:force_trail_type", 0)) {
            case 0:
                this->m_bStroke = false;
                break;
            case 1:
                this->m_bStroke = true;
                break;
            case 2:
                {
                    auto p1 = PlayLayer::get()->m_player1;
                    auto p2 = PlayLayer::get()->m_player2;
                    if (this == p1->m_regularTrail) this->m_bStroke = p1->isVisible() && (p1->getOpacity() == 255);
                    else if (this == p2->m_regularTrail) this->m_bStroke = p2->isVisible() && (p2->getOpacity() == 255);
                }
                break;
            }
        }
        CCMotionStreak::update(dt);
    }
};

class $modify(PlayerObject) {
    void update(float p0) {
        PlayerObject::update(p0);
        this->m_regularTrail->setOpacity(0);
    }
};