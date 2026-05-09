#include "player.h"
#include "bullet.h"
#include "weapon.h"
#include <qmath.h>
#include<QBrush>
#include<QGraphicsScene>
#include<QCursor>
#include <QApplication>
Player::Player(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setRect(0, 0, 40, 40);
    setBrush(Qt::blue);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    health = 100;
    maxHealth = 100;
    damage = 50;
    shootInterval = 300;
    isFiring = false;

    shootTimer = new QTimer(this);
    shootTimer->setInterval(250);
    connect(shootTimer, &QTimer::timeout, this, &Player::shootLoop);
    // 血条
    healthBarBg = new QGraphicsRectItem(0, -10, 40, 5, this);
    healthBarBg->setBrush(Qt::gray);

    healthBar = new QGraphicsRectItem(0, -10, 40, 5, this);
    healthBar->setBrush(Qt::green);
}

// 玩家移动（上下左右）
void Player::keyPressEvent(QKeyEvent *event)
{
    const int moveSpeed = 12; // 这里就是移动速度，数值越大越快

    if (event->key() == Qt::Key_Left)  setPos(x() - moveSpeed, y());
    if (event->key() == Qt::Key_Right) setPos(x() + moveSpeed, y());
    if (event->key() == Qt::Key_Up)    setPos(x(), y() - moveSpeed);
    if (event->key() == Qt::Key_Down)  setPos(x(), y() + moveSpeed);

    // 拾取武器 E 键
    if (event->key() == Qt::Key_E) {
        for (auto item : collidingItems()) {
            Weapon* w = dynamic_cast<Weapon*>(item);
            if (w) {
                damage = w->getDamage();
                shootInterval = w->getSpeed();
                emit weaponChanged(damage, w->getName());

                scene()->removeItem(w);
                delete w;
                break;
            }
        }
    }
}

// 按住鼠标开始连发
void Player::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isFiring = true;
        shootLoop();        // 立刻开第一枪
        shootTimer->start();// 循环连发
    }
}
void Player::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    mouseScenePos = event->scenePos();
}
// 松开鼠标停止
void Player::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isFiring = false;
        shootTimer->stop();
    }
}

void Player::shootLoop()
{
    if (!isFiring) return;


    QLineF aimLine(pos(), mouseScenePos);
    double angle = qAtan2(aimLine.dy(), aimLine.dx());

    Bullet* b = new Bullet(angle, damage, true);
    b->setPos(pos());
    scene()->addItem(b);
}

// 受伤逻辑（不变）
void Player::takeDamage(int dmg)
{
    health -= dmg;
    if (health < 0) health = 0;

    double ratio = static_cast<double>(health) / maxHealth;
    healthBar->setRect(0, -10, 40 * ratio, 5);

    if (ratio > 0.6)
        healthBar->setBrush(Qt::green);
    else if (ratio > 0.3)
        healthBar->setBrush(Qt::yellow);
    else
        healthBar->setBrush(Qt::red);

    if (health <= 0) {
        qApp->quit();
    }
}