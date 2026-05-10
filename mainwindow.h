#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include "player.h"
#include "enemy.h"
#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void addScore();      // 你的击杀加分槽
    void spawnEnemy();    // 定时生成敌人的槽函数
    void spawnWeapon(QPointF pos, int type);
protected:

    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    Player *player;

    QTimer *enemySpawnTimer; // 敌人生成定时器
    int score;
    QGraphicsTextItem *scoreText;
    QGraphicsTextItem *weaponText;
    //左下血量显示
    QGraphicsRectItem *uiHpBg;
    QGraphicsRectItem *uiHpBar;
    QGraphicsTextItem *hpText;
};

#endif