#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "SelectedItem/selecteditem.h"
#include "EditorItems/textitem.h"
#include "EditorItems/imageitem.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent = nullptr);

    TextItem* addCustomTextItem();
    ImageItem* addCustomImageItem();

    SelectedItem* createCopyItem();

    void deleteCustomItem(SelectedItem* item);
    void clearScene();
    void change_ItemText_text(const QString& str);
    void change_text_color(const QColor color);
    void select_Item(SelectedItem *item);

    void set_Image_path(const QString& path);

    void start_print();
    void end_print();

    QString save_scene_item();
    SelectedItem *temp_load_item(const QString& str);
    bool has_changes();

    void set_scene_changed(bool b);

protected:
    void mousePressEvent (QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);

private:
    TextItem* get_selectedTextItem();
    ImageItem* get_selectedImageItem();
    void clear_selection();
    void sel_Item(SelectedItem *item);
    qreal get_max_z_value();
    qreal set_z_value(SelectedItem *item);

public slots:
    void addCustomTextItem_slot();
    void addCustomImageItem_slot();
    void deleteCustomItem();

    void set_TextItem_point_size_slot(int f_size);
    void set_TextItem_family_slot(const QString& str);
    void set_TextItem_aligment_slot(int ind);

    void set_TextItem_bold_slot(bool b);
    void set_TextItem_italic_slot(bool b);
    void set_TextItem_underlined_slot(bool b);

    void text_changed_slot(const QString& str);
    void set_save_proportion_slot(bool b);

    void item_changed_slot();

signals:
    void set_TextItem_active_s(QTextEdit*);
    void set_ImageItem_active_s(const QString&, bool );
    void no_active_Items_s();
    void text_changed_signal(const QString& );
    void scene_clicked();

private:
    QList<SelectedItem* > m_selectedItems;
    bool scene_is_changed;

};

#endif // SCENE_H
