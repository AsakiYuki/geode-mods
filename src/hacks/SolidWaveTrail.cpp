#include <Geode/Geode.hpp>
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