#pragma once

#include <cstdint>
#include <vector>
#include <QString>

//-----------------------------------------------------------------------------
class QJsonObject;

//-----------------------------------------------------------------------------
class CMap
{
public:
    CMap() = default;
    CMap(int w, int h);

    int width() const { return m_width; }
    int height() const { return m_height; }

    uint32_t tileAt(int x, int y) const;
    void setTile(int x, int y, uint32_t value);

    void resize(int w, int h, uint32_t fill = 0);
    void clear(uint32_t fill = 0);

    QJsonObject toJson() const;
    bool fromJson(const QJsonObject& obj);

private:
    int m_width = 0;
    int m_height = 0;
    std::vector<uint32_t> m_tiles;
    
    bool isValidPosition(int x, int y) const;
};
