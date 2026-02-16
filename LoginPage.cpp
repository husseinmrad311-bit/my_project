#include "LoginPage.h"
#include "Game.h"
#include "ui_LoginPage.h"
#include <QPixmap>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include "MapSelectionWindow.h"
#include "GameBoardWindow.h"
#include <QMessageBox>


/*
ui_LoginPage.h: generated UI (widgets like labels/lineEdits/buttons)
QPixmap: load images
QCoreApplication + QDir: build a path to the maps folder next to the .exe
QMessageBox: show validation , map loading errors
MapSelectionWindow, GameBoardWindow: next windows in the app flow
*/

LoginPage::LoginPage(QWidget *parent)//This builds the UI designed in LoginPage.ui
    : QWidget(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    QPixmap bg("D:/Desktop/Undaunted-Phase1/Undaunted-Phase1/images/login_background.jpg");//load the image

    if (bg.isNull()) {
        qDebug() << "ERROR: login background image not found!";
    } else {
        ui->backgroundLabel->setPixmap(bg);
        ui->backgroundLabel->setScaledContents(true);//setScaledContents(true) makes the image stretch to fit the label
    }

    setWindowTitle(tr("Undaunted - Login"));

    setupConnections();
}
//Reads text from the QLineEdit and trims spaces
//These are clean getter functions for other classes if needed

QString LoginPage::player1Name() const
{
    return ui->player1LineEdit->text().trimmed();
}

QString LoginPage::player2Name() const
{
    return ui->player2LineEdit->text().trimmed();
}

LoginPage::~LoginPage()//destructor
{
    delete ui;
}

void LoginPage::setupConnections(){
    // UI WIDGETS :
    //  - QLabel = backgroundLabel    (background image)
    //  - QLabel = instructionsLabel  (text explaining the rules)
    //  - QLineEdit = player1LineEdit
    //  - QLineEdit = player2LineEdit
    //  - QPushButton = selectMapButton

    connect(ui->selectMapButton, &QPushButton::clicked,//When user clicks "Select Map" -> run onSelectMapButtonClicked()
            this, &LoginPage::onSelectMapButtonClicked);
}

bool LoginPage::validatePlayerName(const QString &name,
                                   QString &errorMessage) const
{
    errorMessage.clear();

    //length at least 8
    if (name.length() < 8) {
        errorMessage = tr("Name must be at least 8 characters long.");
        return false;
    }

    //first character must be an English letter
    const QChar first = name.at(0);
    if (!first.isLetter() ||
        !(first.unicode() < 128)) { //ensure ASCII letter if you like
        errorMessage = tr("Name must start with an English letter (A-Z or a-z).");
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (const QChar &ch : name) {
        if (ch.isUpper())
            hasUpper = true;
        if (ch.isLower())
            hasLower = true;
        if (ch.isDigit())
            hasDigit = true;
        if (!ch.isLetterOrNumber())
            hasSpecial = true;
    }

    QStringList problems;
    if (!hasUpper || !hasLower)
        problems << tr("must contain both uppercase and lowercase English letters");
    if (!hasDigit)
        problems << tr("must contain at least one number");
    if (!hasSpecial)
        problems << tr("must contain at least one special character");

    if (!problems.isEmpty()) {
        errorMessage = tr("Name %1.").arg(problems.join(tr(", ")));
        return false;
    }
    //we can show now one combined message like:name must contain both uppercase and lowercase…, must contain at least one number…
    return true;
}

void LoginPage::onSelectMapButtonClicked()
{

    const QString p1 = ui->player1LineEdit->text().trimmed();
    const QString p2 = ui->player2LineEdit->text().trimmed();

    QStringList errorMessages;

    QString err1;
    if (!validatePlayerName(p1, err1))
        errorMessages << tr("Player 1: %1").arg(err1);

    QString err2;
    if (!validatePlayerName(p2, err2))
        errorMessages << tr("Player 2: %1").arg(err2);

    if (!errorMessages.isEmpty()) {
        QMessageBox::warning(this,
                             tr("Invalid Player Name(s)"),
                             errorMessages.join("\n"));
        return;
    }

    // Maps folder next to executable
    const QString mapsFolder =
        QDir(QCoreApplication::applicationDirPath()).filePath("maps");

    MapSelectionWindow* mapWin = new MapSelectionWindow();
    mapWin->setAttribute(Qt::WA_DeleteOnClose, true);
    mapWin->setMapsFolder(mapsFolder);
    mapWin->show();
 qDebug() << "Maps folder path:" << mapsFolder;
    // ✅ SINGLE connection only
    connect(mapWin, &MapSelectionWindow::mapChosen, this,
            [this, p1, p2](const QString& mapPath,
                           const QString& statePath)
            {
                // Create Game engine
                Game* game = new Game(
                    p1.toStdString(),
                    p2.toStdString()
                    );

                // Load map INTO GAME
                if (!game->loadMap(mapPath.toStdString())) {
                    QMessageBox::warning(this,
                                         "Map Load Failed",
                                         "Failed to load map.");
                    delete game;
                    return;
                }

                // Load state file
                if (!game->loadStateFile(statePath.toStdString())) {
                    QMessageBox::warning(this,
                                         "State Load Failed",
                                         "Failed to load state file.");
                    delete game;
                    return;
                }

                // Start game
                game->startGame();

                // Create Game Board UI
                GameBoardWindow* board = new GameBoardWindow();
                board->setAttribute(Qt::WA_DeleteOnClose, true);

                // Inject Game into UI
                board->setGame(game);

                board->show();
                this->hide();
            });
}
