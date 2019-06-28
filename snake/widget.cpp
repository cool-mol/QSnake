//#include "ui_widget.h"
#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent) {
    this->setWindowTitle("欢迎进入贪吃蛇！");
    // this->setWindowIcon(QIcon(":/Picture/img/WindowPicture.jpg"));
    //添加背景图片TODO
    startPush = new QPushButton(this);
    exitPush = new QPushButton(this);
    this->resize(400, 400);
    startPush->setGeometry(150, 100, 100, 30);
    exitPush->setGeometry(150, 250, 100, 30);
    startPush->setText("开始游戏");
    exitPush->setText("结束游戏");
    exitBox = new QMessageBox;
    connect(startPush, SIGNAL(clicked(bool)), this, SLOT(showGameWindow()));
    connect(exitPush, SIGNAL(clicked(bool)), this, SLOT(exitWindow()));
}

Widget::~Widget() {}

void Widget::showGameWindow() {
    gameWindow = new gameWidget;
    gameWindow->show();
}

void Widget::exitWindow() {
    if (QMessageBox::Yes ==
        QMessageBox::question(this, "Tips：", "Do you want to leave the game?",
                              QMessageBox::Yes | QMessageBox::No)) {
        delete this;
        exit(0);
    }
}
