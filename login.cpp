#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <digitallibrary.h>
#include <userwindow.h>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("Login");
    setValidator();
}

Login::~Login()
{
    delete ui;
}

void Login::on_loginButton_clicked()
{
    //Database
//    //call the main Database
    digitalLibrary lib;
    auto db = lib.db;

    //Get the username
    QString username = ui->username->text();

    //Get the password
    QString password = ui->password->text();



     //Database
if(!username.isEmpty() & !password.isEmpty())
{
    if(username=="admin" && password=="adminpassword")
    {

                lib.setUsername("admin");

                //Hide the current window
                this->hide();

                //show the mainwindow
                lib.exec();
       }
       else
           QMessageBox::warning(this, "Empty", "You are not registered as an admin. Please try user login!");
}
      else
        QMessageBox::warning(this, "Empty", "Fields are empty!");

}



void Login::setValidator()
{
    //regx for username
    QRegularExpression rxName("^[A-Za-z][A-Za-z0-9_]{7,29}$");
    QRegularExpressionValidator *valName = new QRegularExpressionValidator(rxName, this);
    ui->username->setValidator(valName);
}


void Login::on_userLoginButton_clicked()
{
    //call the main Database
    digitalLibrary lib;
    auto db = lib.db;

    //Get the username
    QString username = ui->username->text();

    //Get the password
    QString password = ui->password->text();

    //Check if the db is opened
    if(!db.isOpen())
    {
        qDebug() << "Failed to open the database";
    }

    //Define the query
    auto query = QSqlQuery(db);

    if(!username.isEmpty() & !password.isEmpty())
    {
        //Create the body of the query
        QString checkLogin = {"SELECT * FROM accounts WHERE username='"
                              +username+"' AND password='"+password+"'"};

        if(query.exec(checkLogin))
        {
            int count = 0;
            while(query.next())
            {
                count++;
            }
            if(count == 1)
            {

                //Hide the current window
                this->hide();

                userwindow userwindow;

                //set the username to show in the user window
                userwindow.setUsername(username);
                //show the userwindow
                userwindow.exec();
            }
            else if(count < 1)
                QMessageBox::warning(this, "Empty", "You are not registered!");
        }
        else
            qDebug() << "cannot select from accounts";
    }
    else
        QMessageBox::warning(this, "Empty", "Fields are empty!");
}

