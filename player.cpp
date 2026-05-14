#include "player.h"
#include "bullet.h"
#include <qmath.h>
#include<QBrush>
#include<QGraphicsScene>
#include<QPainter>
#include<QPixmap>
Player::Player(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    isFirstPerson = false;
    setVisible(true);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    health = 100;
    maxHealth = 100;
    // 固定尺寸 24*24 透明画布
    QPixmap pix(24, 24);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);
    // AI辅助编写
    // 1. 八路军军帽 灰色
    p.setBrush(QColor(120,125,130));
    p.setPen(Qt::black);
    p.drawEllipse(6, 1, 12, 6);

    // 2. 脸部肤色
    p.setBrush(QColor(235,215,190));
    p.drawEllipse(7, 3, 10, 10);

    // 3. 眼睛 简约线条
    p.setPen(Qt::black);
    p.drawLine(9,7,10,7);
    p.drawLine(14,7,15,7);

    // 4. 军绿色身体
    p.setBrush(QColor(30,70,40));
    p.drawRect(6, 12, 12, 9);

    // 5. 红领章
    p.setBrush(Qt::red);
    p.drawRect(7,12,3,2);
    p.drawRect(14,12,3,2);

    p.end();

    setPixmap(pix);
    setOffset(-12, -12); // 居中对齐，碰撞正常

    // 手枪
    damage[0] = 50;
    shootInterval[0] = 300;
    weaponName[0] = "手枪";

    // 步枪
    damage[1] = 80;
    shootInterval[1] = 90;
    weaponName[1] = "步枪";

    currentWeaponIndex = 0;
    isFiring = false;
    // AI辅助编写
    shootTimer = new QTimer(this);
    shootTimer->setInterval(shootInterval[currentWeaponIndex]);
    shootTimer->setTimerType(Qt::PreciseTimer);
    connect(shootTimer, &QTimer::timeout, this, &Player::shootLoop);

    // 血条
    healthBarBg = new QGraphicsRectItem(0, -15, 50, 6, this);
    healthBarBg->setBrush(Qt::darkGray);
    healthBar = new QGraphicsRectItem(0, -15, 50, 6, this);
    healthBar->setBrush(Qt::green);
}

void Player::keyPressEvent(QKeyEvent *event)
{
    int speed = 12;
    if (event->key() == Qt::Key_A)  setPos(x() - speed, y());
    if (event->key() == Qt::Key_D) setPos(x() + speed, y());
    if (event->key() == Qt::Key_W)    setPos(x(), y() - speed);
    if (event->key() == Qt::Key_S)  setPos(x(), y() + speed);

    // E 切换武器
    if (event->key() == Qt::Key_E) {
        currentWeaponIndex = 1 - currentWeaponIndex;
        shootTimer->setInterval(shootInterval[currentWeaponIndex]);
        emit weaponChanged(damage[currentWeaponIndex], weaponName[currentWeaponIndex]);
    }

}

void Player::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isFiring = true;
        mouseScenePos = event->scenePos();
        shootLoop();
        shootTimer->start();
    }
}

void Player::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    mouseScenePos = event->scenePos();
}

void Player::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isFiring = false;
        shootTimer->stop();
    }
}
// AI辅助编写
void Player::shootLoop()
{
    if (!isFiring) return;

    QLineF line(pos(), mouseScenePos);
    double angle = qAtan2(line.dy(), line.dx());

    Bullet* b = new Bullet(angle, damage[currentWeaponIndex], true);
    b->setPos(pos());
    scene()->addItem(b);
}

void Player::takeDamage(int dmg)
{
    health -= dmg;
    if (health < 0) health = 0;

    double ratio = (double)health / maxHealth;
    healthBar->setRect(0, -15, 50 * ratio, 6);

    healthBar->setRect(0, -15, 50 * ratio, 6);
    if (ratio > 0.6)
        healthBar->setBrush(Qt::green);
    else if (ratio > 0.3)
        healthBar->setBrush(Qt::yellow);
    else
        healthBar->setBrush(Qt::red);

    emit hpChanged(health, maxHealth);

    if (health <= 0) {
        qApp->quit();// AI辅助编写
    }
}