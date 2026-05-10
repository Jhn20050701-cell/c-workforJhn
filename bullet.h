#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>

// 提前声明类，避免头文件循环包含
class Player;
class Enemy;
class Obstacle;

class Bullet : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Bullet(double angle, int dmg, bool isPlayerBullet = true); // 重载构造
    int getDamage() const { return damage; }
public slots:
    void move();
private:
    double angle;
    int damage;
    bool isPlayerBullet; // 新增：标记子弹归属
};

#endif