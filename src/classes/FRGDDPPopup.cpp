#include "FRGDDPPopup.hpp"
#include "TableNode.hpp"
#include "../FakeRate.hpp"
#include <Geode/binding/ButtonSprite.hpp>

using namespace geode::prelude;

FRGDDPPopup* FRGDDPPopup::create(int gddpIntegrationOverride, SetGDDPCallback callback) {
    auto ret = new FRGDDPPopup();
    if (ret->initAnchored(350.0f, 250.0f, gddpIntegrationOverride, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FRGDDPPopup::setup(int gddpIntegrationOverride, SetGDDPCallback callback) {
    setID("FRGDDPPopup");
    setTitle("GDDP Integration");
    m_title->setID("gddp-integration-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;
    m_gddpIntegrationOverride = gddpIntegrationOverride;

    auto table = TableNode::create(5, 3);
    table->setColumnLayout(ColumnLayout::create()->setAxisReverse(true));
    table->setRowLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
    table->setRowHeight(60.0f);
    table->setRowPrefix("gddp-button-row");
    table->setContentSize({ 350.0f, 180.0f });
    table->setPosition({ 175.0f, 130.0f });
    table->setID("gddp-buttons");
    m_mainLayer->addChild(table);

    for (int i = 1; i < 16; i++) {
        auto toggle = CCMenuItemExt::createSpriteExtraWithFrameName(FakeRate::getGDDPFrame(i, GJDifficultyName::Long).c_str(), 1.0f,
            [this, i](CCMenuItemSpriteExtra* sender) {
                m_gddpIntegrationOverride = sender != m_selected ? i : 0;
                if (m_selected) FakeRate::toggle(m_selected->getNormalImage(), false);
                if (sender != m_selected) FakeRate::toggle(sender->getNormalImage(), true);
                m_selected = sender != m_selected ? sender : nullptr;
            });
        toggle->setID(fmt::format("gddp-button-{}", i));
        FakeRate::toggle(toggle->getNormalImage(), i == m_gddpIntegrationOverride);
        m_selected = i == m_gddpIntegrationOverride ? toggle : m_selected;
        table->addButton(toggle);
    }

    table->updateAllLayouts();

    auto confirmButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Confirm", "goldFont.fnt", "GJ_button_01.png", 0.8f), [this, callback](auto) {
        callback(m_gddpIntegrationOverride);
        onClose(nullptr);
    });
    confirmButton->setPosition({ 175.0f, 25.0f });
    confirmButton->setID("confirm-button");
    m_buttonMenu->addChild(confirmButton);

    return true;
}
