#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent), m_uiIndex(-1)
{

}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent* /* event */)
{
    emit clicked();
}

void ClickableLabel::setIndex(uint idx)
{
    m_uiIndex = idx;
}

uint ClickableLabel::index()
{
    return m_uiIndex;
}
