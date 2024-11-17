#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent),
	scene_id(0)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
	if (scene_id==0) {
		scene_0();
	}
	else if (scene_id == 1) {
		scene_1();
	}
    else {
        scene_2();
    }
}

void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}
void MyGLWidget::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_0) {
        scene_id = 0;
        update();
    }
    else if (e->key() == Qt::Key_1) {
        scene_id = 1;
        update();
    }
    else if (e->key() == Qt::Key_2) {
        scene_id = 2;
        update();
    }
    else if (e->key() == Qt::Key_W) { // 上方向键，绕X轴正方向旋转
        rotationX += 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_S) { // 下方向键，绕X轴负方向旋转
        rotationX -= 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_A) { // 左方向键，绕Y轴正方向旋转
        rotationY += 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_D) { // 右方向键，绕Y轴负方向旋转
        rotationY -= 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_Q) { // Q键，绕Z轴正方向旋转
        rotationZ += 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_E) { // E键，绕Z轴负方向旋转
        rotationZ -= 5.0f;
        update();
    }
}

void MyGLWidget::scene_0()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(50.0f, 50.0f, 0.0f);

    //draw a diagonal "I"
    glPushMatrix();
    glColor3f(0.839f, 0.153f, 0.157f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-2.5f, -22.5f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(5.0f, 0.0f);
    glVertex2f(0.0f, 45.0f);

    glVertex2f(5.0f, 0.0f);
    glVertex2f(0.0f, 45.0f);
    glVertex2f(5.0f, 45.0f);

    glEnd();
    glPopMatrix();
}

void MyGLWidget::scene_1()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);
    //gluPerspective(45.0f, width() / height(), 1.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.5 * width(), 0.5 * height(), 0.0f);
    //gluLookAt(0.0f, 0.5 * height(), 1000.0f,  // 观察点位置
    //    0.0f, 0.0f, 0.0f,    // 目标点位置
    //    0.0f, 1.0f, 0.0f);   // 上方向


    glPushMatrix();

    // 绘制字母 M
    glColor3f(0.847f, 0.219f, 0.227f);
    glPushMatrix();
    glTranslatef(-200.0f, -50.0f, 0.0f);

    // 左竖和左撇
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-80.0f, 0.0f);
    glVertex2f(-60.0f, 0.0f);
    glVertex2f(-80.0f, 140.0f);
    glVertex2f(-60.0f, 140.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 40.0f);
    glEnd();

    // 右撇和右竖
    glBegin(GL_QUAD_STRIP);
    glVertex2f(80.0f, 0.0f);
    glVertex2f(60.0f, 0.0f);
    glVertex2f(80.0f, 140.0f);
    glVertex2f(60.0f, 140.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 40.0f);
    glEnd();

    glPopMatrix();

    // 绘制字母 Z
    glColor3f(0.953f, 0.823f, 0.4f);
    glPushMatrix();
    glTranslatef(0.0f, -50.0f, 0.0f);

    // 顶部和底部横线
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-80.0f, 140.0f);
    glVertex2f(-80.0f, 120.0f);
    glVertex2f(80.0f, 140.0f);
    glVertex2f(80.0f, 120.0f);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex2f(-80.0f, 20.0f);
    glVertex2f(-80.0f, 0.0f);
    glVertex2f(80.0f, 20.0f);
    glVertex2f(80.0f, 0.0f);
    glEnd();

    // 对角线
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-80.0f, 20.0f);
    glVertex2f(-40.0f, 20.0f);
    glVertex2f(40.0f, 120.0f);
    glVertex2f(80.0f, 120.0f);
    glEnd();

    glPopMatrix();

    // 绘制字母 H
    glColor3f(0.588f, 0.765f, 0.49f);
    glPushMatrix();
    glTranslatef(200.0f, -50.0f, 0.0f);

    // 左竖
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-80.0f, 0.0f);
    glVertex2f(-60.0f, 0.0f);
    glVertex2f(-80.0f, 140.0f);
    glVertex2f(-60.0f, 140.0f);
    glEnd();

    // 中横
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-60.0f, 80.0f);
    glVertex2f(-60.0f, 60.0f);
    glVertex2f(60.0f, 80.0f);
    glVertex2f(60.0f, 60.0f);
    glEnd();

    // 右竖
    glBegin(GL_QUAD_STRIP);
    glVertex2f(60.0f, 0.0f);
    glVertex2f(80.0f, 0.0f);
    glVertex2f(60.0f, 140.0f);
    glVertex2f(80.0f, 140.0f);
    glEnd();

    glPopMatrix();
    glPopMatrix();
}


