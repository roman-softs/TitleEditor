#ifndef SELECTEDITEM_H
#define SELECTEDITEM_H

#include <QObject>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>


class SelectedItem;

class HandleItem :public QGraphicsItem
{
public:
    enum Type
    {
        LeftTop,
        Top,
        RightTop,
        Right,
        RightBottom,
        Bottom,
        LeftBottom,
        Left,

        TypeCount
    };

    HandleItem(Type t, SelectedItem* selected_item);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    QRectF boundingRect() const;
    void updatePosition(const QRectF &rect);
    Type getType() const;
    void set_selected(bool select);
    void select_all();
    SelectedItem* get_selected_item() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    void updateCursor();

    SelectedItem* m_selected_item;
    QRectF m_geom, m_origGeom;
    QRectF sel_item_scene_geom;
    QRectF m_bounding_rect_press;
    QRectF m_min_rect_size;
    const Type m_type;

    qreal old_x_cord;
    qreal old_y_cord;

    bool m_selected;
};


//////////////////////////////////////////////////

class SelectedItem : public QObject, public QGraphicsItem
{
   Q_OBJECT
public:
    SelectedItem(QGraphicsScene *scene);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    QRectF boundingRect() const;
    void setGeometry(const QRectF& rect);
    void updateHandlesPos();

    void set_item(QGraphicsItem* item);
    void set_proxy_widget(QGraphicsProxyWidget* proxy);

    void set_selected(bool select);
    bool is_selected() const;

    QPointF getItemPos() const;

    void deleteHadles(QGraphicsScene *scene);
    virtual void deleteItem(QGraphicsScene *scene);
    virtual void set_lay_z_val(qreal z);

    virtual void start_print();
    virtual void end_print();


    virtual QString save_data() = 0;
    virtual void load_data(const QString &str) = 0;

    void set_starting_position(QRectF rect);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void update_handles_selection();

    QRectF m_rect;
    HandleItem *m_handles[HandleItem::TypeCount];

    QGraphicsItem* m_item;
    QGraphicsProxyWidget* m_proxy_widget;
    QGraphicsPixmapItem* m_pixmap_item;
    bool m_selected;

signals:
    void update_pixmap_s();
    void item_changed_s();
};




#endif // SELECTEDITEM_H
