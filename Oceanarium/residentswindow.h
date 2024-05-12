#ifndef RESIDENTSWINDOW_H
#define RESIDENTSWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardItem>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class ResidentsWindow;
}
QT_END_NAMESPACE

class ResidentsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ResidentsWindow(const QString& login, QWidget* parent = nullptr);
    ~ResidentsWindow();

private slots:
    void on_Add_Button_clicked();

    void on_Refresh_Button_clicked();

    void on_Save_Button_clicked();

    void on_Del_Button_clicked();

    void on_Image_Button_clicked();

private:
    Ui::ResidentsWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "resindentswindow");
    QString UserLogin;

    QHash<int, QHash<int, QString>> modifiedData;

    void setFormState();
};
#endif // RESIDENTSWINDOW_H
