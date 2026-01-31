#include "SplashScreen.h"
#include "ui_SplashScreen.h"   //generated from SplashScreen.ui

#include <QDebug>
#include <QResizeEvent>
/*
"SplashScreen.h": class declaration
"ui_SplashScreen.h": auto-generated from the .ui file, contains the Ui::SplashScreen class and all widgets
QDebug: to print debug messages to console
QResizeEvent: for handling resize events
*/


SplashScreen::SplashScreen(QWidget *parent)//constructor creates the UI object and builds the widgets designed in Qt Designer onto this widget
    : QWidget(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);

    m_originalSplash = QPixmap("D:/Desktop/Undaunted-Phase1/Undaunted-Phase1/images/splash.jpg");

    if (m_originalSplash.isNull()) {//if the image can’t be found , isNull() becomes true
        qDebug() << "ERROR: splash image not found!";
    }

    //scales the image to the label size right away
    updateSplashImage();

    setWindowTitle(tr("Undaunted - Splash"));//tr(...) is for translations (Qt internationalization)

    setupConnections();//connects UI signals (button click) to your slot
}

SplashScreen::~SplashScreen() //destructor
{
    delete ui;
}

void SplashScreen::setupConnections()
{
    connect(ui->playButton, &QPushButton::clicked,
            this, &SplashScreen::onPlayButtonClicked);//when the Play button is clicked -> call onPlayButtonClicked()
}

void SplashScreen::onPlayButtonClicked()
{
    emit playClicked();
}

//automatical call on maximize/minimize/resize
void SplashScreen::resizeEvent(QResizeEvent *event)//Qt calls resizeEvent whenever the widget changes size (maximize/minimize/manual resize
{
    QWidget::resizeEvent(event);
    updateSplashImage();
}

//rescale original pixmap to current label size
void SplashScreen::updateSplashImage()
{
    if (m_originalSplash.isNull() || !ui->splashImage)
        return;//if the image isn’t loaded OR the label doesn’t exist -> stop

    //we want full stretch (fill the label completely)->    ussus keep IgnoreAspectRatio.
    //if you want it NOT to distort, change to KeepAspectRatio.
    ui->splashImage->setPixmap(
        m_originalSplash.scaled(
            ui->splashImage->size(),
            Qt::IgnoreAspectRatio,          //fills but stretchs
            Qt::SmoothTransformation
            )
        );
}
