#include "CMap.h"

#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>
#include <utility>

//-----------------------------------------------------------------------------
CMap::CMap(int w, int h)
{
    resize(w, h, 0);
}

//-----------------------------------------------------------------------------
bool CMap::isValidPosition(int x, int y) const
{
    return x >= 0 && y >= 0 && x < m_width && y < m_height;
}

//-----------------------------------------------------------------------------
uint32_t CMap::tileAt(int x, int y) const
{
    if (!isValidPosition(x, y)) return 0;
    return m_tiles[y * m_width + x];
}

//-----------------------------------------------------------------------------
void CMap::setTile(int x, int y, uint32_t value)
{
    if (!isValidPosition(x, y)) return;
    m_tiles[y * m_width + x] = value;
}

//-----------------------------------------------------------------------------
void CMap::resize(int w, int h, uint32_t fill)
{
    int newWidth = std::max(0, w);
    int newHeight = std::max(0, h);
    
    std::vector<uint32_t> newTiles(newWidth * newHeight, fill);
    
    // Copy existing tiles that fit in new dimensions
    int copyWidth = std::min(m_width, newWidth);
    int copyHeight = std::min(m_height, newHeight);
    
    for (int y = 0; y < copyHeight; ++y) {
        for (int x = 0; x < copyWidth; ++x) {
            newTiles[y * newWidth + x] = m_tiles[y * m_width + x];
        }
    }
    
    m_width = newWidth;
    m_height = newHeight;
    m_tiles = std::move(newTiles);
}

//-----------------------------------------------------------------------------
void CMap::clear(uint32_t fill)
{
    std::fill(m_tiles.begin(), m_tiles.end(), fill);
}

//-----------------------------------------------------------------------------
QJsonObject CMap::toJson() const
{
    QJsonObject obj;
    obj["width"] = m_width;
    obj["height"] = m_height;
    QJsonArray arr;
    for (const uint32_t& v : m_tiles)
        arr.append(static_cast<qint64>(v));
    obj["tiles"] = arr;
    return obj;
}

//-----------------------------------------------------------------------------
bool CMap::fromJson(const QJsonObject& obj)
{
    if (!obj.contains("width") || !obj.contains("height") || !obj.contains("tiles"))
        return false;
    int w = obj["width"].toInt();
    int h = obj["height"].toInt();
    QJsonArray arr = obj["tiles"].toArray();
    if (arr.size() != w * h) return false;
    resize(w, h);
    for (int i = 0; i < arr.size(); ++i)
        m_tiles[i] = static_cast<uint32_t>(arr[i].toInt());
    return true;
}
