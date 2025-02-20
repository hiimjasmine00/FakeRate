#include <Geode/ui/Layout.hpp>

class TableNode : public cocos2d::CCNode {
protected:
    cocos2d::CCArray* m_menus;
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
    void setRowPrefix(const std::string&);
    void updateAllLayouts();
    void addButton(cocos2d::CCMenuItem*);

    ~TableNode() override;
};
