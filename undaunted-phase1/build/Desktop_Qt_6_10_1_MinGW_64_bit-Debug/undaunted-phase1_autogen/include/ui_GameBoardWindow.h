/********************************************************************************
** Form generated from reading UI file 'GameBoardWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEBOARDWINDOW_H
#define UI_GAMEBOARDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameBoardWindow
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *player1Label;
    QLabel *mapLabel;
    QLabel *player2Label;
    QGraphicsView *boardView;

    void setupUi(QWidget *GameBoardWindow)
    {
        if (GameBoardWindow->objectName().isEmpty())
            GameBoardWindow->setObjectName("GameBoardWindow");
        GameBoardWindow->resize(1283, 689);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GameBoardWindow->sizePolicy().hasHeightForWidth());
        GameBoardWindow->setSizePolicy(sizePolicy);
        layoutWidget = new QWidget(GameBoardWindow);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(12, 9, 1261, 611));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        player1Label = new QLabel(layoutWidget);
        player1Label->setObjectName("player1Label");
        player1Label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(player1Label);

        mapLabel = new QLabel(layoutWidget);
        mapLabel->setObjectName("mapLabel");
        mapLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(mapLabel);

        player2Label = new QLabel(layoutWidget);
        player2Label->setObjectName("player2Label");
        player2Label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(player2Label);


        verticalLayout->addLayout(horizontalLayout);

        boardView = new QGraphicsView(layoutWidget);
        boardView->setObjectName("boardView");

        verticalLayout->addWidget(boardView);


        retranslateUi(GameBoardWindow);

        QMetaObject::connectSlotsByName(GameBoardWindow);
    } // setupUi

    void retranslateUi(QWidget *GameBoardWindow)
    {
        GameBoardWindow->setWindowTitle(QCoreApplication::translate("GameBoardWindow", "Form", nullptr));
        player1Label->setText(QCoreApplication::translate("GameBoardWindow", "Player1", nullptr));
        mapLabel->setText(QCoreApplication::translate("GameBoardWindow", "Map", nullptr));
        player2Label->setText(QCoreApplication::translate("GameBoardWindow", "Player2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameBoardWindow: public Ui_GameBoardWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEBOARDWINDOW_H
