#include "FRDIBPopup.hpp"
#include "../FakeRate.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <jasmine/nodes.hpp>

using namespace geode::prelude;
using namespace jasmine::nodes;

FRDIBPopup* FRDIBPopup::create(int demonsInBetweenOverride, SetDIBCallback callback) {
    auto ret = new FRDIBPopup();
    if (ret->init(demonsInBetweenOverride, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FRDIBPopup::init(int demonsInBetweenOverride, SetDIBCallback callback) {
    if (!Popup::init(350.0f, 310.0f)) return false;

    setID("FRDIBPopup");
    setTitle("Demons In Between");
    m_title->setID("demons-in-between-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;
    m_demonsInBetweenOverride = demonsInBetweenOverride;

    auto table = TableNode::create(5, 4, 350.0f, 240.0f, "dib-button-row");
    table->setPosition({ 175.0f, 160.0f });
    table->setID("dib-buttons");
    m_mainLayer->addChild(table);

    for (int i = 1; i < 21; i++) {
        auto toggle = CCMenuItemExt::createSpriteExtraWithFrameName(
            fmt::format("hiimjustin000.demons_in_between/DIB_{:02}_btn2_001.png", i).c_str(), 1.0f, [this, i](CCMenuItemSpriteExtra* sender) {
                m_demonsInBetweenOverride = sender != m_selected ? i : 0;
                if (m_selected) FakeRate::toggle(m_selected->getNormalImage(), false);
                if (sender != m_selected) FakeRate::toggle(sender->getNormalImage(), true);
                m_selected = sender != m_selected ? sender : nullptr;
            });
        toggle->setID(fmt::format("dib-button-{}", i));
        FakeRate::toggle(toggle->getNormalImage(), i == m_demonsInBetweenOverride);
        m_selected = i == m_demonsInBetweenOverride ? toggle : m_selected;
        table->addButton(toggle);
    }

    table->updateAllLayouts();

    auto confirmButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Confirm", 0.8f), [
        this, callback = std::move(callback)
    ](auto) mutable {
        callback(m_demonsInBetweenOverride);
        onClose(nullptr);
    });
    confirmButton->setPosition({ 175.0f, 25.0f });
    confirmButton->setID("confirm-button");
    m_buttonMenu->addChild(confirmButton);

    return true;
}
