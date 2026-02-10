#include <Geode/ui/Popup.hpp>

typedef geode::Function<void(int)> SetGRDCallback;

class FRGRDPopup : public geode::Popup {
protected:
    int m_grandpaDemonOverride;
    CCMenuItemSpriteExtra* m_selected;
    SetGRDCallback m_callback;

    bool init(int, SetGRDCallback);
    void onToggle(cocos2d::CCObject*);
    void onConfirm(cocos2d::CCObject*);
public:
    static FRGRDPopup* create(int, SetGRDCallback);
};
