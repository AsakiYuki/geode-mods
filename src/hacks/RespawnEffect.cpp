#include <Geode/Geode.hpp>
#include <Geode/modify/CCNode.hpp>
#include "../mod/AsakiYuki.hpp"
using namespace geode::prelude;
using namespace AsakiYuki;

class $modify(CCNode) {
    CCAction* runAction(CCAction* p0) {
        auto mod = Mod::get();
        if (mod->getSavedValue<bool>("asakiyuki:no-respawn-blink", false)) 
            if (PlayLayer::get() && ((PlayLayer::get()->m_player1 == as<CCNode*>(this)) || (PlayLayer::get()->m_player2 == as<CCNode*>(this))) && p0->getTag() == 11) return p0;
        return CCNode::runAction(p0);
    }
};