#include "FRSetStarsPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>

using namespace geode::prelude;

FRSetStarsPopup* FRSetStarsPopup::create(int stars, bool platformer, SetStarsCallback callback) {
    auto ret = new FRSetStarsPopup();
    if (ret->initAnchored(250.0f, 150.0f, stars, platformer, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FRSetStarsPopup::setup(int stars, bool platformer, SetStarsCallback callback) {
    setID("FRSetStarsPopup");
    setTitle(platformer ? "Set Moons" : "Set Stars");
    m_title->setID("set-stars-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;
    m_stars = stars;

    m_input = TextInput::create(150.0f, platformer ? "Moons" : "Stars");
    m_input->setCommonFilter(CommonFilter::Int);
    m_input->setPosition({ 125.0f, 80.0f });
    m_input->getInputNode()->setLabelPlaceholderColor({ 120, 170, 240 });
    m_input->setString(std::to_string(m_stars));
    m_input->setMaxCharCount(11);
    m_input->setCallback([this](const std::string& text) {
        auto stars = numFromString<int64_t>(text).unwrapOr(0);
        if (stars < INT_MIN) stars = INT_MIN;
        if (stars > INT_MAX) stars = INT_MAX;
        m_stars = stars;
        m_label->setString(std::to_string(m_stars).c_str());
        m_starLayout->updateLayout();
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

    m_label = CCLabelBMFont::create(std::to_string(m_stars).c_str(), "bigFont.fnt");
    m_label->setScale(0.4f);
    m_label->setID("stars-label");
    m_starLayout->addChild(m_label);

    auto starSprite = CCSprite::createWithSpriteFrameName(platformer ? "moon_small01_001.png" : "star_small01_001.png");
    starSprite->setID("star-sprite");
    m_starLayout->addChild(starSprite);

    m_starLayout->updateLayout();

    auto leftButton = CCMenuItemExt::createSpriteExtraWithFrameName("edit_leftBtn_001.png", 1.1f, [this](auto) {
        if (m_stars != INT_MIN) m_stars--;
        auto stars = std::to_string(m_stars);
        m_input->setString(stars);
        m_label->setString(stars.c_str());
        m_starLayout->updateLayout();
    });
    leftButton->setPosition({ 30.0f, 80.0f });
    leftButton->setID("left-button");
    m_buttonMenu->addChild(leftButton);

    auto rightButton = CCMenuItemExt::createSpriteExtraWithFrameName("edit_rightBtn_001.png", 1.1f, [this](auto) {
        if (m_stars != INT_MAX) m_stars++;
        auto stars = std::to_string(m_stars);
        m_input->setString(stars);
        m_label->setString(stars.c_str());
        m_starLayout->updateLayout();
    });
    rightButton->setPosition({ 220.0f, 80.0f });
    rightButton->setID("right-button");
    m_buttonMenu->addChild(rightButton);

    auto confirmButton = CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Confirm", "goldFont.fnt", "GJ_button_01.png", 0.8f),
        [this, callback = std::move(callback)](auto) {
            callback(m_stars);
            onClose(nullptr);
        });
    confirmButton->setPosition({ 125.0f, 25.0f });
    confirmButton->setID("confirm-button");
    m_buttonMenu->addChild(confirmButton);

    return true;
}
