#include "../classes/FREditPopup.hpp"
#include "../classes/FREffects.hpp"
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

class $modify(FRLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        FakeRateSaveData m_fakeRateData;
        CCSprite* m_grandpaDemon;
        CCSprite* m_grandpaInfinity;
        CCSprite* m_betweenDemon;
        CCSprite* m_gddpDemon;
        CCSprite* m_grandpaBackground1;
        CCSprite* m_grandpaBackground2;
        CCParticleSystemQuad* m_grandpaParticles1;
        CCParticleSystemQuad* m_grandpaParticles2;
        ccColor3B m_backgroundColor;
        bool m_fakeRateApplied;
    };

    static void onModify(ModifyBase<ModifyDerive<FRLevelInfoLayer, LevelInfoLayer>>& self) {
        (void)self.setHookPriority("LevelInfoLayer::init", -100);
        (void)self.setHookPriority("LevelInfoLayer::levelDownloadFinished", -100);
        (void)self.setHookPriority("LevelInfoLayer::likedItem", -100);
    }

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        m_fields->m_backgroundColor = static_cast<CCSprite*>(getChildByID("background"))->getColor();

        auto buttonSprite = CircleButtonSprite::createWithSprite(
            "FR_fakeRateBtn_001.png"_spr, 1.0f, CircleBaseColor::Green, CircleBaseSize::Medium);
        buttonSprite->getTopNode()->setScale(1.0f);
        auto fakeRateButton = CCMenuItemSpriteExtra::create(buttonSprite, this, menu_selector(FRLevelInfoLayer::onFakeRate));
        fakeRateButton->setID("fake-rate-button"_spr);
        auto leftSideMenu = getChildByID("left-side-menu");
        leftSideMenu->addChild(fakeRateButton);
        leftSideMenu->updateLayout();

        checkFakeRate();

        return true;
    }

    void onFakeRate(CCObject* sender) {
        FREditPopup::create(m_level, m_fields->m_fakeRateData, [this](FakeRateSaveData data, bool remove) {
            updateFakeRate(data, remove);
        })->show();
    }

    void levelDownloadFinished(GJGameLevel* level) override {
        LevelInfoLayer::levelDownloadFinished(level);
        checkFakeRate();
    }

    void likedItem(LikeItemType type, int id, bool liked) override {
        LevelInfoLayer::likedItem(type, id, liked);
        checkFakeRate();
    }

    void checkFakeRate() {
        if (auto data = FakeRate::getFakeRate(m_level)) updateFakeRate(*data, false);
        else defaultFakeRate();
    }

    void defaultFakeRate() {
        auto level = m_level;
        auto stars = level->m_stars.value();
        auto starsRequested = level->m_starsRequested;
        auto grandpaDemon = static_cast<CCSprite*>(getChildByID("grd-difficulty"));
        auto demonInBetween = static_cast<CCSprite*>(getChildByID("hiimjustin000.demons_in_between/between-difficulty-sprite"));
        auto gddpDifficulty = static_cast<CCSprite*>(getChildByID("gddp-difficulty"));
        auto loader = Loader::get();
        auto gddpIntegration = loader->getLoadedMod("minemaker0430.gddp_integration");
        auto gddpOverride = gddpIntegration ? gddpIntegration->getSettingValue<bool>("override-grandpa-demon") : false;
        auto mdo = 0;
        if (auto moreDifficulties = loader->getLoadedMod("uproxide.more_difficulties")) {
            auto casual = moreDifficulties->getSavedValue<bool>("casual", true);
            auto tough = moreDifficulties->getSavedValue<bool>("tough", true);
            auto cruel = moreDifficulties->getSavedValue<bool>("cruel", true);
            if (stars == 4 && casual) mdo = 4;
            else if (stars == 7 && tough) mdo = 7;
            else if (stars == 9 && cruel) mdo = 9;
            else if (stars == 0) {
                if (starsRequested == 4 && casual) mdo = 4;
                else if (starsRequested == 7 && tough) mdo = 7;
                else if (starsRequested == 9 && cruel) mdo = 9;
            }
        }
        auto& data = m_fields->m_fakeRateData;
        data.id = level->m_levelID.value();
        data.stars = stars;
        data.feature = level->m_featured > 0 ? level->m_isEpic + 1 : 0;
        data.difficulty = FakeRate::getDifficultyFromLevel(m_level);
        data.moreDifficultiesOverride = mdo;
        data.grandpaDemonOverride = grandpaDemon && (!gddpOverride || !gddpDifficulty) ? FakeRate::getGRDOverride(grandpaDemon) : 0;
        data.demonsInBetweenOverride = demonInBetween ? FakeRate::getDIBOverride(demonInBetween) : 0;
        data.gddpIntegrationOverride =
            gddpDifficulty && (!grandpaDemon || gddpOverride) && !demonInBetween ? FakeRate::getGDDPOverride(gddpDifficulty) : 0;
        data.coins = level->m_coinsVerified > 0;
    }

    void updateFakeRate(const FakeRateSaveData& data, bool remove) {
        auto [levelID, stars, feature, difficulty, mdo, gdo, dbo, gio, coins] = data;
        auto level = m_level;
        auto f = m_fields.self();
        f->m_fakeRateData = data;

        auto loader = Loader::get();

        if (!f->m_fakeRateApplied) {
            f->m_fakeRateApplied = true;
            f->m_backgroundColor = static_cast<CCSprite*>(getChildByID("background"))->getColor();
            auto gdutils = loader->getLoadedMod("gdutilsdevs.gdutils");
            if (gdutils && gdutils->getSettingValue<bool>("activate-background")) {
                f->m_backgroundColor = gdutils->getSettingValue<ccColor3B>("background");
            }
        }

        auto hide = false;
        if (auto betweenDifficultySprite = static_cast<CCSprite*>(getChildByID("hiimjustin000.demons_in_between/between-difficulty-sprite"))) {
            betweenDifficultySprite->setVisible(remove);
            m_difficultySprite->setOpacity(remove ? 0 : 255);
            hide = remove || hide;
            f->m_fakeRateData.demonsInBetweenOverride = remove ? FakeRate::getDIBOverride(betweenDifficultySprite) : dbo;
        }
        auto gddpOverride = false;
        auto animatedFire = loader->isModLoaded("uproxide.animated_fire");
        auto gddpIntegration = loader->getLoadedMod("minemaker0430.gddp_integration");
        if (auto gddpDifficultySprite = static_cast<CCSprite*>(getChildByID("gddp-difficulty"))) {
            gddpOverride = gddpIntegration ? gddpIntegration->getSettingValue<bool>("override-grandpa-demon") : false;
            gddpDifficultySprite->setVisible(remove);
            m_difficultySprite->setOpacity(remove ? 0 : 255);
            if (FakeRate::getSpriteName(gddpDifficultySprite).contains("Plus")) {
                if (auto epicSprite = getChildBySpriteFrameName(m_difficultySprite, "GJ_epicCoin_001.png")) {
                    epicSprite->setVisible(!animatedFire && !remove);
                }
            }
            hide = remove || hide;
            f->m_fakeRateData.gddpIntegrationOverride = remove ? FakeRate::getGDDPOverride(gddpDifficultySprite) : gio;
        }
        auto hasEffects = false;
        auto winSize = CCDirector::get()->getWinSize();
        auto grandpaDemon = loader->getLoadedMod("itzkiba.grandpa_demon");
        if (grandpaDemon && !gddpOverride) {
            removeChildByTag(69420);
            if (animatedFire) removeChildByTag(69420);
            auto hasDemon = false;
            for (auto child : CCArrayExt<CCNode*>(getChildren())) {
                auto& childID = child->getID();
                if (childID == "grd-difficulty") {
                    child->setVisible(remove);
                    hasDemon = true;
                }

                auto bgNum = 0;
                if (isSpriteName(child, "itzkiba.grandpa_demon/GrD_demon4_bg.png") &&
                    child != f->m_grandpaBackground1 && child != f->m_grandpaBackground2) {
                    if (childID == "") child->setID(fmt::format("grd-bg-{}", ++bgNum));
                    if (childID == "grd-bg-1") child->setVisible(remove);
                    else if (childID == "grd-bg-2") child->setVisible(remove);
                    hasDemon = true;
                    hasEffects = true;
                }

                auto particleNum = 0;
                if (typeinfo_cast<CCParticleSystemQuad*>(child) && child->getPositionY() == winSize.height / 2 + 76.0f &&
                    child != f->m_grandpaParticles1 && child != f->m_grandpaParticles2) {
                    if (childID == "") child->setID(fmt::format("grd-particles-{}", ++particleNum));
                    if (childID == "grd-particles-1") child->setVisible(remove);
                    else if (childID == "grd-particles-2") child->setVisible(remove);
                    hasDemon = true;
                    hasEffects = true;
                }
            }
            if (auto grdInfinity = getChildByID("grd-infinity")) grdInfinity->setVisible(remove);
            m_difficultySprite->setVisible(true);
            m_difficultySprite->setOpacity(hasDemon && remove ? 0 : 255);
            hide = (hasDemon && remove) || hide;
            if (auto featureGlow = m_difficultySprite->getChildByTag(69420)) {
                featureGlow->setPosition(m_difficultySprite->getContentSize() / 2);
            }
            f->m_fakeRateData.grandpaDemonOverride =
                hasDemon && remove ? FakeRate::getGRDOverride(static_cast<CCSprite*>(getChildByID("grd-difficulty"))) : gdo;
        }

        auto gsm = GameStatsManager::get();
        auto dailyID = level->m_dailyID.value();
        auto showStars = stars != 0 || dailyID > 0 || level->m_gauntletLevel;
        m_difficultySprite->updateFeatureState((GJFeatureState)feature);
        m_difficultySprite->updateDifficultyFrame(difficulty, GJDifficultyName::Long);
        auto isDemon = difficulty > 5 || gdo > 0 || dbo > 0 || gio > 0;
        m_difficultySprite->setPositionY(winSize.height / 2 + 56.0f + (isDemon ? 5.0f : 0.0f) + (showStars ? 10.0f : 0.0f));
        auto& position = m_difficultySprite->getPosition();
        m_starsIcon->setPosition({ position.x + 8.0f, position.y - 30.0f - (isDemon ? 9.0f : 0.0f) });
        m_starsIcon->setVisible(showStars);
        m_starsLabel->setPosition({ m_starsIcon->getPositionX() - 8.0f, m_starsIcon->getPositionY() });
        m_starsLabel->setString(fmt::to_string(stars).c_str());
        m_starsLabel->setVisible(showStars);
        if (gsm->hasCompletedLevel(level)) m_starsLabel->setColor({ 255, 255, 50 });

        auto coinY = position.y - 31.5f - (showStars ? 14.0f : 0.0f)
            - (level->m_gauntletLevel || dailyID > 0 ? 14.5f : 0.0f) - (isDemon ? 9.0f : 0.0f);
        for (int i = 0; i < m_coins->count(); i++) {
            auto coin = static_cast<CCSprite*>(m_coins->objectAtIndex(i));
            coin->setPositionY(coinY);
            auto coinKey = level->getCoinKey(i + 1);
            auto hasCoin = gsm->hasUserCoin(coinKey) || gsm->hasPendingUserCoin(coinKey);
            if (coins) coin->setColor(hasCoin ? ccColor3B { 255, 255, 255 } : ccColor3B { 165, 165, 165 });
            else coin->setColor(hasCoin ? ccColor3B { 255, 175, 75 } : ccColor3B { 165, 113, 48 });
        }

        auto normalPercent = level->m_normalPercent.value();
        if (dailyID > 0 || level->m_gauntletLevel) {
            auto diamondLabel = static_cast<CCLabelBMFont*>(getChildByID("diamond-label"));
            auto diamondIcon = getChildByID("diamond-icon");
            auto diamonds = gsm->getBaseDiamonds(stars);
            diamondLabel->setString(fmt::format("{}/{}", (int)floor(diamonds * normalPercent / 100.0), diamonds).c_str());
            diamondIcon->setPosition({
                position.x + diamondLabel->getScaledContentWidth() * 0.5f + 2.0f,
                position.y - (isDemon ? 9.0f : 0.0f) - 44.5f
            });
            diamondLabel->setPosition({ diamondIcon->getPositionX() - 8.0f, diamondIcon->getPositionY() });
        }

        auto yPos = winSize.height / 2 + 51.0f + (showStars ? 10.0f : 0.0f);
        auto yOffset = showStars ? 28.0f : 30.0f;
        auto downloadsIcon = static_cast<CCNode*>(m_icons->objectAtIndex(1));
        downloadsIcon->setPositionY(yPos + yOffset);
        m_downloadsLabel->setPositionY(downloadsIcon->getPositionY());
        m_likesIcon->setPositionY(yPos + 1.0f);
        m_likesLabel->setPositionY(m_likesIcon->getPositionY());
        auto lengthIcon = static_cast<CCNode*>(m_icons->objectAtIndex(0));
        lengthIcon->setPositionY(yPos - yOffset);
        m_lengthLabel->setPositionY(lengthIcon->getPositionY() + (m_exactLengthLabel->isVisible() ? 6.0f : 0.0f));
        m_orbsIcon->setVisible(showStars);
        m_orbsLabel->setVisible(showStars);
        if (showStars) {
            m_orbsIcon->setPositionY(yPos - yOffset * 2.0f);
            m_orbsLabel->setPositionY(m_orbsIcon->getPositionY());
            auto orbs = gsm->getBaseCurrency(stars, false, 0);
            int totalOrbs = floor(orbs * 1.25);
            m_orbsLabel->setString(
                fmt::format("{}/{}", (int)floor(normalPercent != 100 ? orbs * normalPercent / 100.0 : totalOrbs), totalOrbs).c_str());
            m_orbsLabel->limitLabelWidth(60.0f, 0.5f, 0.0f);
        }
        if (m_exactLengthLabel->isVisible()) m_exactLengthLabel->setPositionY(m_lengthLabel->getPositionY() - 14.0f);

        m_difficultySprite->setOpacity(hide ? 0 : 255);

        if (auto moreDifficulties = loader->getLoadedMod("uproxide.more_difficulties")) {
            auto moreDifficultiesSprite = static_cast<CCSprite*>(getChildByID("uproxide.more_difficulties/more-difficulties-spr"));
            if (moreDifficultiesSprite) moreDifficultiesSprite->setVisible(false);
            m_difficultySprite->setOpacity(hide ? 0 : 255);

            auto moreDifficultiesOverride = mdo;
            auto starsRequested = level->m_starsRequested;
            auto stars = level->m_stars.value();
            if (remove && stars == 0 && (starsRequested == 4 || starsRequested == 7 || starsRequested == 9)) {
                moreDifficultiesOverride = starsRequested;
            }
            if (remove && (stars == 4 || stars == 7 || stars == 9)) moreDifficultiesOverride = stars;
            if (moreDifficultiesOverride == 4 && !moreDifficulties->getSavedValue("casual", true)) moreDifficultiesOverride = 0;
            if (moreDifficultiesOverride == 7 && !moreDifficulties->getSavedValue("tough", true)) moreDifficultiesOverride = 0;
            if (moreDifficultiesOverride == 9 && !moreDifficulties->getSavedValue("cruel", true)) moreDifficultiesOverride = 0;

            if (
                (moreDifficultiesOverride == 4 || moreDifficultiesOverride == 7 || moreDifficultiesOverride == 9)
                && gdo <= 0 && dbo <= 0 && gio <= 0
            ) {
                auto legacy = moreDifficulties->getSettingValue<bool>("legacy-difficulties");
                if (!moreDifficultiesSprite) {
                    moreDifficultiesSprite = CCSprite::createWithSpriteFrameName(fmt::format("uproxide.more_difficulties/MD_Difficulty{:02d}{}.png",
                        moreDifficultiesOverride, legacy ? "_Legacy" : "").c_str());
                    moreDifficultiesSprite->setID("uproxide.more_difficulties/more-difficulties-spr");
                    addChild(moreDifficultiesSprite, 3);
                }
                else {
                    moreDifficultiesSprite->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(fmt::format(
                        "uproxide.more_difficulties/MD_Difficulty{:02d}{}.png", moreDifficultiesOverride, legacy ? "_Legacy" : "").c_str()));
                    moreDifficultiesSprite->setVisible(true);
                }

                moreDifficultiesSprite->setPosition({
                    m_difficultySprite->getPositionX() + (legacy ? 0.0f : 0.25f),
                    m_difficultySprite->getPositionY() - (legacy ? 0.0f : 0.1f)
                });
                m_difficultySprite->setOpacity(0);
            }
        }

        auto showBackground =
            (hasEffects && remove) || (gdo > 2 && (grandpaDemon ? !grandpaDemon->getSettingValue<bool>("infinite-demon-disable") : false));
        auto background = static_cast<CCSprite*>(getChildByID("background"));
        background->setZOrder(showBackground ? -10 : -2);
        background->setOpacity(showBackground ? 50 : 255);
        background->setColor(
            (gddpOverride && remove) || (gio > 0 && (gddpIntegration ? !gddpIntegration->getSettingValue<bool>("restore-bg-color") : false)) ?
            ccColor3B { 18, 18, 86 } : f->m_backgroundColor);
        static_cast<CCSprite*>(getChildByID("bottom-left-art"))->setOpacity(showBackground ? 50 : 255);
        static_cast<CCSprite*>(getChildByID("bottom-right-art"))->setOpacity(showBackground ? 50 : 255);

        if (f->m_grandpaDemon) {
            f->m_grandpaDemon->removeFromParent();
            f->m_grandpaDemon = nullptr;
        }
        if (f->m_grandpaInfinity) {
            f->m_grandpaInfinity->removeFromParent();
            f->m_grandpaInfinity = nullptr;
        }
        if (f->m_grandpaBackground1) {
            f->m_grandpaBackground1->removeFromParent();
            f->m_grandpaBackground1 = nullptr;
        }
        if (f->m_grandpaBackground2) {
            f->m_grandpaBackground2->removeFromParent();
            f->m_grandpaBackground2 = nullptr;
        }
        if (f->m_grandpaParticles1) {
            f->m_grandpaParticles1->removeFromParent();
            f->m_grandpaParticles1 = nullptr;
        }
        if (f->m_grandpaParticles2) {
            f->m_grandpaParticles2->removeFromParent();
            f->m_grandpaParticles2 = nullptr;
        }

        if (grandpaDemon && gdo > 0 && gdo < 7) {
            f->m_grandpaDemon = CCSprite::createWithSpriteFrameName(fmt::format("itzkiba.grandpa_demon/GrD_demon{}_text.png", gdo - 1).c_str());
            f->m_grandpaDemon->setID("grandpa-demon-sprite"_spr);
            f->m_grandpaDemon->setPosition(position);
            addChild(f->m_grandpaDemon, 13);
            if (gdo == 5) {
                f->m_grandpaInfinity = FREffects::grdInfinity();
                f->m_grandpaInfinity->setID("grandpa-demon-infinity"_spr);
                f->m_grandpaInfinity->setPosition(position + CCPoint { -0.4f, 14.0f });
                addChild(f->m_grandpaInfinity);
            }

            if (showBackground) {
                switch (gdo) {
                    case 3: {
                        f->m_grandpaBackground1 = FREffects::legendaryBackground();
                        f->m_grandpaBackground1->setID("grandpa-background-1"_spr);
                        addChild(f->m_grandpaBackground1);
                        break;
                    }
                    case 4: {
                        f->m_grandpaBackground1 = FREffects::mythicalBackground();
                        f->m_grandpaBackground1->setID("grandpa-background-1"_spr);
                        addChild(f->m_grandpaBackground1);
                        break;
                    }
                    case 5: case 6: {
                        auto backgrounds = FREffects::infinityBackgrounds(gdo == 6);
                        f->m_grandpaBackground1 = backgrounds.first;
                        f->m_grandpaBackground1->setID("grandpa-background-1"_spr);
                        addChild(f->m_grandpaBackground1);
                        f->m_grandpaBackground2 = backgrounds.second;
                        f->m_grandpaBackground2->setID("grandpa-background-2"_spr);
                        addChild(f->m_grandpaBackground2);
                        break;
                    }
                }
            }

            if (!grandpaDemon->getSettingValue<bool>("particles-disable") && gdo > 2) {
                auto particlePos = position + CCPoint { 0.0f, 5.0f };
                if (gdo < 7) {
                    constexpr std::array particles = {
                        "legendaryEffect.plist"_spr, "mythicalEffect.plist"_spr, "infiniteEffect.plist"_spr, "grandpaEffect.plist"_spr
                    };
                    f->m_grandpaParticles1 = CCParticleSystemQuad::create();
                    f->m_grandpaParticles1->initWithDictionary(CCDictionary::createWithContentsOfFile(particles[gdo - 3]), false);
                    f->m_grandpaParticles1->setPosition(particlePos);
                    f->m_grandpaParticles1->setID("grandpa-particles-1"_spr);
                    addChild(f->m_grandpaParticles1);
                }
                if (gdo > 4 && gdo < 7) {
                    f->m_grandpaParticles2 = CCParticleSystemQuad::create();
                    f->m_grandpaParticles2->initWithDictionary(CCDictionary::createWithContentsOfFile("infiniteSunEffect.plist"_spr), false);
                    f->m_grandpaParticles2->setPosition(particlePos);
                    f->m_grandpaParticles2->setID("grandpa-particles-2"_spr);
                    addChild(f->m_grandpaParticles2);
                }
            }

            m_difficultySprite->setOpacity(0);
        }

        if (f->m_betweenDemon) {
            f->m_betweenDemon->removeFromParent();
            f->m_betweenDemon = nullptr;
        }

        if (auto demonsInBetween = loader->getLoadedMod("hiimjustin000.demons_in_between"); demonsInBetween && dbo > 0 && dbo < 21) {
            auto dibFeature = "";
            if (feature == 3 && demonsInBetween->getSettingValue<bool>("enable-legendary")) dibFeature = "_4";
            else if (feature == 4 && demonsInBetween->getSettingValue<bool>("enable-mythic")) dibFeature = "_5";
            f->m_betweenDemon = CCSprite::createWithSpriteFrameName(
                fmt::format("hiimjustin000.demons_in_between/DIB_{:02d}{}_btn2_001.png", dbo, dibFeature).c_str());
            f->m_betweenDemon->setID("between-difficulty-sprite"_spr);
            f->m_betweenDemon->setPosition(position + FakeRate::getDIBOffset(dbo, GJDifficultyName::Long));
            addChild(f->m_betweenDemon, 3);
            m_difficultySprite->setOpacity(0);
        }

        if (f->m_gddpDemon) {
            f->m_gddpDemon->removeFromParent();
            f->m_gddpDemon = nullptr;
        }

        if (gddpIntegration && gio > 0 && gio < 17) {
            f->m_gddpDemon = CCSprite::createWithSpriteFrameName(FakeRate::getGDDPFrame(gio, GJDifficultyName::Long).c_str());
            f->m_gddpDemon->setID("gddp-difficulty"_spr);
            f->m_gddpDemon->setPosition(position + CCPoint { 0.25f, 30.0f });
            f->m_gddpDemon->setAnchorPoint({ 0.5f, 1.0f });
            addChild(f->m_gddpDemon, 3);
            m_difficultySprite->setOpacity(0);
        }
    }
};

#ifdef GEODE_IS_MACOS // Stupid likedItem inline expansion hook
static_assert(GEODE_COMP_GD_VERSION == 22074, "Please update this hook for the current GD version");

uintptr_t likedItemAddress = base::get() + GEODE_ARM_MAC(0x256f58) GEODE_INTEL_MAC(0x2b1040);
void likedItemHook(LikeItemDelegate* self, LikeItemType type, int id, bool liked) {
    reinterpret_cast<void(*)(LikeItemDelegate*, LikeItemType, int, bool)>(likedItemAddress)(self, type, id, liked);
    base_cast<FRLevelInfoLayer*>(self)->checkFakeRate();
}

$execute {
    auto hook = Hook::create(
        reinterpret_cast<void*>(likedItemAddress),
        &likedItemHook,
        "LevelInfoLayer::LikeItemDelegate::likedItem",
        tulip::hook::TulipConvention::Default
    );
    hook->setPriority(-100);
    if (auto res = Mod::get()->claimHook(hook); res.isErr()) {
        log::error("Failed to hook LevelInfoLayer::LikeItemDelegate::likedItem: {}", res.unwrapErr());
    }
};
#endif
