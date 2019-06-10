#pragma once

#include <QtWidgets/QtWidgets>

class Rect : public QWidget
{
    Q_OBJECT
public:
     Rect(QWidget* parent, QColor color, int size)
    :   QWidget(parent),
        m_color(color),
        m_size(size)
    { }

    void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.fillRect(0, 0, m_size, m_size, m_color);
    }

private:
    QColor m_color;
    int m_size;
};