#include "player.h"
#include "bullet.h"
#include <QLineF>
#include <math.h>
#include<QBrush>
#include<QGraphicsScene>
#include "weapon.h"
#include <QDebug>
Player::Player()
{
    setRect(0,0,40,40);
    setBrush(Qt::blue);

    setFlag(QGraphicsRectItem::ItemIsFocusable);
    setFocus();

    damage = 50; // 初始手枪
}

void Player::keyPressEvent(QKeyEvent *event)
{
    int speed = 10;
    if(event->key()==Qt::Key_W) setPos(x(), y()-speed);
    if(event->key()==Qt::Key_S) setPos(x(), y()+speed);
    if(event->key()==Qt::Key_A) setPos(x()-speed, y());
    if(event->key()==Qt::Key_D) setPos(x()+speed, y());
    if(event->key() == Qt::Key_E)
    {
        QList<QGraphicsItem*> items = collidingItems();

        for(auto item : items)
        {
            Weapon* weapon = dynamic_cast<Weapon*>(item);

            if(weapon)
            {
                damage = weapon->getDamage();

                qDebug() << "拾取武器，伤害:" << damage;

                scene()->removeItem(weapon);
                delete weapon;
                break;
            }
        }
    }
}

void Player::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF target = event->scenePos();
    QLineF line(pos(), target);
    double angle = atan2(line.dy(), line.dx());

    Bullet *bullet = new Bullet(angle, damage);
    bullet->setPos(pos());
    scene()->addItem(bullet);
}