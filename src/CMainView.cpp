#include "CMainView.h"
#include "Constants.h"
#include "CMap.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QSet>
#include <QWheelEvent>
#include <stack>
#include <utility>

//-----------------------------------------------------------------------------
class MapItem : public QGraphicsItem {
public:
    MapItem(CMap* map, QGraphicsItem* parent = nullptr) : QGraphicsItem(parent), m_map(map) {}
    QRectF boundingRect() const override { 
        if (!m_map) return QRectF();
        return QRectF(0, 0, m_map->width() * Constants::DEFAULT_TILE_SIZE, m_map->height() * Constants::DEFAULT_TILE_SIZE);
    }
    void setTileset(const QImage& tileset, int tileSize) { 
        m_tileset = tileset;
        m_tileSize = tileSize;
    }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        if (!m_map) return;
        
        static const QColor colors[] = {
            Qt::white, Qt::black, Qt::red, Qt::green, Qt::blue, Qt::yellow,
            Qt::cyan, Qt::magenta, Qt::gray, Qt::darkRed, Qt::darkGreen, Qt::darkBlue
        };
        
        for (int y = 0; y < m_map->height(); ++y) {
            for (int x = 0; x < m_map->width(); ++x) {
                uint32_t tile = m_map->tileAt(x, y);
                if (tile == 0) continue;
                
                QRectF rect(x * Constants::DEFAULT_TILE_SIZE, y * Constants::DEFAULT_TILE_SIZE,
                           Constants::DEFAULT_TILE_SIZE, Constants::DEFAULT_TILE_SIZE);
                
                if (m_tileset.isNull()) {
                    painter->fillRect(rect, colors[(tile - 1) % 12]);
                } else {
                    // Extract tile from tileset using configured tile size
                    int tilesPerRow = m_tileset.width() / m_tileSize;
                    if (tilesPerRow > 0 && m_tileSize > 0) {
                        int tileIndex = tile - 1;
                        int tx = (tileIndex % tilesPerRow) * m_tileSize;
                        int ty = (tileIndex / tilesPerRow) * m_tileSize;
                        if (tx + m_tileSize <= m_tileset.width() && ty + m_tileSize <= m_tileset.height()) {
                            QImage tileImg = m_tileset.copy(tx, ty, m_tileSize, m_tileSize);
                            painter->drawImage(rect, tileImg);
                        }
                    }
                }
            }
        }
    }
private:
    CMap* m_map = nullptr;
    QImage m_tileset;
    int m_tileSize = 32;
};

//-----------------------------------------------------------------------------
class GridItem : public QGraphicsItem {
public:
    GridItem(CMap* map, QGraphicsItem* parent = nullptr) : QGraphicsItem(parent), m_map(map) {}
    QRectF boundingRect() const override { 
        if (!m_map) return QRectF();
        return QRectF(0, 0, m_map->width() * Constants::DEFAULT_TILE_SIZE, m_map->height() * Constants::DEFAULT_TILE_SIZE);
    }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        if (!m_map) return;
        int w = m_map->width() * Constants::DEFAULT_TILE_SIZE;
        int h = m_map->height() * Constants::DEFAULT_TILE_SIZE;
        painter->fillRect(0, 0, w, h, Qt::white);
        painter->setPen(QPen(Qt::lightGray));
        for (int x = 0; x <= w; x += Constants::DEFAULT_TILE_SIZE)
            painter->drawLine(x, 0, x, h);
        for (int y = 0; y <= h; y += Constants::DEFAULT_TILE_SIZE)
            painter->drawLine(0, y, w, y);
        painter->setPen(QPen(Qt::black));
        painter->drawRect(0, 0, w, h);
    }

private:
    CMap* m_map = nullptr;
};

//-----------------------------------------------------------------------------
CMainView::CMainView(QWidget* parent)
    : QGraphicsView(parent),
      m_gridItem(nullptr),
      m_mapItem(nullptr),
      m_zoom(1.0)
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing, true);
    setMouseTracking(true);
    setBackgroundBrush(QBrush(Qt::gray));
}

//-----------------------------------------------------------------------------
CMainView::~CMainView()
{
}

//-----------------------------------------------------------------------------
void CMainView::setMap(CMap* map)
{
    m_map = map;
    if (m_gridItem) {
        m_scene->removeItem(m_gridItem);
        delete m_gridItem;
        m_gridItem = nullptr;
    }
    if (m_mapItem) {
        m_scene->removeItem(m_mapItem);
        delete m_mapItem;
        m_mapItem = nullptr;
    }
    m_gridItem = new GridItem(m_map);
    m_gridItem->setZValue(0);
    m_scene->addItem(m_gridItem);
    m_mapItem = new MapItem(m_map);
    m_mapItem->setZValue(1);
    m_scene->addItem(m_mapItem);
    
    if (m_map) {
        QRectF mapRect(0, 0, m_map->width() * Constants::DEFAULT_TILE_SIZE, m_map->height() * Constants::DEFAULT_TILE_SIZE);
        m_scene->setSceneRect(mapRect);
    }
}

//-----------------------------------------------------------------------------
void CMainView::setTileset(const QImage& tileset)
{
    m_tileset = tileset;
    if (m_mapItem) {
        m_mapItem->setTileset(tileset, m_tileSize);
        m_scene->update();
    }
}

