#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class AddWindow;
}

class AddWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddWindow(QWidget *parent = nullptr);
    ~AddWindow();

private slots:
    void on_Add_Button_clicked();

    void on_Image_Button_clicked();

private:
    Ui::AddWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "addResident");

    void createRequest();
    void checkFields(QString imagePath, QString name, QString species, QString age, QString description);
};

#endif // ADDWINDOW_H
