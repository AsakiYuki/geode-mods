#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
using namespace geode::prelude;

class $modify(PlayerObject) {
    void addAllParticles() {
        PlayerObject::addAllParticles();
        if (Mod::get()->getSavedValue<bool>("asakiyuki:no-player-particle", false)) {
            auto mod = Mod::get();
            if (mod->getSavedValue<bool>("asakiyuki:no-player-particle:0", true))
                this->m_trailingParticles->setVisible(false);
            if (mod->getSavedValue<bool>("asakiyuki:no-player-particle:1", true))
                this->m_shipClickParticles->setVisible(false);
            if (mod->getSavedValue<bool>("asakiyuki:no-player-particle:2", true))
                this->m_ufoClickParticles->setVisible(false);
            if (mod->getSavedValue<bool>("asakiyuki:no-player-particle:3", true))
                this->m_robotBurstParticles->setVisible(false);
            if (mod->getSavedValue<bool>("asakiyuki:no-player-particle:4", true))
                this->m_swingBurstParticles1->setVisible(false);
            if (mod->getSavedValue<bool>("asakiyuki:no-player-particle:5", true))
                this->m_swingBurstParticles2->setVisible(false);
            if (mod->getSavedValue<bool>("asakiyuki:no-player-particle:6", true)) {
                this->m_unk6dc->setVisible(false);
                this->m_unk6e8->setVisible(false);
                this->m_unk6f4->setVisible(false);
                this->m_unk704->setVisible(false);
                this->m_unk708->setVisible(false);
            }
        }
    }
};