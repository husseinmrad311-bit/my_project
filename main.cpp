#include <QApplication>
#include <QObject>

#include "SplashScreen.h"
#include "LoginPage.h"
#include "MapSelectionWindow.h"
#include "GameBoardWindow.h"
#include "Game.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SplashScreen splash;
    LoginPage login;
    MapSelectionWindow mapWindow;
    GameBoardWindow boardWindow;

    // Splash → Login
    QObject::connect(&splash, &SplashScreen::playClicked, [&]() {
        splash.hide();
        login.show();
    });

    // Login → Map Selection
    QObject::connect(&login, &LoginPage::onSelectMapButtonClicked, [&]() {
        login.hide();
        mapWindow.show();
    });

    // Map Selected → Start Game
    QObject::connect(&mapWindow, &MapSelectionWindow::mapChosen,
                     [&](const QString &mapPath,
                         const QString &statePath)
                     {
                         mapWindow.hide();

                         // Create Game
                         Game* game = new Game(
                             login.player1Name().toStdString(),
                             login.player2Name().toStdString()
                             );

                         if (!game->loadMap(mapPath.toStdString()))
                         {
                             qDebug("Failed to load map");
                             return;
                         }

                         if (!game->loadStateFile(statePath.toStdString()))
                         {
                             qDebug("Failed to load state");
                             return;
                         }

                         game->startGame();

                         boardWindow.setGame(game);
                         boardWindow.show();
                     });

    splash.show();
    return app.exec();
}
