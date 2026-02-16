#pragma once

#include "CMap.h"
#include "Constants.h"

#include <QGraphicsView>
#include <QPoint>

//-----------------------------------------------------------------------------
class MapItem;
class QGraphicsItem;
class QGraphicsScene;
class QMouseEvent;
class QWheelEvent;

//-----------------------------------------------------------------------------
class CMainView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CMainView(QWidget* parent = nullptr);
    ~CMainView() override;

    void zoomIn();
    void zoomOut();
    void resetZoom();
    
    void setMap(CMap* map);
    void setSelectedTile(int tile) { m_selectedTile = tile; }
    void setTileset(const QImage& tileset);
    void setTileSize(int tileSize);
    void setTool(int tool) { m_currentTool = tool; }

signals:
    void mouseTileChanged(int x, int y);
    void tileChanged(int x, int y, uint32_t value, QGraphicsItem* mapItem);
    void fillApplied(const QVector<QPair<int, int>>& tiles, uint32_t value, QGraphicsItem* mapItem);

private:
    QGraphicsScene* m_scene = nullptr;
    QGraphicsItem* m_gridItem = nullptr;
    MapItem* m_mapItem = nullptr;
    CMap* m_map = nullptr;
    
    // pan state
    bool m_panning = false;
    bool m_painting = false;
    QPoint m_lastPanPoint;
    double m_zoom = 1.0;
    int m_selectedTile = 0;
    int m_tileSize = 32;
    int m_currentTool = Constants::TOOL_PAINT;
    QImage m_tileset;

    void applyZoom();
    void paintTile(const QPointF& scenePos, int tileValue);
    QVector<QPair<int, int>> collectFillTiles(int x, int y, uint32_t targetTile);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
};
