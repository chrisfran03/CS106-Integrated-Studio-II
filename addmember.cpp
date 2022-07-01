#include "addmember.h"
#include "ui_addmember.h"
#include "digitallibrary.h"
#include <QMessageBox>
#include <QDebug>

addMember::addMember(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addMember)
{
    ui->setupUi(this);
    this->resize(350, 400);
    this->setWindowTitle("Add Member");

    setValidator();
}

addMember::~addMember()
{
    delete ui;
}

void addMember::on_addMemberButton_clicked()
{
    //get the content of the fields
    QString firstName =ui->firstName->text();
    QString lastName = ui->lastName->text();
    QString  phone = ui->phone->text();
    QString email = ui->email->text();
    QString gender = ui->gender->currentText();
    QString username = ui->username->text();
    QString password = ui->password->text();
    QString name = firstName + " " + lastName;



    //call the mail Database
    digitalLibrary lib;
    auto db = lib.db;

    //Make sure that the fields are not empty
    if((!firstName.isEmpty() & !lastName.isEmpty())&(!phone.isEmpty() & !email.isEmpty()) & !gender.isEmpty()& (!username.isEmpty() & !password.isEmpty()))
    {
        //Create the query
        auto query = QSqlQuery(db);

        //Insert into db
        QString insert = {"INSERT INTO members (firstName, lastName, phone, email, gender)"
                          "VALUES ('%1', '%2', '%3', '%4', '%5')"};

        auto search = QSqlQuery(db);
        QString searchAcc = {"SELECT * FROM accounts WHERE username='"
                             +username+"' AND name='"+name+"' AND password='"
                             +password+"'"};
        if(!search.exec(searchAcc))
             qDebug() << "Cannot select";
        int count = 0;
        while(search.next())
        {
            count++;
        }

        if(count>=1)
            QMessageBox::warning(this, "Failed", "Account Already Exist!");
        else
        {
            QString addEntry = {"INSERT INTO accounts (username, name, password)"
                                "VALUES ('%1', '%2', '%3')"};
            auto query = QSqlQuery(db);
        //Execute the query
        if(!query.exec(insert.arg(firstName).arg(lastName).arg(phone).arg(email).arg(gender))) {
            qDebug() << "Cannot insert into members"; }
        else if(!query.exec(addEntry.arg(username).arg(name).arg(password))) {
            qDebug() << "Cannot add account"; }
        else{
            QMessageBox::information(this, "SUCCESS", "Member added succesfully!");
            this->hide(); }
        }
    }
    else
       QMessageBox::warning(this, "Empty", "Fields are empty!");

}

void addMember::setValidator(){
    //regx for name
    QRegularExpression Name("^[A-Za-z]{7,29}$");
     QRegularExpressionValidator *valName = new  QRegularExpressionValidator(Name, this);
    ui->firstName->setValidator(valName);
    ui->lastName->setValidator(valName);

    //Validator for phone number
    QRegularExpression phone("[0-9]{10}");
     QRegularExpressionValidator *valPhone = new  QRegularExpressionValidator(phone, this);
    ui->phone->setValidator(valPhone);

    //Validator for email
    QRegularExpression email("^[a-zA-Z0-9_.-]+@[a-zA-Z0-9.-]+$");
     QRegularExpressionValidator *valMail = new  QRegularExpressionValidator(email, this);
    ui->email->setValidator(valMail);


}
