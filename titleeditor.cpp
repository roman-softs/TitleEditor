#include "titleeditor.h"
#include "ui_titleeditor.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>


const int indent = 40;
const int page_width = 708;
const int page_height = 1002;

const int left_margin = 78;
const int right_margin = 39;
const int top_margin = 39;
const int bottom_margin = 39;

TitleEditor::TitleEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TitleEditor)
{
    ui->setupUi(this);
    setWindowTitle("Редактор");
    initScene();
    initConnects();
    initTextMenu();
    initImageMenu();
    initComboBoxScale();

}

TitleEditor::~TitleEditor()
{
    delete ui;
}

void TitleEditor::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

QGraphicsScene* TitleEditor::get_scene() const
{
    return m_scene;
}



QString TitleEditor::save_scene()
{
    m_not_saving_items.clear();
    return m_scene->save_scene_item();

}



void TitleEditor::print(QPainter* painter, QPrinter* printer, QRect printer_rect)
{
    m_print_rect->hide();
    m_scene->start_print();
    printer->setFullPage(true);
    QRectF rect = QRectF(0, 0, printer_rect.width(), printer_rect.height());


    QRectF source_rect = QRectF(indent + left_margin, indent + top_margin, page_width, page_height);
    if(printer->orientation() == QPrinter::Portrait)
    {

        m_scene->render(painter, rect, source_rect, Qt::IgnoreAspectRatio );

    }
    else
    {
        qreal width = source_rect.width() * (rect.width()/source_rect.height());

        rect.setX(rect.x() + (rect.width() - width)/1.2 );
        m_scene->render(painter, rect, source_rect, Qt::KeepAspectRatio );
    }
    //printer->newPage();
    printer->setFullPage(false);
    m_scene->end_print();
    m_print_rect->show();
}


void TitleEditor::initScene()
{
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(Qt::gray)));
    m_scene = new Scene(this);
    //m_scene->setSceneRect(QRectF(0, 0, page_width + 2*indent, page_height + 2*indent));

    m_scene->setSceneRect(QRectF(0, 0, page_width + 2*indent + (right_margin + left_margin), page_height + 2*indent + (top_margin + bottom_margin)));


    QGraphicsRectItem* paper_rect = m_scene->addRect(QRectF( indent, indent, page_width + (right_margin + left_margin), page_height + (top_margin + bottom_margin) ), QPen(QColor(Qt::white)), QBrush(QColor(Qt::white)));
    //paper_rect->setZValue(std::numeric_limits<qreal>::min());
    paper_rect->setZValue(-11);

    m_print_rect = m_scene->addRect(QRectF( indent + left_margin, indent + right_margin, page_width, page_height), QPen(QColor(202, 202, 202)), QBrush(QColor(255, 255, 255, 0)));
    m_print_rect->setZValue(-11);

    m_scene->setBackgroundBrush(QBrush(QColor(Qt::white)));

    ui->graphicsView->setScene(m_scene);


    m_date_item = addTextItem(QString(), 21);
    m_date_item->setPos(indent + left_margin, (980 + indent + top_margin)/1.1);



    connect(m_scene, SIGNAL(scene_clicked()), this, SLOT(clickedListSelection_slot() ));
}

void TitleEditor::initConnects()
{
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectItemList_slot(QListWidgetItem*)));

    connect(ui->toolButton_addpix, SIGNAL(clicked()), this, SLOT(addCustomImageItem_slot()));

    connect(ui->toolButton_addtext, SIGNAL(clicked()), this, SLOT(addCustomTextItem_slot()));
    connect(ui->toolButton_delete, SIGNAL(clicked()), this, SLOT(deleteCustomItem_slot()));

    connect(m_scene, SIGNAL( set_TextItem_active_s(QTextEdit*) ), this, SLOT( set_TextItem_active_slot(QTextEdit*) ) );
    connect(m_scene, SIGNAL( set_ImageItem_active_s(const QString&, bool ) ), this, SLOT( set_ImageItem_active_slot(const QString&, bool ) ) );
    connect(m_scene, SIGNAL( no_active_Items_s() ), this, SLOT( no_active_Items_slot() ) );

    connect(ui->toolButton_copy, SIGNAL( clicked() ), this, SLOT( create_copy_item_slot() ) );


    connect(ui->toolButton_layup, SIGNAL(clicked()), this, SLOT(lay_up_slot()) );
    connect(ui->toolButton_laydown, SIGNAL(clicked()), this, SLOT(lay_down_slot()) );
    //connect(ui->pushButton_pdf, SIGNAL(clicked()), this, SLOT(saveToPDF_slot()));
   //connect(ui->pushButton_print, SIGNAL(clicked() ), this, SLOT(print_clicked_slot() ));

    //connect(ui->pushButton_save, SIGNAL(clicked() ), this, SLOT( save_scene_to_file_slot() ));
    connect(ui->pushButton_save, SIGNAL(clicked() ), this, SLOT( print_clicked_slot() ));

    //connect(ui->pushButton_open, SIGNAL(clicked() ), this, SLOT( open_scene_slot() ));
}

