#include "player.h"
#include "bullet.h"
#include <QLineF>
#include <math.h>
#include<QBrush>
#include<QGraphicsScene>
Player::Player()
{
    setRect(0,0,40,40);
    setBrush(Qt::blue);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Player::keyPressEvent(QKeyEvent *event)
{
    int speed = 10;
    if(event->key()==Qt::Key_W) setPos(x(), y()-speed);
    if(event->key()==Qt::Key_S) setPos(x(), y()+speed);
    if(event->key()==Qt::Key_A) setPos(x()-speed, y());
    if(event->key()==Qt::Key_D) setPos(x()+speed, y());
}

void Player::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF target = event->scenePos();
    QLineF line(pos(), target);
    double angle = atan2(line.dy(), line.dx());

    Bullet *bullet = new Bullet(angle);
    bullet->setPos(pos());
    scene()->addItem(bullet);
}