#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include "../mod/AsakiYuki.hpp"
using namespace geode::prelude;
using namespace AsakiYuki;

class SelectOptionMode : public Popup<> {
    protected:
    bool setup() {
        this->setTitle(this->option_title, "bigFont.fnt", 0.6f);
        this->m_mainLayer->addChild(Node::LabelBMFont::create(this->option_decscription, "goldFont.fnt", 0.5f,
            { this->m_title->getPositionX(), this->m_title->getPositionY() - 15.f }));

        auto bg = extension::CCScale9Sprite::create("square02_small.png");
        bg->setContentSize({this->m_mainLayer->getContentSize().width - 20.f, this->option_names.size() * 30.f + 10.f});
        bg->setPosition(this->m_mainLayer->getContentSize().width / 2, this->m_mainLayer->getContentSize().height / 2 - 20.f);
        bg->setOpacity(125);

        auto layer = CCNode::create();
        layer->setContentWidth(bg->getContentWidth() - 10.f);
        layer->setContentHeight(bg->getContentHeight());
        layer->setPositionX(5.f);
        bg->addChild(layer);
        
        auto mod = Mod::get();
        for (int i = this->option_names.size() - 1; i >= 0; i--) {
            auto options = CCMenu::create();
            options->setContentSize({layer->getContentWidth(), 30.f});

            auto name = Node::LabelBMFont::create(this->option_names[i], "bigFont.fnt", 0.5f, { 5.f, 15.f }, { 0.f, 0.5f });
            auto toggle = CCMenuItemToggler::create(
                Node::Sprite::createWithSpriteFrameName("GJ_checkOff_001.png", 0.7f),
                Node::Sprite::createWithSpriteFrameName("GJ_checkOn_001.png", 0.7f),
                this, menu_selector(SelectOptionMode::setupMod)
            );
            toggle->setPosition({options->getContentWidth() - 20.f, 15.f});
            toggle->setTag(i);
            toggle->toggle(
                mod->getSavedValue<bool>(CCString::createWithFormat("%s:%i", SelectOptionMode::option_id, i)->m_sString, true)
            );
            auto info = Node::MenuItemSpriteExtra::create(Node::Sprite::createWithSpriteFrameName("GJ_infoIcon_001.png", 0.7f), this, menu_selector(SelectOptionMode::openInfo));
            info->setTag(i);
            info->setPosition({name->getContentWidth() / 2 + 20.f, 15.f});

            options->addChild(name);
            options->addChild(toggle);
            options->addChild(info);
            layer->addChild(options);
        }

        layer->setLayout(RowLayout::create()->setGap(0.f)->setAxis(Axis::Column));
        this->m_mainLayer->addChild(bg);
        return true;
    }

    public:
    static inline const char* option_id;
    static inline const char* option_title;
    static inline const char* option_decscription;
    static inline std::vector<const char*> option_names;
    static inline std::vector<const char*> option_descs;

    void openInfo(CCObject* sender) {
        Popup::create(SelectOptionMode::option_names[sender->getTag()], SelectOptionMode::option_descs[sender->getTag()], "OK")->show();
    }
    void setupMod(CCObject* sender) {
        auto m = Mod::get();
        m->setSavedValue<bool>(CCString::createWithFormat("%s:%i", SelectOptionMode::option_id, sender->getTag())->m_sString, !static_cast<CCMenuItemToggler*>(sender)->isToggled());
    }

    static SelectOptionMode* create() {
        auto ret = new SelectOptionMode();
        if (ret && ret->initAnchored(300, (SelectOptionMode::option_names.size() * 30 + 70.f), "GJ_square05.png")) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    };
};

