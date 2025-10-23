#include <Geode/ui/Layout.hpp>
#include <Geode/utils/cocos.hpp>

class TableNode : public cocos2d::CCLayer {
protected:
    geode::Ref<cocos2d::CCArray> m_menus;
    geode::AxisLayout* m_columnLayout;
    geode::AxisLayout* m_rowLayout;
    int m_columns;
    int m_rows;
    float m_rowHeight;
    std::string m_rowPrefix;

    bool init(int columns, int rows);
public:
    static TableNode* create(int columns, int rows);

    void setColumnLayout(geode::AxisLayout*);
    void setRowLayout(geode::AxisLayout*);
    void setRowHeight(float);
    void setRowPrefix(std::string_view);
    void updateAllLayouts();
    void addButton(cocos2d::CCMenuItem*);
};
