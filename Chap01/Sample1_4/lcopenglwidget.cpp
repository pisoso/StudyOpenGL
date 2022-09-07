#include "lcopenglwidget.h"

float firstTriangle[] = {
    -0.9f, -0.5f, 0.0f, // left
    -0.0f, -0.5f, 0.0f, // right
    -0.45f, 0.5f, 0.0f, // top
};

float secondTriangle[] = {
    0.0f, -0.5f, 0.0f, // left
    0.9f, -0.5f, 0.0f, // right
    0.45f, 0.5f, 0.0f // top
};


// vertex--顶点
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// fragment--片段
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const char *fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";



LCOpenGLWidget::LCOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void LCOpenGLWidget::initializeGL()
{
    // 初始化openGL函数，将QT里面的函数指针指向显卡的函数
    initializeOpenGLFunctions();

    // 创建VBO和VAO对象，并赋予ID
    // 一次创建多个对象
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);;

    // 绑定VBOs[0]和VAOs[0]对象
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);

    // glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
    // 为当前绑定到target的缓冲区对象创建一个新的数据储存
    // 如果data不是NULL，则使用来自此指针的数据初始化数据结构
    // 在显存区创建一个缓存区域，并把数据放入显存区（这里的数据为vertices）
    glBufferData(GL_ARRAY_BUFFER,sizeof(firstTriangle),firstTriangle,GL_STATIC_DRAW);

    // 告知显卡如何解析缓冲里的属性值，即VBO中的信息应该以几个为一组
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    // 开启VAOs[0]管理的第一个属性值
    glEnableVertexAttribArray(0);

//    // 解除绑定VBO和VAO
//    glBindBuffer(GL_ARRAY_BUFFER,0);
//    glBindVertexArray(0);

    // 绑定VBOs[1]和VAOs[1]对象
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);

    // glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
    // 为当前绑定到target的缓冲区对象创建一个新的数据储存
    // 如果data不是NULL，则使用来自此指针的数据初始化数据结构
    // 在显存区创建一个缓存区域，并把数据放入显存区（这里的数据为vertices）
    glBufferData(GL_ARRAY_BUFFER,sizeof(secondTriangle),secondTriangle,GL_STATIC_DRAW);

    // 告知显卡如何解析缓冲里的属性值，即VBO中的信息应该以几个为一组
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    // 开启VAOs[1]管理的第一个属性值
    glEnableVertexAttribArray(0);


    // build and compile our shader program //在运行时实时编译着色器的源码
    // 顶点着色器
    // 创建一个顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 替换着色器对象中的源代码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // 编译
    glCompileShader(vertexShader);

    // 片段着色器1
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 片段着色器2
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShader2);

    // link shaders 链接着色器1
    shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader);
    glLinkProgram(shaderProgram1);

    // link shaders 链接着色器2
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    // 删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader);

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


    // 当我们渲染一个物体时要使用的着色器程序
    glUseProgram(shaderProgram1);
    // 画之前绑定VAO对象，VAO小助理上班了，即使VBO和EBO没绑定，但是他们的数据索引都已经记录在VAO了
    glBindVertexArray(VAOs[0]);
    // 从当前绑定的缓冲区中读取数据按照基本图元的形式绘制几何图形（从第0个点开始取，取三个点）
    glDrawArrays(GL_TRIANGLES,0,3);

    // 给第二个着色器程序，绑定第二个VAO对象，并绘制图形
    glUseProgram(shaderProgram2);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES,0,3);
}
