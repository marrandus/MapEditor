#pragma once

//-----------------------------------------------------------------------------
#include <QDialog>
#include <QImage>

//-----------------------------------------------------------------------------
class QSpinBox;
class QLabel;

//-----------------------------------------------------------------------------
class CTilesetSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CTilesetSettingsDialog(const QImage& tileset, QWidget* parent = nullptr);

    int tileSize() const;
    int tileCount() const;

private:
    QSpinBox* m_tileSizeSpinBox = nullptr;
    QSpinBox* m_tileCountSpinBox = nullptr;
    QLabel* m_previewLabel = nullptr;
    QImage m_tileset;
};
