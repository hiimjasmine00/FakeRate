#include "FRGRDPopup.hpp"
#include "../FakeRate.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <jasmine/nodes.hpp>

using namespace geode::prelude;
using namespace jasmine::nodes;

FRGRDPopup* FRGRDPopup::create(int grandpaDemonOverride, SetGRDCallback callback) {
    auto ret = new FRGRDPopup();
    if (ret->init(grandpaDemonOverride, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FRGRDPopup::init(int grandpaDemonOverride, SetGRDCallback callback) {
    if (!Popup::init(250.0f, 200.0f)) return false;

    setID("FRGRDPopup");
    setTitle("Grandpa Demon");
    m_title->setID("grandpa-demon-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;
    m_grandpaDemonOverride = grandpaDemonOverride;

    auto table = TableNode::create(3, 2, 250.0f, 130.0f, "grd-button-row");
    table->setContentSize({ 250.0f, 130.0f });
    table->setPosition({ 125.0f, 107.5f });
    table->setID("grd-buttons");
    m_mainLayer->addChild(table);

    for (int i = 1; i < 7; i++) {
        auto toggle = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(fmt::format("itzkiba.grandpa_demon/GrD_demon{}_text.png", i - 1).c_str()),
            this, menu_selector(FRGRDPopup::onToggle)
        );
        toggle->setTag(i);
        toggle->setID(fmt::format("grd-button-{}", i));
        FakeRate::toggle(toggle->getNormalImage(), i == m_grandpaDemonOverride);
        m_selected = i == m_grandpaDemonOverride ? toggle : m_selected;
        table->addButton(toggle);
    }

    table->updateAllLayouts();

    auto confirmButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("Confirm", 0.8f), this, menu_selector(FRGRDPopup::onConfirm));
    confirmButton->setPosition({ 125.0f, 25.0f });
    confirmButton->setID("confirm-button");
    m_buttonMenu->addChild(confirmButton);

    return true;
}

void FRGRDPopup::onToggle(CCObject* sender) {
    m_grandpaDemonOverride = sender != m_selected ? sender->getTag() : 0;
    if (m_selected) FakeRate::toggle(m_selected->getNormalImage(), false);
    auto toggle = static_cast<CCMenuItemSpriteExtra*>(sender);
    if (sender != m_selected) FakeRate::toggle(toggle->getNormalImage(), true);
    m_selected = sender != m_selected ? toggle : nullptr;
}

void FRGRDPopup::onConfirm(CCObject* sender) {
    m_callback(m_grandpaDemonOverride);
    onClose(nullptr);
}
