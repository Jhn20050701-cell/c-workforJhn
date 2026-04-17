#include "weapon.h"
#include <QBrush>

Weapon::Weapon(WeaponType type)
{
    this->type = type;

    setRect(0,0,20,20);

    // 根据武器类型设置属性
    if(type == Pistol)
    {
        damage = 50;
        fireRate = 300;
        setBrush(Qt::yellow);   // 手枪
    }
    else if(type == Rifle)
    {
        damage = 100;
        fireRate = 150;
        setBrush(Qt::red);      // 步枪
    }
}

int Weapon::getDamage()
{
    return damage;
}

int Weapon::getFireRate()
{
    return fireRate;
}

WeaponType Weapon::getType()
{
    return type;
}