#include "enemy.h"
#include "bullet.h"
#include <QGraphicsScene>
#include <QList>
#include <math.h>

Enemy::Enemy(QGraphicsRectItem* player)
{
    setRect(0,0,40,40);
    setBrush(Qt::green);
    setZValue(1);

    health = 100;
    playerRef = player;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50);
}

void Enemy::move()
{
    if(!scene()) return;

    // 追踪玩家
    QLineF line(pos(), playerRef->pos());
    double angle = atan2(line.dy(), line.dx());
    double step = 2;
    setPos(x() + step*cos(angle), y() + step*sin(angle));

    // 碰撞子弹
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for(auto item: colliding_items)
    {
        Bullet* bullet = dynamic_cast<Bullet*>(item);
        if(bullet)
        {
            health -= 50;
            scene()->removeItem(bullet);
            delete bullet;

            if(health<=0)
            {
                // 死亡掉落武器
                QGraphicsRectItem* weapon = new QGraphicsRectItem(0,0,20,20);
                weapon->setBrush(Qt::yellow);
                weapon->setPos(x(), y());
                scene()->addItem(weapon);

                scene()->removeItem(this);
                delete this;
                return;
            }
        }
    }
}