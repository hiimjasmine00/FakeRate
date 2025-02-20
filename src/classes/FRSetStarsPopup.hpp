#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

typedef const std::function<void(int)>& SetStarsCallback;

class FRSetStarsPopup : public geode::Popup<int, bool, SetStarsCallback> {
protected:
    int m_stars;
    geode::TextInput* m_input;
    cocos2d::CCLabelBMFont* m_label;
    CCNode* m_starLayout;

    bool setup(int, bool, SetStarsCallback) override;
public:
    static FRSetStarsPopup* create(int, bool, SetStarsCallback);
};
