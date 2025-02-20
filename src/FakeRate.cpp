#include "FakeRate.hpp"
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

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
        for (auto [key, frame] : CCDictionaryExt<std::string, CCSpriteFrame*>(CCSpriteFrameCache::get()->m_pSpriteFrames)) {
            if (frame->getTexture() == texture && frame->getRect() == sprite->getTextureRect()) return key;
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

        if (auto children = node->getChildren()) for (auto child : CCArrayExt<CCNode*>(children)) {
            if (auto sprite = typeinfo_cast<CCRGBAProtocol*>(child)) sprite->setColor(color);
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
    return pos != std::string::npos ? numFromString<int>(sprName.substr(pos + 9)).unwrapOr(0) : 0;
}

int FakeRate::getDIBOverride(CCSprite* sprite) {
    auto sprName = getSpriteName(sprite);

    auto pos = sprName.find("DIB_");
    return pos != std::string::npos ? numFromString<int>(sprName.substr(pos + 4)).unwrapOr(0) : 0;
}

int FakeRate::getGDDPOverride(CCSprite* sprite) {
    auto sprName = getSpriteName(sprite);
    if (sprName.substr(sprName.size() - 8) == "Text.png") sprName = sprName.substr(0, sprName.size() - 8);
    if (sprName.substr(sprName.size() - 5) == "Small") sprName = sprName.substr(0, sprName.size() - 5);
    if (sprName.substr(sprName.size() - 4) == "Plus") sprName = sprName.substr(0, sprName.size() - 4);

    auto pos = sprName.find("DP_");
    if (pos != std::string::npos) {
        auto str = GDDP_INDICES.find(sprName.substr(pos + 3));
        return str != GDDP_INDICES.end() ? str->second : 0;
    }
    else return 0;
}

std::string FakeRate::getGDDPFrame(int difficulty, GJDifficultyName name) {
    auto diff = "";
    switch (difficulty) {
        case 1: diff = "Beginner"; break;
        case 2: diff = "Bronze"; break;
        case 3: diff = "Silver"; break;
        case 4: diff = "Gold"; break;
        case 5: diff = "Amber"; break;
        case 6: diff = "Platinum"; break;
        case 7: diff = "Sapphire"; break;
        case 8: diff = "Jade"; break;
        case 9: diff = "Emerald"; break;
        case 10: diff = "Ruby"; break;
        case 11: diff = "Diamond"; break;
        case 12: diff = "Onyx"; break;
        case 13: diff = "Amethyst"; break;
        case 14: diff = "Azurite"; break;
        case 15: diff = "Obsidian"; break;
        default: diff = "Beginner"; break;
    }

    return fmt::format("minemaker0430.gddp_integration/DP_{}{}Text.png", diff, name == GJDifficultyName::Short ? "Small" : "");
}

Result<std::vector<FakeRateSaveData>> matjson::Serialize<std::vector<FakeRateSaveData>>::fromJson(const matjson::Value& value) {
    if (!value.isArray()) return Err("Expected array");

    return Ok(ranges::map<std::vector<FakeRateSaveData>>(value.asArray().unwrap(), [](const matjson::Value& item) {
        return FakeRateSaveData {
            .id = (int)item["id"].asInt().unwrapOr(0),
            .stars = (int)item["stars"].asInt().unwrapOr(0),
            .feature = (int)item["feature"].asInt().unwrapOr(0),
            .difficulty = (int)item["difficulty"].asInt().unwrapOr(0),
            .moreDifficultiesOverride = (int)item["more-difficulties-override"].asInt().unwrapOr(0),
            .grandpaDemonOverride = (int)item["grandpa-demon-override"].asInt().unwrapOr(0),
            .demonsInBetweenOverride = (int)item["demons-in-between-override"].asInt().unwrapOr(0),
            .gddpIntegrationOverride = (int)item["gddp-integration-override"].asInt().unwrapOr(0),
            .coins = item["coins"].asBool().unwrapOr(true)
        };
    }));
}

matjson::Value matjson::Serialize<std::vector<FakeRateSaveData>>::toJson(const std::vector<FakeRateSaveData>& vec) {
    return ranges::map<std::vector<matjson::Value>>(vec, [](const FakeRateSaveData& item) {
        return matjson::makeObject({
            { "id", item.id },
            { "stars", item.stars },
            { "feature", item.feature },
            { "difficulty", item.difficulty },
            { "more-difficulties-override", item.moreDifficultiesOverride },
            { "grandpa-demon-override", item.grandpaDemonOverride },
            { "demons-in-between-override", item.demonsInBetweenOverride },
            { "gddp-integration-override", item.gddpIntegrationOverride },
            { "coins", item.coins }
        });
    });
}
