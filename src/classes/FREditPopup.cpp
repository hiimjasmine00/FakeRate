#include "FREditPopup.hpp"
#include "FREffects.hpp"
#include "FRSetDifficultyPopup.hpp"
#include "FRSetFeaturePopup.hpp"
#include "FRSetStarsPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

FREditPopup* FREditPopup::create(GJGameLevel* level, const FakeRateSaveData& data, UpdateFakeRateCallback callback) {
    auto ret = new FREditPopup();
    if (ret->initAnchored(300.0f, 200.0f, level, data, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FREditPopup::setup(GJGameLevel* level, const FakeRateSaveData& data, UpdateFakeRateCallback callback) {
    setID("FREditPopup");
    setTitle("Fake Rate");
    m_title->setID("fake-rate-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_level = level;
    m_stars = data.stars;
    m_feature = data.feature;
    m_difficulty = data.difficulty;
    m_moreDifficultiesOverride = data.moreDifficultiesOverride;
    m_grandpaDemonOverride = data.grandpaDemonOverride;
    m_demonsInBetweenOverride = data.demonsInBetweenOverride;
    m_gddpIntegrationOverride = data.gddpIntegrationOverride;
    m_coins = data.coins;
    m_legacy = false;

    m_difficultySprite = GJDifficultySprite::create(data.difficulty, GJDifficultyName::Long);
    m_difficultySprite->setPositionX(60.0f);
    m_difficultySprite->setID("difficulty-sprite");
    m_mainLayer->addChild(m_difficultySprite);

    if (Loader::get()->isModLoaded("uproxide.more_difficulties")) {
        m_legacy = Loader::get()->getLoadedMod("uproxide.more_difficulties")->getSettingValue<bool>("legacy-difficulties");
        m_mdSprite = CCSprite::createWithSpriteFrameName(m_legacy ?
            "uproxide.more_difficulties/MD_Difficulty04_Legacy.png" : "uproxide.more_difficulties/MD_Difficulty04.png");
        m_mdSprite->setPosition(m_legacy ? CCPoint { 60.0f, 100.0f } : CCPoint { 60.25f, 99.9f });
        m_mdSprite->setVisible(false);
        m_mdSprite->setID("md-sprite");
        m_mainLayer->addChild(m_mdSprite);
    }
    if (Loader::get()->isModLoaded("itzkiba.grandpa_demon")) {
        m_grdSprite = CCSprite::createWithSpriteFrameName("itzkiba.grandpa_demon/GrD_demon0_text.png");
        m_grdSprite->setPosition({ 60.0f, 105.0f });
        m_grdSprite->setVisible(false);
        m_grdSprite->setID("grd-sprite");
        m_mainLayer->addChild(m_grdSprite);

        m_grdInfinity = FREffects::grdInfinity();
        m_grdInfinity->setPosition({ 59.6f, 119.0f });
        m_grdInfinity->setVisible(false);
        m_grdInfinity->setID("grd-infinity-sprite");
        m_mainLayer->addChild(m_grdInfinity);
    }
    if (Loader::get()->isModLoaded("hiimjustin000.demons_in_between")) {
        m_dibSprite = CCSprite::createWithSpriteFrameName("hiimjustin000.demons_in_between/DIB_01_btn2_001.png");
        m_dibSprite->setPosition({ 60.0f, 105.0f });
        m_dibSprite->setVisible(false);
        m_dibSprite->setID("dib-sprite");
        m_mainLayer->addChild(m_dibSprite);
    }
    if (Loader::get()->isModLoaded("minemaker0430.gddp_integration")) {
        m_gddpSprite = CCSprite::createWithSpriteFrameName("minemaker0430.gddp_integration/DP_BeginnerText.png");
        m_gddpSprite->setPosition({ 60.25f, 140.0f });
        m_gddpSprite->setAnchorPoint({ 0.5f, 1.0f });
        m_gddpSprite->setVisible(false);
        m_gddpSprite->setID("gddp-sprite");
        m_mainLayer->addChild(m_gddpSprite);
    }

    m_starSprite = CCSprite::createWithSpriteFrameName(m_level->m_levelLength < 5 ? "star_small01_001.png" : "moon_small01_001.png");
    m_starSprite->setID("star-sprite");
    m_mainLayer->addChild(m_starSprite);

    m_starsLabel = CCLabelBMFont::create(std::to_string(data.stars).c_str(), "bigFont.fnt");
    m_starsLabel->setScale(0.4f);
    m_starsLabel->setAnchorPoint({ 1.0f, 0.5f });
    m_starsLabel->setID("stars-label");
    m_mainLayer->addChild(m_starsLabel);

    m_coinSprites = CCArray::create();
    m_coinSprites->retain();

    for (int i = 0; i < m_level->m_coins; i++) {
        auto coin = CCSprite::createWithSpriteFrameName("usercoin_small01_001.png");
        coin->setPositionX(60.0f + (
            i == 2 ? 10.0f :
            i == 1 && m_level->m_coins == 2 ? 5.0f :
            i == 0 && m_level->m_coins == 2 ? -5.0f :
            i == 0 && m_level->m_coins == 3 ? -10.0f : 0.0f
        ));
        coin->setID(fmt::format("coin-sprite-{}", i + 1));
        m_mainLayer->addChild(coin);
        m_coinSprites->addObject(coin);
    }

    auto difficultyButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Difficulty", "goldFont.fnt", "GJ_button_02.png", 0.8f), [this](auto) {
        FRSetDifficultyPopup::create({
            .difficulty = m_difficulty,
            .moreDifficultiesOverride = m_moreDifficultiesOverride,
            .grandpaDemonOverride = m_grandpaDemonOverride,
            .demonsInBetweenOverride = m_demonsInBetweenOverride,
            .gddpIntegrationOverride = m_gddpIntegrationOverride
        }, m_legacy, [this](int difficulty, int mdo, int gdo, int dbo, int gio) {
            m_difficulty = difficulty;
            m_moreDifficultiesOverride = mdo;
            m_grandpaDemonOverride = gdo;
            m_demonsInBetweenOverride = dbo;
            m_gddpIntegrationOverride = gio;
            updateLabels();
        })->show();
    });
    difficultyButton->setPosition({ 200.0f, 150.0f });
    difficultyButton->setID("difficulty-button");
    m_buttonMenu->addChild(difficultyButton);

    auto starsButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create(m_level->m_levelLength < 5 ? "Stars" : "Moons",
        "goldFont.fnt", "GJ_button_02.png", 0.8f), [this](auto) {
        FRSetStarsPopup::create(m_stars, m_level->m_levelLength == 5, [this](int stars) {
            m_stars = stars;
            updateLabels();
        })->show();
    });
    starsButton->setPosition({ 200.0f, 110.0f });
    starsButton->setID("stars-button");
    m_buttonMenu->addChild(starsButton);

    auto featureButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Feature", "goldFont.fnt", "GJ_button_02.png", 0.8f), [this](auto) {
        FRSetFeaturePopup::create({
            .feature = m_feature,
            .difficulty = m_difficulty,
            .moreDifficultiesOverride = m_moreDifficultiesOverride,
            .grandpaDemonOverride = m_grandpaDemonOverride,
            .demonsInBetweenOverride = m_demonsInBetweenOverride,
            .gddpIntegrationOverride = m_gddpIntegrationOverride
        }, m_legacy, [this](int feature) {
            m_feature = feature;
            updateLabels();
        })->show();
    });
    featureButton->setPosition({ 200.0f, 70.0f });
    featureButton->setID("feature-button");
    m_buttonMenu->addChild(featureButton);

    auto coinsToggle = CCMenuItemExt::createTogglerWithStandardSprites(0.7f, [this](auto) {
        m_coins = !m_coins;
        updateLabels();
    });
    coinsToggle->setPosition({ 30.0f, 30.0f });
    coinsToggle->toggle(m_coins);
    coinsToggle->setID("coins-toggle");
    m_buttonMenu->addChild(coinsToggle);

    auto coinsLabel = CCLabelBMFont::create("Coins", "bigFont.fnt");
    coinsLabel->setScale(0.5f);
    coinsLabel->setPosition({ 70.0f, 30.0f });
    coinsLabel->setID("coins-label");
    m_mainLayer->addChild(coinsLabel);

    auto addButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Add", "goldFont.fnt", "GJ_button_01.png", 0.8f),
        [this, callback = std::move(callback)](auto) {
            auto levelID = m_level->m_levelID.value();
            FakeRateSaveData data = {
                .id = levelID,
                .stars = m_stars,
                .feature = m_feature,
                .difficulty = m_difficulty,
                .moreDifficultiesOverride = m_moreDifficultiesOverride,
                .grandpaDemonOverride = m_grandpaDemonOverride,
                .demonsInBetweenOverride = m_demonsInBetweenOverride,
                .gddpIntegrationOverride = m_gddpIntegrationOverride,
                .coins = m_coins
            };

            auto vec = Mod::get()->getSavedValue<std::vector<FakeRateSaveData>>("fake-rate", {});
            auto it = std::ranges::find_if(vec, [levelID](const FakeRateSaveData& item) { return item.id == levelID; });
            if (it != vec.end()) *it = data;
            else vec.push_back(data);
            Mod::get()->setSavedValue("fake-rate", vec);

            callback(data, false);
            onClose(nullptr);
        });
    addButton->setPosition({ 150.0f, 30.0f });
    addButton->setID("add-button");
    m_buttonMenu->addChild(addButton);

    auto removeButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Remove", "goldFont.fnt", "GJ_button_06.png", 0.8f),
        [this, callback = std::move(callback)](auto) {
            auto levelID = m_level->m_levelID.value();

            auto vec = Mod::get()->getSavedValue<std::vector<FakeRateSaveData>>("fake-rate", {});
            if (vec.empty()) return;

            auto vecSize = vec.size();
            ranges::remove(vec, [levelID](const FakeRateSaveData& item) { return item.id == levelID; });
            if (vec.size() == vecSize) return;

            Mod::get()->setSavedValue("fake-rate", vec);

            auto stars = m_level->m_stars.value();
            callback({
                .id = m_level->m_levelID,
                .stars = stars,
                .feature = m_level->m_featured > 1 ? m_level->m_isEpic + 1 : 0,
                .difficulty = FakeRate::getDifficultyFromLevel(m_level),
                .moreDifficultiesOverride =
                    Loader::get()->isModLoaded("uproxide.more_difficulties") && (stars == 4 || stars == 7 || stars == 9) ? stars : 0,
                .grandpaDemonOverride = 0,
                .demonsInBetweenOverride = 0,
                .gddpIntegrationOverride = 0,
                .coins = m_level->m_coinsVerified > 0
            }, true);
            onClose(nullptr);
        });
    removeButton->setPosition({ 235.0f, 30.0f });
    removeButton->setID("remove-button");
    m_buttonMenu->addChild(removeButton);

    updateLabels();

    return true;
}

