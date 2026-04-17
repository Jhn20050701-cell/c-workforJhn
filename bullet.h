#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>

class Bullet : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    Bullet(double angle,int dmg);
    // 获取子弹伤害值的接口
    int getDamage() const {
        return damage;
    }
public slots:
    void move();
private:
    double angle; // 弧度
    int damage;//伤害
};

#endif