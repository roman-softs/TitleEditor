#ifndef TITLEITEMS_H
#define TITLEITEMS_H



#include <QPainter>
#include <QGraphicsItem>
#include <QTextDocument>

class TitleGraphicsItem :public QGraphicsItem
{
public:
    TitleGraphicsItem();

    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    QRectF boundingRect() const;
    void setHtml(QString str);
    void setFont(const QFont& font);
private:
   QTextDocument m_document_header;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};



#endif // TITLEITEMS_H
