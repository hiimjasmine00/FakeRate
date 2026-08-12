#pragma once
#include <cocos2d.h>
#include <Geode/Enums.hpp>
#include <Geode/GeneratedPredeclare.hpp>
#include <matjson.hpp>

struct FakeRateSaveData {
    int id = 0;
    int stars = 0;
    int feature = 0;
    int difficulty = 0;
    int moreDifficultiesOverride = 0;
    int grandpaDemonOverride = 0;
    int demonsInBetweenOverride = 0;
    int gddpIntegrationOverride = 0;
    bool coins = true;
};

namespace FakeRate {
    extern std::vector<FakeRateSaveData> fakeRates;

    FakeRateSaveData* getFakeRate(GJGameLevel* level);
    std::string_view getSpriteName(cocos2d::CCSprite* sprite);
    void toggle(cocos2d::CCNode* node, bool enabled);
    cocos2d::CCPoint getDIBOffset(int difficulty, GJDifficultyName name);
    int getGRDOverride(cocos2d::CCSprite* sprite);
    int getDIBOverride(cocos2d::CCSprite* sprite);
    int getGDDPOverride(cocos2d::CCSprite* sprite);
    std::string getGDDPFrame(int difficulty, GJDifficultyName name);
}

template<>
struct matjson::Serialize<FakeRateSaveData> {
    static geode::Result<FakeRateSaveData> fromJson(const matjson::Value& value);
    static matjson::Value toJson(const FakeRateSaveData& data);
};
