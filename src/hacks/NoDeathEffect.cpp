#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../mod/AsakiYuki.hpp"
using namespace geode::prelude;
using namespace AsakiYuki;

class $modify(PlayerObject) {
    void playDeathEffect() {
        auto mod = Mod::get();
        if (!(
            (mod->getSavedValue<bool>("asakiyuki:no-player-effect", false) && mod->getSavedValue<bool>("asakiyuki:no-player-effect:4", true))) ||
            Save::getPlayerFrame(ModTypes::Gamemode::COLOR_1) ||
            Save::getPlayerFrame(ModTypes::Gamemode::COLOR_1, true) ||
            Save::getPlayerFrame(ModTypes::Gamemode::COLOR_2) ||
            Save::getPlayerFrame(ModTypes::Gamemode::COLOR_2, true) ||
            Save::getPlayerFrame(ModTypes::Gamemode::COLOR_GLOW) ||
            Save::getPlayerFrame(ModTypes::Gamemode::COLOR_GLOW, true)
        ) return PlayerObject::playDeathEffect();
    }
};