void TitleEditor::initTextMenu()
{
    alignment_group = new QButtonGroup(this);
    alignment_group->addButton( ui->toolButton_A_left_up, int(Qt::AlignLeft) );
    alignment_group->addButton( ui->toolButton_A_center_up, int(Qt::AlignCenter) );
    alignment_group->addButton( ui->toolButton_A_right_up, int(Qt::AlignRight) );
    alignment_group->addButton( ui->toolButton_A_justify, int(Qt::AlignJustify) );

    connect(ui->spinBox, SIGNAL( valueChanged(int) ), m_scene, SLOT( set_TextItem_point_size_slot(int) ) );
    connect(ui->fontComboBox, SIGNAL( currentTextChanged(const QString &) ), m_scene, SLOT( set_TextItem_family_slot(const QString&) ) );

    connect(alignment_group, SIGNAL( buttonClicked(int) ), m_scene, SLOT( set_TextItem_aligment_slot(int) ) );

    connect(ui->toolButton_bold, SIGNAL( clicked(bool) ), m_scene, SLOT( set_TextItem_bold_slot(bool) ) );
    connect(ui->toolButton_italic, SIGNAL( clicked(bool) ), m_scene, SLOT( set_TextItem_italic_slot(bool) ) );
    connect(ui->toolButton_underlined, SIGNAL( clicked(bool) ), m_scene, SLOT( set_TextItem_underlined_slot(bool) ) );

    connect(m_scene, SIGNAL( text_changed_signal(const QString&) ), this, SLOT( setPlainText(const QString&) ) );

    connect(ui->textEdit_text, SIGNAL( textChanged() ), this, SLOT(textedit_change_text_slot() ));

    connect(ui->toolButton_color, SIGNAL(clicked() ), this, SLOT(change_text_color_slot()) );
}

void TitleEditor::initImageMenu()
{
    connect(ui->toolButton_pathAccepted, SIGNAL( clicked() ), this, SLOT( set_picture_path_slot() ) );
    connect(ui->toolButton_PicPath, SIGNAL( clicked() ), this, SLOT( open_picture_filedialog_slot() ) );
    connect(ui->lineEdit_PicPath, SIGNAL( returnPressed() ), this, SLOT( set_picture_path_slot() ) );

    connect(ui->checkBox_proportions, SIGNAL(clicked(bool)), m_scene, SLOT(set_save_proportion_slot(bool)));
}

void TitleEditor::initComboBoxScale()
{
    QStringList list_combo_box;
    for(int i = 25; i < 225; i+=25)
    {
        QString name = QString::number(i);
        name += "%";
        list_combo_box << name;
    }
    ui->comboBox_scale->addItems(list_combo_box);
    ui->comboBox_scale->setEditText(QString("100%") );

    connect(ui->comboBox_scale, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(changeSceneScale_slot(const QString& )));
}

QListWidgetItem* TitleEditor::find_selected_item()
{
    QListWidgetItem* item = nullptr;
    for(int i=0; i<ui->listWidget->count(); ++i)
    {
        item = ui->listWidget->item(i);
        if(ListWidgetSelectedItem* list_item = dynamic_cast<ListWidgetSelectedItem*>(item))
        {
            SelectedItem* sel_item = list_item->get_SelectedItem();
            if(sel_item->is_selected())
                return item;
        }
    }
    return nullptr;
}

void TitleEditor::unselect_list_items()
{
    for(int i=0; i<ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setSelected(false);
    }
}

void TitleEditor::closeEvent(QCloseEvent *event)
{
    /*if(!m_scene->has_changes())
    {
        QMainWindow::closeEvent(event);
        return;
    }

    QString mes(tr("ВНИМАНИЕ!\nВ дизайн оформления отчетов титульной страницы отчетов были внесены изменения.\n"
                   "Вы уверены что не хотите сохранить изменения?"));
    int res = QMessageBox::question(this, tr("Выход"),
                           mes, QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes)
    {
        deleteNotSavingItems();
        m_scene->set_scene_changed(false);
        QMainWindow::closeEvent(event);
    }
    else
    {
        event->ignore();
    }*/
}


