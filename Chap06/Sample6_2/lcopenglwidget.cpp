#include "lcopenglwidget.h"

#define TIMEOUTMSEC 100

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
float PI=3.1415926;
QPoint deltaPos;
float fov = 45;

LCOpenGLWidget::LCOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    // ?????????????????????????????????
    setFocusPolicy(Qt::StrongFocus);
    // ????????????????????????
    setMouseTracking(true);

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(on_timeout()));

    m_timer.start(TIMEOUTMSEC);
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

    m_cameraFront=QVector3D(0.0, 0.0, -1.0);

    m_cameraSpeed=2.5*TIMEOUTMSEC/1000;
}

LCOpenGLWidget::~LCOpenGLWidget()
{
    // ?????????gl?????????????????????makeCurrent,????????????initializeGL(), resizeGL(), paintGL()?????????
    makeCurrent();
    glDeleteBuffers(1,&VBO);      //??????VBO
    glDeleteBuffers(1,&EBO);      //??????EBO
    glDeleteVertexArrays(1,&VAO); //??????VAO

    doneCurrent();
}

void LCOpenGLWidget::drawShape(LCOpenGLWidget::Shape _shape)
{
    m_shape = _shape;

    update(); // ????????????paintGL??????
}

void LCOpenGLWidget::setWireframe(bool _wireframe)
{
    makeCurrent();
    if(_wireframe){
        // ???????????????????????????????????????????????????????????????????????????????????????GL_FILL???
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    update(); // ????????????paintGL??????

    doneCurrent();
}

void LCOpenGLWidget::initializeGL()
{
    // ?????????openGL????????????QT??????????????????????????????????????????
    initializeOpenGLFunctions();

    // ??????VBO???VAO??????????????????ID
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    // ??????VBO???VAO??????
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    // glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
    // ??????????????????target????????????????????????????????????????????????
    // ??????data??????NULL?????????????????????????????????????????????????????????
    // ???????????????????????????????????????????????????????????????????????????????????????vertices???
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    // ?????????????????????????????????????????????,???????????????0???VAO??????????????????????????????????????????aPos,???????????????????????????aPos
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    // ??????VAO???????????????????????????
    glEnableVertexAttribArray(0);
    // ????????????????????????????????????????????????,???????????????1???VAO??????????????????????????????????????????aTexCord??????????????????????????????aColor
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3* sizeof(float)));
    // ??????VAO???????????????????????????
    glEnableVertexAttribArray(1);

    // ????????????VBO
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //    glBindVertexArray(0);

    // ???QT??????????????????????????????
    bool success;
    // ??????????????????????????????????????????
    //    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderSource);    //????????????
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shapes.vert"); //????????????
    // ??????????????????????????????????????????
    //    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShaderSource);
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shapes.frag");
    //link shaders ???????????????
    success = m_shaderProgram.link();
    if(!success){
        qDebug() << "Err:" << m_shaderProgram.log();
    }

    // ???????????????????????????????????????????????????????????????????????????????????????GL_FILL???
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ??????EBO??????????????????ID
    glGenBuffers(1, &EBO);
    // ??????EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STATIC_DRAW);
    // ????????????EAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    // ????????????????????????PNG??????????????????????????????
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    m_textureWall = new QOpenGLTexture(QImage(":/images/images/wall.jpg").mirrored());
    m_textureSmile = new QOpenGLTexture(QImage(":/images/images/awesomeface.png").mirrored());
    m_textureSmall = new QOpenGLTexture(QImage(":/images/images/small.png").mirrored());


    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("textureWall",0);
    m_shaderProgram.setUniformValue("textureSmile",1);
    m_shaderProgram.setUniformValue("textureSmall",2);

    // ????????????????????????
    m_textureSmall->generateMipMaps();

    // ????????????VAO
    glBindVertexArray(0);

    // ?????????????????????????????????????????????????????????
    m_textureSmile->bind(1);
//    m_textureSmall->bind(2);
    // ??????????????????
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


}

void LCOpenGLWidget::resizeGL(int w, int h)
{

}

void LCOpenGLWidget::paintGL()
{
//    // ??????????????????????????????
//    glViewport(0,0,width()/2,height());

    QMatrix4x4 model; //???????????????????????????????????????
    QMatrix4x4 view;  //QMatrix4x4??????????????????rotate??????translate??????????????????????????????????????????????????????????????????
    QMatrix4x4 projection;
    projection.perspective(fov,(float)width()/height(),0.1f,100);//??????????????????
//    projection.perspective(60,(float)width()/(2*height()),0.1f,100);//??????????????????


    float time=m_time.elapsed()/1000.0;

//    matrix.rotate(time, 0.0f, 0.0f, 1.0f);
//    matrix.translate(0.5,-0.5,0); // ????????????????????????????????????????????????????????????????????????

//    view.translate(0.0,0.0,-3.0); // ???????????? ?????????????????????????????????????????????
    view.lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_up);


