#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QIcon>
#include <QPalette>
#include <QBrush>
#include <QPixmap>
#include <QPushButton>
#include <QMessageBox>
#include "gamewidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    QPushButton *startPush;
    QPushButton *exitPush;
    QMessageBox *exitBox;
    gameWidget *gameWindow;
public slots:
    void showGameWindow();
    void exitWindow();
private:


};

#endif // WIDGET_H
