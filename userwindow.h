#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include<QSqlQueryModel>
#include <QDir>

namespace Ui {
class userwindow;
}

class userwindow : public QDialog
{
    Q_OBJECT

public:
    explicit userwindow(QWidget *parent = nullptr);
    ~userwindow();

    QSqlDatabase db;
    void connectDB();

    void setUsername(QString username);
    void showBookNum();
    void showCover();

private slots:

    void on_booksListBtn_clicked();

    void on_issueBookBtn_clicked();

    void on_returnBookBtn_clicked();

    void on_Logoutbutton_clicked();

private:
    Ui::userwindow *ui;
    QString filename = QDir::homePath()+ "/libraryDB.sqlite";

    QString username;
};

#endif // USERWINDOW_H
