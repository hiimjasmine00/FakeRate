#include "../FakeRate.hpp"
#include <Geode/ui/Popup.hpp>

typedef std::function<void(int, int, int, int, int)> SetDifficultyCallback;

class FRSetDifficultyPopup : public geode::Popup<const FakeRateSaveData&, bool, SetDifficultyCallback> {
protected:
    static constexpr std::array<std::pair<int, int>, 15> difficulties = {
        std::make_pair(0, 0), std::make_pair(-1, 0), std::make_pair(1, 0), std::make_pair(2, 0), std::make_pair(3, 4),
        std::make_pair(3, 0), std::make_pair(4, 0), std::make_pair(4, 7), std::make_pair(5, 0), std::make_pair(5, 9),
        std::make_pair(7, 0), std::make_pair(8, 0), std::make_pair(6, 0), std::make_pair(9, 0), std::make_pair(10, 0)
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
