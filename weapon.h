#ifndef WEAPON_H
#define WEAPON_H

#include <QGraphicsRectItem>
#include <QString>

class Weapon : public QGraphicsRectItem
{
public:
    explicit Weapon(int type, QGraphicsItem *parent = nullptr);
    int getDamage() const;
    QString getName() const;
    int getSpeed() const;

private:
    int damage;
    int speed;
    QString name;
};

#endif