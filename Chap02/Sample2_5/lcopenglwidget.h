#ifndef LCOPENGLWIDGET_H
#define LCOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QTimer>

class LCOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape{None,Rect,Circle,Triangle};

    explicit LCOpenGLWidget(QWidget *parent = nullptr);
    ~LCOpenGLWidget();

    void drawShape(Shape _shape);
    void setWireframe(bool _wireframe);



protected:
    // 重载 QOpenGLWidget 中的这三个函数
    virtual void initializeGL();          // 设置OpenGL资源和状态。第一次调用 resizeGL()/paintGL()之前调用一次
    virtual void resizeGL(int w, int h);  // 设置OpenGL视口、投影等。widget调整大小（或首次显示）时调用
    virtual void paintGL();               // 渲染OpenGL场景。widget需要更新时调用


signals:

public slots:
    void slotOnTimeout();

private:
    unsigned int VBO,VAO,EBO;
    Shape m_shape;
    QOpenGLShaderProgram m_shaderProgram;
    QTimer m_timer;


};

#endif // LCOPENGLWIDGET_H