void MyGLWidget::scene_2() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    // 设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width() / (GLfloat)height(), 1.0f, 2000.0f);

    // 设置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 500.0f, // 眼睛位置
        0.0f, 0.0f, 0.0f,  // 看向点
        0.0f, 1.0f, 0.0f); // 上向量

    // 应用旋转
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); // 绕X轴旋转
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // 绕Y轴旋转
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f); // 绕Z轴旋转

    // Draw the letter M
    float thickness = 40.0f; // Set the Z-axis thickness

    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    // Front face of the letter M
    glBegin(GL_QUADS);
    // Left vertical front
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(-80.0f, 0.0f, thickness / 2);
    glVertex3f(-60.0f, 0.0f, thickness / 2);
    glVertex3f(-60.0f, 140.0f, thickness / 2);
    glVertex3f(-80.0f, 140.0f, thickness / 2);

    // Diagonal front
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(-60.0f, 140.0f, thickness / 2);
    glVertex3f(0.0f, 40.0f, thickness / 2);
    glVertex3f(0.0f, 0.0f, thickness / 2);
    glVertex3f(-60.0f, 100.0f, thickness / 2);

    // Right vertical front
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(60.0f, 0.0f, thickness / 2);
    glVertex3f(80.0f, 0.0f, thickness / 2);
    glVertex3f(80.0f, 140.0f, thickness / 2);
    glVertex3f(60.0f, 140.0f, thickness / 2);

    // Right diagonal front
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(60.0f, 140.0f, thickness / 2);
    glVertex3f(0.0f, 40.0f, thickness / 2);
    glVertex3f(0.0f, 0.0f, thickness / 2);
    glVertex3f(60.0f, 100.0f, thickness / 2);
    glEnd();

    // Back face of the letter M
    glBegin(GL_QUADS);
    // Left vertical back
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(-80.0f, 0.0f, -thickness / 2);
    glVertex3f(-60.0f, 0.0f, -thickness / 2);
    glVertex3f(-60.0f, 140.0f, -thickness / 2);
    glVertex3f(-80.0f, 140.0f, -thickness / 2);

    // Diagonal back
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(-60.0f, 140.0f, -thickness / 2);
    glVertex3f(0.0f, 40.0f, -thickness / 2);
    glVertex3f(0.0f, 0.0f, -thickness / 2);
    glVertex3f(-60.0f, 100.0f, -thickness / 2);

    // Right vertical back
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(60.0f, 0.0f, -thickness / 2);
    glVertex3f(80.0f, 0.0f, -thickness / 2);
    glVertex3f(80.0f, 140.0f, -thickness / 2);
    glVertex3f(60.0f, 140.0f, -thickness / 2);

    // Right diagonal back
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(60.0f, 140.0f, -thickness / 2);
    glVertex3f(0.0f, 40.0f, -thickness / 2);
    glVertex3f(0.0f, 0.0f, -thickness / 2);
    glVertex3f(60.0f, 100.0f, -thickness / 2);
    glEnd();

    // Connect the front and back faces with sides
    glBegin(GL_QUADS);
    // Left vertical side
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(-80.0f, 0.0f, thickness / 2);
    glVertex3f(-80.0f, 0.0f, -thickness / 2);
    glVertex3f(-80.0f, 140.0f, -thickness / 2);
    glVertex3f(-80.0f, 140.0f, thickness / 2);

    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(-60.0f, 0.0f, thickness / 2);
    glVertex3f(-60.0f, 0.0f, -thickness / 2);
    glVertex3f(-60.0f, 140.0f, -thickness / 2);
    glVertex3f(-60.0f, 140.0f, thickness / 2);

    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(-60.0f, 140.0f, thickness / 2);
    glVertex3f(-60.0f, 140.0f, -thickness / 2);
    glVertex3f(-80.0f, 140.0f, -thickness / 2);
    glVertex3f(-80.0f, 140.0f, thickness / 2);

    // Diagonal side
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(-60.0f, 140.0f, thickness / 2);
    glVertex3f(-60.0f, 140.0f, -thickness / 2);
    glVertex3f(0.0f, 40.0f, -thickness / 2);
    glVertex3f(0.0f, 40.0f, thickness / 2);

    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(-80.0f, 0.0f, thickness / 2);
    glVertex3f(-80.0f, 0.0f, -thickness / 2);
    glVertex3f(-60.0f, 0.0f, -thickness / 2);
    glVertex3f(-60.0f, 0.0f, thickness / 2);

    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(0.0f, 0.0f, thickness / 2);
    glVertex3f(0.0f, 0.0f, -thickness / 2);
    glVertex3f(-60.0f, 100.0f, -thickness / 2);
    glVertex3f(-60.0f, 100.0f, thickness / 2);

    // Right vertical side
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(60.0f, 0.0f, thickness / 2);
    glVertex3f(60.0f, 0.0f, -thickness / 2);
    glVertex3f(60.0f, 140.0f, -thickness / 2);
    glVertex3f(60.0f, 140.0f, thickness / 2);

    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(80.0f, 0.0f, thickness / 2);
    glVertex3f(80.0f, 0.0f, -thickness / 2);
    glVertex3f(80.0f, 140.0f, -thickness / 2);
    glVertex3f(80.0f, 140.0f, thickness / 2);

    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(60.0f, 140.0f, thickness / 2);
    glVertex3f(60.0f, 140.0f, -thickness / 2);
    glVertex3f(80.0f, 140.0f, -thickness / 2);
    glVertex3f(80.0f, 140.0f, thickness / 2);

    // Right diagonal side
    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(60.0f, 140.0f, thickness / 2);
    glVertex3f(60.0f, 140.0f, -thickness / 2);
    glVertex3f(0.0f, 40.0f, -thickness / 2);
    glVertex3f(0.0f, 40.0f, thickness / 2);

    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(80.0f, 0.0f, thickness / 2);
    glVertex3f(80.0f, 0.0f, -thickness / 2);
    glVertex3f(60.0f, 0.0f, -thickness / 2);
    glVertex3f(60.0f, 0.0f, thickness / 2);

    glColor3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    glVertex3f(0.0f, 0.0f, thickness / 2);
    glVertex3f(0.0f, 0.0f, -thickness / 2);
    glVertex3f(60.0f, 100.0f, -thickness / 2);
    glVertex3f(60.0f, 100.0f, thickness / 2);

    glEnd();
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
}
