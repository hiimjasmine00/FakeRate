#include <Geode/ui/Popup.hpp>

typedef std::function<void(int)> SetGDDPCallback;

class FRGDDPPopup : public geode::Popup<int, SetGDDPCallback> {
protected:
    int m_gddpIntegrationOverride;
    CCMenuItemSpriteExtra* m_selected;

    bool setup(int, SetGDDPCallback) override;
public:
    static FRGDDPPopup* create(int, SetGDDPCallback);
};
