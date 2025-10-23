#include "FRSetFeaturePopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

FRSetFeaturePopup* FRSetFeaturePopup::create(const FakeRateSaveData& data, bool legacy, SetFeatureCallback callback) {
    auto ret = new FRSetFeaturePopup();
    if (ret->initAnchored(300.0f, 150.0f, data, legacy, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FRSetFeaturePopup::setup(const FakeRateSaveData& data, bool legacy, SetFeatureCallback callback) {
    setID("FRSetFeaturePopup");
    setTitle("Select Feature");
    m_title->setID("select-feature-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;
    m_feature = (GJFeatureState)data.feature;
    m_difficulty = data.difficulty;
    m_moreDifficultiesOverride = data.moreDifficultiesOverride;
    m_grandpaDemonOverride = data.grandpaDemonOverride;
    m_demonsInBetweenOverride = data.demonsInBetweenOverride;
    m_gddpIntegrationOverride = data.gddpIntegrationOverride;
    m_legacy = legacy;

    auto menuRow = CCMenu::create();
    menuRow->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
    menuRow->setPosition({ 150.0f, 80.0f + (data.difficulty > 5 || data.grandpaDemonOverride > 0 ||
        data.demonsInBetweenOverride > 0 || data.gddpIntegrationOverride > 0 ? 5.0f : 0.0f) });
    menuRow->setContentSize({ 300.0f, 50.0f });
    menuRow->setID("feature-buttons");
    m_mainLayer->addChild(menuRow);

    for (int i = 0; i < 5; i++) {
        auto feature = (GJFeatureState)i;
        auto difficultySprite = GJDifficultySprite::create(m_difficulty, GJDifficultyName::Long);
        difficultySprite->updateFeatureState(feature);
        if (Loader::get()->isModLoaded("uproxide.more_difficulties") && m_moreDifficultiesOverride > 0
            && m_grandpaDemonOverride == 0 && m_demonsInBetweenOverride == 0 && m_gddpIntegrationOverride == 0) {
            auto mdSprite = CCSprite::createWithSpriteFrameName((m_legacy ?
                fmt::format("uproxide.more_difficulties/MD_Difficulty{:02d}_Legacy.png", m_moreDifficultiesOverride)
                : fmt::format("uproxide.more_difficulties/MD_Difficulty{:02d}.png", m_moreDifficultiesOverride)).c_str());
            mdSprite->setPosition(difficultySprite->getContentSize() / 2 + (m_legacy ? CCPoint { 0.0f, 0.0f } : CCPoint { 0.25f, -0.1f }));
            difficultySprite->setOpacity(0);
            difficultySprite->addChild(mdSprite);
        }
        if (Loader::get()->isModLoaded("itzkiba.grandpa_demon") && m_grandpaDemonOverride > 0) {
            auto grdSprite = CCSprite::createWithSpriteFrameName(
                fmt::format("itzkiba.grandpa_demon/GrD_demon{}_text.png", m_grandpaDemonOverride - 1).c_str());
            grdSprite->setPosition(difficultySprite->getContentSize() / 2);
            difficultySprite->setOpacity(0);
            difficultySprite->addChild(grdSprite);
        }
        if (Loader::get()->isModLoaded("hiimjustin000.demons_in_between") && m_demonsInBetweenOverride > 0) {
            auto demonsInBetween = Loader::get()->getLoadedMod("hiimjustin000.demons_in_between");
            auto dibFeature = "";
            if (i == 3 && demonsInBetween->getSettingValue<bool>("enable-legendary")) dibFeature = "_4";
            else if (i == 4 && demonsInBetween->getSettingValue<bool>("enable-mythic")) dibFeature = "_5";
            auto dibSprite = CCSprite::createWithSpriteFrameName(fmt::format("hiimjustin000.demons_in_between/DIB_{:02d}{}_btn2_001.png",
                m_demonsInBetweenOverride, dibFeature).c_str());
            dibSprite->setPosition(
                difficultySprite->getContentSize() / 2 + FakeRate::getDIBOffset(m_demonsInBetweenOverride, GJDifficultyName::Long));
            difficultySprite->setOpacity(0);
            difficultySprite->addChild(dibSprite);
        }
        if (Loader::get()->isModLoaded("minemaker0430.gddp_integration") && m_gddpIntegrationOverride > 0) {
            auto gddpSprite = CCSprite::createWithSpriteFrameName(FakeRate::getGDDPFrame(m_gddpIntegrationOverride, GJDifficultyName::Long).c_str());
            gddpSprite->setAnchorPoint({ 0.5f, 1.0f });
            gddpSprite->setPosition(difficultySprite->getContentSize() / 2 + CCPoint { 0.25f, 30.0f });
            difficultySprite->setOpacity(0);
            difficultySprite->addChild(gddpSprite);
        }
        auto toggle = CCMenuItemExt::createSpriteExtra(difficultySprite, [this, feature](CCMenuItemSpriteExtra* sender) {
            if (sender == m_selected) return;
            m_feature = feature;
            if (m_selected) {
                FakeRate::toggle(m_selected->getNormalImage(), false);
                if (auto particleSystem = m_selected->getNormalImage()->getChildByType<CCParticleSystemQuad>(0)) particleSystem->setVisible(false);
            }
            FakeRate::toggle(sender->getNormalImage(), true);
            if (auto particleSystem = sender->getNormalImage()->getChildByType<CCParticleSystemQuad>(0)) particleSystem->setVisible(true);
            m_selected = sender;
        });
        toggle->setID(fmt::format("feature-button-{}", i + 1));
        FakeRate::toggle(difficultySprite, feature == m_feature);
        if (auto particleSystem = difficultySprite->getChildByType<CCParticleSystemQuad>(0)) particleSystem->setVisible(feature == m_feature);
        m_selected = feature == m_feature ? toggle : m_selected;
        menuRow->addChild(toggle);
    }

    menuRow->updateLayout();

    auto confirmButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Confirm", 0.8f), [this, callback = std::move(callback)](auto) {
        callback((int)m_feature);
        onClose(nullptr);
    });
    confirmButton->setPosition({ 150.0f, 25.0f });
    confirmButton->setID("confirm-button");
    m_buttonMenu->addChild(confirmButton);

    return true;
}
