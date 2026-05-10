#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator> // 用于生成随机位置
#include <QMessageBox>
#include"obstacle.h"
#include"weapon.h"
#include"player.h"
#include<QBrush>
#include<QPen>
#include <QPixmap>
#include <QGraphicsPixmapItem>
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
    setCentralWidget(view);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(view);




    // 2. 添加森林背景
    QPixmap bgPix("forest_bg.jpg");
    bgPix = bgPix.scaled(800, 600, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QGraphicsPixmapItem *bgItem = new QGraphicsPixmapItem(bgPix);
    bgItem->setZValue(-100);
    scene->addItem(bgItem);

    view->setFocusPolicy(Qt::StrongFocus);
    view->setFocus();
    // 创建玩家
    player = new Player();
    player->setPos(400,300);
    scene->addItem(player);
    player->setFocus();
    connect(player, &Player::hpChanged, this, [=](int nowHp, int maxHp)
            {
                double ratio = static_cast<double>(nowHp) / maxHp;

                // 更新血条宽度
                uiHpBar->setRect(15, 520, 180 * ratio, 22);

                // 更新血条颜色
                if (ratio > 0.6)
                    uiHpBar->setBrush(Qt::green);
                else if (ratio > 0.3)
                    uiHpBar->setBrush(Qt::yellow);
                else
                    uiHpBar->setBrush(Qt::red);

                // 更新百分比文字
                hpText->setPlainText(QString("HP: %1%").arg(static_cast<int>(ratio * 100)));
            });




    // 敌人自动生成定时器
    enemySpawnTimer = new QTimer(this);
    // 每3秒生成一个敌人（3000毫秒，可根据需要调整）
    enemySpawnTimer->start(3000);
    connect(enemySpawnTimer, &QTimer::timeout, this, &MainWindow::spawnEnemy);

    // 生成初始掩体（你原来的代码）
    scene->addItem(new Obstacle(110, 160, 70, 70));  // 左上大岩石
    scene->addItem(new Obstacle(200, 220, 50, 50));  // 左上小岩石
    scene->addItem(new Obstacle(220, 470, 60, 40));  // 左下枯树
    scene->addItem(new Obstacle(360, 330, 80, 50));  // 中间土堆
    scene->addItem(new Obstacle(490, 190, 70, 70));  // 右上大岩石
    scene->addItem(new Obstacle(630, 460, 60, 60));  // 右下树桩
    scene->addItem(new Obstacle(710, 260, 50, 40));  // 右侧河岸岩石
    scene->addItem(new Obstacle(320, 100, 40, 40));  // 上方小岩石
    scene->addItem(new Obstacle(570, 370, 50, 50));  // 右侧高地掩体
    scene->addItem(new Obstacle(160, 400, 40, 60));  // 左侧枯树

    // 血条背景
    uiHpBg = scene->addRect(15, 520, 180, 22);
    uiHpBg->setBrush(Qt::darkGray);
    uiHpBg->setPen(QPen(Qt::white)); // 这里要包成 QPen

    // 血量条
    uiHpBar = scene->addRect(15, 520, 180, 22);
    uiHpBar->setBrush(Qt::green);
    uiHpBar->setPen(Qt::NoPen); // 这行是对的，Qt::NoPen 是特殊值

    // 血量文字
    hpText = scene->addText("HP: 100%");
    hpText->setPos(25, 522);
    hpText->setDefaultTextColor(Qt::white);

    // 分数文字美化
    scoreText = scene->addText("击杀: 0");
    scoreText->setDefaultTextColor(Qt::yellow);
    scoreText->setPos(15,15);
    // 字体变大加粗
    QFont f1 = scoreText->font();
    f1.setPointSize(12);
    f1.setBold(true);
    scoreText->setFont(f1);

    // 武器文字美化
    weaponText = scene->addText("武器: 手枪 伤害:50");
    weaponText->setDefaultTextColor(Qt::yellow);
    weaponText->setPos(15,45);
    QFont f2 = weaponText->font();
    f2.setPointSize(11);
    f2.setBold(true);
    weaponText->setFont(f2);
    connect(player, &Player::weaponChanged, this, [=](int dmg, QString name){
        weaponText->setPlainText(QString("武器: %1 伤害:%2").arg(name).arg(dmg));
    });
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
    Weapon* w = new Weapon(type);
    w->setPos(pos);
    scene->addItem(w);
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close(); // 关闭窗口，退出全屏
    }
    QMainWindow::keyPressEvent(event);
}