QGraphicsTextItem* TitleEditor::addTextItem(const QString& text, int font_sz)
{
    QGraphicsTextItem* title_item = new QGraphicsTextItem;
    m_scene->addItem(title_item);
    title_item->setHtml(text);
    QFont font = title_item->font();
    font.setPixelSize(font_sz);
    title_item->setFont(font);

    QTextOption option = title_item->document()->defaultTextOption();
    option.setAlignment(Qt::AlignCenter);
    title_item->document()->setDefaultTextOption(option);
    title_item->setTextWidth(page_width);

    return title_item;
}



void TitleEditor::set_TextItem_active_slot(QTextEdit* text_edit)
{
    QFont font = text_edit->font();
    ui->stackedWidget->setCurrentIndex(1);
    ui->fontComboBox->setCurrentFont(font);
    ui->spinBox->setValue(font.pointSize());
    Qt::Alignment align = text_edit->alignment();

    switch (align)
    {
    case Qt::AlignLeft:
        ui->toolButton_A_left_up->setChecked(true);
        break;
    case Qt::AlignCenter:
        ui->toolButton_A_center_up->setChecked(true);
        break;
    case Qt::AlignRight:
        ui->toolButton_A_right_up->setChecked(true);
        break;
    case Qt::AlignJustify:
        ui->toolButton_A_justify->setChecked(true);
        break;
    }

    ui->toolButton_bold->setChecked(font.bold());
    ui->toolButton_italic->setChecked(font.italic());
    ui->toolButton_underlined->setChecked(font.underline());
    ui->textEdit_text->setPlainText(text_edit->toPlainText());


    QColor color = text_edit->textColor();
    QPixmap px(ui->toolButton_color->iconSize());
    px.fill(color);
    ui->toolButton_color->setIcon(px);
}


void TitleEditor::set_ImageItem_active_slot(const QString & str, bool b)
{
    ui->lineEdit_PicPath->setText(str);
    ui->stackedWidget->setCurrentIndex(2);
    ui->checkBox_proportions->setChecked(b);
}

void TitleEditor::no_active_Items_slot()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void TitleEditor::textedit_change_text_slot()
{
    m_scene->change_ItemText_text(ui->textEdit_text->toPlainText());
}

void TitleEditor::setPlainText(const QString& str)
{
    if(ui->textEdit_text->toPlainText() != str)
        ui->textEdit_text->setPlainText(str);
}


void TitleEditor::change_text_color_slot()
{
    QColor color = QColorDialog::getColor();
    QPixmap px(ui->toolButton_color->iconSize());
    px.fill(color);
    ui->toolButton_color->setIcon(px);
    m_scene->change_text_color(color);
}

void TitleEditor::add_to_list(SelectedItem* sel_item, QString name)
{
    ListWidgetSelectedItem* w_item = new ListWidgetSelectedItem(sel_item);
    QString str = name;
    w_item->setText(str);
    ui->listWidget->addItem(w_item);
    ui->listWidget->sortItems(Qt::AscendingOrder);

}

void TitleEditor::addCustomTextItem_slot()
{
    SelectedItem* sel_item = m_scene->addCustomTextItem();
    add_to_list(sel_item, QString("Text Item"));
    m_not_saving_items.append(sel_item);
}

void TitleEditor::addCustomImageItem_slot()
{
    SelectedItem* sel_item = m_scene->addCustomImageItem();
    add_to_list(sel_item, QString("Image Item"));
    m_not_saving_items.append(sel_item);
}


void TitleEditor::selectItemList_slot(QListWidgetItem* item)
{
    if( ListWidgetSelectedItem* sel_item = dynamic_cast<ListWidgetSelectedItem*>(item) )
        m_scene->select_Item(sel_item->get_SelectedItem());
}

void TitleEditor::clickedListSelection_slot()
{
    unselect_list_items();
    QListWidgetItem *item = find_selected_item();
    if(item)
        item->setSelected(true);
}

void TitleEditor::lay_up_slot()
{

    QList<QListWidgetItem *> sel_item_list = ui->listWidget->selectedItems();
    if(sel_item_list.size() > 0)
    {
        QListWidgetItem * sel_item = sel_item_list[0];
        int sel_row = ui->listWidget->row(sel_item);
        if(sel_row > 0)
        {
            if(ListWidgetSelectedItem* list_item = dynamic_cast<ListWidgetSelectedItem*>(sel_item))
            {
                QListWidgetItem * upper_item = ui->listWidget->item(sel_row - 1);
                if(ListWidgetSelectedItem* upper_list_item = dynamic_cast<ListWidgetSelectedItem*>(upper_item))
                {
                    qreal buff_z_val = upper_list_item->get_SelectedItem()->zValue();
                    upper_list_item->get_SelectedItem()->set_lay_z_val(list_item->get_SelectedItem()->zValue());
                    list_item->get_SelectedItem()->set_lay_z_val(buff_z_val);
                    ui->listWidget->sortItems(Qt::AscendingOrder);
                }
            }
        }

    }
    else
    {
        return;
    }
}

