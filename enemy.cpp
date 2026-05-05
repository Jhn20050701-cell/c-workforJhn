#include "enemy.h"
#include "bullet.h"
#include <QGraphicsScene>
#include <QList>
#include <math.h>
#include "weapon.h"
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
            health -= bullet->getDamage();   // 用子弹伤害
            scene()->removeItem(bullet);
            delete bullet;

            if(health<=0)
            {
                // 1. 创建武器实例
                Weapon *weapon = new Weapon(Weapon::Rifle);
                // 2. 设置武器掉落位置（和敌人死亡位置一致）
                weapon->setPos(this->x(), this->y());
                // 3. 将武器添加到游戏场景（和子弹、敌人共用同一个场景）
                this->scene()->addItem(weapon);
                // 4. 敌人死亡后从场景移除并释放内存
                this->scene()->removeItem(this);
                delete this;
            }
        }
    }
}