#include "listwidgetselecteditem.h"

#include <QDebug>

ListWidgetSelectedItem::ListWidgetSelectedItem() :
    m_sel_item(nullptr)
{

}

ListWidgetSelectedItem::ListWidgetSelectedItem(SelectedItem* sel_item) :
    m_sel_item(sel_item)
{

}

void ListWidgetSelectedItem::set_SelectedItem(SelectedItem* sel_item)
{
    m_sel_item = sel_item;
}

SelectedItem* ListWidgetSelectedItem::get_SelectedItem() const
{
    return m_sel_item;
}


bool ListWidgetSelectedItem::operator< ( const QListWidgetItem & other ) const
{
    const QListWidgetItem* other_item_p = &other;
    if(const ListWidgetSelectedItem* list_item = dynamic_cast<const ListWidgetSelectedItem*>(other_item_p))
    {
        const SelectedItem* sel_item = list_item->get_SelectedItem();
        if(this->get_SelectedItem()->zValue() > sel_item->zValue())
            return true;
    }
    return false;
}
