#ifndef PLAYER_H
#define PLAYER_H

// 替换qint64.h为QtGlobal（或直接用long long）
#include <QtGlobal>
#include <QGraphicsItem>
#include <QGraphicsTextItem>

// 前置声明（避免循环包含）
class Weapon;
class Bullet;

class Player : public QGraphicsItem
{
private:
    int health;                  // 玩家血量
    Weapon* currentWeapon;       // 当前持有的武器
    long long lastShootTime;     // 改用long long替代qint64，避免头文件问题
    QGraphicsTextItem* healthText; // 血量UI文本
    QGraphicsTextItem* ammoText;   // 弹药UI文本

public:
    Player();                    // 构造函数
    ~Player();                   // 析构函数

    // 核心功能函数声明（补全checkWeaponPickup）
    void checkWeaponPickup();    // 检测武器拾取
    void shoot();                // 射击逻辑
    void reduceHealth(int damage); // 受攻击减血
    void updateUI();             // 更新UI

    // Qt重写函数
    void advance(int phase) override;
    void keyPressEvent(QKeyEvent *event) override;

    // QGraphicsItem纯虚函数实现
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // PLAYER_H