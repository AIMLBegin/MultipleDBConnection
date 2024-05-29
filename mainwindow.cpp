#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include "mainpage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint); // Remove the frame (borders) from the main window, creating a frameless window.

    // Database connection 1
    db1 = QSqlDatabase::addDatabase("QSQLITE", "DBConnection1");
    db1.setDatabaseName("/home/vision/fileOne.db");

    if (db1.open())
    {
        qDebug() << "Database 1 connected";
    }
    else
    {
        qDebug() << "Database 1 not connected";
    }

    // Database connection 2
    db2 = QSqlDatabase::addDatabase("QSQLITE", "DBConnection2");
    db2.setDatabaseName("/home/vision/fileTwo.db");

    if (db2.open())
    {
        qDebug() << "Database 2 connected";
    }
    else
    {
        qDebug() << "Database 2 not connected";
    }

    // Connect signals and slots
    connect(ui->pushButton_Login, &QPushButton::clicked, this, &MainWindow::on_pushButton_Login_clicked);
    connect(ui->pushButton_Cancel, &QPushButton::clicked, this, &MainWindow::on_pushButton_Cancel_clicked);
}

MainWindow::~MainWindow()
{
    if (db1.isOpen())
    {
        db1.close();
    }
    if (db2.isOpen())
    {
        db2.close();
    }
    QSqlDatabase::removeDatabase("DBConnection1");
    QSqlDatabase::removeDatabase("DBConnection2");

    delete ui;
}

void MainWindow::on_pushButton_Login_clicked()
{
    QString UserName = ui->lineEdit_UserName->text();
    QString Password = ui->lineEdit_Password->text();

    // Check login in Database 1
    QSqlQuery query1(db1);
    query1.prepare("SELECT * FROM user WHERE UserName = :username AND Password = :password");
    query1.bindValue(":username", UserName);
    query1.bindValue(":password", Password);

    if (query1.exec() && query1.next())
    {
        if (UserName == "testuser" && Password == "123") {
            QMessageBox::information(this, "Login", "Login success from Database 1");
            this->hide();
            MainPage *mainpage = new MainPage(this);
            mainpage->show();
            return;
        }
    }

    // Check login in Database 2
    QSqlQuery query2(db2);
    query2.prepare("SELECT * FROM user WHERE UserName = :username AND Password = :password");
    query2.bindValue(":username", UserName);
    query2.bindValue(":password", Password);

    if (query2.exec() && query2.next())
    {
        if (UserName == "testuserOne" && Password == "456") {
            QMessageBox::information(this, "Login", "Login success from Database 2");
            this->hide();
            MainPage *mainpage = new MainPage(this);
            mainpage->show();
            return;
        }
    }

    QMessageBox::warning(this, "Login", "Login failed");
}

void MainWindow::on_pushButton_Cancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "QT with me", "Do you want to close the application?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}
