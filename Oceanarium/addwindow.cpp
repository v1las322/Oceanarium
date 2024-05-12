#include "addwindow.h"
#include "ui_addwindow.h"
#include "globals.h"

#include <QFileDialog>

AddWindow::AddWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddWindow)
{
    ui->setupUi(this);
}

AddWindow::~AddWindow()
{
    delete ui;
}

void AddWindow::createRequest() {
    db = QSqlDatabase::database("addResident");
    if (getSqlConnection(db)) {
        QString name = ui->Name_Edit->text();
        QString species = ui->Species_Edit->text();
        QString age = ui->Age_Edit->text();
        QString description = ui->Description_Edit->text();
        QString imagePath = ui->Path_Edit->text();

        checkFields(imagePath, name, species, age, description);

        QSqlQuery query(db);
        query.prepare("INSERT INTO Residents (ImageURL, Name, Species, Age, Description) VALUES (?, ?, ?, ?, ?)");
        query.addBindValue(imagePath);
        query.addBindValue(name);
        query.addBindValue(species);
        query.addBindValue(age);
        query.addBindValue(description);

        if(!query.exec()){
            showMessageBox("Обитатель уже зарегистрирован",
                           "Действие прервано", QMessageBox::Ok,
                           QMessageBox::Warning);
        } else {
            showMessageBox("Обитатель зарегистрирован",
                           "Удачно", QMessageBox::Ok,
                           QMessageBox::Warning);

        }
    } else {
        showMessageBox("Отсутствует подключение к БД.", "Действие прервано",
                       QMessageBox::Ok, QMessageBox::Critical);
        this->close();
    }
}

void AddWindow::on_Add_Button_clicked()
{
    createRequest();
}

void AddWindow::checkFields(QString imagePath, QString name, QString species, QString age, QString description)
{
    if (name.isEmpty()) {
        showMessageBox("Поле 'Имя' должно быть заполнено.", "Действие прервано",
                       QMessageBox::Ok, QMessageBox::Warning);
        return;
    }
    if (species.isEmpty()) {
        showMessageBox("Поле 'Вид' должно быть заполнено.",
                       "Действие прервано", QMessageBox::Ok,
                       QMessageBox::Warning);
        return;
    }
    if (age.isEmpty()) {
        showMessageBox("Поле 'Возраст' должно быть заполнено.",
                       "Действие прервано", QMessageBox::Ok,
                       QMessageBox::Warning);
        return;
    }
    if (description.isEmpty()) {
        showMessageBox("Поле 'Описание' должно быть заполнено.",
                       "Действие прервано", QMessageBox::Ok,
                       QMessageBox::Warning);
        return;
    }
    if (imagePath.isEmpty()) {
        showMessageBox("Поле 'Путь' должно быть заполнено.",
                       "Действие прервано", QMessageBox::Ok,
                       QMessageBox::Warning);
        return;
    }
}

void AddWindow::on_Image_Button_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Выберите изображение"), "", tr("Изображения (*.png *.jpg *.jpeg)"));
    if (!imagePath.isEmpty()) {
        QFileInfo fileInfo(imagePath);
        QString fileName = fileInfo.fileName();
        ui->Path_Edit->setText(fileName);
    }
}

