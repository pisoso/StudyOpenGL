#include "lcopenglwidget.h"

float vertices[] = {
    // positions         // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

QVector<QVector3D> cubePositions= {
    QVector3D( 0.0f, 0.0f, 0.0f),
    QVector3D( 2.0f, 5.0f, -15.0f),
    QVector3D(-1.5f, -2.2f, -2.5f),
    QVector3D(-3.8f, -2.0f, -12.3f),
    QVector3D( 2.4f, -0.4f, -3.5f),
    QVector3D(-1.7f, 3.0f, -7.5f),
    QVector3D( 1.3f, -2.0f, -2.5f),
    QVector3D( 1.5f, 2.0f, -2.5f),
    QVector3D( 1.5f, 0.2f, -1.5f),
    QVector3D(-1.3f, 1.0f, -1.5f)
};


unsigned int indices[] = { // note that we start from 0!
                           0, 1, 3, // first triangle
                           1, 2, 3 // second triangle
                         };

float ratio = 0.5;


LCOpenGLWidget::LCOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    // 窗口可以获取键盘的按键
    setFocusPolicy(Qt::StrongFocus);

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(on_timeout()));

    m_timer.start(100);
    m_time.start();


    //Camera position
    m_cameraPos = QVector3D(0.0f, 0.0f, 2.0f);
    //Camera direction
    m_cameraTarget = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D m_cameraDirection = QVector3D(m_cameraPos - m_cameraTarget);
    m_cameraDirection.normalize();
    //Right axis
    m_up = QVector3D(0.0f, 1.0f, 0.0f);
    m_cameraRight  = QVector3D::crossProduct(m_up, m_cameraDirection);
    m_cameraRight.normalize();
    //Up axis
    m_cameraUp = QVector3D::crossProduct(m_cameraDirection, m_cameraRight);

}

LCOpenGLWidget::~LCOpenGLWidget()
{
    // 在使用gl相关函数之前要makeCurrent,除非是在initializeGL(), resizeGL(), paintGL()中使用
    makeCurrent();
    glDeleteBuffers(1,&VBO);      //回收VBO
    glDeleteBuffers(1,&EBO);      //回收EBO
    glDeleteVertexArrays(1,&VAO); //回收VAO

    doneCurrent();
}

void LCOpenGLWidget::drawShape(LCOpenGLWidget::Shape _shape)
{
    m_shape = _shape;

    update(); // 重新触发paintGL函数
}

void LCOpenGLWidget::setWireframe(bool _wireframe)
{
    makeCurrent();
    if(_wireframe){
        // 用线条填充图形，如果没有这条语句，则默认是用颜色来填充（即GL_FILL）
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    update(); // 重新触发paintGL函数

    doneCurrent();
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

    // 告知显卡如何解析缓冲里的属性值,告知属性为0的VAO该怎么解析缓存里的值，这里是aPos,并把显存里的值赋给aPos
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    // 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);
    // 告知显卡如何解析缓冲里的属性值，,告知属性为1的VAO该怎么解析缓存里的值，这里是aTexCord，并把显存里的值赋给aColor
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3* sizeof(float)));
    // 开启VAO管理的第二个属性值
    glEnableVertexAttribArray(1);

    // 解除绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //    glBindVertexArray(0);

    // 用QT自带的库来创建着色器
    bool success;
    // 替换顶点着色器对象中的源代码
    //    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderSource);    //代码替换
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shapes.vert"); //文件替换
    // 替换片段着色器对象中的源代码
    //    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShaderSource);
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shapes.frag");
    //link shaders 链接着色器
    success = m_shaderProgram.link();
    if(!success){
        qDebug() << "Err:" << m_shaderProgram.log();
    }

    // 用线条填充图形，如果没有这条语句，则默认是用颜色来填充（即GL_FILL）
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 创建EBO对象，并赋予ID
    glGenBuffers(1, &EBO);
    // 绑定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STATIC_DRAW);
    // 解除绑定EAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    // 去掉照片的边框（PNG格式照片的透明部分）
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    m_textureWall = new QOpenGLTexture(QImage(":/images/images/wall.jpg").mirrored());
    m_textureSmile = new QOpenGLTexture(QImage(":/images/images/awesomeface.png").mirrored());
    m_textureSmall = new QOpenGLTexture(QImage(":/images/images/small.png").mirrored());


    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("textureWall",0);
    m_shaderProgram.setUniformValue("textureSmile",1);
    m_shaderProgram.setUniformValue("textureSmall",2);

    // 生成多级渐远纹理
    m_textureSmall->generateMipMaps();

    // 解除绑定VAO
    glBindVertexArray(0);

    // 绑定的几号。下面这一坨就设定的哪个纹理
    m_textureSmile->bind(1);
