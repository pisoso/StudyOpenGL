#ifndef LCOPENGLWIDGET_H
#define LCOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class LCOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit LCOpenGLWidget(QWidget *parent = nullptr);

    unsigned int VBO,VAO;

    unsigned int shaderProgram;


protected:
    // 重载 QOpenGLWidget 中的这三个函数
    virtual void initializeGL();          // 设置OpenGL资源和状态。第一次调用 resizeGL()/paintGL()之前调用一次
    virtual void resizeGL(int w, int h);  // 设置OpenGL视口、投影等。widget调整大小（或首次显示）时调用
    virtual void paintGL();               // 渲染OpenGL场景。widget需要更新时调用


signals:

};

#endif // LCOPENGLWIDGET_H
