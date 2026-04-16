#include "mainwindow.h"
#include "player.h"
#include "enemy.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,800,600);

    view = new QGraphicsView(scene,this);
    setCentralWidget(view);
    view->setFixedSize(800,600);

    Player* player = new Player();
    player->setPos(400,300);
    scene->addItem(player);

    Enemy* enemy1 = new Enemy(player);
    enemy1->setPos(200,200);
    scene->addItem(enemy1);

    Enemy* enemy2 = new Enemy(player);
    enemy2->setPos(600,400);
    scene->addItem(enemy2);
}

MainWindow::~MainWindow() {}