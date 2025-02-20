#include "../FakeRate.hpp"
#include <Geode/ui/Popup.hpp>

typedef const std::function<void(int, int, int, int, int)>& SetDifficultyCallback;

class FRSetDifficultyPopup : public geode::Popup<const FakeRateSaveData&, bool, SetDifficultyCallback> {
protected:
    inline static constexpr std::array<std::pair<int, int>, 15> DIFFICULTIES = { std::pair<int, int>
        { 0, 0 }, { -1, 0 }, { 1, 0 }, { 2, 0 }, { 3, 4 }, { 3, 0 }, { 4, 0 }, { 4, 7 },
        { 5, 0 }, { 5, 9 }, { 7, 0 }, { 8, 0 }, { 6, 0 }, { 9, 0 }, { 10, 0 }
    };

    int m_difficulty;
    int m_moreDifficultiesOverride;
    int m_grandpaDemonOverride;
    int m_demonsInBetweenOverride;
    int m_gddpIntegrationOverride;
    bool m_legacy;
    CCMenuItemSpriteExtra* m_selected;

    bool setup(const FakeRateSaveData&, bool, SetDifficultyCallback) override;
public:
    static FRSetDifficultyPopup* create(const FakeRateSaveData&, bool, SetDifficultyCallback);
};