void TitleEditor::lay_down_slot()
{
    QList<QListWidgetItem *> sel_item_list = ui->listWidget->selectedItems();
    if(sel_item_list.size() > 0)
    {
        QListWidgetItem * sel_item = sel_item_list[0];
        int sel_row = ui->listWidget->row(sel_item);
        if(sel_row < (ui->listWidget->count() - 1) )
        {
            if(ListWidgetSelectedItem* list_item = dynamic_cast<ListWidgetSelectedItem*>(sel_item))
            {
                QListWidgetItem * downner_item = ui->listWidget->item(sel_row + 1);
                if(ListWidgetSelectedItem* downner_list_item = dynamic_cast<ListWidgetSelectedItem*>(downner_item))
                {
                    qreal buff_z_val = downner_list_item->get_SelectedItem()->zValue();
                    downner_list_item->get_SelectedItem()->set_lay_z_val(list_item->get_SelectedItem()->zValue());
                    list_item->get_SelectedItem()->set_lay_z_val(buff_z_val);
                    ui->listWidget->sortItems(Qt::AscendingOrder);
                }
            }
        }
    }
    else
    {
        return;
    }
}

void TitleEditor::deleteCustomItem_slot()
{
    QListWidgetItem *item = find_selected_item();
    if(item)
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    m_scene->deleteCustomItem();
}

void TitleEditor::deleteNotSavingItems()
{
    for(int j=0; j < m_not_saving_items.size(); ++j)
    {
        QListWidgetItem* item = nullptr;
        SelectedItem* sel_item = nullptr;
        for(int i=0; i<ui->listWidget->count(); ++i)
        {
            item = ui->listWidget->item(i);
            if(ListWidgetSelectedItem* list_item = dynamic_cast<ListWidgetSelectedItem*>(item))
            {
                sel_item = list_item->get_SelectedItem();
                if(m_not_saving_items[j] == sel_item)
                {
                    delete ui->listWidget->takeItem(ui->listWidget->row(item));
                    m_scene->deleteCustomItem(sel_item);
                }
            }
        }


    }
    m_not_saving_items.clear();
}

void TitleEditor::open_picture_filedialog_slot()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Открыть картинку", "pic", "*.png *.jpg *.bmp");
    ui->lineEdit_PicPath->setText(str);
    m_scene->set_Image_path(str);
}

void TitleEditor::set_picture_path_slot()
{
   QString str = ui->lineEdit_PicPath->text();
   QFile file(str);
   if(file.exists())
       m_scene->set_Image_path(str);
}

void TitleEditor::create_copy_item_slot()
{
    SelectedItem* sel_item = m_scene->createCopyItem();
    if( TextItem* item = dynamic_cast<TextItem*>(sel_item) )
        add_to_list(item, QString("Text Item"));
    else if( ImageItem* item = dynamic_cast<ImageItem*>(sel_item) )
        add_to_list(item, QString("Image Item"));
}

void TitleEditor::changeSceneScale_slot(const QString& text)
{
    QString str = text;
    str.remove(QChar('%'));
    int val = str.toInt();
    qreal scale = qreal(val) / qreal(100);
    ui->graphicsView->resetMatrix();
    ui->graphicsView->scale(scale, scale);
}

void TitleEditor::saveToPDF_slot()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Сохранить в pdf", "report", "*.pdf");


    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setPageSize(QPrinter::A4);

    m_scene->start_print();
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(str);
    QPainter painter;
    painter.begin(&printer);
    m_scene->render(&painter, printer.pageRect(), QRectF(indent, indent, page_width, page_height));
    m_scene->end_print();
    painter.end();
}

void TitleEditor::print_clicked_slot()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    printer.setPageMargins(0,0,0,0,QPrinter::Millimeter);
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setPageSize(QPrinter::A4);
    if (!m_printPreview.isNull())
        m_printPreview->deleteLater();
    m_printPreview = new QPrintPreviewDialog(&printer, this, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    m_printPreview->setWindowTitle(tr("Печать"));
    connect(m_printPreview, &QPrintPreviewDialog::paintRequested, this, &TitleEditor::print_slot);
    m_printPreview->exec();
}


void TitleEditor::print_slot(QPrinter *printer)
{
    m_scene->start_print();
    printer->setOutputFormat(QPrinter::NativeFormat);
    QPainter painter;
    painter.begin(printer);
    m_scene->render(&painter, printer->pageRect(), QRectF(indent, indent, page_width + 100 , page_height));
    m_scene->end_print();
    painter.end();
}



