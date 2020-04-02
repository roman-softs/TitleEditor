#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPointer>

#include "scene.h"
#include "listwidgetselecteditem.h"
#include "titleitems.h"

namespace Ui {
class TitleEditor;
}

class TitleEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit TitleEditor(QWidget *parent = nullptr);
    ~TitleEditor();

    void showEvent(QShowEvent *event);

    QGraphicsScene* get_scene() const;

    QString save_scene();
    void print(QPainter *painter, QPrinter* printer, QRect printer_rect);


private:
    Ui::TitleEditor *ui;
    void initScene();
    void initConnects();
    void initTextMenu();
    void initImageMenu();
    void initComboBoxScale();
    void add_to_list(SelectedItem* sel_item, QString name);

    void deleteNotSavingItems();

    QListWidgetItem* find_selected_item();
    void unselect_list_items();
    void closeEvent(QCloseEvent *event);

    QGraphicsTextItem* addTextItem(const QString& text, int font_sz);


    Scene* m_scene;
    QButtonGroup* alignment_group;
    QPointer<QPrintPreviewDialog> m_printPreview;

    QGraphicsTextItem* m_title_item;

    QList<SelectedItem* > m_not_saving_items;

    QGraphicsRectItem* m_print_rect;


    QGraphicsTextItem* m_date_item;

public slots:
    void set_TextItem_active_slot(QTextEdit* text_edit);
    void set_ImageItem_active_slot(const QString& str, bool b);
    void no_active_Items_slot();

    void textedit_change_text_slot();
    void setPlainText(const QString& str);
    void change_text_color_slot();

    void addCustomTextItem_slot();
    void addCustomImageItem_slot();

    void deleteCustomItem_slot();

    void selectItemList_slot(QListWidgetItem* item);
    void clickedListSelection_slot();

    void lay_up_slot();
    void lay_down_slot();

    void open_picture_filedialog_slot();
    void set_picture_path_slot();

    void create_copy_item_slot();

    void changeSceneScale_slot(const QString& text);
    void saveToPDF_slot();
    void print_clicked_slot();
    void print_slot(QPrinter* printer);

};

#endif // MAINWINDOW_H
