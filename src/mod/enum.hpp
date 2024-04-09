// enum.hpp
#ifndef __ENUM_HPP__
#define __ENUM_HPP__
namespace AsakiYuki {
    namespace ModTypes {
        enum ColorType {
            FADE,
            PASTEL,
            RAINBOW
        };
        enum PlayerType {
            FIRST_PLAYER,
            SECOND_PLAYER,
            BOTH_PLAYERS
        };
        enum Gamemode {
            CUBE,
            SHIP,
            BALL,
            UFO,
            WAVE,
            ROBOT,
            SPIDER,
            SWING,
            JETPACK,
            DEATH_EFFECT = 50,
            STREAK,
            SHIP_STREAK,
            COLOR_1 = 100,
            COLOR_2,
            COLOR_GLOW
        };
        enum IconItemType {
            GLOW,
            EXPLORE_EFFECT,
            ROBOT_ANIMATE,
            ROBOT_ANIMATE_2,
            SPIDER_ANIMATE
        };
        enum Custom {
            WAVE_TRAIL
        };
        enum CustomBool {
            CUSTOM_WAVE_TRAIL
        };
    };
};
#endif