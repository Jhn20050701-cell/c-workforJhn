#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

class Player : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Player();
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    int damage;
};

#endif