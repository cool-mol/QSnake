#include <QDebug>
#include "gamewidget.h"
#include "widget.h"

eng gameWidget::engine = eng(time(NULL));

Food::Food(FoodType t, std::vector<Snake *> snakeList, uni uniform) : type(t) {
    // snakeList为空
    if (!snakeList.size()) {
        qDebug() << "Food构造函数参数为空";
        QPoint point =
            QPoint(uniform(gameWidget::engine), uniform(gameWidget::engine));
        pos.push_back(point);
        // qDebug()<<pos.x()<<pos.y();
        return;
    }

    bool flag = 1;

    // 防止食物生成在蛇身上
    while (flag) {
        flag = 0;

        pos[0] =
            QPoint(uniform(gameWidget::engine), uniform(gameWidget::engine));

        for (auto i : snakeList) {
            if (i->insideSnake(pos[0])) {
                flag = 1;
                break;
            }
        }
    }
}

std::vector<QPoint> Food::getPos() const { return pos; }

void Food::update(int level) {
    uni uniform(2, posMax - 1);  // uniform(gameWidget::engine)
    if (type == normalFood) {
        pos[0].setX(uniform(gameWidget::engine));
        pos[0].setY(uniform(gameWidget::engine));
    }
    if (type == yellowFood) {
        for (int i = pos.size(); i <= level; i++) {
            pos.push_back(QPoint(uniform(gameWidget::engine),
                                 uniform(gameWidget::engine)));
        }
        pos[0].setX(uniform(gameWidget::engine));
        pos[0].setY(uniform(gameWidget::engine));
    }
}

//🐍类的构造函数，把头和尾随机生成放进去
Snake::Snake() : length(2), score(0), level(1) {
    uni uniform(2, posMax - 1);

    QPoint head(uniform(gameWidget::engine), uniform(gameWidget::engine)), tail;
    Direction dir = (Direction)((uniform(gameWidget::engine) % 4) + 1);
    if (dir == Up) {
        tail = QPoint(head.x(), head.y() + 1);
    } else if (dir == Down) {
        tail = QPoint(head.x(), head.y() - 1);
    } else if (dir == Left) {
        tail = QPoint(head.x() + 1, head.y());
    } else if (dir == Right) {
        tail = QPoint(head.x() - 1, head.y());
    }
    dirKey = dir;
    points.push_back(snakeNode(head, dir));
    points.push_back(snakeNode(tail, dir));
    // qDebug() << head.x()<<head.y()<<dir;
}
// 修改蛇的方向
void Snake::changeDirect(Direction d) {
    snakeNode tmp(points[0].pos, d);
    if (d == Up && points[0].facePos != Down && points[0].facePos != Up) {
        std::vector<snakeNode>::iterator theIterator = points.begin();
        points[0].facePos = Up;
        points.insert(theIterator + 1, tmp);
    }
    if (d == Down && points[0].facePos != Up && points[0].facePos != Down) {
        std::vector<snakeNode>::iterator theIterator = points.begin();
        points[0].facePos = Down;
        points.insert(theIterator + 1, tmp);
    }
    if (d == Left && points[0].facePos != Right && points[0].facePos != Left) {
        std::vector<snakeNode>::iterator theIterator = points.begin();
        points[0].facePos = Left;
        points.insert(theIterator + 1, tmp);
    }
    if (d == Right && points[0].facePos != Left && points[0].facePos != Right) {
        std::vector<snakeNode>::iterator theIterator = points.begin();
        points[0].facePos = Right;
        points.insert(theIterator + 1, tmp);
    }
}

//当吃到毒苹果的时候，调用这个函数来改变方向
void Snake::yellowDirect(Direction d) {
    snakeNode tmp(points[0].pos, d);

    if (d == Up && points[0].facePos != Down && points[0].facePos != Up) {
        std::vector<snakeNode>::iterator theIterator = points.begin();
        points[0].facePos = Down;
        tmp.facePos = Down;
        points.insert(theIterator + 1, tmp);
    }
    if (d == Down && points[0].facePos != Up && points[0].facePos != Down) {
        std::vector<snakeNode>::iterator theIterator = points.begin();
        points[0].facePos = Up;
        tmp.facePos = Up;
        points.insert(theIterator + 1, tmp);
    }
    if (d == Left && points[0].facePos != Right && points[0].facePos != Left) {
        std::vector<snakeNode>::iterator theIterator = points.begin();
        points[0].facePos = Right;
        tmp.facePos = Right;
        points.insert(theIterator + 1, tmp);
    }
    if (d == Right && points[0].facePos != Left && points[0].facePos != Right) {
        std::vector<snakeNode>::iterator theIterator = points.begin();
        points[0].facePos = Left;
        tmp.facePos = Left;
        points.insert(theIterator + 1, tmp);
    }
}

