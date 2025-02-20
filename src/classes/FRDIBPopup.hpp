#include <Geode/ui/Popup.hpp>

typedef const std::function<void(int)>& SetDIBCallback;

class FRDIBPopup : public geode::Popup<int, SetDIBCallback> {
protected:
    int m_demonsInBetweenOverride;
    CCMenuItemSpriteExtra* m_selected;

    bool setup(int, SetDIBCallback) override;
public:
    static FRDIBPopup* create(int, SetDIBCallback);
};
