#include "MapSelectionWindow.h"
#include "ui_MapSelectionWindow.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QResizeEvent>
#include <QPixmap>  // Include for handling images

MapSelectionWindow::MapSelectionWindow(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MapSelectionWindow)
{
    ui->setupUi(this);
    setWindowTitle("Select Map");

    // Load the background image using QPixmap
    m_bgPixmap = QPixmap("D:/Desktop/Undaunted-Phase1/Undaunted-Phase1/images/Map_selection.jpg");  // Add your image path in the resource system

    if (m_bgPixmap.isNull()) {
        qDebug() << "ERROR: background image not found!";
    }

    // UI WIDGETS expected in MapSelectionWindow.ui:
    // - QListWidget mapListWidget
    // - QPushButton selectButton
    // - QPushButton cancelButton
    // - QLabel backgroundLabel (optional, for stretched background)

    connect(ui->selectButton, &QPushButton::clicked,
            this, &MapSelectionWindow::onSelectClicked);

    connect(ui->cancelButton, &QPushButton::clicked,
            this, &MapSelectionWindow::onCancelClicked);
}

MapSelectionWindow::~MapSelectionWindow()
{
    delete ui;
}

void MapSelectionWindow::setMapsFolder(const QString &folderPath)
{
    m_mapsFolder = folderPath;

    // Debug print to check the folder path
    qDebug() << "Maps folder path: " << m_mapsFolder;

    loadMaps();
}


void MapSelectionWindow::loadMaps()
{
    ui->mapListWidget->clear();
    m_maps.clear();

    QDir dir(m_mapsFolder);
    if (!dir.exists()) {
        QMessageBox::warning(this, "Maps Missing",
                             "Maps folder not found:\n" + m_mapsFolder);
        return;
    }

    // loads map1.txt, map2.txt ... and ANY other .txt (dynamic)
    QStringList files = dir.entryList(QStringList() << "*.txt",
                                      QDir::Files, QDir::Name);

    if (files.isEmpty()) {
        QMessageBox::warning(this, "No Maps",
                             "No .txt map files found in:\n" + m_mapsFolder);
        return;
    }

    for (const QString &file : files) {
        const QString fullPath = dir.absoluteFilePath(file);

        MapItem item;
        item.filePath = fullPath;
        item.displayName = QFileInfo(file).baseName(); // map1, map2, ...

        m_maps.push_back(item);

        // show in list
        QListWidgetItem *w = new QListWidgetItem(item.displayName);
        w->setData(Qt::UserRole, item.filePath);
        ui->mapListWidget->addItem(w);
    }

    ui->mapListWidget->setCurrentRow(0);
}

void MapSelectionWindow::onSelectClicked()
{
    QListWidgetItem *current = ui->mapListWidget->currentItem();
    if (!current) {
        QMessageBox::warning(this, "No Selection", "Please select a map.");
        return;
    }

    const QString selectedPath = current->data(Qt::UserRole).toString();
    if (selectedPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Invalid selected map.");
        return;
    }

    emit mapChosen(selectedPath);
    close();
}

void MapSelectionWindow::onCancelClicked()
{
    emit canceled();
    close();
}

void MapSelectionWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateBackground();  // Update background whenever window is resized
}

void MapSelectionWindow::updateBackground()
{
    if (m_bgPixmap.isNull()) {
        return;
    }

    // If you didn't add backgroundLabel in UI, comment next line
    ui->backgroundLabel->setPixmap(
        m_bgPixmap.scaled(ui->backgroundLabel->size(),
                          Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation));  // Set image to fit label size
}
