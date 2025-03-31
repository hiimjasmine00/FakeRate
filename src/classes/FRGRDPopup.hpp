#include <Geode/ui/Popup.hpp>

typedef std::function<void(int)> SetGRDCallback;

class FRGRDPopup : public geode::Popup<int, SetGRDCallback> {
protected:
    int m_grandpaDemonOverride;
    CCMenuItemSpriteExtra* m_selected;

    bool setup(int, SetGRDCallback) override;
public:
    static FRGRDPopup* create(int, SetGRDCallback);
};
