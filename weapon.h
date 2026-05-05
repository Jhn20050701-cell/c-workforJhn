#ifndef WEAPON_H
#define WEAPON_H

// 必须包含Qt图形项头文件，才能用QGraphicsRectItem的方法
#include <QGraphicsRectItem>
// 包含QBrush头文件，用于设置画笔/画刷
#include <QBrush>

class Weapon : public QGraphicsRectItem { // 关键：继承QGraphicsRectItem（才能用setRect/setBrush）
public:
    // 1. 声明武器类型枚举（放在public，外部能访问）
    enum WeaponType { Pistol, Rifle };

    // 2. 声明构造函数（和cpp中实现匹配）
    Weapon(WeaponType type);

    // 3. 声明成员函数（和cpp中getDamage匹配）
    int getDamage() const;
    int getFireRate() const;
    int getAmmo() const;
    void reduceAmmo();

private:
    // 4. 声明私有成员变量（cpp中用到的damage/fireRate/ammo/type）
    WeaponType type;
    int damage;    // 武器伤害
    int fireRate;  // 射速（毫秒/发）
    int ammo;      // 弹药量
};

#endif // WEAPON_H