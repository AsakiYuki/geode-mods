#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "./mod/AsakiYuki.hpp"
using namespace geode::prelude;
using namespace AsakiYuki;

class $modify(CCScheduler) {
    void update(float delta) {
        CCScheduler::update(delta);
        Color::update(delta);
    };
};