//判断果子是否在蛇内
bool Snake::insideSnake(QPoint p) {
    // bool flag = true;
    for (int i = 0; i < points.size() - 1; i++) {
        if (points[i].pos.x() == points[i + 1].pos.x() &&
            points[i].pos.x() == p.x()) {
            if (points[i].pos.y() >= p.y() && points[i + 1].pos.y() <= p.y())
                return true;
            if (points[i].pos.y() <= p.y() && points[i + 1].pos.y() >= p.y())
                return true;
        }
        if (points[i].pos.y() == points[i + 1].pos.y() &&
            points[i].pos.y() == p.y()) {
            if (points[i].pos.x() >= p.x() && points[i + 1].pos.x() <= p.x())
                return true;
            if (points[i].pos.y() <= p.y() && points[i + 1].pos.y() >= p.y())
                return true;
        }
    }

    return false;
}

// 维护头和尾巴移动，并检查最后一个拐点是否需要删去
void Snake::update(int is_eatFood) {
    auto Head = points.begin(), Tail = points.end() - 1,
         BeforeTail = points.end() - 2;

    switch (Head->facePos) {
        case Up:
            Head->pos.setY(Head->pos.y() - 1);
            break;
        case Down:
            Head->pos.setY(Head->pos.y() + 1);
            break;
        case Left:
            Head->pos.setX(Head->pos.x() - 1);
            break;
        case Right:
            Head->pos.setX(Head->pos.x() + 1);
            break;
        default:
            break;
    }

    if (is_eatFood == 0) {
        switch (Tail->facePos) {
            case Up:
                if (Tail->pos.y() == BeforeTail->pos.y() + 1)
                    points.pop_back();
                else {
                    Tail->pos.setY(Tail->pos.y() - 1);
                }
                break;
            case Down:
                if (Tail->pos.y() == BeforeTail->pos.y() - 1)
                    points.pop_back();
                else {
                    Tail->pos.setY(Tail->pos.y() + 1);
                }
                break;
            case Left:
                if (Tail->pos.x() == BeforeTail->pos.x() + 1)
                    points.pop_back();
                else {
                    Tail->pos.setX(Tail->pos.x() - 1);
                }
                break;
            case Right:
                if (Tail->pos.x() == BeforeTail->pos.x() - 1)
                    points.pop_back();
                else {
                    Tail->pos.setX(Tail->pos.x() + 1);
                }
                break;
            default:
                break;
        }
    } else {
        length++;
    }
    //        for(int i = 0;i < points.size();i ++){
    //            qDebug() << points[i].pos.x()
    //            <<points[i].pos.y()<<points[i].facePos;
    //        }
    //        qDebug() << "fas";
}

void Snake::keyCatch(Direction d) { dirKey = d; }

gameWidget::gameWidget(QWidget *parent) : QWidget(parent) {
    this->setWindowTitle("贪吃蛇");
    this->setGeometry(300, 150, 900, 660);

    //设置背景 todo
    this->setAutoFillBackground(true);  //设置父窗口背景可被覆盖填充
    this->setWindowIcon(QIcon(":/new/Picture/img/Apple.png"));
    QPalette palette;
    palette.setBrush(
        QPalette::Background,
        QBrush(QPixmap(":/new/Picture/img/gameBack.png").scaled(this->size())));
    this->setPalette(palette);

    upButton = new QPushButton(this);
    upButton->setGeometry(750, 100, 50, 50);
    upButton->setText("上");

    downButton = new QPushButton(this);
    downButton->setGeometry(750, 200, 50, 50);
    downButton->setText("下");

    leftButton = new QPushButton(this);
    leftButton->setGeometry(700, 150, 50, 50);
    leftButton->setText("左");

    rightButton = new QPushButton(this);
    rightButton->setGeometry(800, 150, 50, 50);
    rightButton->setText("右");

    startButton = new QPushButton(this);
    startButton->setGeometry(725, 300, 100, 50);
    startButton->setText("普通模式");

    continueButton = new QPushButton(this);
    continueButton->setGeometry(725, 300, 100, 50);
    continueButton->setText("继续游戏");
    continueButton->hide();

    doubleButton = new QPushButton(this);
    doubleButton->setGeometry(725, 350, 100, 50);
    doubleButton->setText("双人模式");

    crazyButton = new QPushButton(this);
    crazyButton->setGeometry(725, 400, 100, 50);
    crazyButton->setText("疯狂模式");

    exitButton = new QPushButton(this);
    exitButton->setGeometry(725, 450, 100, 50);
    exitButton->setText("结束游戏");

    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(upButton, 1);
    buttonGroup->addButton(downButton, 2);
    buttonGroup->addButton(leftButton, 3);
    buttonGroup->addButton(rightButton, 4);

    scoreLabel = new QLabel(this);
    scoreLabel->setText("分数：");
    scoreLabel->setGeometry(725, 500, 50, 20);

    levelLabel = new QLabel(this);
    levelLabel->setText("等级：");
    levelLabel->setGeometry(725, 525, 50, 20);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startPush()));
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(exitPush()));
    connect(doubleButton, SIGNAL(clicked(bool)), this, SLOT(doublePush()));
    connect(crazyButton, SIGNAL(clicked(bool)), this, SLOT(crazyPush()));

    this->repaint();
}

