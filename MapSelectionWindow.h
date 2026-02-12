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
    QString filePath;
    QString displayName;
};

class MapSelectionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MapSelectionWindow(QWidget *parent = nullptr);
    ~MapSelectionWindow();

    void setMapsFolder(const QString &folderPath);
    void setStatesFolder(const QString &folderPath);   // NEW

signals:
    // UPDATED SIGNAL (map + state)
    void mapChosen(const QString &mapFilePath,
                   const QString &stateFilePath);

    void canceled();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onSelectClicked();
    void onCancelClicked();

private:
    Ui::MapSelectionWindow *ui;

    QString m_mapsFolder;
    QString m_stateFolder;          // NEW

    QVector<MapItem> m_maps;
    QVector<MapItem> m_states;      // NEW

    QPixmap m_bgPixmap;

    void updateBackground();
    void loadMaps();
    void loadStates();              // NEW
};

#endif
