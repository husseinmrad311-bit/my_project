#include "LoginPage.h"
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

    //keep your validation exactly as you already have it
    QStringList errorMessages;

    QString err1;
    if (!validatePlayerName(p1, err1))
        errorMessages << tr("Player 1: %1").arg(err1);

    QString err2;
    if (!validatePlayerName(p2, err2))
        errorMessages << tr("Player 2: %1").arg(err2);

    if (!errorMessages.isEmpty()){
        QMessageBox::warning(this, tr("Invalid Player Name(s)"),errorMessages.join("\n"));
        return;
    }
    //So you might get: Player 1: Name must be at least 8 character or Player 2: Name must contain at least one special character or .....

    //maps folder in the same directory as the executable
    const QString mapsFolder = QDir(QCoreApplication::applicationDirPath()).filePath("maps");

    //open MapSelectionWindow
    auto *mapWin = new MapSelectionWindow();//
    mapWin->setAttribute(Qt::WA_DeleteOnClose, true);//WA_DeleteOnClose means when the window closes, Qt deletes it automatically (prevents memory leaks)
    mapWin->setMapsFolder(mapsFolder);
    mapWin->show();

    //if user selects a map then open GameBoard
    QObject::connect(mapWin, &MapSelectionWindow::mapChosen, this,
                     [this, p1, p2](const QString &mapPath)
                     {
                         QString error;
                         auto *game = new GameBoardWindow();
                         game->setAttribute(Qt::WA_DeleteOnClose, true);

                         //put player names into labels
                         game->setPlayerNames(p1, p2);

                         //load + render map
                         if (!game->loadAndShowMap(mapPath, error)) {
                             QMessageBox::warning(this, "Map load failed", error);
                             delete game;
                             return;
                         }

                         game->show();
                         this->hide(); //hide login after launching the gameboard
                     });

    //if canceled then do nothing
    QObject::connect(mapWin, &MapSelectionWindow::canceled, this, []() {
        //user canceled
    });
}

