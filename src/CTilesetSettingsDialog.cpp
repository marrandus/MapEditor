#include "CTilesetSettingsDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QPixmap>

//-----------------------------------------------------------------------------
CTilesetSettingsDialog::CTilesetSettingsDialog(const QImage& tileset, QWidget* parent)
    : QDialog(parent), m_tileset(tileset)
{
    setWindowTitle(tr("Tileset Settings"));
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Preview
    m_previewLabel = new QLabel(this);
    QPixmap preview = QPixmap::fromImage(m_tileset);
    if (preview.width() > 256 || preview.height() > 256) {
        preview = preview.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    m_previewLabel->setPixmap(preview);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    mainLayout->addWidget(m_previewLabel);
    
    // Settings form
    QFormLayout* formLayout = new QFormLayout();
    
    m_tileSizeSpinBox = new QSpinBox(this);
    m_tileSizeSpinBox->setRange(16, 128);
    m_tileSizeSpinBox->setValue(32);
    formLayout->addRow(tr("Tile size:"), m_tileSizeSpinBox);
    
    m_tileCountSpinBox = new QSpinBox(this);
    m_tileCountSpinBox->setRange(1, 128);
    m_tileCountSpinBox->setValue(16);
    formLayout->addRow(tr("Tile count:"), m_tileCountSpinBox);
    
    mainLayout->addLayout(formLayout);
    
    // Buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

//-----------------------------------------------------------------------------
int CTilesetSettingsDialog::tileSize() const
{
    return m_tileSizeSpinBox->value();
}

//-----------------------------------------------------------------------------
int CTilesetSettingsDialog::tileCount() const
{
    return m_tileCountSpinBox->value();
}
