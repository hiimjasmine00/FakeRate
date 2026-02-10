#include <Geode/ui/Popup.hpp>

typedef geode::Function<void(int)> SetGDDPCallback;

class FRGDDPPopup : public geode::Popup {
protected:
    int m_gddpIntegrationOverride;
    CCMenuItemSpriteExtra* m_selected;
    SetGDDPCallback m_callback;

    bool init(int, SetGDDPCallback);
    void onToggle(cocos2d::CCObject*);
    void onConfirm(cocos2d::CCObject*);
public:
    static FRGDDPPopup* create(int, SetGDDPCallback);
};
