#include "weapon.h"
#include <QBrush>

Weapon::Weapon(int type, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setRect(0, 0, 25, 25); // 稍微大一点，更明显

    if (type == 0) {
        // 手枪（普通）
        damage = 50;
        speed = 300;
        name = "手枪";
        setBrush(Qt::yellow);
    } else {
        // 步枪（高级 → 金色！）
        damage = 80;
        speed = 90;   // 超快射速
        name = "步枪";
        setBrush(QColor(255, 215, 0)); // 金色！高级武器颜色
    }
}

int Weapon::getDamage() const { return damage; }
QString Weapon::getName() const { return name; }
int Weapon::getSpeed() const { return speed; }