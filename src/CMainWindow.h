#pragma once

#include "Constants.h"

#include <QImage>
#include <QMainWindow>
#include <QString>
#include <QVector>

//-----------------------------------------------------------------------------
class QLabel;
class CMainView;
class CMap;
class QToolBar;
class QToolButton;
class QUndoStack;

//-----------------------------------------------------------------------------
class CMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CMainWindow(QWidget* parent = nullptr);
    ~CMainWindow() override;

private slots:
    void onNewMap();
    void onOpenMap();
    bool onSaveMap();
    void onSaveMapAs();
    void onOpenTileset();
    void onMapPreferences();
    void onExit();
    void onAbout();
    void onTileSelected();
    void onMouseTileChanged(int x, int y);
    void onPaintTool();
    void onFillTool();
    void selectTile(int index);
    void cycleTileNext();
    void cycleTilePrev();
    
private:
    void closeEvent(QCloseEvent* event) override;
    void createPalette();
    void updatePalette();
    void updateWindowTitle();

    bool m_modified = false;
    int m_selectedTile = 0;
    int m_tileSize = 32;
    int m_tileCount = 12;
    int m_currentTool = Constants::TOOL_PAINT;

    QLabel* m_statusLabel = nullptr;
    QLabel* m_positionLabel = nullptr;
    CMainView* m_view = nullptr;
    CMap* m_map = nullptr;
    QUndoStack* m_undoStack = nullptr;
    QString m_currentMapPath;
    QToolBar* m_mainToolBar = nullptr;
    QToolBar* m_toolsToolBar = nullptr;
    QToolBar* m_paletteToolBar = nullptr;
    QVector<QToolButton*> m_paletteButtons;
    QImage m_tileset;
};
