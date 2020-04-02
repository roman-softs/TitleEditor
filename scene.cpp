#include "scene.h"

#include <QDebug>

Scene::Scene(QObject *parent) : QGraphicsScene(parent),
    scene_is_changed(false)
{

}

TextItem* Scene::addCustomTextItem()
{
    item_changed_slot();
    TextItem* item = new TextItem(this);
    m_selectedItems.append(item);
    set_z_value(item);
    item->setFontSize(8);

    connect(item, SIGNAL( textChanged_signal(const QString& ) ), this, SLOT( text_changed_slot(const QString& ) ));
    connect(item, SIGNAL( item_changed_s() ), this, SLOT( item_changed_slot() ));
    for(int i = 0; i < m_selectedItems.size(); ++i)
    {
       if(item != m_selectedItems[i])
            m_selectedItems[i]->set_selected(false);
    }
    return  item;
}

ImageItem* Scene::addCustomImageItem()
{
    item_changed_slot();
    ImageItem* item = new ImageItem(this);
    item->set_save_proportions(false);
    m_selectedItems.append(item);
    set_z_value(item);

    connect(item, SIGNAL( item_changed_s() ), this, SLOT( item_changed_slot() ));
    for(int i = 0; i < m_selectedItems.size(); ++i)
    {
       if(item != m_selectedItems[i])
            m_selectedItems[i]->set_selected(false);
    }
    return  item;
}

SelectedItem* Scene::createCopyItem()
{
    if(TextItem* text_item = get_selectedTextItem())
    {
        TextItem* item = new TextItem(*text_item);
        m_selectedItems.append(item);
        set_z_value(item);

        connect(item, SIGNAL( textChanged_signal(const QString& ) ), this, SLOT( text_changed_slot(const QString& ) ));
        return  item;
    }
    else if(ImageItem* imageitem = get_selectedImageItem())
    {
        ImageItem* item = new ImageItem(*imageitem);
        m_selectedItems.append(item);
        set_z_value(item);
        return  item;

    }
    return  nullptr;
}

void Scene::deleteCustomItem(SelectedItem* item)
{
    //this->removeItem(item);
    disconnect(item, SIGNAL( item_changed_s() ), this, SLOT( item_changed_slot() ));
    if( dynamic_cast<TextItem*>(item) )
        disconnect(item, SIGNAL( textChanged_signal(const QString&) ), this, SLOT( text_changed_slot(const QString& ) ));
    item->deleteItem(this);
    if(item)
    {
        m_selectedItems.removeAll(item);
        delete item;
    }
    //this->itemAt(100, 100, QTransform());
}

void Scene::clearScene()
{
    while(m_selectedItems.size() > 0)
        deleteCustomItem(m_selectedItems[0]);
}

void Scene::change_text_color(const QColor color)
{
    if(TextItem* text_item = get_selectedTextItem())
        text_item->setColor(color);
}

void Scene::set_Image_path(const QString& path)
{
    if(ImageItem* imageitem = get_selectedImageItem())
        imageitem->open_picture(path, this);
}

void Scene::start_print()
{
    for(int i = 0; i < m_selectedItems.size(); ++i)
    {
        m_selectedItems[i]->set_selected(false);
        m_selectedItems[i]->start_print();
    }
}

void Scene::end_print()
{
    for(int i = 0; i < m_selectedItems.size(); ++i)
        m_selectedItems[i]->end_print();
}

void Scene::select_Item(SelectedItem *item)
{
    clear_selection();
    //if( TextItem* textitem = dynamic_cast<TextItem*>(item) )
   //     textitem->set_selected(true);
    item->set_selected(true);
    sel_Item(item);
}


void Scene::sel_Item(SelectedItem* item)
{
    if( TextItem* textitem = dynamic_cast<TextItem*>(item) )
    {
        QTextEdit* text_edit = textitem->getTextEdit();
        emit set_TextItem_active_s(text_edit);
    } else if( ImageItem* imageitem =  dynamic_cast<ImageItem*>(item))
    {
        QString str = imageitem->get_path();
        bool b = imageitem->is_save_proportions();
        emit set_ImageItem_active_s(str, b );
    } else
    {
        emit no_active_Items_s();
    }


    for(int i = 0; i < m_selectedItems.size(); ++i)
    {
       if(item != m_selectedItems[i])
            m_selectedItems[i]->set_selected(false);
    }
}

qreal Scene::get_max_z_value()
{
    //qreal z_val = std::numeric_limits<qreal>::min();
    qreal z_val = 0;
    for(int i = 0; i < m_selectedItems.size(); ++i)
    {
       if(m_selectedItems[i]->zValue() > z_val)
            z_val = m_selectedItems[i]->zValue();
    }
    return z_val;
}

qreal Scene::set_z_value(SelectedItem *item)
{
    qreal max_z_val = get_max_z_value();
    max_z_val ++;
    item->set_lay_z_val(max_z_val);
}


