#pragma once
#include <cocos2d.h>
#include <Geode/Enums.hpp>
#include <Geode/GeneratedPredeclare.hpp>
#include <matjson.hpp>

struct FakeRateSaveData {
    int id;
    int stars;
    int feature;
    int difficulty;
    int moreDifficultiesOverride;
    int grandpaDemonOverride;
    int demonsInBetweenOverride;
    int gddpIntegrationOverride;
    bool coins;
};

class FakeRate {
private:
    inline static std::unordered_map<std::string, int> GDDP_INDICES = {
        { "Beginner", 1 },
        { "Bronze", 2 },
        { "Silver", 3 },
        { "Gold", 4 },
        { "Amber", 5 },
        { "Platinum", 6 },
        { "Sapphire", 7 },
        { "Jade", 8 },
        { "Emerald", 9 },
        { "Ruby", 10 },
        { "Diamond", 11 },
        { "Onyx", 12 },
        { "Amethyst", 13 },
        { "Azurite", 14 },
        { "Obsidian", 15 }
    };
public:
    static int getBaseCurrency(int);
    static int getDifficultyFromLevel(GJGameLevel*);
    static std::string getSpriteName(cocos2d::CCSprite*);
    static void toggle(cocos2d::CCNode*, bool);
    static cocos2d::CCPoint getDIBOffset(int, GJDifficultyName);
    static int getGRDOverride(cocos2d::CCSprite*);
    static int getDIBOverride(cocos2d::CCSprite*);
    static int getGDDPOverride(cocos2d::CCSprite*);
    static std::string getGDDPFrame(int, GJDifficultyName);
};

template<>
struct matjson::Serialize<std::vector<FakeRateSaveData>> {
    static geode::Result<std::vector<FakeRateSaveData>> fromJson(const matjson::Value& value);
    static matjson::Value toJson(const std::vector<FakeRateSaveData>& vec);
};
