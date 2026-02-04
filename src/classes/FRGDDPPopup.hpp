#include <Geode/ui/Popup.hpp>

typedef geode::Function<void(int)> SetGDDPCallback;

class FRGDDPPopup : public geode::Popup {
protected:
    int m_gddpIntegrationOverride;
    CCMenuItemSpriteExtra* m_selected;

    bool init(int, SetGDDPCallback);
public:
    static FRGDDPPopup* create(int, SetGDDPCallback);
};
