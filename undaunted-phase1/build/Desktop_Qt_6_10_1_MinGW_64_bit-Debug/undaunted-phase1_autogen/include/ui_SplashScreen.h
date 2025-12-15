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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SplashScreen
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *splashImage;
    QLabel *welcomeLabel;
    QPushButton *playButton;

    void setupUi(QWidget *SplashScreen)
    {
        if (SplashScreen->objectName().isEmpty())
            SplashScreen->setObjectName("SplashScreen");
        SplashScreen->resize(400, 300);
        verticalLayout = new QVBoxLayout(SplashScreen);
        verticalLayout->setObjectName("verticalLayout");
        splashImage = new QLabel(SplashScreen);
        splashImage->setObjectName("splashImage");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splashImage->sizePolicy().hasHeightForWidth());
        splashImage->setSizePolicy(sizePolicy);
        splashImage->setPixmap(QPixmap(QString::fromUtf8("images/splash.jpg")));
        splashImage->setScaledContents(false);
        splashImage->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(splashImage);

        welcomeLabel = new QLabel(SplashScreen);
        welcomeLabel->setObjectName("welcomeLabel");
        QFont font;
        font.setBold(true);
        welcomeLabel->setFont(font);
        welcomeLabel->setStyleSheet(QString::fromUtf8("\n"
"color: rgb(0, 85, 0);"));
        welcomeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(welcomeLabel);

        playButton = new QPushButton(SplashScreen);
        playButton->setObjectName("playButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(playButton->sizePolicy().hasHeightForWidth());
        playButton->setSizePolicy(sizePolicy1);
        playButton->setFont(font);
        playButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 85, 0);"));

        verticalLayout->addWidget(playButton);


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
