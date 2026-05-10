#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include"player.h"
#include<QGraphicsItem>
class Player;

class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
   explicit Enemy(Player* player, QGraphicsItem *parent = nullptr);
    int health;

signals:
    void enemyKilled();

private slots:
    void move();

private:
    Player* playerRef;
    int attackCooldown;
};

#endif