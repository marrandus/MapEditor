#pragma once

//-----------------------------------------------------------------------------
#include <QDialog>

//-----------------------------------------------------------------------------
class QSpinBox;

//-----------------------------------------------------------------------------
class CMapPreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CMapPreferencesDialog(int currentWidth, int currentHeight, QWidget* parent = nullptr);

    int width() const;
    int height() const;

private:
    QSpinBox* m_widthSpinBox = nullptr;
    QSpinBox* m_heightSpinBox = nullptr;
};
