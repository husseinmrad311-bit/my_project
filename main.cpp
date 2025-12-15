// ===== FILE: main.cpp =====
#include <QApplication>
#include <QObject>

#include "SplashScreen.h"
#include "LoginPage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SplashScreen splash;
    LoginPage login;

    // Splash -> Login
    QObject::connect(&splash, &SplashScreen::playClicked, [&]() {
        splash.hide();
        login.show();
    });

    splash.show();
    return app.exec();
}
