#include "weapon.h"
#include <QBrush>

Weapon::Weapon(int type, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setRect(0,0,20,20);

    if(type == 0){
        // 手枪
        damage = 50;
        speed = 300;
        name = "手枪";
        setBrush(Qt::yellow);
    }else{
        // 步枪 → 射速超快
        damage = 70;
        speed = 80;
        name = "步枪";
        setBrush(Qt::cyan);
    }
}

int Weapon::getDamage() const { return damage; }
QString Weapon::getName() const { return name; }
int Weapon::getSpeed() const { return speed; }