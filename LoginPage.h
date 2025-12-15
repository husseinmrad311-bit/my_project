#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginPage; }
QT_END_NAMESPACE

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();
    QString player1Name() const;
    QString player2Name() const;


public slots:
    void onSelectMapButtonClicked();

private:
    Ui::LoginPage *ui;

    void setupConnections();

    //validates a single player name according to the rules.
    //returns true if valid; otherwise sets errorMessage.
    bool validatePlayerName(const QString &name,
                            QString &errorMessage) const;
};

#endif
