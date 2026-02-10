#include "FRGDDPPopup.hpp"
#include "../FakeRate.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <jasmine/nodes.hpp>

using namespace geode::prelude;
using namespace jasmine::nodes;

FRGDDPPopup* FRGDDPPopup::create(int gddpIntegrationOverride, SetGDDPCallback callback) {
    auto ret = new FRGDDPPopup();
    if (ret->init(gddpIntegrationOverride, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FRGDDPPopup::init(int gddpIntegrationOverride, SetGDDPCallback callback) {
    if (!Popup::init(250.0f, 250.0f)) return false;

    setID("FRGDDPPopup");
    setTitle("GDDP Integration");
    m_title->setID("gddp-integration-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;
    m_gddpIntegrationOverride = gddpIntegrationOverride;

    auto table = TableNode::create(4, 4, 250.0f, 180.0f, "gddp-button-row");
    table->setPosition({ 125.0f, 130.0f });
    table->setID("gddp-buttons");
    m_mainLayer->addChild(table);

    for (int i = 1; i < 17; i++) {
        auto toggle = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(FakeRate::getGDDPFrame(i, GJDifficultyName::Long).c_str()),
            this, menu_selector(FRGDDPPopup::onToggle)
        );
        toggle->setTag(i);
        toggle->setID(fmt::format("gddp-button-{}", i));
        FakeRate::toggle(toggle->getNormalImage(), i == m_gddpIntegrationOverride);
        m_selected = i == m_gddpIntegrationOverride ? toggle : m_selected;
        table->addButton(toggle);
    }

    table->updateAllLayouts();

    auto confirmButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("Confirm", 0.8f), this, menu_selector(FRGDDPPopup::onConfirm));
    confirmButton->setPosition({ 125.0f, 25.0f });
    confirmButton->setID("confirm-button");
    m_buttonMenu->addChild(confirmButton);

    return true;
}

void FRGDDPPopup::onToggle(CCObject* sender) {
    m_gddpIntegrationOverride = sender != m_selected ? sender->getTag() : 0;
    if (m_selected) FakeRate::toggle(m_selected->getNormalImage(), false);
    auto toggle = static_cast<CCMenuItemSpriteExtra*>(sender);
    FakeRate::toggle(toggle->getNormalImage(), toggle != m_selected);
    m_selected = toggle != m_selected ? toggle : nullptr;
}

void FRGDDPPopup::onConfirm(CCObject* sender) {
    m_callback(m_gddpIntegrationOverride);
    onClose(nullptr);
}