void FREditPopup::updateLabels() {
    m_difficultySprite->updateFeatureState((GJFeatureState)m_feature);
    m_difficultySprite->updateDifficultyFrame(m_difficulty, GJDifficultyName::Long);
    auto isDemon = m_difficulty > 5 || m_grandpaDemonOverride > 0 || m_demonsInBetweenOverride > 0 || m_gddpIntegrationOverride > 0;
    m_difficultySprite->setPositionY(100.0f + (isDemon ? 5.0f : 0.0f) + (m_stars != 0 ? 10.0f : 0.0f));
    m_starSprite->setPosition({ m_difficultySprite->getPositionX() + 8.0f, m_difficultySprite->getPositionY() - 30.0f - (isDemon ? 9.0f : 0.0f) });
    m_starSprite->setVisible(m_stars != 0);
    m_starsLabel->setPosition({ m_starSprite->getPositionX() - 8.0f, m_starSprite->getPositionY() });
    m_starsLabel->setString(std::to_string(m_stars).c_str());
    m_starsLabel->setVisible(m_stars != 0);
    for (auto coin : CCArrayExt<CCSprite*>(m_coinSprites)) {
        coin->setPositionY(m_difficultySprite->getPositionY() - 31.5f - (m_stars != 0 ? 14.0f : 0.0f) - (isDemon ? 9.0f : 0.0f));
        coin->setColor(m_coins ? ccColor3B { 255, 255, 255 } : ccColor3B { 255, 175, 75 });
    }
    m_difficultySprite->setOpacity(255);
    auto sfc = CCSpriteFrameCache::get();
    if (auto moreDifficulties = Loader::get()->getLoadedMod("uproxide.more_difficulties")) {
        if (m_moreDifficultiesOverride == 4 && !moreDifficulties->getSavedValue("casual", true)) m_moreDifficultiesOverride = 0;
        if (m_moreDifficultiesOverride == 7 && !moreDifficulties->getSavedValue("tough", true)) m_moreDifficultiesOverride = 0;
        if (m_moreDifficultiesOverride == 9 && !moreDifficulties->getSavedValue("cruel", true)) m_moreDifficultiesOverride = 0;
        if (m_moreDifficultiesOverride == 4 || m_moreDifficultiesOverride == 7 || m_moreDifficultiesOverride == 9) {
            m_mdSprite->setDisplayFrame(sfc->spriteFrameByName(
                fmt::format("uproxide.more_difficulties/MD_Difficulty{:02d}{}.png", m_moreDifficultiesOverride, m_legacy ? "_Legacy" : "").c_str()));
            m_mdSprite->setPosition(m_difficultySprite->getPosition() + (m_legacy ? CCPoint { 0.0f, 0.0f } : CCPoint { 0.25f, -0.1f }));
            m_mdSprite->setVisible(true);
            m_difficultySprite->setOpacity(0);
        }
        else m_mdSprite->setVisible(false);
    }
    if (Loader::get()->isModLoaded("itzkiba.grandpa_demon")) {
        if (m_grandpaDemonOverride > 0 && m_grandpaDemonOverride < 7) {
            m_grdSprite->setDisplayFrame(sfc->spriteFrameByName(
                fmt::format("itzkiba.grandpa_demon/GrD_demon{}_text.png", m_grandpaDemonOverride - 1).c_str()));
            m_grdSprite->setPosition(m_difficultySprite->getPosition());
            m_grdSprite->setVisible(true);
            if (m_mdSprite) m_mdSprite->setVisible(false);
            m_grdInfinity->setPosition(m_difficultySprite->getPosition() + CCPoint { -0.4f, 14.0f });
            m_difficultySprite->setOpacity(0);
        }
        else m_grdSprite->setVisible(false);

        m_grdInfinity->setVisible(m_grandpaDemonOverride == 5);
    }
    if (auto demonsInBetween = Loader::get()->getLoadedMod("hiimjustin000.demons_in_between")) {
        if (!demonsInBetween->getSettingValue<bool>("enable-difficulties")) m_demonsInBetweenOverride = 0;
        if (m_demonsInBetweenOverride > 0 && m_demonsInBetweenOverride < 21) {
            auto demonsInBetween = Loader::get()->getLoadedMod("hiimjustin000.demons_in_between");
            auto dibFeature = "";
            if (m_feature == 3 && demonsInBetween->getSettingValue<bool>("enable-legendary")) dibFeature = "_4";
            else if (m_feature == 4 && demonsInBetween->getSettingValue<bool>("enable-mythic")) dibFeature = "_5";
            m_dibSprite->setDisplayFrame(sfc->spriteFrameByName(
                fmt::format("hiimjustin000.demons_in_between/DIB_{:02d}{}_btn2_001.png", m_demonsInBetweenOverride, dibFeature).c_str()));
            m_dibSprite->setPosition(m_difficultySprite->getPosition() + FakeRate::getDIBOffset(m_demonsInBetweenOverride, GJDifficultyName::Long));
            m_dibSprite->setVisible(true);
            if (m_mdSprite) m_mdSprite->setVisible(false);
            m_difficultySprite->setOpacity(0);
        }
        else m_dibSprite->setVisible(false);
    }
    if (auto gddpIntegration = Loader::get()->getLoadedMod("minemaker0430.gddp_integration")) {
        if (!gddpIntegration->getSettingValue<bool>("custom-difficulty-faces")) m_gddpIntegrationOverride = 0;
        if (m_gddpIntegrationOverride > 0 && m_gddpIntegrationOverride < 16) {
            m_gddpSprite->setDisplayFrame(sfc->spriteFrameByName(
                FakeRate::getGDDPFrame(m_gddpIntegrationOverride, GJDifficultyName::Long).c_str()));
            m_gddpSprite->setPosition(m_difficultySprite->getPosition() + CCPoint { 0.25f, 30.0f });
            m_gddpSprite->setVisible(true);
            if (m_mdSprite) m_mdSprite->setVisible(false);
            m_difficultySprite->setOpacity(0);
        }
        else m_gddpSprite->setVisible(false);
    }
}

FREditPopup::~FREditPopup() {
    CC_SAFE_RELEASE(m_coinSprites);
}
