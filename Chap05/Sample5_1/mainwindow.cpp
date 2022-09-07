#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actDrawRect,SIGNAL(triggered()),this,SLOT(slotDrawRect()));
    connect(ui->actClear,SIGNAL(triggered()),this,SLOT(slotClear()));
    connect(ui->actWireframe,SIGNAL(triggered()),this,SLOT(slotWireframe()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotDrawRect()
{
    ui->openGLWidget->drawShape(LCOpenGLWidget::Rect);
}

void MainWindow::slotClear()
{
    ui->openGLWidget->drawShape(LCOpenGLWidget::None);
}

void MainWindow::slotWireframe()
{
    ui->openGLWidget->setWireframe(ui->actWireframe->isChecked());
}

