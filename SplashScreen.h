#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QPixmap>   //store original image pixmap

QT_BEGIN_NAMESPACE
namespace Ui { class SplashScreen; }
QT_END_NAMESPACE

class SplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();

signals:
    void playClicked();

protected:
    //called whenever the window is resized(maximize/minimize)
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onPlayButtonClicked();

private:
    Ui::SplashScreen *ui;

    //keep original image here
    QPixmap m_originalSplash;

    void setupConnections();

    //helper to rescale image to label size
    void updateSplashImage();
};

#endif
