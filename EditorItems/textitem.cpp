#include "textitem.h"

#include <QtDebug>
#define NUM_PARAMS 14

TextItem::TextItem(QGraphicsScene *scene) : SelectedItem(scene),
    /*m_select_item(new SelectedItem(scene)),*/ m_text_edit(new QTextEdit("TextItem")), m_document_item(nullptr)
{
    m_text_edit->setFrameStyle(QFrame::NoFrame);
    m_text_edit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_text_edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_text_edit->setStyleSheet("QTextEdit {background-color: rgba(0, 0, 0, 0);}");
    m_text_edit->setAlignment( Qt::AlignCenter);

    m_text_edit_proxy = scene->addWidget(m_text_edit);
    m_text_edit_proxy->setFlag(QGraphicsItem::ItemIsMovable);
    m_text_edit_proxy->setMinimumSize(0, 0);

    m_text_edit->setAlignment( Qt::AlignCenter);
    set_proxy_widget(m_text_edit_proxy);

    m_document_item = new QGraphicsTextItem;
    scene->addItem(m_document_item);
    m_document_item->hide();

    connect( m_text_edit, SIGNAL(textChanged() ), this, SLOT(text_changed_slot()) );
}

TextItem::TextItem(const TextItem& text_item) : SelectedItem (text_item.scene()),
     m_text_edit(new QTextEdit("TextItem")), m_document_item(nullptr)
{
    setGeometry(text_item.boundingRect());

    m_text_edit->setFrameStyle(QFrame::NoFrame);
    m_text_edit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_text_edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_text_edit->setStyleSheet("QTextEdit {background-color: rgba(0, 0, 0, 0);}");

    m_text_edit->setText( text_item.m_text_edit->toPlainText());
    m_text_edit->setAlignment( text_item.m_text_edit->alignment());
    m_text_edit->setFont(text_item.m_text_edit->font() );
    this->setColor(text_item.m_text_edit->textColor());

    m_text_edit_proxy = text_item.scene()->addWidget(m_text_edit);
    m_text_edit_proxy->setFlag(QGraphicsItem::ItemIsMovable);
    m_text_edit_proxy->setMinimumSize(0, 0);

    set_proxy_widget(m_text_edit_proxy);

    m_document_item = new QGraphicsTextItem;
    text_item.scene()->addItem(m_document_item);
    m_document_item->hide();
    updateHandlesPos();

    connect( m_text_edit, SIGNAL(textChanged() ), this, SLOT(text_changed_slot()) );
}


void TextItem::start_print()
{
    hide();
    m_text_edit_proxy->hide();
    QTextDocument* text_doc = m_text_edit->document();

    m_document_item->setDocument(text_doc);

    QPointF point = getItemPos();

    m_document_item->setPos(point);
    m_document_item->show();
}

void TextItem::end_print()
{
    m_document_item->hide();
    show();
    m_text_edit_proxy->show();
}

void TextItem::deleteItem(QGraphicsScene *scene)
{
    disconnect( m_text_edit, SIGNAL(textChanged() ), this, SLOT(text_changed_slot()) );
    scene->removeItem(m_document_item);
    scene->removeItem(m_text_edit_proxy);
    deleteHadles(scene);
    delete m_document_item;
    delete m_text_edit_proxy;
    scene->removeItem(this);
}

QTextEdit* TextItem::getTextEdit() const
{
    return  m_text_edit;
}

int TextItem::getFontSize() const
{
    return  int(m_text_edit->fontPointSize());
}

void TextItem::setFontSize(int size)
{
    QFont font = m_text_edit->font();
    if(font.pointSize() != size)
    {
        font.setPointSize(size);
        m_text_edit->setFont(font);
    }
}

void TextItem::setFontFamily(const QString& str)
{
    QFont font = m_text_edit->font();
    if(font.family() != str)
    {
        font.setFamily(str);
        m_text_edit->selectAll();
        m_text_edit->setFont(font);
        QTextCursor cursor = m_text_edit->textCursor();
        cursor.clearSelection();
        m_text_edit->setTextCursor(cursor);
    }
}

void TextItem::setAlignment(int ind)
{
    Qt::Alignment align = m_text_edit->alignment();
    if(align != Qt::Alignment(ind) )
        m_text_edit->setAlignment( Qt::Alignment(ind) );
}

void TextItem::setFontBold(bool b)
{
    QFont font = m_text_edit->font();
    if(font.bold() != b)
    {
        font.setBold(b);
        m_text_edit->setFont(font);
    }
}

void TextItem::setFontItalic(bool b)
{
    QFont font = m_text_edit->font();
    if(font.italic() != b)
    {
        font.setItalic(b);
        m_text_edit->setFont(font);
    }
}

void TextItem::setFontUnderline(bool b)
{
    QFont font = m_text_edit->font();
    if(font.underline() != b)
    {
        font.setUnderline(b);
        m_text_edit->setFont(font);
    }
}

void TextItem::setText(const QString& str)
{
    if(str != m_text_edit->toPlainText() )
    {
        Qt::Alignment align = m_text_edit->alignment();
        m_text_edit->setPlainText(str);
        m_text_edit->setAlignment(align);
    }
}

void TextItem::setColor(const QColor& color)
{
    m_text_edit->selectAll();
    m_text_edit->setTextColor(color);
    QTextCursor cursor = m_text_edit->textCursor();
    cursor.clearSelection();
    m_text_edit->setTextCursor(cursor);
}

void TextItem::set_lay_z_val(qreal z)
{
    setZValue(z);
    m_text_edit_proxy->setZValue(z);
    m_document_item->setZValue(z);
}

QString TextItem::save_data()
{
    QString str("Text");
    QString separator("|");
    QRectF scene_rect = sceneBoundingRect();
    str += separator;
    str += QString::number( int(scene_rect.x() )) + separator;
    str += QString::number( int(scene_rect.y() )) + separator;
    str += QString::number( int(scene_rect.width() )) + separator;
    str += QString::number( int(scene_rect.height() )) + separator;

    QFont font = m_text_edit->font();
    str += QString::number( font.pointSize() ) + separator;
    str += QString(font.family()) + separator;
    str += QString::number(font.bold()) + separator;
    str += QString::number(font.italic()) + separator;
    str += QString::number(font.underline()) + separator;

    str += QString::number(m_text_edit->alignment()) + separator;
    str += m_text_edit->toPlainText() + separator;

    QColor color = m_text_edit->textColor();
    QString color_name = color.name();
    str += color_name + separator;
    str += QString::number(zValue()) + separator;

    str += "\n";
    return str;
}

void TextItem::load_data(const QString &str)
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

    QFont font = m_text_edit->font();
    font.setPointSize(list[5].toInt());
    font.setFamily(list[6]);
    font.setBold(list[7].toInt());
    font.setItalic(list[8].toInt());
    font.setUnderline(list[9].toInt());
    m_text_edit->setFont(font);


    m_text_edit->setText(list[11]);
    m_text_edit->setAlignment(Qt::Alignment(list[10].toInt()) );

    QColor color(list[12]);
    setColor(color);
    set_lay_z_val(list[13].toInt());
}

void TextItem::text_changed_slot()
{
    QString str = m_text_edit->toPlainText();
    emit textChanged_signal(str);
}



