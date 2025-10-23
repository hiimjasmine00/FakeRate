#include "FakeRate.hpp"
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

std::vector<FakeRateSaveData> FakeRate::fakeRates;

std::unordered_map<std::string, int> gddpIndices = {
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
    { "Pearl", 12 },
    { "Onyx", 13 },
    { "Amethyst", 14 },
    { "Azurite", 15 },
    { "Obsidian", 16 }
};

$on_mod(Loaded) {
    FakeRate::fakeRates = Mod::get()->getSavedValue<std::vector<FakeRateSaveData>>("fake-rate");
}

$on_mod(DataSaved) {
    Mod::get()->setSavedValue("fake-rate", FakeRate::fakeRates);
}

int FakeRate::getBaseCurrency(int stars) {
    switch (stars) {
        case 0: case 1: return 0;
        case 2: return 40;
        case 3: return 60;
        case 4: return 100;
        case 5: return 140;
        case 6: return 180;
        case 7: return 220;
        case 8: return 280;
        case 9: return 340;
        default: return 400;
    }
}

int FakeRate::getDifficultyFromLevel(GJGameLevel* level) {
    if (level->m_demon > 0) return level->m_demonDifficulty > 0 ? level->m_demonDifficulty + 4 : 6;
    else if (level->m_autoLevel) return -1;
    else if (level->m_ratings < 5) return 0;
    else return level->m_ratingsSum / level->m_ratings;
}

std::string FakeRate::getSpriteName(CCSprite* sprite) {
    if (auto texture = sprite->getTexture()) {
        auto& textureRect = sprite->getTextureRect();
        for (auto [key, frame] : CCDictionaryExt<std::string, CCSpriteFrame*>(CCSpriteFrameCache::get()->m_pSpriteFrames)) {
            if (frame->getTexture() == texture && frame->getRect() == textureRect) return key;
        }
        for (auto [key, obj] : CCDictionaryExt<std::string, CCTexture2D*>(CCTextureCache::get()->m_pTextures)) {
            if (obj == texture) return key;
        }
    }
    return "";
}

void FakeRate::toggle(CCNode* node, bool enabled) {
    if (auto sprite = typeinfo_cast<CCRGBAProtocol*>(node)) {
        auto color = enabled ? ccColor3B { 255, 255, 255 } : ccColor3B { 125, 125, 125 };
        sprite->setColor(color);

        if (auto children = node->getChildren()) {
            for (auto child : CCArrayExt<CCNode*>(children)) {
                if (auto sprite = typeinfo_cast<CCRGBAProtocol*>(child)) sprite->setColor(color);
            }
        }
    }
}

CCPoint FakeRate::getDIBOffset(int difficulty, GJDifficultyName name) {
    switch (difficulty) {
        case 1: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -5.0f } : CCPoint { -0.125f, -0.25f };
        case 2: return name == GJDifficultyName::Long ? CCPoint { 0.125f, -5.0f } : CCPoint { -0.125f, -0.25f };
        case 3: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -5.0f } : CCPoint { -0.125f, -0.25f };
        case 4: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -5.125f } : CCPoint { -0.125f, -0.375f };
        case 5: return name == GJDifficultyName::Long ? CCPoint { 0.25f, -5.0f } : CCPoint { -0.125f, -0.25f };
        case 6: return name == GJDifficultyName::Long ? CCPoint { 0.125f, -4.75f } : CCPoint { -0.125f, -0.25f };
        case 7: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -5.0f } : CCPoint { -0.125f, -0.375f };
        case 8: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.125f } : CCPoint { -0.125f, 0.5f };
        case 9: return name == GJDifficultyName::Long ? CCPoint { -0.125f, -4.125f } : CCPoint { -0.125f, 0.5f };
        case 10: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.0f } : CCPoint { -0.125f, 0.25f };
        case 11: return name == GJDifficultyName::Long ? CCPoint { -0.125f, -4.125f } : CCPoint { -0.125f, 0.5f };
        case 12: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.125f } : CCPoint { 0.125f, 0.5f };
        case 13: return name == GJDifficultyName::Long ? CCPoint { 0.125f, -4.125f } : CCPoint { 0.125f, 0.5f };
        case 14: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.125f } : CCPoint { 0.125f, 0.5f };
        case 15: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -4.125f } : CCPoint { 0.0f, 0.5f };
        case 16: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.625f } : CCPoint { 0.0f, 1.25f };
        case 17: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.625f } : CCPoint { 0.0f, 1.25f };
        case 18: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.5f } : CCPoint { 0.0f, 1.125f };
        case 19: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.5f } : CCPoint { 0.0f, 1.125f };
        case 20: return name == GJDifficultyName::Long ? CCPoint { 0.0f, -3.5f } : CCPoint { 0.0f, 1.125f };
        default: return CCPoint { 0.0f, 0.0f };
    }
}