void Scene::mousePressEvent (QGraphicsSceneMouseEvent * event)
{

    clear_selection();
    QPointF point = event->scenePos();

    QList<QGraphicsItem *> list;
    list = this->items(point );

    SelectedItem * after_item = nullptr;
    qreal zVal = std::numeric_limits<qreal>::min();
    foreach (QGraphicsItem * const item, m_selectedItems)
    {
        QRectF itemGeometry = item->sceneBoundingRect();
        itemGeometry.setX(itemGeometry.x() - 8);
        itemGeometry.setY(itemGeometry.y() - 8);
        itemGeometry.setWidth(itemGeometry.width() + 8);
        itemGeometry.setHeight(itemGeometry.height() + 8);
        if(itemGeometry.contains(point))
        {
            if(SelectedItem* sel_item = dynamic_cast<SelectedItem*>(item))
            {
                if( sel_item->zValue() > zVal )
                {
                    zVal = sel_item->zValue();
                    after_item = sel_item;
                }
            }
        }
    }

    if(after_item)
    {
        after_item->set_selected(true);
    }

    sel_Item(after_item);
    emit scene_clicked();
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseReleaseEvent (QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

TextItem* Scene::get_selectedTextItem()
{
    for(int i = 0; i < m_selectedItems.size(); ++i)
    {
        SelectedItem* sel_item = m_selectedItems[i];
        if(sel_item->is_selected())
        {
            if(  TextItem* text_item = dynamic_cast<TextItem*>(sel_item)    )
            {
                return text_item;
            }
        }
    }
    return nullptr;
}

ImageItem* Scene::get_selectedImageItem()
{
    for(int i = 0; i < m_selectedItems.size(); ++i)
    {
        SelectedItem* sel_item = m_selectedItems[i];
        if(sel_item->is_selected())
        {
            if(  ImageItem* imageitem = dynamic_cast<ImageItem*>(sel_item)    )
            {
                return imageitem;
            }
        }
    }
    return nullptr;
}

void Scene::clear_selection()
{
    for(int i = 0; i < m_selectedItems.size(); ++i)
        m_selectedItems[i]->set_selected(false);
}

void Scene::addCustomTextItem_slot()
{
    addCustomTextItem();
}

void Scene::addCustomImageItem_slot()
{
    addCustomImageItem();
}

void Scene::deleteCustomItem()
{
    for(int i = 0; i < m_selectedItems.size(); ++i)
    {
       if(m_selectedItems[i]->is_selected())
       {
           deleteCustomItem(m_selectedItems[i]);
           item_changed_slot();
       }
    }
}


void Scene::set_TextItem_point_size_slot(int f_size)
{
    if(TextItem* text_item = get_selectedTextItem())
        text_item->setFontSize(f_size);

}

void Scene::set_TextItem_family_slot(const QString& str)
{
    if(TextItem* text_item = get_selectedTextItem())
        text_item->setFontFamily(str);
}

void Scene::set_TextItem_aligment_slot(int ind)
{
    if(TextItem* text_item = get_selectedTextItem())
        text_item->setAlignment(ind);
}

void Scene::set_TextItem_bold_slot(bool b)
{
    if(TextItem* text_item = get_selectedTextItem())
        text_item->setFontBold(b);
}

void Scene::set_TextItem_italic_slot(bool b)
{
    if(TextItem* text_item = get_selectedTextItem())
        text_item->setFontItalic(b);
}

void Scene::set_TextItem_underlined_slot(bool b)
{
    if(TextItem* text_item = get_selectedTextItem())
        text_item->setFontUnderline(b);
}


void Scene::text_changed_slot(const QString& str)
{
    emit text_changed_signal(str);
}

void Scene::set_save_proportion_slot(bool b)
{
    if(ImageItem* imageitem = get_selectedImageItem())
        imageitem->set_save_proportions(b);
}

void Scene::item_changed_slot()
{
    scene_is_changed = true;
}

void Scene::change_ItemText_text(const QString& str)
{
    if(TextItem* text_item = get_selectedTextItem())
        text_item->setText(str);
}

QString Scene::save_scene_item()
{
    SelectedItem* item = nullptr;
    QString str;
    for(int i = 0; i < m_selectedItems.size(); ++i)
    {
        SelectedItem* sel_item = m_selectedItems[i];

            if(  TextItem* text_item = dynamic_cast<TextItem*>(sel_item)    )
            {
                item = text_item;
                str += item->save_data();
            }
            else if (  ImageItem* image_item = dynamic_cast<ImageItem*>(sel_item) )
            {
                item = image_item;
                str += item->save_data();
            }
    }
    scene_is_changed = false;
    return str;
}


SelectedItem* Scene::temp_load_item(const QString& str)
{
    QStringList list;
    list = str.split("|");

    SelectedItem* item = nullptr;
    if(list[0] == "Text")
    {
        item = addCustomTextItem();
        item->load_data(str);
    }
    else if(list[0] == "Image")
    {
        item = addCustomImageItem();
        item->load_data(str);
    }
    scene_is_changed = false;
    return item;
}

bool Scene::has_changes()
{
    return scene_is_changed;
}

void Scene::set_scene_changed(bool b)
{
    scene_is_changed = b;
}
