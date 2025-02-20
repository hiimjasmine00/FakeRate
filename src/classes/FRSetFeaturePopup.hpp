#include "../FakeRate.hpp"
#include <Geode/ui/Popup.hpp>

typedef const std::function<void(int)>& SetFeatureCallback;

class FRSetFeaturePopup : public geode::Popup<const FakeRateSaveData&, bool, SetFeatureCallback> {
protected:
    GJFeatureState m_feature;
    int m_difficulty;
    int m_moreDifficultiesOverride;
    int m_grandpaDemonOverride;
    int m_demonsInBetweenOverride;
    int m_gddpIntegrationOverride;
    bool m_legacy;
    CCMenuItemSpriteExtra* m_selected;

    bool setup(const FakeRateSaveData&, bool, SetFeatureCallback) override;
public:
    static FRSetFeaturePopup* create(const FakeRateSaveData&, bool, SetFeatureCallback);
};
