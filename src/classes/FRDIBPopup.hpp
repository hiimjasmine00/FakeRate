#include <Geode/ui/Popup.hpp>

typedef geode::Function<void(int)> SetDIBCallback;

class FRDIBPopup : public geode::Popup {
protected:
    int m_demonsInBetweenOverride;
    CCMenuItemSpriteExtra* m_selected;

    bool init(int, SetDIBCallback);
public:
    static FRDIBPopup* create(int, SetDIBCallback);
};
