#include "FRGRDPopup.hpp"
#include "TableNode.hpp"
#include "../FakeRate.hpp"
#include <Geode/binding/ButtonSprite.hpp>

using namespace geode::prelude;

FRGRDPopup* FRGRDPopup::create(int grandpaDemonOverride, SetGRDCallback callback) {
    auto ret = new FRGRDPopup();
    if (ret->initAnchored(250.0f, 200.0f, grandpaDemonOverride, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FRGRDPopup::setup(int grandpaDemonOverride, SetGRDCallback callback) {
    setID("FRGRDPopup");
    setTitle("Grandpa Demon");
    m_title->setID("grandpa-demon-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;
    m_grandpaDemonOverride = grandpaDemonOverride;

    auto table = TableNode::create(3, 2);
    table->setColumnLayout(ColumnLayout::create()->setAxisReverse(true));
    table->setRowLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Even));
    table->setRowHeight(65.0f);
    table->setRowPrefix("grd-button-row");
    table->setContentSize({ 250.0f, 130.0f });
    table->setPosition({ 125.0f, 107.5f });
    table->setID("grd-buttons");
    m_mainLayer->addChild(table);

    for (int i = 1; i < 7; i++) {
        auto toggle = CCMenuItemExt::createSpriteExtraWithFrameName(fmt::format("itzkiba.grandpa_demon/GrD_demon{}_text.png", i - 1).c_str(), 1.0f,
            [this, i](CCMenuItemSpriteExtra* sender) {
                m_grandpaDemonOverride = sender != m_selected ? i : 0;
                if (m_selected) FakeRate::toggle(m_selected->getNormalImage(), false);
                if (sender != m_selected) FakeRate::toggle(sender->getNormalImage(), true);
                m_selected = sender != m_selected ? sender : nullptr;
            });
        FakeRate::toggle(toggle->getNormalImage(), i == m_grandpaDemonOverride);
        m_selected = i == m_grandpaDemonOverride ? toggle : m_selected;
        table->addButton(toggle);
    }

    table->updateAllLayouts();

    auto confirmButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Confirm", 0.8f), [this, callback = std::move(callback)](auto) {
        callback(m_grandpaDemonOverride);
        onClose(nullptr);
    });
    confirmButton->setPosition({ 125.0f, 25.0f });
    confirmButton->setID("confirm-button");
    m_buttonMenu->addChild(confirmButton);

    return true;
}
