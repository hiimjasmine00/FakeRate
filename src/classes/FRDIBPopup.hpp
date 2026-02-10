#include <Geode/ui/Popup.hpp>

typedef geode::Function<void(int)> SetDIBCallback;

class FRDIBPopup : public geode::Popup {
protected:
    int m_demonsInBetweenOverride;
    CCMenuItemSpriteExtra* m_selected;
    SetDIBCallback m_callback;

    bool init(int, SetDIBCallback);
    void onToggle(cocos2d::CCObject*);
    void onConfirm(cocos2d::CCObject*);
public:
    static FRDIBPopup* create(int, SetDIBCallback);
};
