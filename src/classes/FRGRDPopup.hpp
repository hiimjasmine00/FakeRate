#include <Geode/ui/Popup.hpp>

typedef geode::Function<void(int)> SetGRDCallback;

class FRGRDPopup : public geode::Popup {
protected:
    int m_grandpaDemonOverride;
    CCMenuItemSpriteExtra* m_selected;

    bool init(int, SetGRDCallback);
public:
    static FRGRDPopup* create(int, SetGRDCallback);
};
