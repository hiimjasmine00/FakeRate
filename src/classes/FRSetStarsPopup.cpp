#include "FRSetStarsPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>

using namespace geode::prelude;

FRSetStarsPopup* FRSetStarsPopup::create(int stars, bool platformer, SetStarsCallback callback) {
    auto ret = new FRSetStarsPopup();
    if (ret->init(stars, platformer, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FRSetStarsPopup::init(int stars, bool platformer, SetStarsCallback callback) {
    if (!Popup::init(250.0f, 150.0f)) return false;

    setID("FRSetStarsPopup");
    setTitle(platformer ? "Set Moons" : "Set Stars");
    m_title->setID("set-stars-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;
    m_stars = stars;
    m_callback = std::move(callback);

    m_input = TextInput::create(150.0f, platformer ? "Moons" : "Stars");
    m_input->setCommonFilter(CommonFilter::Int);
    m_input->setPosition({ 125.0f, 80.0f });
    m_input->getInputNode()->setLabelPlaceholderColor({ 120, 170, 240 });
    m_input->setString(fmt::to_string(m_stars));
    m_input->setMaxCharCount(11);
    m_input->setCallback([this](const std::string& text) {
        if (auto stars = numFromString<int>(text)) {
            m_stars = stars.unwrap();
            m_label->setString(fmt::to_string(m_stars).c_str());
            m_starLayout->updateLayout();
        }
    });
    m_input->setID("stars-input");
    m_mainLayer->addChild(m_input);

    m_starLayout = CCNode::create();
    m_starLayout->setPosition({ 125.0f, 52.5f });
    m_starLayout->setContentSize({ 250.0f, 15.0f });
    m_starLayout->setAnchorPoint({ 0.5f, 0.5f });
    m_starLayout->setLayout(RowLayout::create()->setGap(1.75f)->setAutoScale(false));
    m_starLayout->setID("stars-layout");
    m_mainLayer->addChild(m_starLayout);

    m_label = CCLabelBMFont::create(fmt::to_string(m_stars).c_str(), "bigFont.fnt");
    m_label->setScale(0.4f);
    m_label->setID("stars-label");
    m_starLayout->addChild(m_label);

    auto starSprite = CCSprite::createWithSpriteFrameName(platformer ? "moon_small01_001.png" : "star_small01_001.png");
    starSprite->setID("star-sprite");
    m_starLayout->addChild(starSprite);

    m_starLayout->updateLayout();

    auto leftSprite = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
    leftSprite->setScale(1.1f);
    auto leftButton = CCMenuItemSpriteExtra::create(leftSprite, this, menu_selector(FRSetStarsPopup::onLeft));
    leftButton->setPosition({ 30.0f, 80.0f });
    leftButton->setID("left-button");
    m_buttonMenu->addChild(leftButton);

    auto rightSprite = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
    rightSprite->setScale(1.1f);
    auto rightButton = CCMenuItemSpriteExtra::create(rightSprite, this, menu_selector(FRSetStarsPopup::onRight));
    rightButton->setPosition({ 220.0f, 80.0f });
    rightButton->setID("right-button");
    m_buttonMenu->addChild(rightButton);

    auto confirmButton = CCMenuItemSpriteExtra::create(ButtonSprite::create("Confirm", 0.8f), this, menu_selector(FRSetStarsPopup::onConfirm));
    confirmButton->setPosition({ 125.0f, 25.0f });
    confirmButton->setID("confirm-button");
    m_buttonMenu->addChild(confirmButton);

    return true;
}

void FRSetStarsPopup::onLeft(CCObject* sender) {
    if (m_stars > INT_MIN) m_stars--;
    auto stars = fmt::to_string(m_stars);
    m_input->setString(stars);
    m_label->setString(stars.c_str());
    m_starLayout->updateLayout();
}

void FRSetStarsPopup::onRight(CCObject* sender) {
    if (m_stars < INT_MAX) m_stars++;
    auto stars = fmt::to_string(m_stars);
    m_input->setString(stars);
    m_label->setString(stars.c_str());
    m_starLayout->updateLayout();
}

void FRSetStarsPopup::onConfirm(CCObject* sender) {
    m_callback(m_stars);
    onClose(nullptr);
}
