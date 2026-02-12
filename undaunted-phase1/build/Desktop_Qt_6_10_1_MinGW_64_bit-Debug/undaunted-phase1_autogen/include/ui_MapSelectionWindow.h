/********************************************************************************
** Form generated from reading UI file 'MapSelectionWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPSELECTIONWINDOW_H
#define UI_MAPSELECTIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapSelectionWindow
{
public:
    QLabel *backgroundLabel;
    QListWidget *mapListWidget;
    QPushButton *selectButton;
    QPushButton *cancelButton;
    QLabel *titleLabel;
    QLabel *stateLabel;
    QListWidget *stateListWidget;

    void setupUi(QWidget *MapSelectionWindow)
    {
        if (MapSelectionWindow->objectName().isEmpty())
            MapSelectionWindow->setObjectName("MapSelectionWindow");
        MapSelectionWindow->resize(886, 827);
        backgroundLabel = new QLabel(MapSelectionWindow);
        backgroundLabel->setObjectName("backgroundLabel");
        backgroundLabel->setGeometry(QRect(0, 0, 881, 361));
        backgroundLabel->setPixmap(QPixmap(QString::fromUtf8("images/Map_selection.jpg")));
        backgroundLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        mapListWidget = new QListWidget(MapSelectionWindow);
        mapListWidget->setObjectName("mapListWidget");
        mapListWidget->setGeometry(QRect(10, 370, 381, 251));
        mapListWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(184, 174, 100);\n"
"selection-background-color: rgb(0, 85, 0);\n"
"selection-color: rgb(255, 255, 255);"));
        selectButton = new QPushButton(MapSelectionWindow);
        selectButton->setObjectName("selectButton");
        selectButton->setGeometry(QRect(410, 500, 461, 29));
        selectButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 85, 0);"));
        cancelButton = new QPushButton(MapSelectionWindow);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setGeometry(QRect(410, 540, 461, 29));
        cancelButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 85, 0);"));
        titleLabel = new QLabel(MapSelectionWindow);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(QRect(412, 389, 441, 101));
        titleLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 85, 0);\n"
"color: rgb(255, 255, 255);"));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        stateLabel = new QLabel(MapSelectionWindow);
        stateLabel->setObjectName("stateLabel");
        stateLabel->setGeometry(QRect(10, 680, 381, 101));
        stateLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 85, 0);\n"
"color: rgb(255, 255, 255);"));
        stateLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        stateListWidget = new QListWidget(MapSelectionWindow);
        stateListWidget->setObjectName("stateListWidget");
        stateListWidget->setGeometry(QRect(430, 590, 411, 231));
        stateListWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(184, 174, 100);\n"
"selection-background-color: rgb(0, 85, 0);\n"
"selection-color: rgb(255, 255, 255);"));

        retranslateUi(MapSelectionWindow);

        QMetaObject::connectSlotsByName(MapSelectionWindow);
    } // setupUi

    void retranslateUi(QWidget *MapSelectionWindow)
    {
        MapSelectionWindow->setWindowTitle(QCoreApplication::translate("MapSelectionWindow", "Form", nullptr));
        backgroundLabel->setText(QString());
        selectButton->setText(QCoreApplication::translate("MapSelectionWindow", "SELECT", nullptr));
        cancelButton->setText(QCoreApplication::translate("MapSelectionWindow", "CANCEL", nullptr));
        titleLabel->setText(QCoreApplication::translate("MapSelectionWindow", "BE CAREFULL WHILE CHOOSING YOU LAND : )", nullptr));
        stateLabel->setText(QCoreApplication::translate("MapSelectionWindow", "Select State File:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MapSelectionWindow: public Ui_MapSelectionWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPSELECTIONWINDOW_H
