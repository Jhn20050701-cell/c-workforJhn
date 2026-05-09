#include "bullet.h"
#include <QGraphicsScene>
#include <math.h>
#include "player.h"
#include "enemy.h"
#include "obstacle.h"

Bullet::Bullet(double angle, int dmg, bool isPlayerBullet)
{
    this->angle = angle;
    this->damage = dmg;
    this->isPlayerBullet = isPlayerBullet;

    setRect(0,0,10,10);
    setBrush(isPlayerBullet ? Qt::red : Qt::blue);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(20);
}

void Bullet::move()
{
    double step = 10;
    setPos(x() + step * cos(angle), y() + step * sin(angle));

    // 1. 越界删除
    if (x() < 0 || x() > 800 || y() < 0 || y() > 600)
    {
        scene()->removeItem(this);
        delete this;
        return;
    }

    // 2. 碰撞检测
    QList<QGraphicsItem*> items = collidingItems();
    for(auto item : items)
    {
        // 玩家子弹 → 打敌人
        if (isPlayerBullet)
        {
            Enemy* enemy = dynamic_cast<Enemy*>(item);
            if(enemy)
            {
                enemy->health -= this->damage;
                if (enemy->health <= 0)
                {
                    emit enemy->enemyKilled();
                    scene()->removeItem(enemy);
                    delete enemy;
                }
                scene()->removeItem(this);
                delete this;
                return;
            }
        }
        // 敌人子弹 → 打玩家
        else
        {
            Player* player = dynamic_cast<Player*>(item);
            if(player)
            {
                player->takeDamage(this->damage);
                scene()->removeItem(this);
                delete this;
                return;
            }
        }

        // 碰撞掩体
        Obstacle* obs = dynamic_cast<Obstacle*>(item);
        if(obs)
        {
            scene()->removeItem(this);
            delete this;
            return;
        }
    }
}