class ChooseOptionMode : public Popup<> {
    protected:
    bool setup() {
        this->setTitle(this->option_title, "bigFont.fnt", 0.6f);
        this->m_mainLayer->addChild(Node::LabelBMFont::create(this->option_decscription, "goldFont.fnt", 0.5f,
            { this->m_title->getPositionX(), this->m_title->getPositionY() - 15.f }));

        auto bg = extension::CCScale9Sprite::create("square02_small.png");
        bg->setContentSize({this->m_mainLayer->getContentSize().width - 20.f, this->option_names.size() * 30.f + 10.f});
        bg->setPosition(this->m_mainLayer->getContentSize().width / 2, this->m_mainLayer->getContentSize().height / 2 - 20.f);
        bg->setOpacity(125);

        auto layer = CCNode::create();
        layer->setContentWidth(bg->getContentWidth() - 10.f);
        layer->setContentHeight(bg->getContentHeight());
        layer->setPositionX(5.f);
        bg->addChild(layer);

        int indexActive = Mod::get()->getSavedValue<int>(this->option_id, 0);
        for (int i = this->option_names.size() - 1; i >= 0; i--) {
            auto options = CCMenu::create();
            options->setContentSize({layer->getContentWidth(), 30.f});

            auto name = Node::LabelBMFont::create(this->option_names[i], "bigFont.fnt", 0.5f, { 5.f, 15.f }, { 0.f, 0.5f });
            auto toggle = CCMenuItemToggler::create(
                Node::Sprite::createWithSpriteFrameName("GJ_checkOff_001.png", 0.7f),
                Node::Sprite::createWithSpriteFrameName("GJ_checkOn_001.png", 0.7f),
                this, menu_selector(ChooseOptionMode::setupMod)
            );
            toggle->setPosition({options->getContentWidth() - 20.f, 15.f});
            toggle->setTag(i);
            toggle->setClickable(false);
            toggle->toggle(i == indexActive);
            auto info = Node::MenuItemSpriteExtra::create(Node::Sprite::createWithSpriteFrameName("GJ_infoIcon_001.png", 0.7f), this, menu_selector(ChooseOptionMode::openInfo));
            info->setTag(i);
            info->setPosition({name->getContentWidth() / 2 + 20.f, 15.f});

            options->addChild(name);
            options->addChild(toggle);
            options->addChild(info);
            layer->addChild(options);
            this->option_toggles.push_back(toggle);
        }

        layer->setLayout(RowLayout::create()->setGap(0.f)->setAxis(Axis::Column));
        this->m_mainLayer->addChild(bg);
        return true;
    }

    public:
    static inline const char* option_id;
    static inline const char* option_title;
    static inline const char* option_decscription;
    static inline std::vector<const char*> option_names;
    static inline std::vector<const char*> option_descs;
    std::vector<CCMenuItemToggler*> option_toggles;

    void openInfo(CCObject* sender) {
        Popup::create(ChooseOptionMode::option_names[sender->getTag()], ChooseOptionMode::option_descs[sender->getTag()], "OK")->show();
    }
    void setupMod(CCObject* sender) {
        auto m = Mod::get();
        (this->option_toggles[ChooseOptionMode::option_toggles.size() - 1 - m->getSavedValue<int>(ChooseOptionMode::option_id, 0)])->toggle(false);
        m->setSavedValue(ChooseOptionMode::option_id, sender->getTag());
        static_cast<CCMenuItemToggler*>(sender)->toggle(true);
    }

    static ChooseOptionMode* create() {
        auto ret = new ChooseOptionMode();
        if (ret && ret->initAnchored(300, (ChooseOptionMode::option_names.size() * 30 + 70.f), "GJ_square05.png")) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    };
};

class IconOptionsPopup : public Popup<> {
    public:
    std::vector<const char*> decs_title;
    std::vector<const char*> decs_decscription;

    std::vector<const char*> option_modID;
    std::vector<const char*> option_modTitle;
    std::vector<const char*> option_decscription;
    std::vector<std::vector<const char*>> option_modDescs;
    std::vector<std::vector<const char*>> option_mod_names;

