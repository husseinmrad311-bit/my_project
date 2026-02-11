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
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameBoardWindow
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QWidget *topStatusBar;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QWidget *feedbackBar;
    QWidget *actionBar;
    QGraphicsView *boardView;
    QWidget *footerBar;

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
        topStatusBar = new QWidget(layoutWidget);
        topStatusBar->setObjectName("topStatusBar");
        label = new QLabel(topStatusBar);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 0, 111, 20));
        label_2 = new QLabel(topStatusBar);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(120, 0, 161, 20));
        label_3 = new QLabel(topStatusBar);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(310, 0, 161, 20));

        verticalLayout->addWidget(topStatusBar);

        feedbackBar = new QWidget(layoutWidget);
        feedbackBar->setObjectName("feedbackBar");

        verticalLayout->addWidget(feedbackBar);

        actionBar = new QWidget(layoutWidget);
        actionBar->setObjectName("actionBar");

        verticalLayout->addWidget(actionBar);

        boardView = new QGraphicsView(layoutWidget);
        boardView->setObjectName("boardView");
        sizePolicy.setHeightForWidth(boardView->sizePolicy().hasHeightForWidth());
        boardView->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(boardView);

        footerBar = new QWidget(layoutWidget);
        footerBar->setObjectName("footerBar");

        verticalLayout->addWidget(footerBar);


        retranslateUi(GameBoardWindow);

        QMetaObject::connectSlotsByName(GameBoardWindow);
    } // setupUi

    void retranslateUi(QWidget *GameBoardWindow)
    {
        GameBoardWindow->setWindowTitle(QCoreApplication::translate("GameBoardWindow", "Form", nullptr));
        label->setText(QCoreApplication::translate("GameBoardWindow", "turnLabel", nullptr));
        label_2->setText(QCoreApplication::translate("GameBoardWindow", "currentPlayerLabel", nullptr));
        label_3->setText(QCoreApplication::translate("GameBoardWindow", "currentCardLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameBoardWindow: public Ui_GameBoardWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEBOARDWINDOW_H
