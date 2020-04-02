#include "imageitem.h"

#define SIZE 8
#define NUM_PARAMS 8

ImageItem::ImageItem(QGraphicsScene *scene) : SelectedItem(scene),
    m_pixmap_item(nullptr), m_save_proportions(true)
{
    connect( this, SIGNAL(update_pixmap_s()), this, SLOT(update_pixmap_slot()) );
}

ImageItem::ImageItem(const ImageItem& image_item) : SelectedItem (image_item.scene()),
    m_pixmap_item(nullptr)
{
    m_save_proportions = image_item.m_save_proportions;
    setGeometry(image_item.boundingRect());
    updateHandlesPos();
    open_picture(image_item.get_path(), image_item.scene());
    connect( this, SIGNAL(update_pixmap_s()), this, SLOT(update_pixmap_slot()) );
}

void ImageItem::open_picture(const QString& str, QGraphicsScene *scene)
{
    if(m_pixmap_item)
    {
        scene->removeItem(m_pixmap_item);
        delete m_pixmap_item;
    }
    m_path = str;
    m_pixmap_item = scene->addPixmap(str);
    update_pixmap_slot();
    emit item_changed_s();
}

const QString& ImageItem::get_path() const
{
    return m_path;
}

void ImageItem::deleteItem(QGraphicsScene *scene)
{
    disconnect( this, SIGNAL(update_pixmap_s()), this, SLOT(update_pixmap_slot()) );
    scene->removeItem(m_pixmap_item);
    deleteHadles(scene);
    delete m_pixmap_item;
    scene->removeItem(this);
}

void ImageItem::set_lay_z_val(qreal z)
{
    setZValue(z);
    if(m_pixmap_item)
        m_pixmap_item->setZValue(z);
    emit item_changed_s();
}

void ImageItem::set_save_proportions(bool b)
{
    m_save_proportions = b;
    reload_pixmap();
    emit item_changed_s();
}

bool ImageItem::is_save_proportions() const
{
    return m_save_proportions;
}


void ImageItem::update_pixmap_slot()
{
    if(m_pixmap_item)
    {
        QRectF m_rect = boundingRect();
        QRectF item_rect = QRectF( m_rect.x() + SIZE / 2, m_rect.y() + SIZE / 2, m_rect.width() - SIZE, m_rect.height() - SIZE);
        QPointF point = getItemPos();
        QRectF pix_rect = m_pixmap_item->boundingRect();
        qreal width_scale = item_rect.width() / pix_rect.width();
        qreal height_scale = item_rect.height() / pix_rect.height();
        qreal w_on_h_item = item_rect.width() / item_rect.height();
        qreal w_on_h_pix = pix_rect.width() / pix_rect.height();

        if(m_save_proportions)
        {
            m_pixmap_item->setScale( (w_on_h_pix > w_on_h_item) ? width_scale: height_scale );
        }
        else
        {
            QTransform transform = m_pixmap_item->transform();
            transform.reset();
            transform.scale(width_scale, height_scale);
            m_pixmap_item->setTransform(transform);
        }

        m_pixmap_item->setPos(point);
    }
}

void ImageItem::reload_pixmap()
{
    if(m_pixmap_item)
    {
        QPixmap pixmap = m_pixmap_item->pixmap();
        scene()->removeItem(m_pixmap_item);
        delete m_pixmap_item;
        m_pixmap_item = scene()->addPixmap(pixmap);
    }
    update_pixmap_slot();
    emit item_changed_s();
}

QString ImageItem::save_data()
{
    QString str("Image");
    QString separator("|");
    QRectF scene_rect = sceneBoundingRect();
    str += separator;
    str += QString::number( int(scene_rect.x() )) + separator;
    str += QString::number( int(scene_rect.y() )) + separator;
    str += QString::number( int(scene_rect.width() )) + separator;
    str += QString::number( int(scene_rect.height() )) + separator;

    str += QString::number( int(is_save_proportions() )) + separator;
    str += m_path + separator;
    str += QString::number( int(zValue() )) + separator;

    str += "\n";
    return str;
}
void ImageItem::load_data(const QString &str)
{
    QStringList list = str.split("|", QString::SkipEmptyParts);

    if(list.size() != NUM_PARAMS)
        return;
    int x = list[1].toInt();
    int y = list[2].toInt();
    int width = list[3].toInt();
    int height = list[4].toInt();

    QRectF scene_start_rect(x, y, width, height);
    set_starting_position(scene_start_rect);

    set_save_proportions(list[5].toInt());
    open_picture(list[6], scene() );
    set_lay_z_val(list[7].toInt());
}