//    view.lookAt(m_cameraPos,m_cameraTarget,m_up);


    // ??????????????????????????????
    // lClearColor ( ) ???????????????????????? ???????????? ?????????????????????????????????
    // glClearColor ( ) ?????????Set ??????????????????Clear ?????????
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // ????????????????????????????????????
    glEnable(GL_DEPTH_TEST); // ?????????????????????????????????????????????????????????
    // glClear  ( ) ???????????????????????????????????????????????????????????????????????? ????????? ??????
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    // ?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
    //    glUseProgram(shaderProgram);
    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("ratio",ratio);

    // ???????????????VAO?????????VAO???????????????????????????VBO???EBO?????????????????????????????????????????????????????????VAO???
    glBindVertexArray(VAO);

//    // ?????????????????????????????????????????????
//    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };

    switch (m_shape) {
    case Rect:
        m_textureWall->bind(0); //??????????????????????????????????????????shapes.frag???????????????texture0
        m_textureSmile->bind(1);
        m_textureSmall->bind(2);
//        // ????????????????????????textureSmall?????????????????????????????????????????????textureSmall
//        // ??????????????????????????????????????????X Y??????????????????;GL_TEXTURE_WRAP_T??????x??????GL_TEXTURE_WRAP_S??????Y???
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//        // ????????????????????????????????????????????????????????????
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // ?????????????????????view??????????????????
        m_shaderProgram.setUniformValue("view", view);

        m_shaderProgram.setUniformValue("projection", projection);

        foreach (auto item, cubePositions) {
            model.setToIdentity();
            model.translate(item);
//            model.rotate(time, 1.0f, 5.0f, 0.5f); // ?????????????????????????????????????????????????????????
            m_shaderProgram.setUniformValue("model", model);
            // ??????????????????
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);
            glDrawArrays(GL_TRIANGLES,0,36);
        }


        break;
    default:
        break;

    }

    // // ?????????????????????
    // ????????????????????????????????????????????????????????????????????????????????????????????????0?????????????????????6?????????
    //    glDrawArrays(GL_TRIANGLES,0,6);
    // ??????EBO??????????????????????????????6??????????????????
    // ???????????????indices?????????6?????????????????????EBO???????????????????????????VAO?????????????????????EBO?????????
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);
    // ?????????????????????,?????????EBO???????????????????????????????????????????????????EBO?????????????????????????????????
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
    case Qt::Key_W:
        m_cameraPos += m_cameraSpeed * m_cameraFront;
        break;
    case Qt::Key_S:
        m_cameraPos -= m_cameraSpeed * m_cameraFront;
        break;
    case Qt::Key_D:
        m_cameraPos += m_cameraSpeed * m_cameraRight;
        break;
    case Qt::Key_A:
        m_cameraPos -= m_cameraSpeed * m_cameraRight;
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

void LCOpenGLWidget::mouseMoveEvent(QMouseEvent *_event)
{
    static float yaw=-90;
    static float pitch=0;
    static QPoint lastPos(width()/2,height()/2);
    auto currentPos=_event->pos();
    deltaPos=currentPos-lastPos;
    lastPos=currentPos;
    float sensitivity = 0.1f; // change this value to your liking
    deltaPos *= sensitivity;
    yaw += deltaPos.x();
    pitch -= deltaPos.y();// reversed since y-coordinates go from bottom to top
    //qDebug()<<deltaPos.x()<<","<<deltaPos.y(); if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;
    m_cameraFront.setX(cos(yaw*PI/180) );
    m_cameraFront.setY(sin(pitch*PI/180));
    m_cameraFront.setZ(sin(yaw*PI/180) * cos(pitch*PI/180));
    m_cameraFront.normalize();
    update();

}

void LCOpenGLWidget::wheelEvent(QWheelEvent *_event)
{
    if(fov >= 1.0f && fov <= 75.0f){
        fov -= _event->angleDelta().y()/120;//?????????120
    }
    if(fov <= 1.0f){

        fov = 1.0f;
    }
    if(fov >= 75.0f){

        fov = 75.0f;
    }
//    std::cout << "test"<< std::endl;
}

void LCOpenGLWidget::on_timeout()
{
    update();
}


