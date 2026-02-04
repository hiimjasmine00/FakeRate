#include "../FakeRate.hpp"
#include "../classes/FREffects.hpp"
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelCell.hpp>

using namespace geode::prelude;

class $modify(FRLevelCell, LevelCell) {
    static void onModify(ModifyBase<ModifyDerive<FRLevelCell, LevelCell>>& self) {
        (void)self.setHookPriority("LevelCell::loadFromLevel", Priority::Early);
    }

    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);

        auto data = FakeRate::getFakeRate(level);
        if (!data) return;

        auto difficultyContainer = m_mainLayer->getChildByID("difficulty-container");
        if (!difficultyContainer) difficultyContainer = m_mainLayer->getChildByID("grd-demon-icon-layer");
        if (!difficultyContainer) return;

        auto difficultySprite = static_cast<GJDifficultySprite*>(difficultyContainer->getChildByID("difficulty-sprite"));
        if (auto betweenDifficultySprite = difficultyContainer->getChildByID("hiimjustin000.demons_in_between/between-difficulty-sprite")) {
            betweenDifficultySprite->setVisible(false);
            difficultySprite->setOpacity(255);
        }
        auto loader = Loader::get();
        auto animatedFire = loader->isModLoaded("uproxide.animated_fire");
        auto gddpOverride = false;
        if (auto gddpDifficultySprite = static_cast<CCSprite*>(difficultyContainer->getChildByID("gddp-difficulty"))) {
            gddpOverride = true;
            gddpDifficultySprite->setVisible(false);
            if (FakeRate::getSpriteName(gddpDifficultySprite).contains("Plus")) {
                if (auto epicSprite = getChildBySpriteFrameName(difficultySprite, "GJ_epicCoin_001.png")) {
                    epicSprite->setVisible(!animatedFire);
                }
            }
            difficultySprite->setOpacity(255);
        }
        if (difficultyContainer->getID() == "grd-demon-icon-layer" && !gddpOverride) {
            difficultyContainer->removeChildByTag(69420);
            if (animatedFire) difficultyContainer->removeChildByTag(69420);
            difficultySprite->setVisible(true);
            auto index = difficultyContainer->getChildrenCount() - 1;
            if (auto grdInfinity = difficultyContainer->getChildByID("grd-infinity")) {
                grdInfinity->setVisible(false);
                index--;
            }
            difficultyContainer->getChildByIndex(index)->setVisible(false);
            if (auto featureGlow = difficultySprite->getChildByTag(69420)) {
                featureGlow->setPosition(difficultySprite->getContentSize() / 2);
            }
        }

        difficultySprite->updateFeatureState((GJFeatureState)data->feature);
        difficultySprite->updateDifficultyFrame(data->difficulty, GJDifficultyName::Short);
        auto addCoins = level->m_coins > 0 && !m_compactView;
        auto dailyID = level->m_dailyID.value();
        auto showStars = data->stars != 0 || dailyID > 0;
        difficultySprite->setPositionY(
            (showStars || addCoins ? 5.0f : 0.0f) + (showStars && addCoins ? 9.0f : 0.0f) + (dailyID > 0 ? 10.0f : 0.0f));
        auto gsm = GameStatsManager::get();
        if (showStars) {
            auto starsIcon = difficultyContainer->getChildByID("stars-icon");
            if (!starsIcon) {
                starsIcon = CCSprite::createWithSpriteFrameName(level->isPlatformer() ? "moon_small01_001.png" : "star_small01_001.png");
                starsIcon->setPosition({ difficultySprite->getPositionX() + 8.0f, difficultySprite->getPositionY() - 30.0f });
                starsIcon->setID("stars-icon");
                difficultyContainer->addChild(starsIcon);
            }
            auto starsLabel = static_cast<CCLabelBMFont*>(difficultyContainer->getChildByID("stars-label"));
            if (!starsLabel) {
                starsLabel = CCLabelBMFont::create("0", "bigFont.fnt");
                starsLabel->setPosition({ starsIcon->getPositionX() - 8.0f, starsIcon->getPositionY() });
                starsLabel->setScale(0.4f);
                starsLabel->setAnchorPoint({ 1.0f, 0.5f });
                starsLabel->setID("stars-label");
                difficultyContainer->addChild(starsLabel);
            }
            starsLabel->setString(fmt::to_string(data->stars).c_str());
            if (gsm->hasCompletedLevel(level)) starsLabel->setColor({ 255, 255, 50 });
        }
        else {
            if (auto starsIcon = difficultyContainer->getChildByID("stars-icon")) starsIcon->removeFromParent();
            if (auto starsLabel = difficultyContainer->getChildByID("stars-label")) starsLabel->removeFromParent();
        }

        auto coinParent = m_compactView ? m_mainLayer : difficultyContainer;
        auto coinY = difficultySprite->getPositionY() - 31.5f - (showStars ? 14.0f : 0.0f) - (level->m_gauntletLevel || dailyID > 0 ? 14.5f : 0.0f);
        auto coins = data->coins;
        for (int i = 1; i <= 3; i++) {
            if (auto coin = static_cast<CCSprite*>(coinParent->getChildByID(fmt::format("coin-icon-{}", i)))) {
                if (!m_compactView) coin->setPositionY(coinY);
                auto coinKey = level->getCoinKey(i);
                auto hasCoin = gsm->hasUserCoin(coinKey) || gsm->hasPendingUserCoin(coinKey);
                if (coins) coin->setColor(hasCoin ? ccColor3B { 255, 255, 255 } : ccColor3B { 165, 165, 165 });
                else coin->setColor(hasCoin ? ccColor3B { 255, 175, 75 } : ccColor3B { 165, 113, 48 });
            }
        }

        if (dailyID > 0) {
            auto diamondLabel = static_cast<CCLabelBMFont*>(difficultyContainer->getChildByID("diamond-label"));
            auto diamondIcon = difficultyContainer->getChildByID("diamond-icon");
            auto diamonds = gsm->getBaseDiamonds(data->stars);
            diamondLabel->setString(fmt::format("{}/{}", (int)floor(diamonds * level->m_normalPercent.value() / 100.0), diamonds).c_str());
            diamondIcon->setPositionX(difficultySprite->getPositionX() + diamondLabel->getScaledContentWidth() * 0.5f + 2.0f);
            diamondLabel->setPositionX(diamondIcon->getPositionX() - 8.0f);
        }

        auto padding = (showStars ? 18.0f : 25.0f) * (m_compactView ? 0.8f : 1.0f);
        auto lengthLabel = m_mainLayer->getChildByID("length-label");
        auto downloadsIcon = m_mainLayer->getChildByID("downloads-icon");
        auto downloadsLabel = m_mainLayer->getChildByID("downloads-label");
        downloadsIcon->setPositionX(lengthLabel->getPositionX() + lengthLabel->getScaledContentWidth() + padding);
        downloadsLabel->setPositionX(downloadsIcon->getPositionX() + 9.0f);
        auto likesLabel = m_mainLayer->getChildByID("likes-label");
        auto likesIcon = m_mainLayer->getChildByID("likes-icon");
        likesIcon->setPositionX(downloadsLabel->getPositionX() + downloadsLabel->getScaledContentWidth() + padding);
        likesLabel->setPositionX(likesIcon->getPositionX() + 10.0f);
        if (showStars) {
            auto orbsIcon = m_mainLayer->getChildByID("orbs-icon");
            if (!orbsIcon) {
                orbsIcon = CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
                orbsIcon->setScale(m_compactView ? 0.4f : 0.6f);
                orbsIcon->setPosition({ likesLabel->getPositionX() + likesLabel->getScaledContentWidth() + padding, likesLabel->getPositionY() });
                orbsIcon->setID("orbs-icon");
                m_mainLayer->addChild(orbsIcon);
            }
            auto orbsLabel = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("orbs-label"));
            if (!orbsLabel) {
                orbsLabel = CCLabelBMFont::create("", "bigFont.fnt");
                orbsLabel->setScale(m_compactView ? 0.3f : 0.4f);
                orbsLabel->setAnchorPoint({ 0.0f, 0.5f });
                orbsLabel->setPosition({ orbsIcon->getPositionX() + 10.0f, orbsIcon->getPositionY() });
                orbsLabel->setID("orbs-label");
                m_mainLayer->addChild(orbsLabel);
            }
            auto orbs = gsm->getBaseCurrency(data->stars, false, 0);
            int totalOrbs = floor(orbs * 1.25);
            auto percent = level->m_normalPercent.value();
            auto savedLevel = GameLevelManager::get()->getSavedLevel(level);
            if (savedLevel) percent = savedLevel->m_normalPercent.value();
            orbsLabel->setString(
                (percent == 100 ? fmt::format("{}", totalOrbs) : fmt::format("{}/{}", (int)floor(orbs * percent / 100.0), totalOrbs)).c_str());
            orbsLabel->limitLabelWidth(45.0f, m_compactView ? 0.3f : 0.4f, 0.0f);
            if (percent == 100) orbsLabel->setColor({ 100, 255, 255 });
        }
        else {
            if (auto orbsIcon = m_mainLayer->getChildByID("orbs-icon")) orbsIcon->removeFromParent();
            if (auto orbsLabel = m_mainLayer->getChildByID("orbs-label")) orbsLabel->removeFromParent();
        }

        auto& position = difficultySprite->getPosition();
        auto mdo = data->moreDifficultiesOverride;
        auto gdo = data->grandpaDemonOverride;
        auto dbo = data->demonsInBetweenOverride;
        auto gio = data->gddpIntegrationOverride;

        if (auto moreDifficulties = loader->getLoadedMod("uproxide.more_difficulties")) {
            auto moreDifficultiesSprite = static_cast<CCSprite*>(difficultyContainer->getChildByID(
                "uproxide.more_difficulties/more-difficulties-spr"));
            if (moreDifficultiesSprite) moreDifficultiesSprite->setVisible(false);

            difficultySprite->setOpacity(255);
            if (mdo == 4 && !moreDifficulties->getSavedValue("casual", true)) mdo = 0;
            if (mdo == 7 && !moreDifficulties->getSavedValue("tough", true)) mdo = 0;
            if (mdo == 9 && !moreDifficulties->getSavedValue("cruel", true)) mdo = 0;

            if ((mdo == 4 || mdo == 7 || mdo == 9) && gdo <= 0 && dbo <= 0 && gio <= 0) {
                auto legacy = moreDifficulties->getSettingValue<bool>("legacy-difficulties");
                if (!moreDifficultiesSprite) {
                    moreDifficultiesSprite = CCSprite::createWithSpriteFrameName(fmt::format("uproxide.more_difficulties/MD_Difficulty{:02d}{}.png",
                        mdo, legacy ? "_Legacy" : "").c_str());
                    moreDifficultiesSprite->setID("uproxide.more_difficulties/more-difficulties-spr");
                    difficultyContainer->addChild(moreDifficultiesSprite, 3);
                }
                else {
                    moreDifficultiesSprite->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(
                        fmt::format("uproxide.more_difficulties/MD_Difficulty{:02d}{}.png", mdo, legacy ? "_Legacy" : "").c_str()));
                    moreDifficultiesSprite->setVisible(true);
                }

                moreDifficultiesSprite->setPosition({
                    difficultySprite->getPositionX() + (legacy ? 0.0f : 0.25f),
                    difficultySprite->getPositionY() - (legacy ? 0.0f : 0.1f)
                });
                difficultySprite->setOpacity(0);
            }
        }

        if (loader->isModLoaded("itzkiba.grandpa_demon") && gdo > 0 && gdo < 7) {
            auto grdSprite = CCSprite::createWithSpriteFrameName(fmt::format("itzkiba.grandpa_demon/GrD_demon{}.png", gdo - 1).c_str());
            grdSprite->setID("grandpa-demon-sprite"_spr);
            grdSprite->setPosition(position);
            difficultyContainer->addChild(grdSprite, 3);
            if (gdo == 5) {
                auto grdInfinity = FREffects::grdInfinity();
                grdInfinity->setID("grandpa-demon-infinity"_spr);
                grdInfinity->setPosition(position + CCPoint { -0.4f, 14.0f });
                difficultyContainer->addChild(grdInfinity);
            }
            difficultySprite->setOpacity(0);
        }

        if (auto demonsInBetween = loader->getLoadedMod("hiimjustin000.demons_in_between"); demonsInBetween && dbo > 0 && dbo < 21) {
            if (demonsInBetween->getSettingValue<bool>("enable-difficulties")) {
                auto dibFeature = "";
                if (data->feature == 3 && demonsInBetween->getSettingValue<bool>("enable-legendary")) dibFeature = "_4";
                else if (data->feature == 4 && demonsInBetween->getSettingValue<bool>("enable-mythic")) dibFeature = "_5";
                auto dibSprite = CCSprite::createWithSpriteFrameName(
                    fmt::format("hiimjustin000.demons_in_between/DIB_{:02d}{}_btn_001.png", dbo, dibFeature).c_str());
                dibSprite->setID("between-difficulty-sprite"_spr);
                dibSprite->setPosition(position + FakeRate::getDIBOffset(dbo, GJDifficultyName::Short));
                difficultyContainer->addChild(dibSprite, 3);
                difficultySprite->setOpacity(0);
            }
        }

        if (auto gddpIntegration = loader->getLoadedMod("minemaker0430.gddp_integration"); gddpIntegration && gio > 0 && gio < 17 &&
            gddpIntegration->getSettingValue<bool>("custom-difficulty-faces")) {
            auto gddpSprite = CCSprite::createWithSpriteFrameName(FakeRate::getGDDPFrame(gio, GJDifficultyName::Short).c_str());
            gddpSprite->setID("gddp-difficulty"_spr);
            gddpSprite->setPosition(position + CCPoint { 0.5f, 0.0f });
            difficultyContainer->addChild(gddpSprite, 3);
            difficultySprite->setOpacity(0);
        }
    }
};
