#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

typedef geode::Function<void(int)> SetStarsCallback;

class FRSetStarsPopup : public geode::Popup {
protected:
    int m_stars;
    geode::TextInput* m_input;
    cocos2d::CCLabelBMFont* m_label;
    cocos2d::CCNode* m_starLayout;
    SetStarsCallback m_callback;

    bool init(int, bool, SetStarsCallback);
    void onLeft(cocos2d::CCObject*);
    void onRight(cocos2d::CCObject*);
    void onConfirm(cocos2d::CCObject*);
public:
    static FRSetStarsPopup* create(int, bool, SetStarsCallback);
};
