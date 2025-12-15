/********************************************************************************
** Form generated from reading UI file 'SplashScreen.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPLASHSCREEN_H
#define UI_SPLASHSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SplashScreen
{
public:
    QLabel *splashImage;
    QLabel *welcomeLabel;
    QPushButton *playButton;

    void setupUi(QWidget *SplashScreen)
    {
        if (SplashScreen->objectName().isEmpty())
            SplashScreen->setObjectName("SplashScreen");
        SplashScreen->resize(400, 300);
        splashImage = new QLabel(SplashScreen);
        splashImage->setObjectName("splashImage");
        splashImage->setGeometry(QRect(0, 0, 401, 201));
        splashImage->setPixmap(QPixmap(QString::fromUtf8("images.jpg")));
        splashImage->setScaledContents(true);
        welcomeLabel = new QLabel(SplashScreen);
        welcomeLabel->setObjectName("welcomeLabel");
        welcomeLabel->setGeometry(QRect(0, 200, 391, 41));
        QFont font;
        font.setBold(true);
        welcomeLabel->setFont(font);
        welcomeLabel->setStyleSheet(QString::fromUtf8("\n"
"color: rgb(0, 85, 0);"));
        welcomeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        playButton = new QPushButton(SplashScreen);
        playButton->setObjectName("playButton");
        playButton->setGeometry(QRect(79, 250, 241, 29));
        playButton->setFont(font);
        playButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 85, 0);"));

        retranslateUi(SplashScreen);

        QMetaObject::connectSlotsByName(SplashScreen);
    } // setupUi

    void retranslateUi(QWidget *SplashScreen)
    {
        SplashScreen->setWindowTitle(QCoreApplication::translate("SplashScreen", "Form", nullptr));
        splashImage->setText(QString());
        welcomeLabel->setText(QCoreApplication::translate("SplashScreen", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">UNDAUNTED NORMANDY</span></p></body></html>", nullptr));
        playButton->setText(QCoreApplication::translate("SplashScreen", "PLAY", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SplashScreen: public Ui_SplashScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPLASHSCREEN_H
