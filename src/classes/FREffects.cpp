#include "FREffects.hpp"
#include <jasmine/random.hpp>

using namespace geode::prelude;

CCSequence* getRandomSequence(float duration, double minimum, double maximum) {
    auto actions = CCArray::create();
    for (int i = 0; i < 19; i++) {
        actions->addObject(CCFadeTo::create(duration, jasmine::random::get() * (maximum - minimum) + minimum));
    }
    actions->addObject(CCFadeTo::create(duration, 0));
    return CCSequence::create(actions);
}

CCSprite* FREffects::grdInfinity() {
    auto infinity = CCSprite::createWithSpriteFrameName("itzkiba.grandpa_demon/GrD_demon4_infinity.png");
    infinity->setZOrder(30);
    infinity->setColor({ 255, 233, 136 });
    infinity->setOpacity(100);
    infinity->setScale(0.4f);
    infinity->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
        CCEaseSineInOut::create(CCFadeTo::create(1.5f, 200)),
        CCEaseSineInOut::create(CCFadeTo::create(1.5f, 60))
    )));
    infinity->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
        CCEaseSineInOut::create(CCScaleTo::create(1.5f, 0.45f)),
        CCEaseSineInOut::create(CCScaleTo::create(1.5f, 0.35f))
    )));
    infinity->setBlendFunc({ GL_ONE, GL_ONE });
    return infinity;
}

std::pair<CCSprite*, CCSprite*> FREffects::infinityBackgrounds(bool grandpa) {
    auto width = CCDirector::get()->getWinSize().width;
    CCPoint position = { width / 2.0f, 0.0f };

    auto bg1 = CCSprite::create("itzkiba.grandpa_demon/GrD_demon4_bg.png");
    bg1->setBlendFunc({ GL_ONE, GL_ONE });
    bg1->setOpacity(0);
    bg1->setScale(width / bg1->getContentWidth());
    bg1->setColor(grandpa ? ccColor3B { 121, 80, 255 } : ccColor3B { 249, 249, 165 });
    bg1->setAnchorPoint({ 0.5f, 0.0f });
    bg1->setPosition(position);
    bg1->setZOrder(-5);
    bg1->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
        CCMoveTo::create(20.0f, position - CCPoint { 0.0f, bg1->getScaledContentHeight() * 0.15f }),
        CCMoveTo::create(0.0f, position)
    )));
    bg1->runAction(CCRepeatForever::create(getRandomSequence(1.0f, 0.0, 180.0)));

    auto bg2 = CCSprite::create("itzkiba.grandpa_demon/GrD_demon4_bg.png");
    bg2->setBlendFunc({ GL_ONE, GL_ONE });
    bg2->setOpacity(0);
    bg2->setScale(width * 2.0f / bg2->getContentWidth());
    bg2->setColor(grandpa ? ccColor3B { 233, 200, 255 } : ccColor3B { 231, 231, 195 });
    bg2->setAnchorPoint({ 0.5f, 0.0f });
    bg2->setPosition(position);
    bg2->setZOrder(-4);
    bg2->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
        CCMoveTo::create(30.0f, position - CCPoint { 0.0f, bg2->getScaledContentHeight() * 0.75f }),
        CCMoveTo::create(0.0f, position)
    )));
    bg2->runAction(CCRepeatForever::create(getRandomSequence(1.5f, -40.0, 140.0)));

    return { bg1, bg2 };
}

CCSprite* FREffects::mythicalBackground() {
    auto width = CCDirector::get()->getWinSize().width;
    CCPoint position = { width / 2.0f, 0.0f };

    auto bg1 = CCSprite::create("itzkiba.grandpa_demon/GrD_demon4_bg.png");
    bg1->setBlendFunc({ GL_ONE, GL_ONE });
    bg1->setOpacity(0);
    bg1->setScale(width / bg1->getContentWidth());
    bg1->setColor({ 76, 63, 118 });
    bg1->setAnchorPoint({ 0.5f, 0.0f });
    bg1->setPosition(position);
    bg1->setZOrder(-5);
    bg1->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
        CCMoveTo::create(20.0f, position - CCPoint { 0.0f, bg1->getScaledContentHeight() * 0.35f }),
        CCMoveTo::create(0.0f, position)
    )));
    bg1->runAction(CCRepeatForever::create(CCSequence::create(getRandomSequence(1.0f, 80.0, 180.0))));

    return bg1;
}

CCSprite* FREffects::legendaryBackground() {
    auto width = CCDirector::get()->getWinSize().width;
    CCPoint position = { width / 2.0f, 0.0f };

    auto bg1 = CCSprite::create("itzkiba.grandpa_demon/GrD_demon4_bg.png");
    bg1->setBlendFunc({ GL_ONE, GL_ONE });
    bg1->setOpacity(0);
    bg1->setScale(width / bg1->getContentWidth());
    bg1->setColor({ 55, 48, 78 });
    bg1->setAnchorPoint({ 0.5f, 0.0f });
    bg1->setPosition(position);
    bg1->setZOrder(-5);
    bg1->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
        CCMoveTo::create(20.0f, position - CCPoint { 0.0f, bg1->getScaledContentHeight() * 0.25f }),
        CCMoveTo::create(0.0f, position)
    )));
    bg1->runAction(CCRepeatForever::create(CCSequence::create(getRandomSequence(1.0f, 60.0, 140.0))));

    return bg1;
}
