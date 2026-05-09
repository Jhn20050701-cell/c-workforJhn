#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

class Player : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit Player(QGraphicsItem *parent = nullptr);
    void takeDamage(int dmg);

    int damage;
    int shootInterval;

signals:
    void weaponChanged(int dmg, QString name);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void shootLoop();

private:
    bool isFiring;
    QTimer* shootTimer;
    QPointF mouseScenePos;


    int health;
    int maxHealth;
    bool isJumping;

    QGraphicsRectItem *healthBarBg;
    QGraphicsRectItem *healthBar;
};

#endif