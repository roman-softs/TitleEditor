#include "selecteditem.h"

#include <QDebug>
#include <QtGui>

#include <QtMath>

#define SIZE 8

HandleItem::HandleItem(Type t, SelectedItem* selected_item) : m_selected_item(selected_item), m_min_rect_size(0, 0, 25, 25), m_type(t),
    m_selected(true)
{
    setZValue(std::numeric_limits<qreal>::max());
    setFlags(QGraphicsItem::ItemIsMovable);
    updateCursor();
    m_origGeom = m_selected_item->sceneBoundingRect();
    this->updatePosition(m_origGeom);
}

void HandleItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(m_selected)
        painter->fillRect( 0, 0, SIZE - 1, SIZE - 1, QBrush(QColor("#096BFF")) );
}

QRectF HandleItem::boundingRect() const
{
    return QRectF(0,0,SIZE,SIZE);
}

HandleItem::Type HandleItem::getType() const
{
    return m_type;
}

void HandleItem::set_selected(bool select)
{
    m_selected = select;
}

void HandleItem::select_all()
{
    m_selected_item->set_selected(true);
}

SelectedItem* HandleItem::get_selected_item() const
{
    return m_selected_item;
}


void HandleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    m_origGeom = m_selected_item->boundingRect();
    sel_item_scene_geom = m_selected_item->sceneBoundingRect();
    m_bounding_rect_press = sceneBoundingRect();
    old_x_cord = m_origGeom.x();
    old_y_cord = m_origGeom.y();
}

void HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    m_geom = m_origGeom;

    const int w = SIZE;
    const int h = SIZE;

    QRectF scene_bounding_rect = sceneBoundingRect();

    QPointF alignedPoint = QPointF(scene_bounding_rect.x() + w / 2 - sel_item_scene_geom.x() + m_origGeom.x() , scene_bounding_rect.y() + h/2 - sel_item_scene_geom.y() + m_origGeom.y());
    switch (m_type) {
        case LeftTop:
            m_geom.setTopLeft(alignedPoint);
            break;
        case Top:
            {
                m_geom.setTop(alignedPoint.y());
                this->setX(m_bounding_rect_press.x());
            }
            break;
        case RightTop:
            m_geom.setTopRight(alignedPoint);
            break;
        case Right:
            {
                m_geom.setRight(alignedPoint.x());
                this->setX(m_bounding_rect_press.y());
            }
            break;
        case RightBottom:
            m_geom.setBottomRight(alignedPoint);
            break;
        case Bottom:
            {
                m_geom.setBottom(alignedPoint.y());
                this->setX(m_bounding_rect_press.x());
            }
            break;
        case LeftBottom:
            m_geom.setBottomLeft(alignedPoint);
            break;
        case Left:
            {
                m_geom.setLeft(alignedPoint.x());
                this->setX(m_bounding_rect_press.y());
            }
            break;
        default:
            Q_ASSERT(0);
    }


    bool update_geometry = true;
    if (m_geom.width() < m_min_rect_size.width())
    {
        m_geom.setX(old_x_cord);
        m_geom.setWidth(m_min_rect_size.width());
        update_geometry = false;
    }
    else
    {
        old_x_cord = m_geom.x() ;
    }
    if (m_geom.height() < m_min_rect_size.height())
    {
        m_geom.setY(old_y_cord);
        m_geom.setHeight(m_min_rect_size.height());
        update_geometry = false;
    }
    else
    {
        old_y_cord = m_geom.y() ;
    }


    m_selected_item->setGeometry(m_geom);
    m_selected_item->updateHandlesPos();

    QGraphicsScene* scene = this->scene();
    scene->itemsBoundingRect();
    scene->update();
}

void HandleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void HandleItem::updatePosition(const QRectF& rect)
{
    const int w = SIZE;
    const int h = SIZE;

    QPointF point;

    switch (m_type) {
        case LeftTop:
            point = QPointF(rect.topLeft().x() - w / 2, rect.topLeft().y() - h / 2);
            break;
        case Top:
            point = QPointF(rect.x() + rect.width()/2 - w / 2, rect.y() - h / 2);
            break;
        case RightTop:
            point = QPointF(rect.x() + rect.width() - w / 2, rect.y() - h / 2);
            break;
        case Right:
            point = QPointF(rect.x() + rect.width() - w / 2, rect.y() + rect.height() / 2 - h / 2);
            break;
        case RightBottom:
            point = QPointF(rect.bottomRight().x() - w / 2, rect.bottomRight().y() - h / 2);
            break;
        case Bottom:
            point = QPointF(rect.x() + rect.width() / 2 - w / 2, rect.y() + rect.height() - h / 2);
            break;
        case LeftBottom:
            point = QPointF(rect.x() - w / 2, rect.y() + rect.height() - h / 2);
            break;
        case Left:
            point = QPointF(rect.x() - w / 2, rect.y() + rect.height() / 2 - h / 2);
            break;
        default:
            Q_ASSERT(0);
    }
    setPos(point);
}

