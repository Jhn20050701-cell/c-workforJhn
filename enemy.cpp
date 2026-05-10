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
#include <QPixmap>
#include<QPainter>
Enemy::Enemy(Player* player, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    playerRef = player;
    health = 100;
    attackCooldown = 0;

    // 尺寸改为 14×14，适配你的场景
    QPixmap pix(24, 24);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);

    // 1. 土黄色军装身体
    p.setBrush(QColor(180, 160, 100));
    p.setPen(QPen(Qt::black, 0.5));
    p.drawRect(2, 3, 10, 8); // 身体

    // 2. 军帽
    p.setBrush(QColor(120, 100, 60));
    p.drawEllipse(3, 1, 8, 5); // 帽子

    // 3. 肤色+脸部
    p.setBrush(QColor(230, 210, 180));
    p.drawEllipse(4, 2, 6, 6); // 脸

    // 4. 眼睛、嘴巴、八字胡（简化适配小尺寸）
    p.setPen(QPen(Qt::black, 1));
    p.drawLine(5, 4, 6, 4.5);  // 左眼
    p.drawLine(8, 4, 7, 4.5);  // 右眼
    p.drawLine(6, 6, 8, 6);    // 嘴
    p.drawLine(5.5, 6.5, 6.5, 7); // 八字胡左
    p.drawLine(7.5, 6.5, 6.5, 7); // 八字胡右

    // 5. 带刺刀的步枪（简化）
    p.setPen(QPen(Qt::black, 0.5));
    p.drawLine(12, 5, 12, 10);  // 枪身
    p.drawLine(12, 5, 13.5, 4); // 刺刀

    p.end();

    setPixmap(pix);
    setOffset(-7, -7); // 居中，碰撞盒对齐14×14

    // 定时器（保持你的移动逻辑）
    QTimer* t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &Enemy::move);
    t->start(30);
}

void Enemy::move()
{
    if (!playerRef || health <= 0) return;

    // 追踪玩家
    QPointF dir = playerRef->pos() - pos();
    double len = qSqrt(dir.x()*dir.x() + dir.y()*dir.y());
    if (len > 0) dir /= len;

    double speed = 1.3;
    setPos(pos() + dir * speed);

    // 射击
    attackCooldown++;
    if (attackCooldown > 60 && len < 300) {
        attackCooldown = 0;
        double ang = qAtan2(dir.y(), dir.x());
        Bullet* b = new Bullet(ang, 10, false);
        b->setPos(pos());
        scene()->addItem(b);
    }

    // 死亡
    if (health <= 0) {
        emit enemyKilled();
        scene()->removeItem(this);
        delete this;
    }
}
