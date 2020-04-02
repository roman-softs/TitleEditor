#ifndef LISTWIDGETSELECTEDITEM_H
#define LISTWIDGETSELECTEDITEM_H

#include <QListWidgetItem>
#include <SelectedItem/selecteditem.h>

class ListWidgetSelectedItem : public QListWidgetItem
{
public:
    ListWidgetSelectedItem();
    ListWidgetSelectedItem(SelectedItem* sel_item);
    void set_SelectedItem(SelectedItem* sel_item);
    SelectedItem* get_SelectedItem() const;

    bool operator< ( const QListWidgetItem & other ) const;


private:
    SelectedItem* m_sel_item;
};

#endif // LISTWIDGETSELECTEDITEM_H