    void openDecscription(CCObject* sender) {
        Popup::create(this->decs_title[sender->getTag()], this->decs_decscription[sender->getTag()], "OK")->show();
    }
    void toggleMod(CCObject* sender) {
        auto toggle = static_cast<CCMenuItemToggler*>(sender);
        Mod::get()->setSavedValue(std::string_view(toggle->getID()), !toggle->isToggled());
    }
    void openOptionPopup(CCObject* sender) {
        ChooseOptionMode::option_id = this->option_modID[sender->getTag()];
        ChooseOptionMode::option_title = this->option_modTitle[sender->getTag()];
        ChooseOptionMode::option_decscription = this->option_decscription[sender->getTag()];
        ChooseOptionMode::option_names = this->option_mod_names[sender->getTag()];
        ChooseOptionMode::option_descs = this->option_modDescs[sender->getTag()];
        
        ChooseOptionMode::create()->show();
    }
    void openSelectOptionPopup(CCObject* sender) {
        SelectOptionMode::option_id = this->option_modID[sender->getTag()];
        SelectOptionMode::option_title = this->option_modTitle[sender->getTag()];
        SelectOptionMode::option_decscription = this->option_decscription[sender->getTag()];
        SelectOptionMode::option_names = this->option_mod_names[sender->getTag()];
        SelectOptionMode::option_descs = this->option_modDescs[sender->getTag()];
        
        SelectOptionMode::create()->show();
    }

    protected:
    CCMenuItemToggler* createButton(const char* str, const std::string_view type, float textScale = 0.5f) {
        auto tggOff = extension::CCScale9Sprite::create("GJ_button_05.png");
        auto tggOn = extension::CCScale9Sprite::create("GJ_button_01.png");
        tggOff->setContentSize({125, 30});
        tggOn->setContentSize({125, 30});
        auto tggText = Node::LabelBMFont::create(str, "bigFont.fnt", 1.f, { 62.5f, 16.f });
        float tWidth = tggText->getContentWidth() / 2;
        float tggWidth = tggOff->getContentWidth() - 10.f;
        tggText->setScale(textScale);
        tggOff->addChild(tggText);
        tggOn->addChild(tggText);
        auto toggle = CCMenuItemToggler::create(tggOff, tggOn, this, menu_selector(IconOptionsPopup::toggleMod));
        toggle->toggle(Mod::get()->getSavedValue<bool>(type, false));
        toggle->setID(std::string(type));
        return toggle;
    };

    CCMenuItemSpriteExtra* createInfoButton(const char* title, const char* decs) {
        auto btn = Node::MenuItemSpriteExtra::create(Node::Sprite::createWithSpriteFrameName("GJ_infoIcon_001.png", 0.86f), this, menu_selector(IconOptionsPopup::openDecscription));
        btn->setTag(this->decs_title.size());
        this->decs_title.push_back(title);
        this->decs_decscription.push_back(decs);
        return btn;
    };

    CCMenuItemSpriteExtra* createOptionsButton(const char* title, const char* decs, const char* modID,
        std::vector<const char*> mod_names, std::vector<const char*> mod_descs) {
        auto btn = Node::MenuItemSpriteExtra::create(Node::Sprite::createWithSpriteFrameName("GJ_plus3Btn_001.png", 1.f), this, menu_selector(IconOptionsPopup::openOptionPopup));
        btn->setTag(this->option_modID.size());
        this->option_modID.push_back(modID);
        this->option_decscription.push_back(decs);
        this->option_modTitle.push_back(title);
        this->option_mod_names.push_back(mod_names);
        this->option_modDescs.push_back(mod_descs);
        return btn;
    };

    CCMenuItemSpriteExtra* createSelectOptionsButton(const char* title, const char* decs, const char* modID,
        std::vector<const char*> mod_names, std::vector<const char*> mod_descs) {
        auto btn = Node::MenuItemSpriteExtra::create(Node::Sprite::createWithSpriteFrameName("GJ_plus3Btn_001.png", 1.f), this, menu_selector(IconOptionsPopup::openSelectOptionPopup));
        btn->setTag(this->option_modID.size());
        this->option_modID.push_back(modID);
        this->option_decscription.push_back(decs);
        this->option_modTitle.push_back(title);
        this->option_mod_names.push_back(mod_names);
        this->option_modDescs.push_back(mod_descs);
        return btn;
    };