void gameWidget::keyPressEvent(QKeyEvent *e) {
    if (snakeList.size() == 1) {
        switch (e->key()) {
            case Qt::Key_W:
                snakeList[0]->keyCatch(Up);
                break;
            case Qt::Key_S:
                snakeList[0]->keyCatch(Down);
                break;
            case Qt::Key_A:
                snakeList[0]->keyCatch(Left);
                break;
            case Qt::Key_D:
                snakeList[0]->keyCatch(Right);
                break;
            case Qt::Key_Up:
                snakeList[0]->keyCatch(Up);
                break;
            case Qt::Key_Down:
                snakeList[0]->keyCatch(Down);
                break;
            case Qt::Key_Left:
                snakeList[0]->keyCatch(Left);
                break;
            case Qt::Key_Right:
                snakeList[0]->keyCatch(Right);
                break;
            default:
                break;
        }
    }
    if (snakeList.size() == 2) {
        switch (e->key()) {
            case Qt::Key_W:
                snakeList[0]->keyCatch(Up);
                break;
            case Qt::Key_S:
                snakeList[0]->keyCatch(Down);
                break;
            case Qt::Key_A:
                snakeList[0]->keyCatch(Left);
                break;
            case Qt::Key_D:
                snakeList[0]->keyCatch(Right);
                break;
            case Qt::Key_Up:
                snakeList[1]->keyCatch(Up);
                break;
            case Qt::Key_Down:
                snakeList[1]->keyCatch(Down);
                break;
            case Qt::Key_Left:
                snakeList[1]->keyCatch(Left);
                break;
            case Qt::Key_Right:
                snakeList[1]->keyCatch(Right);
                break;
            default:
                break;
        }
    }
    return;
}

