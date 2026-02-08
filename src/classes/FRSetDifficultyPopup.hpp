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

    bool init(const FakeRateSaveData&, bool, SetDifficultyCallback);
public:
    static FRSetDifficultyPopup* create(const FakeRateSaveData&, bool, SetDifficultyCallback);
};
