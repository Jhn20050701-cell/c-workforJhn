#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>

class Bullet : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    Bullet(double angle);
public slots:
    void move();
private:
    double angle; // 弧度
};

#endif