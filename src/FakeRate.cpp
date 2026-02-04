#include "FakeRate.hpp"
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Mod.hpp>
#include <jasmine/convert.hpp>

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

FakeRateSaveData* FakeRate::getFakeRate(GJGameLevel* level) {
    auto it = std::ranges::find(FakeRate::fakeRates, level->m_levelID.value(), &FakeRateSaveData::id);
    return it != FakeRate::fakeRates.end() ? std::to_address(it) : nullptr;
}

int FakeRate::getDifficultyFromLevel(GJGameLevel* level) {
    if (level->m_demon > 0) return GJGameLevel::demonIconForDifficulty((DemonDifficultyType)level->m_demonDifficulty);
    else if (level->m_autoLevel) return -1;
    else return level->getAverageDifficulty();
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
    constexpr std::array<CCPoint, 21> longOffsets = {
        CCPoint { 0.0f, 0.0f },
        { 0.0f, -5.0f }, { 0.125f, -5.0f }, { 0.0f, -5.0f }, { 0.0f, -5.125f }, { 0.25f, -5.0f },
        { 0.125f, -4.75f }, { 0.0f, -5.0f }, { 0.0f, -4.125f }, { -0.125f, -4.125f }, { 0.0f, -4.0f },
        { -0.125f, -4.125f }, { 0.0f, -4.125f }, { 0.125f, -4.125f }, { 0.0f, -4.125f }, { 0.0f, -4.125f },
        { 0.0f, -3.625f }, { 0.0f, -3.625f }, { 0.0f, -3.5f }, { 0.0f, -3.5f }, { 0.0f, -3.5f }
    };
    constexpr std::array<CCPoint, 21> shortOffsets = {
        CCPoint { 0.0f, 0.0f },
        { -0.125f, -0.25f }, { -0.125f, -0.25f }, { -0.125f, -0.25f }, { -0.125f, -0.375f }, { -0.125f, -0.25f },
        { -0.125f, -0.25f }, { -0.125f, -0.375f }, { -0.125f, 0.5f }, { -0.125f, 0.5f }, { -0.125f, 0.25f },
        { -0.125f, 0.5f }, { 0.125f, 0.5f }, { 0.125f, 0.5f }, { 0.125f, 0.5f }, { 0.0f, 0.5f },
        { 0.0f, 1.25f }, { 0.0f, 1.25f }, { 0.0f, 1.125f }, { 0.0f, 1.125f }, { 0.0f, 1.125f }
    };

    auto& offsets = name == GJDifficultyName::Long ? longOffsets : shortOffsets;
    return difficulty < offsets.size() ? offsets[difficulty] : offsets[0];
}

int FakeRate::getGRDOverride(CCSprite* sprite) {
    auto sprName = getSpriteName(sprite);

    auto pos = sprName.find("GrD_demon");
    if (pos == std::string::npos || pos + 9 >= sprName.size()) return 0;

    return jasmine::convert::get<int>(std::move(sprName).substr(9)).value_or(0);
}

int FakeRate::getDIBOverride(CCSprite* sprite) {
    auto sprName = getSpriteName(sprite);

    auto pos = sprName.find("DIB_");
    if (pos == std::string::npos || pos + 4 >= sprName.size()) return 0;

    return jasmine::convert::get<int>(std::move(sprName).substr(4)).value_or(0);
}

int FakeRate::getGDDPOverride(CCSprite* sprite) {
    auto sprName = getSpriteName(sprite);
    if (sprName.ends_with("Text.png")) sprName = std::move(sprName).substr(0, sprName.size() - 8);
    if (sprName.ends_with("Small")) sprName = std::move(sprName).substr(0, sprName.size() - 5);
    if (sprName.ends_with("Plus")) sprName = std::move(sprName).substr(0, sprName.size() - 4);

    auto pos = sprName.find("DP_");
    if (pos == std::string::npos || pos + 3 >= sprName.size()) return 0;

    auto it = gddpIndices.find(std::move(sprName).substr(pos + 3));
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
    if (auto id = value.get<int>("id")) data.id = id.unwrap();
    if (auto stars = value.get<int>("stars")) data.stars = stars.unwrap();
    if (auto feature = value.get<int>("feature")) data.feature = feature.unwrap();
    if (auto difficulty = value.get<int>("difficulty")) data.difficulty = difficulty.unwrap();
    if (auto mdo = value.get<int>("more-difficulties-override")) data.moreDifficultiesOverride = mdo.unwrap();
    if (auto gdo = value.get<int>("grandpa-demon-override")) data.grandpaDemonOverride = gdo.unwrap();
    if (auto dbo = value.get<int>("demons-in-between-override")) data.demonsInBetweenOverride = dbo.unwrap();
    if (auto gio = value.get<int>("gddp-integration-override")) data.gddpIntegrationOverride = gio.unwrap();
    if (auto coins = value.get<bool>("coins")) data.coins = coins.unwrap();
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