//    m_textureSmall->bind(2);
    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    QMatrix4x4 projection;
    projection.perspective(45,(float)width()/height(),0.1f,100);//透视投影矩阵
//    projection.perspective(60,(float)width()/(2*height()),0.1f,100);//透视投影矩阵
    m_shaderProgram.setUniformValue("projection", projection);
}

void LCOpenGLWidget::resizeGL(int w, int h)
{

}

void LCOpenGLWidget::paintGL()
{
//    // 显示区域占屏幕的多少
//    glViewport(0,0,width()/2,height());

    QMatrix4x4 model; //不初始化，则默认为单位矩阵
    QMatrix4x4 view;  //QMatrix4x4这个类型，用rotate或者translate即表示在上一个基础上乘以相应的旋转或平移矩阵

//    float time=QTime::currentTime().msec();

//    matrix.rotate(time, 0.0f, 0.0f, 1.0f);
//    matrix.translate(0.5,-0.5,0); // 这里是线位移再旋转（仍是绕基坐标系，即中心旋转）

//    view.translate(0.0,0.0,-3.0); // 观察矩阵 （模型相对于相机坐标系的位置）
    const float radius = 10.0f;
    float time=m_time.elapsed()/1000.0;
    float camX = sin(time) * radius;
    float camZ = cos(time) * radius;
    view.lookAt(QVector3D(camX, 0.0, camZ), m_cameraTarget,m_up);


//    view.lookAt(m_cameraPos,m_cameraTarget,m_up);


    // 设置颜色缓存的清除值
    // lClearColor ( ) 就是用来设置这个 “底色” 的，即所谓的背景颜色。
    // glClearColor ( ) 只起到Set 的作用，并不Clear 任何。
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // 将缓存清除为预先的设置值
    glEnable(GL_DEPTH_TEST); // 打开深度缓冲区，即后边的会被前面的遮挡
    // glClear  ( ) 是用来清除屏幕颜色，即将屏幕的所有像素点都还原为 “底色 ”。
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    // 安装一个程序对象作为当前渲染状态的一部分。当我们渲染一个物体时要使用着色器程序
    //    glUseProgram(shaderProgram);
    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("ratio",ratio);

    // 画之前绑定VAO对象，VAO小助理上班了，即使VBO和EBO没绑定，但是他们的数据索引都已经记录在VAO了
    glBindVertexArray(VAO);

//    // 设置纹理超过范围区域的填充颜色
//    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };

    switch (m_shape) {
    case Rect:
        m_textureWall->bind(0); //默认绑定的就是第一个纹理，即shapes.frag文件里面的texture0
        m_textureSmile->bind(1);
        m_textureSmall->bind(2);
//        // 下面这坨只对纹理textureSmall产生作用，因为现在绑定的是纹理textureSmall
//        // 用镜像来填充其他没有的部分，X Y轴做镜像处理;GL_TEXTURE_WRAP_T表示x轴，GL_TEXTURE_WRAP_S表示Y轴
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//        // 设置纹理超过范围的区域的用其他颜色来填充
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // 对着色器里面的matrix的值进行改变
        m_shaderProgram.setUniformValue("view", view);

        foreach (auto item, cubePositions) {
            model.setToIdentity();
            model.translate(item);
//            model.rotate(time, 1.0f, 5.0f, 0.5f); // 模型矩阵（模型相对于世界坐标系的变化）
            m_shaderProgram.setUniformValue("model", model);
            // 开始绘制图形
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);
            glDrawArrays(GL_TRIANGLES,0,36);
        }


        break;
    default:
        break;

    }

    // // 提供绘制功能，
    // 从当前绑定的缓冲区中读取数据按照基本图元的形式绘制几何图形（从第0个点开始取，取6个点）
    //    glDrawArrays(GL_TRIANGLES,0,6);
    // 利用EBO来提取数据点，这里取6个索引（点）
    // 用索引数组indices里面取6个点，此时就算EBO解绑后也能用，因为VAO里面已经记录了EBO的索引
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);
    // 不采用索引数组,只有当EBO没有解绑的时候才能用，但这样就导致EBO小助手始终处于绑定状态
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void LCOpenGLWidget::keyPressEvent(QKeyEvent *_event)
{
    switch (_event->key()) {
    case Qt::Key_Up:
        ratio +=0.1;
        break;
    case Qt::Key_Down:
        ratio -=0.1;
        break;
    default:
        break;
    }
    if(ratio>1){
        ratio = 1;
    }
    if(ratio<0){
        ratio = 0;
    }

    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("ratio",ratio);
    update();
}

void LCOpenGLWidget::on_timeout()
{
    update();
}

