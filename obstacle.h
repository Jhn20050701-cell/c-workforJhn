#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>

class Obstacle : public QGraphicsRectItem
{
public:
    Obstacle(int x, int y, int w, int h);
};

#endif