//-----------------------------------------------------------------------------
void CMainView::setTileSize(int tileSize)
{
    m_tileSize = tileSize;
    if (m_mapItem && !m_tileset.isNull()) {
        m_mapItem->setTileset(m_tileset, m_tileSize);
        m_scene->update();
    }
}

//-----------------------------------------------------------------------------
void CMainView::zoomIn()
{
    m_zoom *= Constants::ZOOM_STEP;
    if (m_zoom > Constants::MAX_ZOOM)
        m_zoom = Constants::MAX_ZOOM;
    applyZoom();
}

//-----------------------------------------------------------------------------
void CMainView::zoomOut()
{
    m_zoom /= Constants::ZOOM_STEP;
    if (m_zoom < Constants::MIN_ZOOM)
        m_zoom = Constants::MIN_ZOOM;
    applyZoom();
}

//-----------------------------------------------------------------------------
void CMainView::resetZoom()
{
    m_zoom = 1.0;
    applyZoom();
}

//-----------------------------------------------------------------------------
void CMainView::applyZoom()
{
    QTransform t;
    t.scale(m_zoom, m_zoom);
    setTransform(t);
}

//-----------------------------------------------------------------------------
void CMainView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_panning) {
        QPoint delta = event->pos() - m_lastPanPoint;
        m_lastPanPoint = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        event->accept();
        return;
    }
    
    // Update tile position
    QPointF scenePos = mapToScene(event->pos());
    int tileX = scenePos.x() < 0 ? -1 : static_cast<int>(scenePos.x()) / Constants::DEFAULT_TILE_SIZE;
    int tileY = scenePos.y() < 0 ? -1 : static_cast<int>(scenePos.y()) / Constants::DEFAULT_TILE_SIZE;
    
    // Only emit position if within valid map bounds
    if (m_map && tileX >= 0 && tileX < m_map->width() && tileY >= 0 && tileY < m_map->height()) {
        emit mouseTileChanged(tileX, tileY);
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
    
    if (m_painting && m_map) {
        int tileValue = (event->buttons() & Qt::RightButton) ? 0 : (m_selectedTile + 1);
        paintTile(scenePos, tileValue);
        event->accept();
        return;
    }
    
    QGraphicsView::mouseMoveEvent(event);
}

//-----------------------------------------------------------------------------
void CMainView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) {
        m_panning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    
    if ((event->button() == Qt::LeftButton || event->button() == Qt::RightButton) && m_map) {
        m_painting = true;
        QPointF scenePos = mapToScene(event->pos());
        int tileValue = (event->button() == Qt::RightButton) ? 0 : (m_selectedTile + 1);
        paintTile(scenePos, tileValue);
        event->accept();
        return;
    }
    
    QGraphicsView::mousePressEvent(event);
}

//-----------------------------------------------------------------------------
void CMainView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton && m_panning) {
        m_panning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    
    if ((event->button() == Qt::LeftButton || event->button() == Qt::RightButton) && m_painting) {
        m_painting = false;
        event->accept();
        return;
    }
    
    QGraphicsView::mouseReleaseEvent(event);
}

//-----------------------------------------------------------------------------
void CMainView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0)
            zoomIn();
        else if (event->angleDelta().y() < 0)
            zoomOut();
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

//-----------------------------------------------------------------------------
void CMainView::paintTile(const QPointF& scenePos, int tileValue)
{
    if (!m_map) return;
    
    int tileX = scenePos.x() < 0 ? -1 : static_cast<int>(scenePos.x()) / Constants::DEFAULT_TILE_SIZE;
    int tileY = scenePos.y() < 0 ? -1 : static_cast<int>(scenePos.y()) / Constants::DEFAULT_TILE_SIZE;
    
    if (tileX >= 0 && tileX < m_map->width() && tileY >= 0 && tileY < m_map->height()) {
        if (m_currentTool == Constants::TOOL_FILL) {
            uint32_t oldTile = m_map->tileAt(tileX, tileY);
            if (oldTile != static_cast<uint32_t>(tileValue)) {
                QVector<QPair<int, int>> tiles = collectFillTiles(tileX, tileY, oldTile);
                if (!tiles.isEmpty()) {
                    emit fillApplied(tiles, static_cast<uint32_t>(tileValue), m_mapItem);
                }
            }
        } else {
            uint32_t oldValue = m_map->tileAt(tileX, tileY);
            if (oldValue != static_cast<uint32_t>(tileValue)) {
                emit tileChanged(tileX, tileY, static_cast<uint32_t>(tileValue), m_mapItem);
            }
        }
    }
}

//-----------------------------------------------------------------------------
QVector<QPair<int, int>> CMainView::collectFillTiles(int x, int y, uint32_t targetTile)
{
    QVector<QPair<int, int>> result;
    if (!m_map)
        return result;
    
    std::stack<std::pair<int, int>> stack;
    QSet<QPair<int, int>> visited;
    stack.push({x, y});
    
    while (!stack.empty()) {
        auto [cx, cy] = stack.top();
        stack.pop();
        
        if (cx < 0 || cy < 0 || cx >= m_map->width() || cy >= m_map->height())
            continue;
        
        QPair<int, int> pos(cx, cy);
        if (visited.contains(pos))
            continue;
        
        if (m_map->tileAt(cx, cy) != targetTile)
            continue;
        
        visited.insert(pos);
        result.append(pos);
        
        stack.push({cx + 1, cy});
        stack.push({cx - 1, cy});
        stack.push({cx, cy + 1});
        stack.push({cx, cy - 1});
    }
    
    return result;
}
