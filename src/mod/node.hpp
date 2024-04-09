#ifndef __MOD_NODE_HPP__
#define __MOD_NODE_HPP__
#include <Geode/Geode.hpp>
#include "./enum.hpp"
using namespace geode::prelude;

namespace AsakiYuki {
    namespace Node {
        class Sprite {
            public:
            static CCSprite* createWithSpriteFrameName(
                const char* name,
                float scale = 1.f,
                const CCPoint &pos = {0.f, 0.f},
                const CCPoint &anchor = {0.5f, 0.5f},
                const bool flipX = false,
                const bool flipY = false
            ) {
                auto sprite = CCSprite::createWithSpriteFrameName(name);
                sprite->setScale(scale);
                sprite->setPosition(pos);
                sprite->setAnchorPoint(anchor);
                sprite->setFlipX(flipX);
                sprite->setFlipY(flipY);
                return sprite;
            }
            static CCSprite* create(
                const char* name,
                float scale = 1.f,
                const CCPoint &pos = {0.f, 0.f},
                const CCPoint &anchor = {0.5f, 0.5f},
                const bool flipX = false,
                const bool flipY = false
            ) {
                auto sprite = CCSprite::create(name);
                sprite->setScale(scale);
                sprite->setPosition(pos);
                sprite->setAnchorPoint(anchor);
                sprite->setFlipX(flipX);
                sprite->setFlipY(flipY);
                return sprite;
            }
        };
        class MenuItemSpriteExtra {
            public:
            static CCMenuItemSpriteExtra* create(
                CCNode* sprite,
                CCObject* target,
                SEL_MenuHandler callback,
                CCMenu* parent = nullptr,
                const CCPoint &pos = {0.f, 0.f},
                const CCPoint &anchor = {0.5f, 0.5f},
                const CCPoint &contentSize = { 0.123321f, 1.123321f },
                int tag = -1
            ) {
                auto btn = CCMenuItemSpriteExtra::create(sprite, target, callback);
                btn->setPosition(pos);
                btn->setAnchorPoint(anchor);
                if (parent != nullptr) parent->addChild(btn);
                if ((contentSize.x != 0.123321f) && (contentSize.y != 0.123321f))
                    btn->setContentSize(contentSize);
                if (tag != -1) btn->setTag(tag);
                return btn;
            }
        };
        class Menu {
            public:
            static CCMenu* create(
                const CCPoint &pos = {0.f, 0.f},
                const CCSize &size = {0.f, 0.f},
                CCNode* parent = nullptr
            ) {
                auto menu = CCMenu::create();
                if (parent != nullptr) parent->addChild(menu);
                menu->setPosition(pos);
                menu->setContentSize(size);
                return menu;
            }
        };
        class SimPlayer {
            public:
            static SimplePlayer* create(
                AsakiYuki::ModTypes::Gamemode gamemode = AsakiYuki::ModTypes::Gamemode::CUBE,
                int id = -1,
                CCPoint pos = {0.f, 0.f},
                CCPoint anchorPoint = {0.5f, 0.5f},
                const float scale = 1.f,
                ccColor3B color = AsakiYuki::Color::playerColor(),
                ccColor3B secondColor = AsakiYuki::Color::playerColor2(),
                ccColor3B glowColor = AsakiYuki::Color::playerGlowColor(),
                bool hasGlow = AsakiYuki::Save::getPlayerFrame(AsakiYuki::ModTypes::IconItemType::GLOW)
            ) {
                auto player = SimplePlayer::create(id);
                player->updatePlayerFrame(
                    (id == -1) ? AsakiYuki::Save::getPlayerFrame(gamemode) : id,
                    static_cast<IconType>(gamemode)
                );
                player->setPosition(pos);
                player->setAnchorPoint(anchorPoint);
                player->setScale(scale);
                player->setColor(color);
                player->setSecondColor(secondColor);
                if (hasGlow) player->setGlowOutline(glowColor);
                return player;
            }
            static SimplePlayer* create(
                CCPoint pos = {0.f, 0.f},
                AsakiYuki::ModTypes::Gamemode gamemode = AsakiYuki::ModTypes::Gamemode::CUBE,
                CCPoint anchorPoint = {0.5f, 0.5f},
                const float scale = 1.f,
                int id = -1,
                ccColor3B color = AsakiYuki::Color::playerColor(),
                ccColor3B secondColor = AsakiYuki::Color::playerColor2(),
                ccColor3B glowColor = AsakiYuki::Color::playerGlowColor(),
                bool hasGlow = AsakiYuki::Save::getPlayerFrame(AsakiYuki::ModTypes::IconItemType::GLOW)
            ) {
                return SimPlayer::create(gamemode, id, pos, anchorPoint, scale, color, secondColor, glowColor, hasGlow);
            }
            static SimplePlayer* create(
                const float scale = 1.f,
                AsakiYuki::ModTypes::Gamemode gamemode = AsakiYuki::ModTypes::Gamemode::CUBE,
                CCPoint pos = {0.f, 0.f},
                CCPoint anchorPoint = {0.5f, 0.5f},
                int id = -1,
                ccColor3B color = AsakiYuki::Color::playerColor(),
                ccColor3B secondColor = AsakiYuki::Color::playerColor2(),
                ccColor3B glowColor = AsakiYuki::Color::playerGlowColor(),
                bool hasGlow = AsakiYuki::Save::getPlayerFrame(AsakiYuki::ModTypes::IconItemType::GLOW)
            ) {
                return SimPlayer::create(gamemode, id, pos, anchorPoint, scale, color, secondColor, glowColor, hasGlow);
            }
        };

        class LabelBMFont {
            public:
            static CCLabelBMFont* create(
                const char *str,
                const char *fntFile = "bigFont.fnt",
                float scale = 1.f,
                const CCPoint &pos = {0.f, 0.f},
                const CCPoint &anchor = {0.5f, 0.5f},
                const ccColor3B &color = {255, 255, 255}
            ) {
                auto p0 = CCLabelBMFont::create(str, fntFile);
                p0->setScale(scale);
                p0->setPosition(pos);
                p0->setAnchorPoint(anchor);
                p0->setColor(color);
                return p0;
            };
            static CCLabelBMFont* create(
                float scale,
                const char *str,
                const CCPoint &pos = {0.f, 0.f},
                const CCPoint &anchor = {0.5f, 0.5f},
                const ccColor3B &color = {255, 255, 255},
                const char *fntFile = "bigFont.fnt"
            ) {
                return LabelBMFont::create(str, fntFile, scale, pos, anchor, color);
            };
            static CCLabelBMFont* create(
                float scale,
                const char *str,
                const ccColor3B &color = {255, 255, 255},
                const CCPoint &pos = {0.f, 0.f},
                const CCPoint &anchor = {0.5f, 0.5f},
                const char *fntFile = "bigFont.fnt"
            ) {
                return LabelBMFont::create(str, fntFile, scale, pos, anchor, color);
            }
        };
    }
}
#endif