//绘画函数
void gameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    //初始化界面
    for (int i = 0; i < 21; i++) {
        painter.drawLine(30 * i + 30, 30, 30 * i + 30, 630);
        painter.drawLine(30, 30 * i + 30, 630, 30 * i + 30);
    }
    painter.setBrush(QColor(187, 229, 253));

    painter.setPen(QPen(QColor(11, 67, 127), 5));
    painter.drawLine(30, 30, 30, 630);
    painter.drawLine(630, 30, 630, 630);
    painter.drawLine(30, 30, 630, 30);
    painter.drawLine(30, 630, 630, 630);

    //画蛇

    if (snakeList.size() == 1 || snakeList.size() == 2) {
        std::vector<snakeNode> node1 = snakeList[0]->getPoints();
        std::vector<QPoint> foodPoint1 = foodList[0]->getPos();
        //第二个黄苹果
        if (foodList.size() == 2) {
            std::vector<QPoint> foodPoint2 = foodList[1]->getPos();
            for (int i = 0; i < foodPoint2.size(); i++) {
                painter.drawImage(QRect(foodPoint2[i].x() * 30,
                                        foodPoint2[i].y() * 30, 30, 30),
                                  QImage(":/new/Picture/img/yellowApple.png"));
            }
        }
        painter.drawImage(
            QRect(foodPoint1[0].x() * 30, foodPoint1[0].y() * 30, 30, 30),
            QImage(":/new/Picture/img/Apple.png"));
        for (int i = 0; i < node1.size(); i++) {
            //头
            if (i == 0) {
                qDebug() << node1[0].pos.x() * 30 << node1[0].pos.y() * 30
                         << node1[1].pos.x() * 30 << node1[1].pos.y() * 30;
                if (snakeList[0]->getDir() == Up)
                    painter.drawImage(QRect(node1[0].pos.x() * 30,
                                            node1[0].pos.y() * 30, 30, 30),
                                      QImage(":/new/Picture/img/headUp.png"));
                if (snakeList[0]->getDir() == Down)
                    painter.drawImage(QRect(node1[0].pos.x() * 30,
                                            node1[0].pos.y() * 30, 30, 30),
                                      QImage(":/new/Picture/img/headDown.png"));
                if (snakeList[0]->getDir() == Left)
                    painter.drawImage(QRect(node1[0].pos.x() * 30,
                                            node1[0].pos.y() * 30, 30, 30),
                                      QImage(":/new/Picture/img/headLeft.png"));
                if (snakeList[0]->getDir() == Right)
                    painter.drawImage(
                        QRect(node1[0].pos.x() * 30, node1[0].pos.y() * 30, 30,
                              30),
                        QImage(":/new/Picture/img/headRight.png"));
            }
            //尾巴
            if (i == node1.size() - 1) {
                if (node1[node1.size() - 1].facePos == Up)
                    painter.drawImage(
                        QRect(node1[node1.size() - 1].pos.x() * 30,
                              node1[node1.size() - 1].pos.y() * 30, 30, 30),
                        QImage(":/new/Picture/img/tailup.png"));
                if (node1[node1.size() - 1].facePos == Down)
                    painter.drawImage(
                        QRect(node1[node1.size() - 1].pos.x() * 30,
                              node1[node1.size() - 1].pos.y() * 30, 30, 30),
                        QImage(":/new/Picture/img/taildown.png"));
                if (node1[node1.size() - 1].facePos == Left)
                    painter.drawImage(
                        QRect(node1[node1.size() - 1].pos.x() * 30,
                              node1[node1.size() - 1].pos.y() * 30, 30, 30),
                        QImage(":/new/Picture/img/tailleft.png"));
                if (node1[node1.size() - 1].facePos == Right)
                    painter.drawImage(
                        QRect(node1[node1.size() - 1].pos.x() * 30,
                              node1[node1.size() - 1].pos.y() * 30, 30, 30),
                        QImage(":/new/Picture/img/tailright.png"));
            }
            //身子
            // qDebug()<<i<<node1[0].facePos<<node1[1].facePos;
            if (node1[i].facePos == Up && i > 0) {
                //

                for (int j = node1[i].pos.y() - 1; j >= node1[i - 1].pos.y();
                     j--) {
                    if (j == node1[i - 1].pos.y()) {
                        if (node1[i - 1].facePos == Left)
                            painter.drawImage(
                                QRect(node1[i - 1].pos.x() * 30,
                                      node1[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/leftdown.png"));
                        if (node1[i - 1].facePos == Right)
                            painter.drawImage(
                                QRect(node1[i - 1].pos.x() * 30,
                                      node1[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/rightdown.png"));
                    } else {
                        //
                        painter.drawImage(
                            QRect(node1[i].pos.x() * 30, j * 30, 30, 30),
                            QImage(":/new/Picture/img/downup.png"));
                    }
                }
            }
            if (node1[i].facePos == Down && i > 0) {
                for (int j = node1[i].pos.y() + 1; j <= node1[i - 1].pos.y();
                     j++) {
                    if (j == node1[i - 1].pos.y()) {
                        if (node1[i - 1].facePos == Left)
                            painter.drawImage(
                                QRect(node1[i - 1].pos.x() * 30,
                                      node1[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/leftup.png"));
                        if (node1[i - 1].facePos == Right)
                            painter.drawImage(
                                QRect(node1[i - 1].pos.x() * 30,
                                      node1[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/rightup.png"));
                    } else {
                        painter.drawImage(
                            QRect(node1[i].pos.x() * 30, j * 30, 30, 30),
                            QImage(":/new/Picture/img/downup.png"));
                    }
                }
            }
            if (node1[i].facePos == Left && i > 0) {
                for (int j = node1[i].pos.x() - 1; j >= node1[i - 1].pos.x();
                     j--) {
                    if (j == node1[i - 1].pos.x()) {
                        if (node1[i - 1].facePos == Up)
                            painter.drawImage(
                                QRect(node1[i - 1].pos.x() * 30,
                                      node1[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/rightup.png"));
                        if (node1[i - 1].facePos == Down)
                            painter.drawImage(
                                QRect(node1[i - 1].pos.x() * 30,
                                      node1[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/rightdown.png"));
                    } else {
                        painter.drawImage(
                            QRect(j * 30, node1[i].pos.y() * 30, 30, 30),
                            QImage(":/new/Picture/img/leftright.png"));
                    }
                }
            }
            if (node1[i].facePos == Right && i > 0) {
                for (int j = node1[i].pos.x() + 1; j <= node1[i - 1].pos.x();
                     j++) {
                    if (j == node1[i - 1].pos.x()) {
                        if (node1[i - 1].facePos == Up)
                            painter.drawImage(
                                QRect(node1[i - 1].pos.x() * 30,
                                      node1[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/leftup.png"));
                        if (node1[i - 1].facePos == Down)
                            painter.drawImage(
                                QRect(node1[i - 1].pos.x() * 30,
                                      node1[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/leftdown.png"));
                    } else {
                        painter.drawImage(
                            QRect(j * 30, node1[i].pos.y() * 30, 30, 30),
                            QImage(":/new/Picture/img/leftright.png"));
                    }
                }
            }
        }
    }
    //两条蛇
    if (snakeList.size() == 2) {
        std::vector<snakeNode> node2 = snakeList[1]->getPoints();
        //        QPoint foodPoint1 = foodList[0]->getPos();
        //        painter.drawImage(
        //            QRect(foodPoint1.x() * 30, foodPoint1.y() * 30, 30, 30),
        //            QImage(":/new/Picture/img/Apple.png"));
        for (int i = 0; i < node2.size(); i++) {
            //头
            if (i == 0) {
                qDebug() << node2[0].pos.x() * 30 << node2[0].pos.y() * 30
                         << node2[1].pos.x() * 30 << node2[1].pos.y() * 30;
                if (snakeList[1]->getDir() == Up)
                    painter.drawImage(QRect(node2[0].pos.x() * 30,
                                            node2[0].pos.y() * 30, 30, 30),
                                      QImage(":/new/Picture/img/headup2.png"));
                if (snakeList[1]->getDir() == Down)
                    painter.drawImage(
                        QRect(node2[0].pos.x() * 30, node2[0].pos.y() * 30, 30,
                              30),
                        QImage(":/new/Picture/img/headdown2.png"));
                if (snakeList[1]->getDir() == Left)
                    painter.drawImage(
                        QRect(node2[0].pos.x() * 30, node2[0].pos.y() * 30, 30,
                              30),
                        QImage(":/new/Picture/img/headleft2.png"));
                if (snakeList[1]->getDir() == Right)
                    painter.drawImage(
                        QRect(node2[0].pos.x() * 30, node2[0].pos.y() * 30, 30,
                              30),
                        QImage(":/new/Picture/img/headright2.png"));
            }
            //尾巴
            if (i == node2.size() - 1) {
                if (node2[node2.size() - 1].facePos == Up)
                    painter.drawImage(
                        QRect(node2[node2.size() - 1].pos.x() * 30,
                              node2[node2.size() - 1].pos.y() * 30, 30, 30),
                        QImage(":/new/Picture/img/tailup.png"));
                if (node2[node2.size() - 1].facePos == Down)
                    painter.drawImage(
                        QRect(node2[node2.size() - 1].pos.x() * 30,
                              node2[node2.size() - 1].pos.y() * 30, 30, 30),
                        QImage(":/new/Picture/img/taildown.png"));
                if (node2[node2.size() - 1].facePos == Left)
                    painter.drawImage(
                        QRect(node2[node2.size() - 1].pos.x() * 30,
                              node2[node2.size() - 1].pos.y() * 30, 30, 30),
                        QImage(":/new/Picture/img/tailleft.png"));
                if (node2[node2.size() - 1].facePos == Right)
                    painter.drawImage(
                        QRect(node2[node2.size() - 1].pos.x() * 30,
                              node2[node2.size() - 1].pos.y() * 30, 30, 30),
                        QImage(":/new/Picture/img/tailright.png"));
            }
            //身子
            // qDebug()<<i<<node2[0].facePos<<node2[1].facePos;
            if (node2[i].facePos == Up && i > 0) {
                //

                for (int j = node2[i].pos.y() - 1; j >= node2[i - 1].pos.y();
                     j--) {
                    if (j == node2[i - 1].pos.y()) {
                        if (node2[i - 1].facePos == Left)
                            painter.drawImage(
                                QRect(node2[i - 1].pos.x() * 30,
                                      node2[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/leftdown.png"));
                        if (node2[i - 1].facePos == Right)
                            painter.drawImage(
                                QRect(node2[i - 1].pos.x() * 30,
                                      node2[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/rightdown.png"));
                    } else {
                        //
                        painter.drawImage(
                            QRect(node2[i].pos.x() * 30, j * 30, 30, 30),
                            QImage(":/new/Picture/img/downup.png"));
                    }
                }
            }
            if (node2[i].facePos == Down && i > 0) {
                for (int j = node2[i].pos.y() + 1; j <= node2[i - 1].pos.y();
                     j++) {
                    if (j == node2[i - 1].pos.y()) {
                        if (node2[i - 1].facePos == Left)
                            painter.drawImage(
                                QRect(node2[i - 1].pos.x() * 30,
                                      node2[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/leftup.png"));
                        if (node2[i - 1].facePos == Right)
                            painter.drawImage(
                                QRect(node2[i - 1].pos.x() * 30,
                                      node2[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/rightup.png"));
                    } else {
                        painter.drawImage(
                            QRect(node2[i].pos.x() * 30, j * 30, 30, 30),
                            QImage(":/new/Picture/img/downup.png"));
                    }
                }
            }
            if (node2[i].facePos == Left && i > 0) {
                for (int j = node2[i].pos.x() - 1; j >= node2[i - 1].pos.x();
                     j--) {
                    if (j == node2[i - 1].pos.x()) {
                        if (node2[i - 1].facePos == Up)
                            painter.drawImage(
                                QRect(node2[i - 1].pos.x() * 30,
                                      node2[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/rightup.png"));
                        if (node2[i - 1].facePos == Down)
                            painter.drawImage(
                                QRect(node2[i - 1].pos.x() * 30,
                                      node2[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/rightdown.png"));
                    } else {
                        painter.drawImage(
                            QRect(j * 30, node2[i].pos.y() * 30, 30, 30),
                            QImage(":/new/Picture/img/leftright.png"));
                    }
                }
            }
            if (node2[i].facePos == Right && i > 0) {
                for (int j = node2[i].pos.x() + 1; j <= node2[i - 1].pos.x();
                     j++) {
                    if (j == node2[i - 1].pos.x()) {
                        if (node2[i - 1].facePos == Up)
                            painter.drawImage(
                                QRect(node2[i - 1].pos.x() * 30,
                                      node2[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/leftup.png"));
                        if (node2[i - 1].facePos == Down)
                            painter.drawImage(
                                QRect(node2[i - 1].pos.x() * 30,
                                      node2[i - 1].pos.y() * 30, 30, 30),
                                QImage(":/new/Picture/img/leftdown.png"));
                    } else {
                        painter.drawImage(
                            QRect(j * 30, node2[i].pos.y() * 30, 30, 30),
                            QImage(":/new/Picture/img/leftright.png"));
                    }
                }
            }
        }
    }
}

//毒苹果Timer 的 槽函数
void gameWidget::yellowTimeout() { yellowTimer->stop(); }
void gameWidget::timeout() {
    Direction d = snakeList[0]->getDirKey();
    if (foodList.size() == 1)  //只有一个食物的时候
        snakeList[0]->changeDirect(d);
    if (snakeList.size() == 2) {  //两条蛇的时候
        Direction d2 = snakeList[1]->getDirKey();

        snakeList[1]->changeDirect(d2);
    }
    if (snakeList.size() == 1 && foodList.size() == 1) {
        std::vector<snakeNode> snake1 = snakeList[0]->getPoints();
        std::vector<QPoint> food1 = foodList[0]->getPos();
        if (snake1[0].pos == food1[0]) {
            snakeList[0]->score += snakeList[0]->level;
            snakeList[0]->level = snakeList[0]->getLength() / 5 + 1;
        }
        ScoreLabelNumber->setText(QString::number(snakeList[0]->score) + "分");
        LevelLabelNumber->setText(QString::number(snakeList[0]->level) + "级");
        // timer->start(800-snakeList[0]->level>0?
        // 800-snakeList[0]->level*80:80/(snakeList[0]->level-8));
        timer->start(30 +
                     570 / snakeList[0]->level);  // 新的数学公式，还挺好用的
    }
    if (snakeList.size() == 1 && foodList.size() == 2) {
        std::vector<snakeNode> snake1 = snakeList[0]->getPoints();
        std::vector<QPoint> food1 = foodList[0]->getPos();
        std::vector<QPoint> food2 = foodList[1]->getPos();
        if (yellowTimer->isActive()) {
            snakeList[0]->yellowDirect(d);
        } else {
            snakeList[0]->changeDirect(d);
        }
        if (snake1[0].pos == food1[0]) {
            snakeList[0]->score += snakeList[0]->level;
            snakeList[0]->level = snakeList[0]->getLength() / 5 + 1;
        }
        if (snake1[0].pos == food2[0]) {
            yellowTimer->start(5000);
            connect(yellowTimer, SIGNAL(timeout()), this,
                    SLOT(yellowTimeout()));
            // snakeList[0]->yellowDirect(d);
        }
        ScoreLabelNumber->setText(QString::number(snakeList[0]->score) + "分");
        LevelLabelNumber->setText(QString::number(snakeList[0]->level) + "级");
        // timer->start(800-snakeList[0]->level>0?
        // 800-snakeList[0]->level*80:80/(snakeList[0]->level-8));
        timer->start(30 + 570 / snakeList[0]->level);
    }
    upDate();

    this->update();
}

//开始按钮
void gameWidget::startPush() {
    timer->start(600);

    Snake *snake1 = new Snake;
    snakeList.push_back(snake1);
    Food *food1 = new Food;
    foodList.push_back(food1);
    ScoreLabelNumber = new QLabel(this);
    LevelLabelNumber = new QLabel(this);
    ScoreLabelNumber->setText(QString::number(snakeList[0]->score) + "分");
    ScoreLabelNumber->setGeometry(775, 500, 50, 20);
    ScoreLabelNumber->show();
    LevelLabelNumber->setText(QString::number(snakeList[0]->score) + "级");
    LevelLabelNumber->setGeometry(775, 525, 50, 20);
    LevelLabelNumber->show();
    disconnect(startButton, SIGNAL(clicked(bool)), this, SLOT(startPush()));
    // startButton->setIcon(QIcon(":/new/Picture/img/Apple.png"));
    startButton->setText("暂停游戏");
    doubleButton->hide();
    crazyButton->hide();
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(pausePush()));
    this->repaint();
}

//双人模式按钮
void gameWidget::doublePush() {
    timer->start(200);
    Snake *snake1 = new Snake;
    Snake *snake2 = new Snake;
    snakeList.push_back(snake1);
    snakeList.push_back(snake2);
    Food *food1 = new Food;
    foodList.push_back(food1);

    disconnect(startButton, SIGNAL(clicked(bool)), this, SLOT(startPush()));
    // startButton->setIcon(QIcon(":/new/Picture/img/Apple.png"));
    startButton->setText("暂停游戏");
    doubleButton->hide();
    crazyButton->hide();
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(pausePush()));
    this->repaint();
}

//疯狂模式按钮
void gameWidget::crazyPush() {
    timer->start(600);
    yellowTimer = new QTimer;
    Snake *snake1 = new Snake;
    snakeList.push_back(snake1);
    Food *food1 = new Food(normalFood);
    Food *food2 = new Food(yellowFood);
    foodList.push_back(food1);
    foodList.push_back(food2);
    ScoreLabelNumber = new QLabel(this);
    LevelLabelNumber = new QLabel(this);
    ScoreLabelNumber->setText(QString::number(snakeList[0]->score) + "分");
    ScoreLabelNumber->setGeometry(775, 500, 50, 20);
    ScoreLabelNumber->show();
    LevelLabelNumber->setText(QString::number(snakeList[0]->score) + "级");
    LevelLabelNumber->setGeometry(775, 525, 50, 20);
    LevelLabelNumber->show();
    disconnect(startButton, SIGNAL(clicked(bool)), this, SLOT(startPush()));
    // startButton->setIcon(QIcon(":/new/Picture/img/Apple.png"));
    startButton->setText("暂停游戏");
    doubleButton->hide();
    crazyButton->hide();
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(pausePush()));
    this->repaint();
}

//退出游戏按钮
void gameWidget::exitPush() {
    this->close();
    delete this;
}

//停止按钮
void gameWidget::pausePush() {
    timer->stop();
    connect(continueButton, SIGNAL(clicked(bool)), this, SLOT(continuePush()));
    // startButton->setIcon(QIcon(":/Picture/img/Start.png"));
    continueButton->show();
    // disconnect(startButton,SIGNAL(clicked(bool)),this,SLOT(pausePush()));
    startButton->hide();
}

void gameWidget::continuePush() {
    if (snakeList.size() == 1) {
        timer->start(600);
    }
    if (snakeList.size() == 2) {
        timer->start(250);
    }
    continueButton->hide();
    startButton->show();

    disconnect(continueButton, SIGNAL(clicked(bool)), this,
               SLOT(continuePush()));
    this->repaint();
}

//每次的维护函数
void gameWidget::upDate() {
    if (snakeList.size() == 1 && foodList.size() == 1) {
        std::vector<snakeNode> snake1 = snakeList[0]->getPoints();
        std::vector<QPoint> food1 = foodList[0]->getPos();
        if (snake1[0].pos == food1[0]) {
            snakeList[0]->update(1);
            while (1) {
                foodList[0]->update(1);
                food1 = foodList[0]->getPos();
                if (!snakeList[0]->insideSnake(food1[0])) break;
            }

        } else {
            snakeList[0]->update(0);
        }
        connect(timer, SIGNAL(timeout()), this, SLOT(checkGameOver()));
    }
    if (snakeList.size() == 1 && foodList.size() == 2) {
        std::vector<snakeNode> snake1 = snakeList[0]->getPoints();
        std::vector<QPoint> food1 = foodList[0]->getPos();
        std::vector<QPoint> food2 = foodList[1]->getPos();
        if (snake1[0].pos == food1[0]) {
            snakeList[0]->update(1);
            while (1) {
                foodList[0]->update();
                food1 = foodList[0]->getPos();
                if (!snakeList[0]->insideSnake(food1[0])) break;
            }
        } else if (snake1[0].pos == food2[0]) {
            snakeList[0]->update(0);
            while (1) {
                foodList[1]->update();
                food2 = foodList[1]->getPos();
                if (!snakeList[0]->insideSnake(food2[0]) &&
                    food1[0] != food2[0])
                    break;
            }
        } else {
            snakeList[0]->update(0);
        }
        connect(timer, SIGNAL(timeout()), this, SLOT(checkGameOver()));
    }
    if (snakeList.size() == 2) {
        std::vector<snakeNode> snake1 = snakeList[0]->getPoints();
        std::vector<snakeNode> snake2 = snakeList[1]->getPoints();
        std::vector<QPoint> food1 = foodList[0]->getPos();
        if (snake1[0].pos == food1[0]) {
            snakeList[0]->update(1);
            while (1) {
                foodList[0]->update();
                food1 = foodList[0]->getPos();
                if (!snakeList[0]->insideSnake(food1[0])) break;
            }

        } else {
            snakeList[0]->update(0);
        }
        if (snake2[0].pos == food1[0]) {
            snakeList[1]->update(1);
            while (1) {
                foodList[0]->update();
                food1 = foodList[0]->getPos();
                if (!snakeList[1]->insideSnake(food1[0])) break;
            }

        } else {
            snakeList[1]->update(0);
        }
        connect(timer, SIGNAL(timeout()), this,
                SLOT(checkGameOver()));  // 每次时间结束的时候都检查一次
    }
}

//检查是否结束游戏
void gameWidget::checkGameOver() {
    std::vector<snakeNode> snake1 = snakeList[0]->getPoints();
    if (snake1[0].pos.x() > 20 || snake1[0].pos.x() < 1 ||
        snake1[0].pos.y() > 20 || snake1[0].pos.y() < 1) {
        this->update();
        timer->stop();
        if (QMessageBox::Yes == QMessageBox::information(this, "Tips：",
                                                         "P1 Game Over!",
                                                         QMessageBox::Yes)) {
            this->hide();
            delete this;
            return;
        }
    }

    for (int i = 2; i < snake1.size(); i++) {
        if (snake1[i].facePos == Up) {
            if (snake1[0].pos.x() == snake1[i].pos.x() &&
                snake1[0].pos.y() >= snake1[i - 1].pos.y() &&
                snake1[0].pos.y() <= snake1[i].pos.y()) {
                this->update();
                timer->stop();
                if (QMessageBox::Yes ==
                    QMessageBox::information(this, "Tips：", "P1 Game Over!",
                                             QMessageBox::Yes)) {
                    this->hide();
                    delete this;
                    return;
                }
            }
        }
        if (snake1[i].facePos == Down) {
            if (snake1[0].pos.x() == snake1[i].pos.x() &&
                snake1[0].pos.y() <= snake1[i - 1].pos.y() &&
                snake1[0].pos.y() >= snake1[i].pos.y()) {
                this->update();
                timer->stop();
                if (QMessageBox::Yes ==
                    QMessageBox::information(this, "Tips：", "P1 Game Over!",
                                             QMessageBox::Yes)) {
                    this->hide();
                    delete this;
                    return;
                }
            }
        }
        if (snake1[i].facePos == Left) {
            if (snake1[0].pos.y() == snake1[i].pos.y() &&
                snake1[0].pos.x() >= snake1[i - 1].pos.x() &&
                snake1[0].pos.x() <= snake1[i].pos.x()) {
                this->update();
                timer->stop();
                if (QMessageBox::Yes ==
                    QMessageBox::information(this, "Tips：", "P1 Game Over!",
                                             QMessageBox::Yes)) {
                    this->hide();
                    delete this;
                    return;
                }
            }
        }
        if (snake1[i].facePos == Right) {
            if (snake1[0].pos.y() == snake1[i].pos.y() &&
                snake1[0].pos.x() <= snake1[i - 1].pos.x() &&
                snake1[0].pos.x() >= snake1[i].pos.x()) {
                this->update();
                timer->stop();
                if (QMessageBox::Yes ==
                    QMessageBox::information(this, "Tips：", "P1 Game Over!",
                                             QMessageBox::Yes)) {
                    this->hide();
                    delete this;
                    return;
                }
            }
        }
    }
    // P2 是否撞墙或者自己咬自己
    if (snakeList.size() == 2) {
        std::vector<snakeNode> snake2 = snakeList[1]->getPoints();
        if (snake2[0].pos.x() > 20 || snake2[0].pos.x() < 1 ||
            snake2[0].pos.y() > 20 || snake2[0].pos.y() < 1) {
            this->update();
            timer->stop();
            if (QMessageBox::Yes ==
                QMessageBox::information(this, "Tips：", "P2 Game Over!",
                                         QMessageBox::Yes)) {
                this->hide();
                delete this;
                return;
            }
        }

        for (int i = 2; i < snake2.size(); i++) {
            if (snake2[i].facePos == Up) {
                if (snake2[0].pos.x() == snake2[i].pos.x() &&
                    snake2[0].pos.y() >= snake2[i - 1].pos.y() &&
                    snake2[0].pos.y() <= snake2[i].pos.y()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P2 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
            if (snake2[i].facePos == Down) {
                if (snake2[0].pos.x() == snake2[i].pos.x() &&
                    snake2[0].pos.y() <= snake2[i - 1].pos.y() &&
                    snake2[0].pos.y() >= snake2[i].pos.y()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P2 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
            if (snake2[i].facePos == Left) {
                if (snake2[0].pos.y() == snake2[i].pos.y() &&
                    snake2[0].pos.x() >= snake2[i - 1].pos.x() &&
                    snake2[0].pos.x() <= snake2[i].pos.x()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P2 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
            if (snake2[i].facePos == Right) {
                if (snake2[0].pos.y() == snake2[i].pos.y() &&
                    snake2[0].pos.x() <= snake2[i - 1].pos.x() &&
                    snake2[0].pos.x() >= snake2[i].pos.x()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P2 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
        }
        //判断两蛇是否相撞
        for (int i = 1; i < snake2.size(); i++) {
            if (snake2[i].facePos == Up) {
                if (snake1[0].pos.x() == snake2[i].pos.x() &&
                    snake1[0].pos.y() >= snake2[i - 1].pos.y() &&
                    snake1[0].pos.y() <= snake2[i].pos.y()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P1 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
            if (snake2[i].facePos == Down) {
                if (snake1[0].pos.x() == snake2[i].pos.x() &&
                    snake1[0].pos.y() <= snake2[i - 1].pos.y() &&
                    snake1[0].pos.y() >= snake2[i].pos.y()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P1 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
            if (snake2[i].facePos == Left) {
                if (snake1[0].pos.y() == snake2[i].pos.y() &&
                    snake1[0].pos.x() >= snake2[i - 1].pos.x() &&
                    snake1[0].pos.x() <= snake2[i].pos.x()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P1 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
            if (snake2[i].facePos == Right) {
                if (snake1[0].pos.y() == snake2[i].pos.y() &&
                    snake1[0].pos.x() <= snake2[i - 1].pos.x() &&
                    snake1[0].pos.x() >= snake2[i].pos.x()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P1 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
        }
        for (int i = 1; i < snake1.size(); i++) {
            if (snake1[i].facePos == Up) {
                if (snake2[0].pos.x() == snake1[i].pos.x() &&
                    snake2[0].pos.y() >= snake1[i - 1].pos.y() &&
                    snake2[0].pos.y() <= snake1[i].pos.y()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P2 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
            if (snake1[i].facePos == Down) {
                if (snake2[0].pos.x() == snake1[i].pos.x() &&
                    snake2[0].pos.y() <= snake1[i - 1].pos.y() &&
                    snake2[0].pos.y() >= snake1[i].pos.y()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P2 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
            if (snake1[i].facePos == Left) {
                if (snake2[0].pos.y() == snake1[i].pos.y() &&
                    snake2[0].pos.x() >= snake1[i - 1].pos.x() &&
                    snake2[0].pos.x() <= snake1[i].pos.x()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P2 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
            if (snake1[i].facePos == Right) {
                if (snake2[0].pos.y() == snake1[i].pos.y() &&
                    snake2[0].pos.x() <= snake1[i - 1].pos.x() &&
                    snake2[0].pos.x() >= snake1[i].pos.x()) {
                    this->update();
                    timer->stop();
                    if (QMessageBox::Yes == QMessageBox::information(
                                                this, "Tips：", "P2 Game Over!",
                                                QMessageBox::Yes)) {
                        this->hide();
                        delete this;
                        return;
                    }
                }
            }
        }
    }
}
