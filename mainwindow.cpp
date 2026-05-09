#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator> // 用于生成随机位置
#include <QMessageBox>
#include"obstacle.h"
#include"weapon.h"
#include"player.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,800,600);

    view = new QGraphicsView(scene,this);
    setCentralWidget(view);
    view->setFixedSize(800,600);

    view->setFocusPolicy(Qt::StrongFocus);
    view->setFocus();
    // 创建玩家
    player = new Player();
    player->setPos(400,300);
    scene->addItem(player);
    player->setFocus();

    // 武器文本（你的原有代码）
    weaponText = scene->addText("武器: 手枪 伤害:50");
    weaponText->setDefaultTextColor(Qt::white);
    weaponText->setPos(10, 25);
    connect(player, &Player::weaponChanged, this, [=](int dmg, QString name){
        weaponText->setPlainText(QString("武器: %1 伤害:%2").arg(name).arg(dmg));
    });

    // 分数文本（你的原有代码）
    score = 0;
    scoreText = scene->addText("击杀: 0");
    scoreText->setDefaultTextColor(Qt::yellow);
    scoreText->setPos(10, 45);
    // 敌人自动生成定时器
    enemySpawnTimer = new QTimer(this);
    // 每3秒生成一个敌人（3000毫秒，可根据需要调整）
    enemySpawnTimer->start(3000);
    connect(enemySpawnTimer, &QTimer::timeout, this, &MainWindow::spawnEnemy);

    // 生成初始掩体（你原来的代码）
    Obstacle* wall1 = new Obstacle(300, 200, 100, 40);
    scene->addItem(wall1);
    Obstacle* wall2 = new Obstacle(500, 350, 120, 40);
    scene->addItem(wall2);
    Obstacle* wall3 = new Obstacle(400, 150, 80, 40);
    scene->addItem(wall3);
    Obstacle* wall4 = new Obstacle(600, 250, 140, 40);
    scene->addItem(wall4);

    view->setStyleSheet("background-color:#2f4f2f;");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 你的原有加分槽函数
void MainWindow::addScore()
{
    score++;
    scoreText->setPlainText(QString("击杀: %1").arg(score));
}

// 新增：定时生成敌人的函数
void MainWindow::spawnEnemy()
{
    // 随机生成敌人位置（在场景边缘，避免直接刷在玩家脸上）
    int side = QRandomGenerator::global()->bounded(4); // 0-3，四个方向
    int x, y;
    switch(side)
    {
    case 0: // 上边
        x = QRandomGenerator::global()->bounded(800);
        y = 0;
        break;
    case 1: // 下边
        x = QRandomGenerator::global()->bounded(800);
        y = 600;
        break;
    case 2: // 左边
        x = 0;
        y = QRandomGenerator::global()->bounded(600);
        break;
    case 3: // 右边
        x = 800;
        y = QRandomGenerator::global()->bounded(600);
        break;
    default:
        x = 0; y = 0;
    }

    // 创建敌人
    Enemy* enemy = new Enemy(player);
    enemy->setPos(x, y);
    scene->addItem(enemy);

    // 连接击杀信号到加分槽
    connect(enemy, &Enemy::enemyKilled, this, &MainWindow::addScore);
}
void MainWindow::spawnWeapon(QPointF pos, int type)
{
    Weapon* weapon = new Weapon(type);
    weapon->setPos(pos);
    scene->addItem(weapon);
}