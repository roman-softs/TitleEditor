#include "titleitems.h"


TitleGraphicsItem::TitleGraphicsItem()
{
    setFlag(ItemIsMovable);
    m_document_header.setHtml("<P align=\"left\">"
                                "<div "
                                "style = \""
                                "font-size: 12pt;\""
                                ">Crownscopy-Lab<hr/></div></P>");
}

void TitleGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF page_rect(0, 0, 708, 100);
    m_document_header.setPageSize(page_rect.size());

    QFont font;
    m_document_header.setDefaultFont(font);
    m_document_header.drawContents(painter, page_rect);
}

QRectF TitleGraphicsItem::boundingRect() const
{
    return QRectF(0,0,708,100);
}

void TitleGraphicsItem::setHtml(QString str)
{
    m_document_header.setHtml(str);
    update();
}

void TitleGraphicsItem::setFont(const QFont& font)
{
    m_document_header.setDefaultFont(font);
}

void TitleGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void TitleGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}



