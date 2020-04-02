#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPainter>

#include "SelectedItem/selecteditem.h"

class ImageItem : public SelectedItem
{
    Q_OBJECT
public:
    ImageItem(QGraphicsScene *scene);
    ImageItem(const ImageItem& image_item);

    void open_picture(const QString& str, QGraphicsScene *scene);
    const QString& get_path() const;
    virtual void deleteItem(QGraphicsScene *scene);
    virtual void set_lay_z_val(qreal z);
    void set_save_proportions(bool b);
    bool is_save_proportions() const;
    void reload_pixmap();

    QString save_data();
    void load_data(const QString &str);

private:
    QGraphicsPixmapItem* m_pixmap_item;
    QString m_path;
    bool m_save_proportions;

public slots:
    void update_pixmap_slot();

};

#endif // IMAGEITEM_H
