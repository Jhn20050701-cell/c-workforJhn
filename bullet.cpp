#include "bullet.h"
#include <QGraphicsScene>
#include <math.h>

Bullet::Bullet(double angle)
{
    this->angle = angle;
    setRect(0,0,10,10);
    setBrush(Qt::red);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(20);
}

void Bullet::move()
{
    double step = 10;
    setPos(x() + step*cos(angle), y() + step*sin(angle));

    if(x()<0 || x()>800 || y()<0 || y()>600)
    {
        if(scene()) scene()->removeItem(this);
        delete this;
    }
}