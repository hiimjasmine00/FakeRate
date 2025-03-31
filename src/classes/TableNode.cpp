#include "TableNode.hpp"
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

TableNode* TableNode::create(int columns, int rows) {
    auto ret = new TableNode();
    if (ret->init(columns, rows)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TableNode::init(int columns, int rows) {
    if (!CCNode::init()) return false;

    setAnchorPoint({ 0.5f, 0.5f });
    m_menus = CCArray::create();
    m_menus->retain();
    m_columns = columns;
    m_rows = rows;

    return true;
}

void TableNode::setColumnLayout(AxisLayout* columnLayout) {
    m_columnLayout = columnLayout;
    setLayout(m_columnLayout);
}

void TableNode::setRowLayout(AxisLayout* rowLayout) {
    m_rowLayout = rowLayout;
    for (auto menu : CCArrayExt<CCNode*>(m_menus)) {
        menu->setLayout(m_rowLayout);
    }
}

void TableNode::setRowHeight(float rowHeight) {
    m_rowHeight = rowHeight;
    for (auto menu : CCArrayExt<CCNode*>(m_menus)) {
        menu->setContentSize({ m_obContentSize.width, rowHeight });
    }
}

void TableNode::setRowPrefix(const std::string& rowPrefix) {
    m_rowPrefix = rowPrefix;
    for (int i = 0; i < m_menus->count(); i++) {
        auto menu = static_cast<CCNode*>(m_menus->objectAtIndex(i));
        menu->setID(fmt::format("{}-{}", rowPrefix, i + 1));
    }
}

void TableNode::updateAllLayouts() {
    for (auto menu : CCArrayExt<CCNode*>(m_menus)) {
        menu->updateLayout();
    }
    updateLayout();
}

void TableNode::addButton(CCMenuItem* button) {
    CCNode* menu = nullptr;
    if (m_menus->count() <= 0 || static_cast<CCNode*>(m_menus->objectAtIndex(m_menus->count() - 1))->getChildrenCount() >= m_columns) {
        menu = CCMenu::create();
        menu->setContentSize({ m_obContentSize.width, m_rowHeight });
        menu->setLayout(m_rowLayout);
        menu->setID(fmt::format("{}-{}", m_rowPrefix, m_menus->count() + 1));
        addChild(menu);
        m_menus->addObject(menu);
    } else menu = static_cast<CCNode*>(m_menus->objectAtIndex(m_menus->count() - 1));

    menu->addChild(button);
}

TableNode::~TableNode() {
    CC_SAFE_RELEASE(m_menus);
}
