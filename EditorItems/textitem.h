#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QWidget>
#include <QTextDocument>
#include <QTextEdit>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QFont>

#include "SelectedItem/selecteditem.h"
#include "savedatatypes.h"

class TextItem : public SelectedItem
{
    Q_OBJECT
public:
    TextItem(QGraphicsScene* scene);
    TextItem(const TextItem& text_item);

    virtual void start_print();
    virtual void end_print();

    virtual void deleteItem(QGraphicsScene *scene);
    QTextEdit* getTextEdit() const;
    int getFontSize() const;
    void setFontSize(int size);
    void setFontFamily(const QString& str);
    void setAlignment(int ind);
    void setFontBold(bool b);
    void setFontItalic(bool b);
    void setFontUnderline(bool b);
    void setText(const QString& str);
    void setColor(const QColor& color);

    void set_lay_z_val(qreal z);

    QString save_data();
    void load_data(const QString &str);

private:
    //TextItemData save_data;
    QGraphicsScene* m_scene;
    QTextEdit* m_text_edit;
    QGraphicsTextItem* m_document_item;

    //QTextDocument* m_text_document;
    QGraphicsProxyWidget* m_text_edit_proxy;    

public slots:
    void text_changed_slot();

signals:
    void textChanged_signal(const QString& str);
};

#endif // TEXTITEM_H
