#include "myglwidget.h"


const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aPos; // 顶点位置
layout(location = 1) in vec3 aNormal; // 法向量

out vec3 FragPos;  // 片段位置
out vec3 Normal;   // 法线

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); 
    Normal = mat3(transpose(inverse(model))) * aNormal;  

    gl_Position = projection * view * vec4(FragPos, 1.0);  
}
)";

const char* fragmentShaderSource = R"(
#version 330 core

in vec3 FragPos;      // 从顶点着色器传递的片段位置
in vec3 Normal;       // 从顶点着色器传递的法线
out vec4 FragColor;   // 输出颜色

uniform vec3 lightPos;    // 光源位置
uniform vec3 viewPos;     // 观察者位置
uniform vec3 lightColor;  // 光源颜色
uniform vec3 objectColor; // 物体颜色

void main()
{
    // 环境光
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 镜面反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // 最终颜色
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";


MyGLWidget::MyGLWidget(QWidget *parent) :QOpenGLWidget(parent){}

MyGLWidget::~MyGLWidget() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}

void MyGLWidget::keyPressEvent(QKeyEvent* e) {

    switch (e->key()) {
        case Qt::Key_9: degree += 0.1; update(); break;
    }
}


void MyGLWidget::initializeGL() {
    WindowSizeW = width();
    WindowSizeH = height();

    // 初始化 OpenGL
    initializeOpenGLFunctions();

    // 设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);

    // 编译和链接着色器程序
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // 检查顶点着色器编译
    GLint success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // 检查片段着色器编译
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 检查着色器程序链接
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 加载模型
    scene_0();

    // 创建顶点数据列表
    std::vector<float> vertexData;
    for (int i = 0; i < objModel.triangleCount; ++i) {
        for (int j = 0; j < 3; ++j) {
            int vertexIndex = objModel.triangles[i][j] ; 
            int normalIndex = objModel.triangle_normals[i][j] ;
            vertexData.push_back(objModel.vertices_data[vertexIndex].x);
            vertexData.push_back(objModel.vertices_data[vertexIndex].y);
            vertexData.push_back(objModel.vertices_data[vertexIndex].z);
            vertexData.push_back(objModel.normals_data[normalIndex].x);
            vertexData.push_back(objModel.normals_data[normalIndex].y);
            vertexData.push_back(objModel.normals_data[normalIndex].z);
        }
        indices.push_back(i * 3);
        indices.push_back(i * 3 + 1);
        indices.push_back(i * 3 + 2);
    }

    // 创建 VAO 和 VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // 将顶点数据传输到缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // 将索引数据传输到缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void MyGLWidget::paintGL() {
    clock_t start_time = clock();

    // 清除颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 使用着色器程序
    glUseProgram(shaderProgram);

    // 设置 uniform 变量
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    GLuint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");

    mat4 model = mat4(1.0f);
    model = glm::rotate(model, (float)degree, vec3(0.0f, 1.0f, 0.0f)); // 旋转模型
    model = glm::scale(model, vec3(1.0f)); // yomiya : 100.0f
    mat4 view = glm::lookAt(camPosition, camLookAt, camUp);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPosition));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(camPosition));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));

    // 绘制物体
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, objModel.triangleCount * 3);
    glDrawElements(GL_TRIANGLES, objModel.triangleCount * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    clock_t end_time = clock();
    double render_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000;
    std::cout << "Render time: " << render_time << " ms" << std::endl;
}



void MyGLWidget::scene_0()
{
	// 选择要加载的model
    //objModel.loadModel("./objs/teapot_600.obj");
    objModel.loadModel("./objs/teapot_8000.obj");
    //objModel.loadModel("./objs/rock.obj");
    //objModel.loadModel("./objs/cube.obj");
	//objModel.loadModel("./objs/singleTriangle.obj");
    //objModel.loadModel("./objs/yomiya.obj");
    //objModel.loadModel("./objs/tighnari.obj");

   /* for (int i = 0; i < objModel.verticesCount; ++i) {
        std::cout << "Vertex " << i << ": " << objModel.vertices_data[i].x << ", "
            << objModel.vertices_data[i].y << ", " << objModel.vertices_data[i].z << std::endl;
    }*/
    //std::cout << "central point: (" << objModel.centralPoint.x << ", " << objModel.centralPoint.y << ", " << objModel.centralPoint.z << ")\n";

	// 自主设置变换矩阵
	camPosition = vec3(objModel.centralPoint.x + 300 * cos(degree * 3.14 / 180.0), 100 + objModel.centralPoint.y, objModel.centralPoint.z + 300 * sin(degree * 3.14 / 180.0));
	camLookAt = objModel.centralPoint + vec3(0.0f, 0.0f, 0.0f);     // tighnari: y + 100
	camUp = vec3(0, 1, 0);         // 上方向向量
	projMatrix = glm::perspective(radians(35.0f), float(WindowSizeW)/float(WindowSizeH), 0.1f, 2000.0f);

	lightPosition = objModel.centralPoint + vec3(0,100,100);

}
