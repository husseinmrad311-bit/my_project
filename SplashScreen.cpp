#include "SplashScreen.h"
#include "ui_SplashScreen.h"   //generated from SplashScreen.ui

#include <QDebug>
#include <QResizeEvent>

SplashScreen::SplashScreen(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);

    // KEEPING YOUR ORIGINAL PATH EXACTLY NO CHANGES
    m_originalSplash = QPixmap("D:/Desktop/Undaunted-Phase1/Undaunted-Phase1/images/splash.jpg");

    if (m_originalSplash.isNull()) {
        qDebug() << "ERROR: splash image not found!";
    }

    //First-time apply (initial size)
    updateSplashImage();

    setWindowTitle(tr("Undaunted - Splash"));

    setupConnections();
}

SplashScreen::~SplashScreen()
{
    delete ui;
}

void SplashScreen::setupConnections()
{
    connect(ui->playButton, &QPushButton::clicked,
            this, &SplashScreen::onPlayButtonClicked);
}

void SplashScreen::onPlayButtonClicked()
{
    emit playClicked();
}

//NEW:automatically called on maximize/minimize/resize
void SplashScreen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateSplashImage();
}

//NEW: rescale original pixmap to current label size
void SplashScreen::updateSplashImage()
{
    if (m_originalSplash.isNull() || !ui->splashImage)
        return;

    //if you want FULL stretch (fill the label completely), keep IgnoreAspectRatio.
    //if you want it NOT to distort, change to KeepAspectRatio.
    ui->splashImage->setPixmap(
        m_originalSplash.scaled(
            ui->splashImage->size(),
            Qt::IgnoreAspectRatio,          //fills, but may stretch
            Qt::SmoothTransformation
            )
        );
}
