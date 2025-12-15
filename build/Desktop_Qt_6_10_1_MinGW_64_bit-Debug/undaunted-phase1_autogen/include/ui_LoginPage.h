/********************************************************************************
** Form generated from reading UI file 'LoginPage.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINPAGE_H
#define UI_LOGINPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginPage
{
public:
    QLabel *backgroundLabel;
    QPushButton *selectMapButton;
    QLabel *instructionsLabel;
    QLineEdit *player1LineEdit;
    QLineEdit *player2LineEdit;

    void setupUi(QWidget *LoginPage)
    {
        if (LoginPage->objectName().isEmpty())
            LoginPage->setObjectName("LoginPage");
        LoginPage->resize(400, 300);
        backgroundLabel = new QLabel(LoginPage);
        backgroundLabel->setObjectName("backgroundLabel");
        backgroundLabel->setGeometry(QRect(-8, 0, 411, 251));
        backgroundLabel->setPixmap(QPixmap(QString::fromUtf8("loginpage.jpg")));
        backgroundLabel->setScaledContents(true);
        selectMapButton = new QPushButton(LoginPage);
        selectMapButton->setObjectName("selectMapButton");
        selectMapButton->setGeometry(QRect(60, 260, 271, 29));
        selectMapButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 9pt \"Arial\";\n"
"background-color: rgb(0, 85, 0);"));
        instructionsLabel = new QLabel(LoginPage);
        instructionsLabel->setObjectName("instructionsLabel");
        instructionsLabel->setGeometry(QRect(10, 10, 391, 71));
        QFont font;
        font.setPointSize(7);
        instructionsLabel->setFont(font);
        instructionsLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 127);\n"
"color: rgb(255, 237, 164);\n"
"color: rgb(253, 255, 221);"));
        instructionsLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        player1LineEdit = new QLineEdit(LoginPage);
        player1LineEdit->setObjectName("player1LineEdit");
        player1LineEdit->setGeometry(QRect(40, 170, 311, 28));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe UI")});
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setItalic(false);
        player1LineEdit->setFont(font1);
        player1LineEdit->setStyleSheet(QString::fromUtf8("color: rgb(0, 85, 0);\n"
"font: 9pt \"Segoe UI\";\n"
"\n"
"color: rgb(83, 83, 83);\n"
"background-color: rgb(254, 255, 212);"));
        player2LineEdit = new QLineEdit(LoginPage);
        player2LineEdit->setObjectName("player2LineEdit");
        player2LineEdit->setGeometry(QRect(40, 210, 311, 28));
        player2LineEdit->setStyleSheet(QString::fromUtf8("color: rgb(0, 85, 0);\n"
"color: rgb(86, 86, 86);\n"
"background-color: rgb(255, 254, 212);"));

        retranslateUi(LoginPage);

        QMetaObject::connectSlotsByName(LoginPage);
    } // setupUi

    void retranslateUi(QWidget *LoginPage)
    {
        LoginPage->setWindowTitle(QCoreApplication::translate("LoginPage", "Form", nullptr));
        backgroundLabel->setText(QString());
        selectMapButton->setText(QCoreApplication::translate("LoginPage", "SELECT MAP", nullptr));
        instructionsLabel->setText(QCoreApplication::translate("LoginPage", "Minimum 8 characters\n"
"Start with letter (A-Z/a-z)\n"
"Must include: 1 number + 1 special char + both upper & lowercase", nullptr));
        player1LineEdit->setText(QCoreApplication::translate("LoginPage", "PLAYER1", nullptr));
        player2LineEdit->setText(QCoreApplication::translate("LoginPage", "PLAYER2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginPage: public Ui_LoginPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINPAGE_H
