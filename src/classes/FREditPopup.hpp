#include "../FakeRate.hpp"
#include <Geode/ui/Popup.hpp>

typedef const std::function<void(FakeRateSaveData, bool)>& UpdateFakeRateCallback;

class FREditPopup : public geode::Popup<GJGameLevel*, const FakeRateSaveData&, UpdateFakeRateCallback> {
protected:
    GJGameLevel* m_level;
    int m_stars;
    int m_feature;
    int m_difficulty;
    int m_moreDifficultiesOverride;
    int m_grandpaDemonOverride;
    int m_demonsInBetweenOverride;
    int m_gddpIntegrationOverride;
    bool m_coins;
    bool m_legacy;
    GJDifficultySprite* m_difficultySprite;
    cocos2d::CCSprite* m_mdSprite;
    cocos2d::CCSprite* m_grdSprite;
    cocos2d::CCSprite* m_grdInfinity;
    cocos2d::CCSprite* m_dibSprite;
    cocos2d::CCSprite* m_gddpSprite;
    cocos2d::CCSprite* m_starSprite;
    cocos2d::CCLabelBMFont* m_starsLabel;
    cocos2d::CCArray* m_coinSprites;

    bool setup(GJGameLevel*, const FakeRateSaveData&, UpdateFakeRateCallback) override;
    void updateLabels();
public:
    static FREditPopup* create(GJGameLevel*, const FakeRateSaveData&, UpdateFakeRateCallback);

    ~FREditPopup() override;
};
