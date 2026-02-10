#include "../FakeRate.hpp"
#include <Geode/ui/Popup.hpp>

typedef geode::Function<void(int)> SetFeatureCallback;

class FRSetFeaturePopup : public geode::Popup {
protected:
    GJFeatureState m_feature;
    int m_difficulty;
    int m_moreDifficultiesOverride;
    int m_grandpaDemonOverride;
    int m_demonsInBetweenOverride;
    int m_gddpIntegrationOverride;
    bool m_legacy;
    CCMenuItemSpriteExtra* m_selected;
    SetFeatureCallback m_callback;

    bool init(const FakeRateSaveData&, bool, SetFeatureCallback);
    void onToggle(cocos2d::CCObject*);
    void onConfirm(cocos2d::CCObject*);
public:
    static FRSetFeaturePopup* create(const FakeRateSaveData&, bool, SetFeatureCallback);
};
