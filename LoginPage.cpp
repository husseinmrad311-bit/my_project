#include "LoginPage.h"
#include "ui_LoginPage.h"
#include <QPixmap>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include "MapSelectionWindow.h"
#include "GameBoardWindow.h"
#include "MapLoader.h"
#include <QMessageBox>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    QPixmap bg("D:/Desktop/Undaunted-Phase1/Undaunted-Phase1/images/login_background.jpg");

    if (bg.isNull()) {
        qDebug() << "ERROR: login background image not found!";
    } else {
        ui->backgroundLabel->setPixmap(bg);
        ui->backgroundLabel->setScaledContents(true);
    }

    setWindowTitle(tr("Undaunted - Login"));

    /*
     note :
     - You set the background image in LoginPage.ui
       on the QLabel with objectName "backgroundLabel".
        - You also set the instructions text there on "instructionsLabel".
    */

    setupConnections();
}
QString LoginPage::player1Name() const
{
    return ui->player1LineEdit->text().trimmed();
}

QString LoginPage::player2Name() const
{
    return ui->player2LineEdit->text().trimmed();
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::setupConnections()
{
    // UI WIDGETS :
    //  - QLabel     backgroundLabel    (background image)
    //  - QLabel     instructionsLabel  (text explaining the rules)
    //  - QLineEdit  player1LineEdit
    //  - QLineEdit  player2LineEdit
    //  - QPushButton selectMapButton

    connect(ui->selectMapButton, &QPushButton::clicked,
            this, &LoginPage::onSelectMapButtonClicked);
}

bool LoginPage::validatePlayerName(const QString &name,
                                   QString &errorMessage) const
{
    errorMessage.clear();

    // 1)length at least 8
    if (name.length() < 8) {
        errorMessage = tr("Name must be at least 8 characters long.");
        return false;
    }

    // 2)first character must be an English letter
    const QChar first = name.at(0);
    if (!first.isLetter() ||
        !(first.unicode() < 128)) { // ensure ASCII letter if you like
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
        errorMessage = tr("Name %1.")
        .arg(problems.join(tr(", ")));
        return false;
    }

    return true;
}

void LoginPage::onSelectMapButtonClicked()
{
    const QString p1 = ui->player1LineEdit->text().trimmed();
    const QString p2 = ui->player2LineEdit->text().trimmed();

    // ---- keep your validation exactly as you already have it ----
    QStringList errorMessages;

    QString err1;
    if (!validatePlayerName(p1, err1))
        errorMessages << tr("Player 1: %1").arg(err1);

    QString err2;
    if (!validatePlayerName(p2, err2))
        errorMessages << tr("Player 2: %1").arg(err2);

    if (!errorMessages.isEmpty()) {
        QMessageBox::warning(this, tr("Invalid Player Name(s)"),
                             errorMessages.join("\n"));
        return;
    }
    // ------------------------------------------------------------

    // maps folder (same folder as exe)
    const QString mapsFolder = QDir(QCoreApplication::applicationDirPath()).filePath("maps");

    // Open Map Selection window
    auto *mapWin = new MapSelectionWindow();
    mapWin->setAttribute(Qt::WA_DeleteOnClose, true);
    mapWin->setMapsFolder(mapsFolder);
    mapWin->show();

    // If user selects a map -> open GameBoard
    QObject::connect(mapWin, &MapSelectionWindow::mapChosen, this,
                     [this, p1, p2](const QString &mapPath)
                     {
                         QString error;
                         auto *game = new GameBoardWindow();
                         game->setAttribute(Qt::WA_DeleteOnClose, true);

                         // Put player names into labels
                         game->setPlayerNames(p1, p2);

                         // Load + render map
                         if (!game->loadAndShowMap(mapPath, error)) {
                             QMessageBox::warning(this, "Map load failed", error);
                             delete game;
                             return;
                         }

                         game->show();
                         this->hide(); // optional: hide login after launching the gameboard
                     });

    // If canceled -> do nothing
    QObject::connect(mapWin, &MapSelectionWindow::canceled, this, []() {
        // user canceled
    });
}