int FakeRate::getGRDOverride(CCSprite* sprite) {
    auto sprName = getSpriteName(sprite);

    auto pos = sprName.find("GrD_demon");
    if (pos == std::string::npos || pos + 9 >= sprName.size()) return 0;

    auto ret = 0;
    std::from_chars(sprName.data() + (pos + 9), sprName.data() + sprName.size(), ret);
    return ret;
}

int FakeRate::getDIBOverride(CCSprite* sprite) {
    auto sprName = getSpriteName(sprite);

    auto pos = sprName.find("DIB_");
    if (pos == std::string::npos || pos + 4 >= sprName.size()) return 0;

    auto ret = 0;
    std::from_chars(sprName.data() + (pos + 4), sprName.data() + sprName.size(), ret);
    return ret;
}

int FakeRate::getGDDPOverride(CCSprite* sprite) {
    auto sprName = getSpriteName(sprite);
    if (sprName.ends_with("Text.png")) sprName = sprName.substr(0, sprName.size() - 8);
    if (sprName.ends_with("Small")) sprName = sprName.substr(0, sprName.size() - 5);
    if (sprName.ends_with("Plus")) sprName = sprName.substr(0, sprName.size() - 4);

    auto pos = sprName.find("DP_");
    if (pos == std::string::npos || pos + 3 >= sprName.size()) return 0;

    auto it = gddpIndices.find(sprName.substr(pos + 3));
    return it != gddpIndices.end() ? it->second : 0;
}

std::string FakeRate::getGDDPFrame(int difficulty, GJDifficultyName name) {
    constexpr std::array difficulties = {
        "Beginner", "Bronze", "Silver", "Gold", "Amber", "Platinum", "Sapphire", "Jade",
        "Emerald", "Ruby", "Diamond", "Pearl", "Onyx", "Amethyst", "Azurite", "Obsidian"
    };

    return fmt::format("minemaker0430.gddp_integration/DP_{}{}Text.png",
        difficulties[difficulty - 1], name == GJDifficultyName::Short ? "Small" : "");
}

Result<FakeRateSaveData> matjson::Serialize<FakeRateSaveData>::fromJson(const matjson::Value& value) {
    if (!value.isObject()) return Err("Expected object");
    FakeRateSaveData data;
    if (auto id = value.get<int>("id").ok()) data.id = *id;
    if (auto stars = value.get<int>("stars").ok()) data.stars = *stars;
    if (auto feature = value.get<int>("feature").ok()) data.feature = *feature;
    if (auto difficulty = value.get<int>("difficulty").ok()) data.difficulty = *difficulty;
    if (auto mdo = value.get<int>("more-difficulties-override").ok()) data.moreDifficultiesOverride = *mdo;
    if (auto gdo = value.get<int>("grandpa-demon-override").ok()) data.grandpaDemonOverride = *gdo;
    if (auto dbo = value.get<int>("demons-in-between-override").ok()) data.demonsInBetweenOverride = *dbo;
    if (auto gio = value.get<int>("gddp-integration-override").ok()) data.gddpIntegrationOverride = *gio;
    if (auto coins = value.get<bool>("coins").ok()) data.coins = *coins;
    return Ok(data);
}

matjson::Value matjson::Serialize<FakeRateSaveData>::toJson(const FakeRateSaveData& data) {
    return matjson::makeObject({
        { "id", data.id },
        { "stars", data.stars },
        { "feature", data.feature },
        { "difficulty", data.difficulty },
        { "more-difficulties-override", data.moreDifficultiesOverride },
        { "grandpa-demon-override", data.grandpaDemonOverride },
        { "demons-in-between-override", data.demonsInBetweenOverride },
        { "gddp-integration-override", data.gddpIntegrationOverride },
        { "coins", data.coins }
    });
}
