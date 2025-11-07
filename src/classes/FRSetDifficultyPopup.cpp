#include "FRSetDifficultyPopup.hpp"
#include "FRDIBPopup.hpp"
#include "FRGDDPPopup.hpp"
#include "FRGRDPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/loader/Mod.hpp>
#include <jasmine/nodes.hpp>

using namespace geode::prelude;
using namespace jasmine::nodes;

FRSetDifficultyPopup* FRSetDifficultyPopup::create(const FakeRateSaveData& data, bool legacy, SetDifficultyCallback callback) {
    auto ret = new FRSetDifficultyPopup();
    if (ret->initAnchored(300.0f, 250.0f, data, legacy, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FRSetDifficultyPopup::setup(const FakeRateSaveData& data, bool legacy, SetDifficultyCallback callback) {
    setID("FRSetDifficultyPopup");
    setTitle("Select Difficulty");
    m_title->setID("select-difficulty-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;
    m_difficulty = data.difficulty;
    m_moreDifficultiesOverride = data.moreDifficultiesOverride;
    m_grandpaDemonOverride = data.grandpaDemonOverride;
    m_demonsInBetweenOverride = data.demonsInBetweenOverride;
    m_gddpIntegrationOverride = data.gddpIntegrationOverride;
    m_legacy = legacy;

    auto casual = false;
    auto tough = false;
    auto cruel = false;
    auto loader = Loader::get();
    auto moreDifficulties = loader->getLoadedMod("uproxide.more_difficulties");
    if (moreDifficulties) {
        casual = moreDifficulties->getSavedValue("casual", true);
        tough = moreDifficulties->getSavedValue("tough", true);
        cruel = moreDifficulties->getSavedValue("cruel", true);
    }

    auto table = TableNode::create(moreDifficulties && (casual || tough || cruel) ? 5 : 4, 3, 300.0f, 170.0f, "difficulty-button-row");
    table->setPosition({ 150.0f, 130.0f });
    table->setID("difficulty-buttons");
    m_mainLayer->addChild(table);

    int i = 1;
    for (auto [d, mdo] : difficulties) {
        auto num = d == -1 ? "auto" : fmt::format("{:02d}", d);
        auto frameName = d > 5 ? fmt::format("difficulty_{}_btn2_001.png", num) : fmt::format("difficulty_{}_btn_001.png", num);
        if (moreDifficulties && mdo > 0) {
            if (mdo == 4 && !casual) continue;
            if (mdo == 7 && !tough) continue;
            if (mdo == 9 && !cruel) continue;
            frameName = fmt::format("uproxide.more_difficulties/MD_Difficulty{:02d}{}.png", mdo, m_legacy ? "_Legacy" : "");
        }
        else if (mdo > 0) continue;
        auto toggle = CCMenuItemExt::createSpriteExtraWithFrameName(frameName.c_str(), 1.0f, [this, d, mdo](CCMenuItemSpriteExtra* sender) {
            m_difficulty = d;
            m_moreDifficultiesOverride = mdo;
            m_grandpaDemonOverride = 0;
            m_demonsInBetweenOverride = 0;
            m_gddpIntegrationOverride = 0;
            if (sender == m_selected) return;
            if (m_selected) FakeRate::toggle(m_selected->getNormalImage(), false);
            FakeRate::toggle(sender->getNormalImage(), true);
            m_selected = sender;
        });
        toggle->setID(fmt::format("difficulty-button-{}", i++));
        auto isToggled = mdo == m_moreDifficultiesOverride && (m_moreDifficultiesOverride <= 0 ? d == m_difficulty : true);
        FakeRate::toggle(toggle->getNormalImage(), isToggled);
        m_selected = isToggled ? toggle : m_selected;
        table->addButton(toggle);
    }

    table->updateAllLayouts();

    auto confirmButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Confirm", 0.8f), [this, callback = std::move(callback)](auto) {
        callback(m_difficulty, m_moreDifficultiesOverride, m_grandpaDemonOverride, m_demonsInBetweenOverride, m_gddpIntegrationOverride);
        onClose(nullptr);
    });
    confirmButton->setPosition({ 150.0f, 25.0f });
    confirmButton->setID("confirm-button");
    m_buttonMenu->addChild(confirmButton);

    auto overrideMenu = CCMenu::create();
    overrideMenu->setLayout(RowLayout::create()->setGap(4.0f)->setAxisAlignment(AxisAlignment::End));
    overrideMenu->setPosition({ 245.0f, 25.0f });
    overrideMenu->setContentSize({ 100.0f, 30.0f });
    overrideMenu->setID("override-menu");
    m_mainLayer->addChild(overrideMenu);

    if (loader->isModLoaded("itzkiba.grandpa_demon")) {
        auto grdButton = CCMenuItemExt::createSpriteExtraWithFilename("FR_grdBtn_001.png"_spr, 0.65f, [this](auto) {
            FRGRDPopup::create(m_grandpaDemonOverride, [this](int grd) {
                m_grandpaDemonOverride = grd;
                m_demonsInBetweenOverride = 0;
                m_gddpIntegrationOverride = 0;
            })->show();
        });
        grdButton->setID("grd-button");
        overrideMenu->addChild(grdButton);
    }

    if (auto demonsInBetween = loader->getLoadedMod("hiimjustin000.demons_in_between");
        demonsInBetween && demonsInBetween->getSettingValue<bool>("enable-difficulties")) {
        auto dibButton = CCMenuItemExt::createSpriteExtraWithFilename("FR_dibBtn_001.png"_spr, 0.75f, [this](auto) {
            FRDIBPopup::create(m_demonsInBetweenOverride, [this](int dib) {
                m_grandpaDemonOverride = 0;
                m_demonsInBetweenOverride = dib;
                m_gddpIntegrationOverride = 0;
            })->show();
        });
        dibButton->setID("dib-button");
        overrideMenu->addChild(dibButton);
    }

    if (auto gddpIntegration = loader->getLoadedMod("minemaker0430.gddp_integration");
        gddpIntegration && gddpIntegration->getSettingValue<bool>("custom-difficulty-faces")) {
        auto gddpButton = CCMenuItemExt::createSpriteExtraWithFilename("FR_gddpBtn_001.png"_spr, 0.65f, [this](auto) {
            FRGDDPPopup::create(m_gddpIntegrationOverride, [this](int gddp) {
                m_grandpaDemonOverride = 0;
                m_demonsInBetweenOverride = 0;
                m_gddpIntegrationOverride = gddp;
            })->show();
        });
        gddpButton->setID("gddp-button");
        overrideMenu->addChild(gddpButton);
    }

    overrideMenu->updateLayout();

    return true;
}
