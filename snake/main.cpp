
/* 姓名：路正亮
 * 班级：还没分配（刚转过来
 * 学号：518030910017
 *
 * 贪吃蛇V1.0版本
 * 三个模式：普通模式 双人模式 疯狂模式
 * 普通模式：可以升级 等级越高速度越快 （无限逼近一个数值
 * 双人模式：两个人吃同一个果子 谁头碰到墙或者对面谁死
 * 疯狂模式：有一个普通苹果和一个毒苹果 毒苹果吃到后上下左右颠倒五秒钟
 * 其他同普通模式（后续加入其他果子
 *
 * 预计未来的V2.0版本：加入自动寻路、墙壁类、新的不同类型果子和另外一种新的功能
 * 蛇的18张图片是从网上嫖的，确实有点丑，想办法在V2.0里面进行该进
 *
 * 注释也是瞎加的
 *
 * 应该把蛇类的那个vector改成list的，因为蛇的主要思想是记录每一个拐点，用list效率会更高
 */

#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}
