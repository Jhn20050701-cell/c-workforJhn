#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>

class Enemy : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Enemy(QGraphicsRectItem* player);
    int health;
public slots:
    void move();
private:
    QGraphicsRectItem* playerRef;
};

#endif