void HandleItem::updateCursor()
{
    switch (m_type) {
        case LeftTop:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case Top:
            setCursor(Qt::SizeVerCursor);
            break;
        case RightTop:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case Right:
            setCursor(Qt::SizeHorCursor);
            break;
        case RightBottom:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case Bottom:
            setCursor(Qt::SizeVerCursor);
            break;
        case LeftBottom:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case Left:
            setCursor(Qt::SizeHorCursor);
            break;
        default:
            Q_ASSERT(0);
    }
}


////////////////////////////////////////////////////////////////



SelectedItem::SelectedItem(QGraphicsScene *scene): QObject(),
    m_item(nullptr), m_proxy_widget(nullptr), m_pixmap_item(nullptr), m_selected(true)
{
    setCursor(Qt::SizeAllCursor);
    scene->addItem(this);
    setFlags(QGraphicsItem::ItemIsMovable);
    m_rect = QRectF(0, 0, 50, 50);
    this->setPos(100, 100);

    for (int i = HandleItem::LeftTop; i < HandleItem::TypeCount; ++i)
    {
        m_handles[i] = new HandleItem(static_cast<HandleItem::Type>(i), this);
        this->scene()->addItem(m_handles[i]);
    }
}

void SelectedItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen(QColor(128,128,128));
    pen.setStyle(Qt::DashDotLine);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->setOpacity(0.5);
    painter->drawRect(m_rect);

    if(m_proxy_widget)
    {
        QRectF item_rect = QRectF( m_rect.x() + SIZE / 2, m_rect.y() + SIZE / 2, m_rect.width() - SIZE, m_rect.height() - SIZE);
        QPointF point = getItemPos();
        m_proxy_widget->resize(item_rect.size());
        m_proxy_widget->setPos(point);
    }
}

QRectF SelectedItem::boundingRect() const
{
    return m_rect;
}

void SelectedItem::setGeometry(const QRectF& rect)
{
    prepareGeometryChange();
    m_rect = rect;
    emit item_changed_s();
    update();
}

void SelectedItem::updateHandlesPos()
{
    for (int i = HandleItem::LeftTop; i < HandleItem::TypeCount; ++i)
    {
        m_handles[i]->updatePosition(sceneBoundingRect());
    }
    emit update_pixmap_s();
}

void SelectedItem::set_item(QGraphicsItem* item)
{
    m_item = item;
}

void SelectedItem::set_proxy_widget(QGraphicsProxyWidget* proxy)
{
    m_proxy_widget = proxy;
}


void SelectedItem::set_selected(bool select)
{
    m_selected = select;
    if(select)
    {
        for (int i = HandleItem::LeftTop; i < HandleItem::TypeCount; ++i)
        {
            m_handles[i]->show();
        }
    }
    else
    {
        for (int i = HandleItem::LeftTop; i < HandleItem::TypeCount; ++i)
        {
            m_handles[i]->hide();
        }
    }
}

bool SelectedItem::is_selected() const
{
    return m_selected;
}


QPointF SelectedItem::getItemPos() const
{
    QRectF scene_rect = sceneBoundingRect();
    QPointF point = QPointF(scene_rect.topLeft().x() +  SIZE / 2, scene_rect.topLeft().y() +  SIZE / 2);
    return point;

}

void SelectedItem::deleteHadles(QGraphicsScene *scene)
{
    for (int i = HandleItem::LeftTop; i < HandleItem::TypeCount; ++i)
    {
        scene->removeItem(m_handles[i]);
        delete m_handles[i];
    }
}

void SelectedItem::deleteItem(QGraphicsScene *scene)
{
    deleteHadles(scene);
}

void SelectedItem::set_lay_z_val(qreal z)
{
    setZValue(z);
}

void SelectedItem::start_print()
{
    hide();
}

void SelectedItem::end_print()
{
    show();
}

void SelectedItem::set_starting_position(QRectF rect)
{
    setGeometry(QRectF(0, 0, rect.width(), rect.height()) );
    setPos(rect.x(), rect.y());
    updateHandlesPos();
}


void SelectedItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);

}

void SelectedItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    QRectF rect = sceneBoundingRect();

    for (int i = HandleItem::LeftTop; i < HandleItem::TypeCount; ++i)
    {
        m_handles[i]->updatePosition(rect);
    }
    emit update_pixmap_s();
    emit item_changed_s();
}

void SelectedItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}


void SelectedItem::update_handles_selection()
{
    for (int i = HandleItem::LeftTop; i < HandleItem::TypeCount; ++i)
    {
        m_handles[i]->set_selected(m_selected);
    }
}
