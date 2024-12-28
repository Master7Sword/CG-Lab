#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "utils.h"


#define MAX_Z_BUFFER 99999999.0f
#define MIN_FLOAT 1e-10f

using namespace glm;

class MyGLWidget : public QOpenGLWidget, public QOpenGLExtraFunctions{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent* e);

private:
    void scene_0();

    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;

    int WindowSizeH = 0;
    int WindowSizeW = 0;
    float degree = 0;

    Model objModel;

    vec3 camPosition;
    vec3 camLookAt;
    vec3 camUp;
    mat4 projMatrix;
    vec3 lightPosition;
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);  // °×¹â
    vec3 objectColor = vec3(1.0f, 0.753f, 0.796f); // ³ÈÉ«

    std::vector<unsigned int> indices; // index array
};

#endif // MYGLWIDGET_H
