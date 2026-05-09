#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>

class Player;

class Enemy : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit Enemy(Player* player);
    int health;

signals:
    void enemyKilled();
    void dropWeapon(QPointF pos, int weaponType);


public slots:
    void move();

private:
    Player* playerRef;
    int attackCooldown;
};

#endif