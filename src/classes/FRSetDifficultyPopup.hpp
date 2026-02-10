#include "../FakeRate.hpp"
#include <Geode/ui/Popup.hpp>

typedef geode::Function<void(int, int, int, int, int)> SetDifficultyCallback;

class FRSetDifficultyPopup : public geode::Popup {
protected:
    int m_difficulty;
    int m_moreDifficultiesOverride;
    int m_grandpaDemonOverride;
    int m_demonsInBetweenOverride;
    int m_gddpIntegrationOverride;
    bool m_legacy;
    CCMenuItemSpriteExtra* m_selected;
    SetDifficultyCallback m_callback;

    bool init(const FakeRateSaveData&, bool, SetDifficultyCallback);
    void onToggle(cocos2d::CCObject*);
    void onGrandpaDemon(cocos2d::CCObject*);
    void onDemonsInBetween(cocos2d::CCObject*);
    void onGDDPIntegration(cocos2d::CCObject*);
    void onConfirm(cocos2d::CCObject*);
public:
    static FRSetDifficultyPopup* create(const FakeRateSaveData&, bool, SetDifficultyCallback);
};
