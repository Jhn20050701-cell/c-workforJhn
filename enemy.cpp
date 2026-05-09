#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "obstacle.h"
#include <QTimer>
#include <qmath.h>
#include <QList>
#include <QRandomGenerator>
#include<QBrush>
#include<QGraphicsScene>
Enemy::Enemy(Player* player)
{
    this->playerRef = player;
    health = 100;
    attackCooldown = 0;

    setRect(0, 0, 40, 40);
    setBrush(Qt::red);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Enemy::move);
    timer->start(30);
}

void Enemy::move()
{
    if (!playerRef) return;
    if (health <= 0) {
        // 随机掉落武器：0=手枪，1=步枪
        int type = QRandomGenerator::global()->bounded(2);
        emit dropWeapon(pos(), type); // 发射掉落信号
        emit enemyKilled();            // 发射击杀加分信号

        scene()->removeItem(this);
        delete this;
        return;
    }

    // 移动逻辑（不变）
    QPointF oldPos = pos();
    QPointF dir = playerRef->pos() - pos();
    double length = qSqrt(dir.x()*dir.x() + dir.y()*dir.y());

    if (length > 0)
        dir /= length;

    double speed = 1.5;
    setPos(x() + dir.x() * speed, y() + dir.y() * speed);

    // 掩体碰撞（不变）
    for (QGraphicsItem* item : collidingItems()) {
        if (dynamic_cast<Obstacle*>(item)) {
            setPos(oldPos);
            break;
        }
    }

    attackCooldown++;

    // 射击逻辑（不变）
    QLineF line(pos(), playerRef->pos());
    if (line.length() < 300)
    {
        bool blocked = false;
        for (QGraphicsItem* item : scene()->items()) {
            Obstacle* obs = dynamic_cast<Obstacle*>(item);
            if (obs && obs->sceneBoundingRect().intersects(QRectF(pos(), playerRef->pos()).normalized())) {
                blocked = true;
                break;
            }
        }

        if (!blocked && attackCooldown > 50)
        {
            attackCooldown = 0;
            double angle = qAtan2(playerRef->y() - y(), playerRef->x() - x());
            Bullet* bullet = new Bullet(angle, 10, false);
            bullet->setPos(pos());
            scene()->addItem(bullet);
        }
    }
}