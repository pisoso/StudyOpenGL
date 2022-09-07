#include "lcopenglwidget.h"

float vertices[] = {
    -0.5f,-0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    0.0f,0.5f,0.0f
};

// vertex--顶点
const char *vertexShaderSource = "#version 330 core\n"
 "layout (location = 0) in vec3 aPos;\n" "void main()\n"
 "{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// fragment--片段
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(0.2f, 0.5f, 0.2f, 1.0f);\n" "}\n\0";


LCOpenGLWidget::LCOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void LCOpenGLWidget::initializeGL()
{
    // 初始化openGL函数，将QT里面的函数指针指向显卡的函数
    initializeOpenGLFunctions();

    // 创建VBO和VAO对象，并赋予ID
    // glGenVertexArrays() 创建一个顶点数组对象
    // 第一个参数：需要创建的缓存数量
    // 第二个参数：存储单一ID或多个ID的GLuint变量或数组的地址。
    glGenVertexArrays(1,&VAO);
    // glGenBuffers() 创建一个缓存对象并且返回缓存对象的标示符。
    glGenBuffers(1,&VBO);

    // 绑定VBO和VAO对象
    // 顶点对象创建之后，在使用缓存对象之前，需要将缓存对象连接到相应的缓存上。
    // glBindBuffer()有1个参数：buffer。（绑定和解绑的顺序很重要，勿更改）
    glBindVertexArray(VAO);
    // 缓存对象创建之后，在使用缓存对象之前，需要将缓存对象连接到相应的缓存上。
    // glBindBuffer()有2个参数：target与buffer。（绑定和解绑的顺序很重要，勿更改)
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    // 当缓存初始化之后，使用glBufferData()将顶点数据拷贝到缓存对象
    // 在显存区创建一个缓存区域，并把数据放入显存区（这里的数据为vertices）
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    // 告知显卡如何解析缓冲里的属性值，即VBO中的信息应该以几个为一组
    // 设置顶点属性指针，glVertexAttribPointer()函数告诉OpenGL该如何解析顶点数据
    // 顶点属性位置 顶点属性大小 数据的类型 是否被标准化 步长 偏移
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    // 开启VAO管理的第一个属性值
    // glEnableVertexAttribArray()以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
    glEnableVertexAttribArray(0);

    // 解除绑定VBO和VAO
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    // build and compile our shader program //
    // 顶点着色器
    // 创建一个顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 替换着色器对象中的源代码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // 编译
    glCompileShader(vertexShader);

    // 片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // link shaders 链接着色器
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 删除着色器
    glDeleteShader(vertexShader);
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


    // 当我们渲染一个物体时要使用着色器程序
    glUseProgram(shaderProgram);
    // 画之前绑定VAO对象，VAO小助理上班了，即使VBO和EBO没绑定，但是他们的数据索引都已经记录在VAO里面了
    glBindVertexArray(VAO);
    // 从当前绑定的缓冲区中读取数据按照基本图元的形式绘制几何图形（从第0个点开始取，取三个点）
//    glDrawArrays(GL_TRIANGLES,0,3);// 绘制三角形：三角形 数组起始索引 绘制多少个顶点
    glDrawArrays(GL_LINES, 0 , 2); // 绘制三角形：线条 数组起始索引 绘制多少个顶点
}
