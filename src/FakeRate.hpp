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

class FakeRate {
public:
    static std::vector<FakeRateSaveData> fakeRates;

    static FakeRateSaveData* getFakeRate(GJGameLevel*);
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
struct matjson::Serialize<FakeRateSaveData> {
    static geode::Result<FakeRateSaveData> fromJson(const matjson::Value& value);
    static matjson::Value toJson(const FakeRateSaveData& data);
};
