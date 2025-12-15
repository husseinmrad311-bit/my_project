#ifndef MAPSELECTIONWINDOW_H
#define MAPSELECTIONWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MapSelectionWindow; }
QT_END_NAMESPACE

struct MapItem
{
    QString filePath;     //absolute path to mapX.txt
    QString displayName;  //shown in the list (e.g. map1)
};

class MapSelectionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MapSelectionWindow(QWidget *parent = nullptr);
    ~MapSelectionWindow();

    //call this right after creating the window
    void setMapsFolder(const QString &folderPath);

signals:
    //emitted when user chooses a map successfully
    void mapChosen(const QString &mapFilePath);

    //emitted when user cancels/back
    void canceled();

protected:
    //for background stretching if you use backgroundLabel
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onSelectClicked();
    void onCancelClicked();

private:
    Ui::MapSelectionWindow *ui;

    QString m_mapsFolder;
    QVector<MapItem> m_maps;

    //optional background scaling (only if you set a background image)
    QPixmap m_bgPixmap;
    void updateBackground();

    void loadMaps();
};

#endif
