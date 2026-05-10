#include "obstacle.h"
#include <QBrush>

Obstacle::Obstacle(int x, int y, int w, int h)
{
    setRect(x, y, w, h);
    setBrush(Qt::transparent);

}