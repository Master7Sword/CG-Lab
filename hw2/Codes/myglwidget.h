#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "utils.h"

#define MAX_Z_BUFFER 99999999.0f
#define MIN_FLOAT 1e-10f

using namespace glm;

class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent* e);

private:
    void scene_0();
    void scene_1();
    void drawTriangle(Triangle triangle);
    int  edge_walking(FragmentAttr* transformedVertices);
    void bresenham(FragmentAttr& start, FragmentAttr& end, int id);
    void DDA(FragmentAttr& start, FragmentAttr& end, int id);
    void clearBuffer(vec3* now_render_buffer);
    void clearBuffer(int* now_buffer);
    void clearZBuffer(float* now_buffer);
    void resizeBuffer(int newW, int newH);
    vec3 PhongShading(FragmentAttr& nowPixelResult);
    vec3 BlinnPhong(FragmentAttr& nowPixelResult);
    void PhongShading(int x, int y, FragmentAttr* transformedVertices, int index);
    void GouraudShading(int x, int y, FragmentAttr* transformedVertices, int index);
    void BlinnPhongShading(int x, int y, FragmentAttr* transformedVertices, int index);

    int WindowSizeH = 0;
    int WindowSizeW = 0;
    int scene_id;
    int degree = 0;

    // buffers
    vec3* render_buffer;
    vec3* temp_render_buffer;
    float* temp_z_buffer;
    float* z_buffer;
    vec2 offset;

    Model objModel;

    vec3 camPosition;
    vec3 camLookAt;
    vec3 camUp;
    mat4 projMatrix;
    vec3 lightPosition;
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);  // °×¹â
    vec3 objectColor = vec3(1.0f, 0.5f, 0.31f); // ³ÈÉ«
    vec3 lineColor = vec3(0.0f, 1.0f, 0.0f); // ÂÌÉ«

    bool use_DDA = true;

    enum ShadingMode { GOURAUD, PHONG, BLINN_PHONG };
    ShadingMode shadingMode = BLINN_PHONG;

    float ambientStrength = 0.1f;
    float specularStrength = 2.0f;
};

#endif // MYGLWIDGET_H
