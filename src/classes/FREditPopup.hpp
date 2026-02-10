#include "../FakeRate.hpp"
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/ui/Popup.hpp>

typedef geode::Function<void(const FakeRateSaveData&, bool)> UpdateFakeRateCallback;

class FREditPopup : public geode::Popup, public FLAlertLayerProtocol {
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
    std::vector<cocos2d::CCSprite*> m_coinSprites;
    UpdateFakeRateCallback m_callback;

    bool init(GJGameLevel*, const FakeRateSaveData&, UpdateFakeRateCallback);
    void onDifficulty(cocos2d::CCObject*);
    void onStars(cocos2d::CCObject*);
    void onFeature(cocos2d::CCObject*);
    void onCoins(cocos2d::CCObject*);
    void onAdd(cocos2d::CCObject*);
    void onRemove(cocos2d::CCObject*);
    void FLAlert_Clicked(FLAlertLayer*, bool) override;
    void updateLabels();
public:
    static FREditPopup* create(GJGameLevel*, const FakeRateSaveData&, UpdateFakeRateCallback);
};
