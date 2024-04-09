#include <Geode/Geode.hpp>
#include <Geode/modify/OptionsLayer.hpp>
#include "../mod/AsakiYuki.hpp"
using namespace geode::prelude;
using namespace AsakiYuki;

class ModSettingPopup : public Popup<> {
    public:
    CCMenu* cagetory_layer;
    std::vector<CCMenu*> mods_menu_layers;
    std::vector<CCMenuItemToggler*> cagetory_togglers;
    int currentCagetory = 0;
    std::vector<const char*> mod_name;
    std::vector<const char*> mod_decs;
    static ModSettingPopup* create() {
        auto ret = new ModSettingPopup();
        if (ret && ret->initAnchored(400.f, 250.f, "GJ_square05.png")) {
            ret->autorelease();
            return ret;
        };
        return nullptr;
    }
    void onChangeCagetory(CCObject* sender) {
        this->mods_menu_layers[this->currentCagetory]->setVisible(false);
        this->cagetory_togglers[this->currentCagetory]->toggle(false);
        this->mods_menu_layers[sender->getTag()]->setVisible(true);
        static_cast<CCMenuItemToggler*>(sender)->toggle(true);
        this->currentCagetory = sender->getTag();
    }
    void createCagetory(const char* cagetory_name) {
        auto cagetory_label_name = Node::LabelBMFont::create(cagetory_name, "bigFont.fnt", 0.5f, { 30.f, 12.5f });
        auto cagetory_toggle_disabled = extension::CCScale9Sprite::create("GJ_button_02.png");
        auto cagetory_toggle_enabled = extension::CCScale9Sprite::create("GJ_button_01.png");
        auto toggle = CCMenuItemToggler::create(cagetory_toggle_disabled, cagetory_toggle_enabled, this, menu_selector(ModSettingPopup::onChangeCagetory));
        cagetory_toggle_disabled->setContentSize({ 60.f, 25.f });
        cagetory_toggle_enabled->setContentSize({ 60.f, 25.f });
        cagetory_toggle_disabled->addChild(cagetory_label_name);
        cagetory_toggle_enabled->addChild(cagetory_label_name);
        toggle->setClickable(false);
        toggle->toggle(this->cagetory_togglers.size() == this->currentCagetory);
        toggle->setTag(this->cagetory_togglers.size());
        toggle->setPositionY(this->cagetory_togglers.size() * -30.f);
        this->mods_menu_layers.push_back(Node::Menu::create(
            { this->m_mainLayer->getContentWidth() - 100.f, this->m_mainLayer->getContentHeight() - 40.f },
            { this->m_mainLayer->getContentWidth() / 2 + 40.f, this->m_mainLayer->getContentHeight() / 2 - 10.f },
            this->m_mainLayer
        ));
        this->mods_menu_layers[this->cagetory_togglers.size()]->setVisible(this->cagetory_togglers.size() == this->currentCagetory);
        this->cagetory_togglers.push_back(toggle);
        this->cagetory_layer->addChild(toggle);
    }
    void onDescription(CCObject* sender) {
        Popup::create(this->mod_name[sender->getTag()], this->mod_decs[sender->getTag()], "Ok")->show();
    }
    void createModButton(int cagetoryID, const char* mod_name, const char* mod_description, std::string modID) {
        auto mod_label_name = Node::LabelBMFont::create(mod_name, "bigFont.fnt", 0.5f, { 62.5f, 15.f });
        auto off = extension::CCScale9Sprite::create("GJ_button_05.png");
        auto on = extension::CCScale9Sprite::create("GJ_button_01.png");
        off->addChild(mod_label_name);
        on->addChild(mod_label_name);
        off->setContentSize({ 125.f, 30.f });
        on->setContentSize({ 125.f, 30.f });
        auto button = CCMenuItemToggler::create(off, on, this, menu_selector(ModSettingPopup::onDescription));
        button->setID(modID);
        button->setTag(this->mod_name.size());
        this->mod_name.push_back(mod_name);
        this->mod_decs.push_back(mod_description);
        this->mods_menu_layers[cagetoryID]->addChild(button);
    }
    void createModButtonWithExtraMod() {

    }
    protected:
    bool setup() {
        this->setTitle("Mods", "goldFont.fnt", 0.7f, 15.f);
        auto bg = extension::CCScale9Sprite::create("square02_small.png");
        bg->setContentSize({ this->m_mainLayer->getContentWidth() - 100.f, this->m_mainLayer->getContentHeight() - 40.f });
        bg->setPosition({ this->m_mainLayer->getContentWidth() / 2 + 40.f, this->m_mainLayer->getContentHeight() / 2 - 10.f });
        bg->setOpacity(125);
        this->m_mainLayer->addChild(bg);
        auto cagetoryBG = extension::CCScale9Sprite::create("square02_small.png");
        cagetoryBG->setContentSize({ 75.f, this->m_mainLayer->getContentHeight() - 40.f });
        cagetoryBG->setPosition({ this->m_mainLayer->getContentWidth() / 2 - 153.f, this->m_mainLayer->getContentHeight() / 2 - 10.f });
        cagetoryBG->setOpacity(125);
        this->m_mainLayer->addChild(cagetoryBG);
        this->cagetory_layer = Node::Menu::create(cagetoryBG->getPosition(), cagetoryBG->getContentSize(), this->m_mainLayer);
        this->createCagetory("HUD");
        this->createCagetory("idk");
        this->createCagetory("idk");
        this->createCagetory("idk");
        this->createCagetory("idk");

        this->createModButton(0, "Idk", "Idk", "asakiyuki:idk");
        this->createModButton(1, "What", "What???", "asakiyuki:what???");
        return true;
    };
};

class $modify(AsaOptionsLayer, OptionsLayer) {
    public:
    void onOpenModSetting(CCObject*) {
        ModSettingPopup::create()->show();
    }
    virtual void customSetup() {
        OptionsLayer::customSetup();
        auto options_menu = static_cast<CCMenu*>(this->m_mainLayer->getChildByID("options-menu"));
        auto oms = options_menu->getContentSize();
        Node::MenuItemSpriteExtra::create(Node::Sprite::createWithSpriteFrameName("accountBtn_settings_001.png", 0.75f), this, menu_selector(AsaOptionsLayer::onOpenModSetting), options_menu, { 16.5f, -25.f });
    } 
};