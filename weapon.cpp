#include "weapon.h"

// 1. 构造函数实现（修复"Redefinition of 'Weapon'"：确保和头文件声明一致）
Weapon::Weapon(WeaponType type) {
    this->type = type;

    // 设置武器碰撞盒/显示大小（继承QGraphicsRectItem后，setRect可用）
    setRect(0, 0, 20, 20);

    // 根据武器类型初始化属性
    if (type == Pistol) {
        damage = 50;
        fireRate = 300;
        ammo = 12;
        setBrush(Qt::yellow); // 手枪显示黄色
    } else if (type == Rifle) {
        damage = 100;
        fireRate = 150;
        ammo = 30;
        setBrush(Qt::red); // 步枪显示红色
    }
}

// 2. getDamage实现（修复"Out-of-line definition不匹配"：和头文件声明一致）
int Weapon::getDamage() const {
    return damage;
}

// 3. 补充其他成员函数实现（和头文件对应）
int Weapon::getFireRate() const {
    return fireRate;
}

int Weapon::getAmmo() const {
    return ammo;
}

void Weapon::reduceAmmo() {
    if (ammo > 0) {
        ammo--;
    }
}