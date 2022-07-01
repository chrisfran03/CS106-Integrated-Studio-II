#include "userwindow.h"
#include "ui_userwindow.h"
#include "login.h"
#include <managebooks.h>
#include <manageauthors.h>
#include <addmember.h>
#include <editmember.h>
#include <deletemember.h>
#include <memberslist.h>
#include <addbook.h>
#include <editbook.h>
#include <deletebook.h>
#include <booklist.h>
#include <issuebook.h>
#include <returnbook.h>
#include <QMessageBox>
#include <QDebug>

userwindow::userwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userwindow)
{
    ui->setupUi(this);
    connectDB();

    setUsername(username);
    showCover();
}

void userwindow::connectDB()
{
    //Add the database with the SQLITE driver
    db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");

    //Set the database path
    db.setDatabaseName(this->filename);

    //check if the database is opened
    if(!db.open())
        QMessageBox::critical(this, "FAILED", "DB is not opened");

    auto query = QSqlQuery(db);

    //Create a table named accounts
    QString accountTable{"CREATE TABLE IF NOT EXISTS accounts"
                         "(username VARCHAR(20), name VARCHAR(20),"
                         "password VARCHAR(20))"};

    if(!query.exec(accountTable))
        QMessageBox::critical(this,"Info","Cannot create accounts");

    //Create a table named genres
    QString genreTable{"CREATE TABLE IF NOT EXISTS genres"
                            "(ID INTEGER, name VARCHAR(20))"};
    if(!query.exec(genreTable))
        QMessageBox::critical(this,"Info","Cannot create genre Table");

    //Create a table named authors
    QString authorTable{"CREATE TABLE IF NOT EXISTS authors"
                      "(ID INTEGER, firstName VARCHAR(20), lastName VARCHAR(20),"
                       "expertise VARCHAR(20), about VARCHAR(40))"};
    if(!query.exec(authorTable))
        QMessageBox::critical(this,"Info","Cannot create authors Table");

    //Create a table named members
    QString memberTable{"CREATE TABLE IF NOT EXISTS members"
                      "(ID INTEGER PRIMARY KEY, firstName VARCHAR(20), lastName VARCHAR(20),"
                       "phone VARCHAR(20), email VARCHAR(40), gender VARCHAR(10))"};
    if(!query.exec(memberTable))
        QMessageBox::critical(this,"Info","Cannot create members Table");

    //Create a table named books
    QString bookTable{"CREATE TABLE IF NOT EXISTS books"
                      "(ID INTEGER PRIMARY KEY, ISBN VARCHAR(20), name VARCHAR(20),"
                      "author VARCHAR(20), genre VARCHAR(20), quantity INT,"
                      "publisher VARCHAR(20), price REAL, date VARCHAR(10),"
                      "description VARCHAR(80), cover VARCHAR(50))"};
    if(!query.exec(bookTable))
        QMessageBox::critical(this,"Failed","Cannot create books Table");

    //Create a table named bookStatus
    QString Table{"CREATE TABLE IF NOT EXISTS bookStatus"
                      "(Book INTEGER, Member INTEGER, Status VARCHAR(10),"
                       "IssueDate date, ReturnDate date, Note VARCHAR(50))"};
    if(!query.exec(Table))
        QMessageBox::critical(this,"Info","Cannot create bookStatus Table");
}

userwindow::~userwindow()
{
    delete ui;
}

void userwindow::showCover(){

    //vector of coverLabels
   QVector<QLabel*> covers;
   covers.append(ui->cover_1);
   covers.append(ui->cover_2);
   covers.append(ui->cover_3);
   covers.append(ui->cover_4);
   covers.append(ui->cover_5);
   covers.append(ui->cover_6);
   covers.append(ui->cover_7);
   covers.append(ui->cover_8);
   covers.append(ui->cover_9);
   covers.append(ui->cover_10);
   covers.append(ui->cover_11);
   covers.append(ui->cover_12);


   //counter
   int i = 0;
   int count = 0;

   //select covers
    QString cover;

    auto query = QSqlQuery(db);
    QString select{"SELECT * FROM books"};
    //execute the query
    if(!query.exec(select))
        qDebug() << "Cannot select from books";
    while(query.next())
        count++;
    qDebug() <<count;

    if(count <= 12)
    {
        //execute the query
        if(!query.exec(select))
            qDebug() << "Cannot select from books";
        while (query.next()) {
            cover = query.value(10).toString();
            if(i>=count)
                 return;
            else if(!covers.isEmpty())
                 covers[i]->setPixmap(cover);
            i++;
        }
    }
    else
    {
        QString selectTop12{"SELECT * FROM books LIMIT 12"};
        //execute the query
        if(!query.exec(selectTop12))
            qDebug() << "Cannot select from books";
        while (query.next()) {

            if(query.first())
            {
                cover = query.value(10).toString();

                if(i>=12)
                    return;
                else if(!covers.isEmpty())
                    covers[i]->setPixmap(cover);
                i++;
            }
        }
    }
}

void userwindow::on_booksListBtn_clicked()
{
    bookList book;
    book.exec();
}

void userwindow::on_issueBookBtn_clicked()
{
    IssueBook book;
    book.exec();
}

void userwindow::on_returnBookBtn_clicked()
{
    ReturnBook book;
    book.exec();
}

void userwindow::setUsername(QString username)
{
    ui->username->setText(username);
}
void userwindow::on_Logoutbutton_clicked()
{
    this->hide();
    Login login;
    login.exec();
}

