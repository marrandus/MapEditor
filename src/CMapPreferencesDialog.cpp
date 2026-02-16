#include "CMapPreferencesDialog.h"
#include "Constants.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QVBoxLayout>

//-----------------------------------------------------------------------------
CMapPreferencesDialog::CMapPreferencesDialog(int currentWidth, int currentHeight, QWidget* parent)
: QDialog(parent)
{
    setWindowTitle(tr("Map Preferences"));

    m_widthSpinBox = new QSpinBox(this);
    m_widthSpinBox->setRange(Constants::MIN_MAP_WIDTH, Constants::MAX_MAP_WIDTH);
    m_widthSpinBox->setValue(currentWidth);

    m_heightSpinBox = new QSpinBox(this);
    m_heightSpinBox->setRange(Constants::MIN_MAP_HEIGHT, Constants::MAX_MAP_HEIGHT);
    m_heightSpinBox->setValue(currentHeight);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(tr("Width:"), m_widthSpinBox);
    formLayout->addRow(tr("Height:"), m_heightSpinBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}

//-----------------------------------------------------------------------------
int CMapPreferencesDialog::width() const
{
    return m_widthSpinBox->value();
}

//-----------------------------------------------------------------------------
int CMapPreferencesDialog::height() const
{
    return m_heightSpinBox->value();
}
