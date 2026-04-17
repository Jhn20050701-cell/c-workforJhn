#ifndef WEAPON_H
#define WEAPON_H

#include <QGraphicsRectItem>

// 武器类型枚举
enum WeaponType
{
    Pistol,
    Rifle
};

class Weapon : public QGraphicsRectItem
{
public:
    Weapon(WeaponType type);

    int getDamage();
    int getFireRate();
    WeaponType getType();

private:
    int damage;
    int fireRate;
    WeaponType type;
};

#endif