    bool setup() {
        this->setTitle("Icon Options", "bigFont.fnt", 0.6f);
        auto ps = this->m_mainLayer->getContentSize();
        auto bg = extension::CCScale9Sprite::create("square02_small.png");
        bg->setContentSize({ps.width - 20, ps.height - 50});
        bg->setPosition(ps.width / 2, ps.height / 2 - 15);
        bg->setOpacity(100);
        auto bs = bg->getContentSize();
        auto menu = Node::Menu::create({ bs.width / 2 + 8.f, bs.height / 2 }, bs, bg);
        menu->addChild(this->createButton("Dual Icons", "asakiyuki:custom_dual_mod"));
        menu->addChild(
            this->createOptionsButton("Dual Icons", "Custom dual mode",
            "asakiyuki:dual_types", 
            {
                "Legacy",
                "Same Color",
                "Separate"
            },
            {
                "Turn off the Custom Glow Color feature so the dual looks like version 2.111 or lower",
                "Make Dual Icons have the same color",
                "Allows customization of the 2nd Player"
            }));
            
        menu->addChild(this->createButton("Separate Swapper", "asakiyuki:separate_swapper", 0.35f));
        menu->addChild(this->createInfoButton("Separate Swapper", "Swap dual icons when Player 2 enter Exit dual portal"));

        menu->addChild(this->createButton("Force Trail", "asakiyuki:force_trail", 0.45f));
        menu->addChild(this->createOptionsButton("Force Trail", "Customize Trail effects", "asakiyuki:force_trail_type",
            {
                "Always Hide",
                "Always Show",
                "Always Show (Fix)"
            },
            {
                "Sets the trail to always hide",
                "Sets the trail to always show",
                "Sets the trail to show when Player visible"
            }));
        
        menu->addChild(this->createButton("Icons Bypass", "asakiyuki:icon-bypass", 0.45f));
        menu->addChild(this->createInfoButton("Icons Bypass", "Unlocks all icons and colors"));

        menu->addChild(this->createButton("Solid Wave Trail", "asakiyuki:solid-wave-trail", 0.35f));
        menu->addChild(this->createInfoButton("Solid Wave Trail", "No blending on the wave trail"));
        
        menu->addChild(this->createButton("No Respawn Blink", "asakiyuki:no-respawn-blink", 0.35f));
        menu->addChild(this->createInfoButton("No Respawn Blink", "Disable respawn flash effect"));

        menu->addChild(this->createButton("No Death Effect", "asakiyuki:no-death-effect", 0.35f));
        menu->addChild(this->createInfoButton("No Death Effect", "Disable death effect"));

        menu->setLayout(RowLayout::create()->setGap(8.f)->setAxisAlignment(AxisAlignment::Start)->setGrowCrossAxis(true));
        this->m_mainLayer->addChild(bg);
        return true;
    };
    public:
    static IconOptionsPopup* create() {
        auto ret = new IconOptionsPopup();
        if (ret && ret->initAnchored(350, 225, "GJ_square02.png")) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    };
    void showup(CCObject*) {
        IconOptionsPopup::create()->show();
    }
};

class $modify(GJGarageLayer) {
    bool init() {
        GJGarageLayer::init();
        auto menu = static_cast<CCMenu*>(this->getChildByID("back-menu"));
        Node::MenuItemSpriteExtra::create(
            Node::Sprite::createWithSpriteFrameName("GJ_optionsBtn_001.png", 0.7f),
            this, menu_selector(IconOptionsPopup::showup), menu
        );
        menu->updateLayout();
        return true; 
    };
};