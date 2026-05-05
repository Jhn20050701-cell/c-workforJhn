#include "player.h"
#include "weapon.h"
#include "bullet.h"
// 补充缺失的头文件
#include <QKeyEvent>
#include <QDateTime>
#include <QGraphicsTextItem>
#include <QGraphicsScene> // 修复QGraphicsScene不完整问题
#include <QDebug>
#include <QPainter>

// 构造函数：初始化玩家属性、UI
Player::Player()
{
    // 基础属性初始化
    health = 100;                  // 初始血量
    currentWeapon = nullptr;       // 初始无武器
    lastShootTime = 0;             // 上次射击时间（限制射速）

    // UI初始化 - 血量文本
    healthText = new QGraphicsTextItem();
    healthText->setDefaultTextColor(Qt::red);
    healthText->setPos(this->x() - 10, this->y() - 20);
    // 确保scene()有效再添加（避免空指针）
    if (this->scene()) {
        this->scene()->addItem(healthText);
    }

    // UI初始化 - 弹药文本
    ammoText = new QGraphicsTextItem();
    ammoText->setDefaultTextColor(Qt::white);
    ammoText->setPos(this->x() - 10, this->y() - 35);
    if (this->scene()) {
        this->scene()->addItem(ammoText);
    }

    // 更新初始UI
    updateUI();
}

// 帧更新函数（Qt场景每帧调用，处理移动/碰撞/UI更新）
void Player::advance(int phase)
{
    if (!phase) return; // Qt的phase机制，避免重复执行

    // 每帧检测是否拾取武器
    checkWeaponPickup();

    // 每帧更新UI（跟随玩家移动）
    updateUI();
}

// 检测武器拾取逻辑
void Player::checkWeaponPickup()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (auto item : colliding_items)
    {
        // 判断碰撞物是否是武器
        Weapon* weapon = dynamic_cast<Weapon*>(item);
        if (weapon)
        {
            // 如果已有武器，先释放旧武器内存
            if (currentWeapon != nullptr)
            {
                delete currentWeapon;
            }
            // 拾取新武器（统一调用getDamage，匹配Weapon类）
            currentWeapon = weapon;
            if (this->scene()) {
                scene()->removeItem(weapon); // 从场景移除（避免重复拾取）
            }
            qDebug() << "拾取武器 - 伤害：" << currentWeapon->getDamage()
                     << " 弹药：" << currentWeapon->getAmmo();
            break;
        }
    }
}

// 按键事件：处理射击（空格）、移动（可选扩展）
void Player::keyPressEvent(QKeyEvent *event)
{
    // 空格键射击
    if (event->key() == Qt::Key_Space)
    {
        shoot();
    }

    // 可选：添加玩家移动逻辑（示例：WASD移动）
    if (event->key() == Qt::Key_W)
    {
        setPos(x(), y() - 5);
    }
    if (event->key() == Qt::Key_S)
    {
        setPos(x(), y() + 5);
    }
    if (event->key() == Qt::Key_A)
    {
        setPos(x() - 5, y());
    }
    if (event->key() == Qt::Key_D)
    {
        setPos(x() + 5, y());
    }
}

// 射击逻辑（封装成独立函数，方便调用）
void Player::shoot()
{
    // 判读：有武器 + 有弹药 + 满足射速限制
    if (currentWeapon == nullptr)
    {
        qDebug() << "无武器，无法射击！";
        return;
    }
    if (currentWeapon->getAmmo() <= 0)
    {
        qDebug() << "弹药耗尽，无法射击！";
        return;
    }

    // 射速限制（根据武器fireRate）
    long long currentTime = QDateTime::currentMSecsSinceEpoch(); // 改用long long
    if (currentTime - lastShootTime < currentWeapon->getFireRate())
    {
        return; // 未到射击间隔，禁止射击
    }

    // 创建子弹（确保Bullet有无参构造，若Bullet需要参数则调整）
    Bullet* bullet = new Bullet();
    // 设置子弹位置（玩家位置偏移，避免打到自己）
    bullet->setPos(this->x() + 20, this->y());
    // 设置子弹伤害（调用getDamage，匹配Weapon类）
    bullet->setDamage(currentWeapon->getDamage());
    // 扣减弹药
    currentWeapon->reduceAmmo();
    // 添加子弹到场景（先判空）
    if (this->scene()) {
        scene()->addItem(bullet);
    }

    // 更新上次射击时间
    lastShootTime = currentTime;
    qDebug() << "射击成功！剩余弹药：" << currentWeapon->getAmmo();
}

// 玩家受攻击减血
void Player::reduceHealth(int damage)
{
    health -= damage;
    if (health < 0) health = 0; // 防止血量为负
    qDebug() << "玩家受攻击！剩余血量：" << health;

    // 可选：玩家死亡逻辑
    if (health <= 0)
    {
        qDebug() << "玩家死亡！";
        if (this->scene()) {
            scene()->removeItem(this);
        }
        delete this;
    }
}

// 更新UI（血量、弹药跟随玩家）
void Player::updateUI()
{
    // 更新血量文本
    healthText->setPlainText(QString("血量：%1").arg(health));
    // 更新弹药文本
    if (currentWeapon != nullptr)
    {
        ammoText->setPlainText(QString("弹药：%1").arg(currentWeapon->getAmmo()));
    }
    else
    {
        ammoText->setPlainText("无武器");
    }
    // UI跟随玩家移动
    healthText->setPos(this->x() - 10, this->y() - 20);
    ammoText->setPos(this->x() - 10, this->y() - 35);
}

// 析构函数：释放内存（避免内存泄漏）
Player::~Player()
{
    // 释放武器
    if (currentWeapon != nullptr)
    {
        delete currentWeapon;
    }
    // 释放UI文本
    delete healthText;
    delete ammoText;
}

// 必须实现的纯虚函数（QGraphicsItem要求）
QRectF Player::boundingRect() const
{
    // 玩家碰撞盒（根据你的精灵大小调整，示例：40x40）
    return QRectF(0, 0, 40, 40);
}

// 必须实现的纯虚函数（QGraphicsItem要求）
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // 绘制玩家（示例：蓝色矩形，你可以替换成精灵图片）
    painter->setBrush(Qt::blue);
    painter->drawRect(boundingRect());
}