#include "lcopenglwidget.h"

float vertices[] = {
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f // top left
};


unsigned int indices[] = { // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
};


const char *vertexShaderSource = "#version 330 core\n"
 "layout (location = 0) in vec3 aPos;\n" "void main()\n"
 "{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

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
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    // 绑定VBO和VAO对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    // glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
    // 为当前绑定到target的缓冲区对象创建一个新的数据结构
    // 如果data不是NULL，则使用来自此指针的数据初始化数据结构
    // 在显存区创建一个缓存区域，并把数据放入显存区（这里的数据为vertices）
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    // 告知显卡如何解析缓冲里的属性值
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    // 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);

    // 解除绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER,0);
//    glBindVertexArray(0);


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

    // 用线条填充图形，如果没有这条语句，则默认是用颜色来填充（即GL_FILL）
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 创建EBO对象，并赋予ID
    glGenBuffers(1, &EBO);
    // 绑定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STATIC_DRAW);
    // 解除绑定EAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // 解除绑定VAO
    glBindVertexArray(0);

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
    // 画之前绑定VAO对象，VAO小助理上班了，即使VBO和EBO没绑定，但是他们的数据索引都已经记录在VAO了
    glBindVertexArray(VAO);
    // // 提供绘制功能，
    // 从当前绑定的缓冲区中读取数据按照基本图元的形式绘制几何图形（从第0个点开始取，取6个点）
//    glDrawArrays(GL_TRIANGLES,0,6);
    // 利用EBO来提取数据点，这里取6个索引（点）
    // 不采用索引数组,只有当EBO没有解绑的时候才能用，但这样就导致EBO小助手始终处于绑定状态
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0/*&indices*/);
    //用索引数组indices里面取6个点，此时就算EBO解绑后也能用，因为VAO里面已经记录了EBO的索引
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);
}
