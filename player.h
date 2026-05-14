#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Player(QGraphicsItem *parent = nullptr);
    void takeDamage(int dmg);


    int damage[2];
    int shootInterval[2];
    QString weaponName[2];
    int currentWeaponIndex;
    bool isFirstPerson;

signals:
    void weaponChanged(int dmg, QString name);
    void hpChanged(int nowHp, int maxHp);



protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void shootLoop();// AI辅助编写

private:
    bool isFiring;
    QTimer *shootTimer;
    QPointF mouseScenePos;

    int health;
    int maxHealth;
    QGraphicsRectItem *healthBarBg;
    QGraphicsRectItem *healthBar;
};

#endif