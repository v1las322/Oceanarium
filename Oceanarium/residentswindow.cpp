#include "residentswindow.h"
#include "ui_residentswindow.h"
#include "addwindow.h"

#include <QSqlQueryModel>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QCoreApplication>
#include <QFileDialog>

#include "globals.h"

ResidentsWindow::ResidentsWindow(const QString &login, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ResidentsWindow), UserLogin(login) {
    ui->setupUi(this);
    setFormState();

    QPixmap image(QCoreApplication::applicationDirPath() + "/resources/user.png");
    image = image.scaled(25, 25, Qt::KeepAspectRatio);
    ui->User_Image->setPixmap(image);

    ui->User_Label->setText(login);
    ui->User_Label->setAlignment(Qt::AlignCenter);
}

ResidentsWindow::~ResidentsWindow()
{
    delete ui;
}

void ResidentsWindow::setFormState() {
    db = QSqlDatabase::database("resindentswindow");

    if (getSqlConnection(db)) {
        QSqlQuery query(db);

        query.prepare("SELECT ImageURL, Name, Species, Age, Description FROM Residents");
        if (query.exec()) {
            QTableWidget *tableWidget = ui->tableWidget;

            tableWidget->clear();
            tableWidget->clearContents();

            tableWidget->setRowCount(query.size());
            tableWidget->setColumnCount(5);

            QStringList headers;
            headers << "Изображение" << "Название" << "Вид" << "Возраст" << "Описание";
            tableWidget->setHorizontalHeaderLabels(headers);

            int row = 0;
            while (query.next()) {
                QString imageName = query.value(0).toString();
                QString imageUrl = QCoreApplication::applicationDirPath() + "/images/" + imageName;
                QLabel *imageLabel = new QLabel;
                QPixmap image(imageUrl);
                image = image.scaled(100, 100, Qt::KeepAspectRatio);
                imageLabel->setPixmap(image);
                tableWidget->setCellWidget(row, 0, imageLabel);

                QString name = query.value(1).toString();
                QTableWidgetItem *nameItem = new QTableWidgetItem(name);
                tableWidget->setItem(row, 1, nameItem);

                QString species = query.value(2).toString();
                QTableWidgetItem *speciesItem = new QTableWidgetItem(species);
                tableWidget->setItem(row, 2, speciesItem);

                int age = query.value(3).toInt();
                QTableWidgetItem *ageItem = new QTableWidgetItem(QString::number(age));
                tableWidget->setItem(row, 3, ageItem);

                QString description = query.value(4).toString();
                QTableWidgetItem *descriptionItem = new QTableWidgetItem(description);
                tableWidget->setItem(row, 4, descriptionItem);

                row++;
            }
            tableWidget->setWordWrap(true);
            tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            tableWidget->horizontalHeader()->setStretchLastSection(true);
            tableWidget->resizeColumnsToContents();

        } else {
            showMessageBox("Не удалось выполнить запрос",
                           "Неудачно", QMessageBox::Ok, QMessageBox::Warning);
        }
    } else {
        showMessageBox("Отсутствует подключение к БД.", "Действие прервано",
                       QMessageBox::Ok, QMessageBox::Critical);
    }
}

void ResidentsWindow::on_Add_Button_clicked()
{
    AddWindow *addWindow = new AddWindow(this);
    this->hide();
    addWindow->show();
    while (addWindow->isVisible()){
        QApplication::processEvents();
    }
    this->show();
    setFormState();
}

void ResidentsWindow::on_Refresh_Button_clicked()
{
    setFormState();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void ResidentsWindow::on_Del_Button_clicked()
{
    int selectedRow = ui->tableWidget->currentRow();

    if (selectedRow >= 0) {
        QTableWidgetItem *nameItem = ui->tableWidget->item(selectedRow, 1);
        QString name = nameItem->text();
        QSqlQuery query(db);
        query.prepare("DELETE FROM Residents WHERE Name = :name");
        query.bindValue(":name", name);
        if (query.exec()) {
            showMessageBox( "Животное успешно удалено из базы данных", "Удачно", QMessageBox::Ok, QMessageBox::Warning);
            setFormState();
        } else {
            showMessageBox("Ошибка при удалении животного из базы данных",
                           "Неудачно", QMessageBox::Ok, QMessageBox::Warning);
        }
    } else {
        showMessageBox("Выберите животное для удаления",
                       "Неудачно", QMessageBox::Ok, QMessageBox::Warning);
    }
}

void ResidentsWindow::on_Save_Button_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow >= 0) {
        QString imageURL = ui->tableWidget->cellWidget(currentRow, 0)->property("imageURL").toString();
        QString name = ui->tableWidget->item(currentRow, 1)->text();
        QString species = ui->tableWidget->item(currentRow, 2)->text();
        int age = ui->tableWidget->item(currentRow, 3)->text().toInt();
        QString description = ui->tableWidget->item(currentRow, 4)->text();

        QSqlQuery query(db);
        query.prepare("UPDATE Residents SET ImageURL = ImageURL, Species = :species, Age = :age, Description = :description WHERE Name = :name");
        query.bindValue(":species", species);
        query.bindValue(":age", age);
        query.bindValue(":description", description);
        query.bindValue(":name", name);

        if (query.exec()) {
            showMessageBox("Данные успешно обновлены в базе данных", "Удачно", QMessageBox::Ok, QMessageBox::Warning);
        } else {
            showMessageBox("Ошибка при обновлении данных в базе данных.", "Неудачно", QMessageBox::Ok, QMessageBox::Warning);
        }
    }
}

void ResidentsWindow::on_Image_Button_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow >= 0) {
        QString newImagePath = QFileDialog::getOpenFileName(this, tr("Выберите новое изображение"), "", tr("Изображения (*.png *.jpg *.jpeg)"));
        if (!newImagePath.isEmpty()) {
            QFileInfo fileInfo(newImagePath);
            QString fileName = fileInfo.fileName();
            QString imageURL = fileName;

            QLabel* imageLabel = new QLabel();
            QPixmap image(newImagePath);
            imageLabel->setPixmap(image.scaled(100, 100, Qt::KeepAspectRatio));
            ui->tableWidget->setCellWidget(currentRow, 0, imageLabel);

            ui->tableWidget->cellWidget(currentRow, 0)->setProperty("imageURL", imageURL);

            QSqlQuery query(db);
            query.prepare("UPDATE Residents SET ImageURL = :imageURL WHERE Name = :name");
            query.bindValue(":imageURL", imageURL);
            query.bindValue(":name", ui->tableWidget->item(currentRow, 1)->text());
            if (query.exec()) {
                showMessageBox("Изображение успешно обновлено в базе данных", "Удачно", QMessageBox::Ok, QMessageBox::Warning);
            } else {
                showMessageBox("Ошибка при обновлении изображения в базе данных.", "Неудачно", QMessageBox::Ok, QMessageBox::Warning);
            }
        }
    } else {
        showMessageBox("Выберите изображение для изменения",
                       "Неудачно", QMessageBox::Ok, QMessageBox::Warning);
    }
}
