#include "lcopenglwidget.h"

LCOpenGLWidget::LCOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void LCOpenGLWidget::initializeGL()
{
    // 初始化openGL函数，将QT里面的函数指针指向显卡的函数
    initializeOpenGLFunctions();
}

void LCOpenGLWidget::resizeGL(int w, int h)
{

}

void LCOpenGLWidget::paintGL()
{
    // 设置颜色缓存的清除值
    // lClearColor ( ) 就是用来设置这个 “底色” 的，即所谓的背景颜色。
    // glClearColor ( ) 只起到Set 的作用，并不Clear 任何。
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // 将缓存清除为预先的设置值
    // glClear  ( ) 是用来清除屏幕颜色，即将屏幕的所有像素点都还原为 “底色 ”。
    glClear(GL_COLOR_BUFFER_BIT);
}
