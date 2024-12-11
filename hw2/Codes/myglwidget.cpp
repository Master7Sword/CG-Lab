#include "myglwidget.h"
#include <GL/glew.h>
#include <algorithm>
#include <ctime>


MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
}

MyGLWidget::~MyGLWidget()
{
	delete[] render_buffer;
	delete[] temp_render_buffer;
	delete[] temp_z_buffer;
	delete[] z_buffer;
}

void MyGLWidget::resizeBuffer(int newW, int newH) {
	delete[] render_buffer;
	delete[] temp_render_buffer;
	delete[] temp_z_buffer;
	delete[] z_buffer;
	WindowSizeW = newW;
	WindowSizeH = newH;
	render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_z_buffer = new float[WindowSizeH*WindowSizeW];
	z_buffer = new float[WindowSizeH*WindowSizeW];
}

void MyGLWidget::initializeGL()
{
	WindowSizeW = width();
	WindowSizeH = height();
	glViewport(0, 0, WindowSizeW, WindowSizeH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	offset = vec2(WindowSizeH / 2, WindowSizeW / 2);
	// 对定义的数组初始化
	render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_z_buffer = new float[WindowSizeH*WindowSizeW];
	z_buffer = new float[WindowSizeH*WindowSizeW];
	for (int i = 0; i < WindowSizeH*WindowSizeW; i++) {
		render_buffer[i] = vec3(0, 0, 0);
		temp_render_buffer[i] = vec3(0, 0, 0);
		temp_z_buffer[i] = MAX_Z_BUFFER;			
		z_buffer[i] = MAX_Z_BUFFER;
	}
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	
	switch (e->key()) {
		case Qt::Key_0: scene_id = 0;update(); break;
		case Qt::Key_1: scene_id = 1;update(); break;
		case Qt::Key_9: degree += 5;update(); break;
	}
}

void MyGLWidget::paintGL()
{
	clock_t start_time = clock();
	switch (scene_id) {
	case 0: scene_0(); break;
	case 1: scene_1(); break;
	}
	clock_t end_time = clock();

	double render_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000;

	std::cout << "Render time: " << render_time << " ms" << std::endl;
}

void MyGLWidget::clearBuffer(vec3* now_buffer) {
	for (int i = 0; i < WindowSizeH*WindowSizeW; i++) {
		now_buffer[i] = vec3(0,0,0);
	}
}

void MyGLWidget::clearBuffer(int* now_buffer) {
	memset(now_buffer, 0, WindowSizeW * WindowSizeH * sizeof(int));
}

void MyGLWidget::clearZBuffer(float* now_buffer) {
	std::fill(now_buffer,now_buffer+WindowSizeW * WindowSizeH, MAX_Z_BUFFER);
}

// 窗口大小变动后，需要重新生成render_buffer等数组
void MyGLWidget::resizeGL(int w, int h)
{
	resizeBuffer(w, h);
	offset = vec2(WindowSizeW / 2, WindowSizeH / 2);
	clearBuffer(render_buffer);
}

void MyGLWidget::scene_0()
{
	// 选择要加载的model
	objModel.loadModel("./objs/singleTriangle.obj");

	// 自主设置变换矩阵
	camPosition = vec3(100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.x,
		100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.y,
		10 + objModel.centralPoint.z);
	camLookAt = objModel.centralPoint;     // 看向物体中心
	camUp = vec3(0, 1, 0);         // 上方向向量
	projMatrix = glm::perspective(radians(20.0f), 1.0f, 0.1f, 2000.0f);

	// 单一点光源，可以改为数组实现多光源
	lightPosition = objModel.centralPoint + vec3(0,100,100);
	clearBuffer(render_buffer);
	clearZBuffer(z_buffer);
	for (int i = 0; i < objModel.triangleCount; i++) {
		Triangle nowTriangle = objModel.getTriangleByID(i);
		//std::cout << "\nDrawing triangle " << i << std::endl;
		drawTriangle(nowTriangle);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	renderWithTexture(render_buffer,WindowSizeH,WindowSizeW);
}


void MyGLWidget::scene_1()
{
	//objModel.loadModel("./objs/teapot_600.obj");
	objModel.loadModel("./objs/teapot_8000.obj");
	//objModel.loadModel("./objs/rock.obj");
	//objModel.loadModel("./objs/cube.obj");
	//objModel.loadModel("./objs/singleTriangle.obj");
	
	camPosition = vec3(100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.x,
		100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.y,
		10 + objModel.centralPoint.z);

	camLookAt = objModel.centralPoint;     // 看向物体中心
	camUp = vec3(0, 1, 0);         // 上方向向量
	projMatrix = glm::perspective(radians(25.0f), 1.0f, 0.1f, 2000.0f);

	// 单一点光源，可以改为数组实现多光源
	lightPosition = objModel.centralPoint + vec3(0,100,100);
	clearBuffer(render_buffer);
	clearZBuffer(z_buffer);
	for (int i = 0; i < objModel.triangleCount; i++) {
		Triangle nowTriangle = objModel.getTriangleByID(i);
		drawTriangle(nowTriangle);
		//std::cout << "Drawing triangle " << i << std::endl;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	renderWithTexture(render_buffer, WindowSizeH, WindowSizeW);
}


void MyGLWidget::drawTriangle(Triangle triangle) {
	// 三维顶点映射到二维平面
	vec3* vertices = triangle.triangleVertices;
	vec3* normals = triangle.triangleNormals;
	FragmentAttr transformedVertices[3];
	clearBuffer(this->temp_render_buffer);
	clearZBuffer(this->temp_z_buffer);
	mat4 viewMatrix = glm::lookAt(camPosition, camLookAt, camUp);

    for (int i = 0; i < 3; ++i) {
		vec4 ver_mv = viewMatrix * vec4(vertices[i], 1.0f);
		float nowz = glm::length(camPosition - vec3(ver_mv));
		vec4 ver_proj = projMatrix * ver_mv;
		transformedVertices[i].x = ver_proj.x + offset.x;
		transformedVertices[i].y = ver_proj.y + offset.y;
		transformedVertices[i].z = nowz;
		transformedVertices[i].pos_mv = ver_mv;  
		mat3 normalMatrix = mat3(viewMatrix);
		vec3 normal_mv = normalMatrix * normals[i];
		transformedVertices[i].normal = normal_mv;

		transformedVertices[i].color = PhongShading(transformedVertices[i]); 
		//std::cout << transformedVertices[i].color.x << " " << transformedVertices[i].color.y << " " << transformedVertices[i].color.z << std::endl;
    }

	// 将当前三角形渲染在temp_buffer中
		
	// HomeWork: 1、绘制三角形三边
	if (use_DDA) {
		DDA(transformedVertices[0], transformedVertices[1], 1);
		DDA(transformedVertices[1], transformedVertices[2], 2);
		DDA(transformedVertices[2], transformedVertices[0], 3);
	}
	else {
		bresenham(transformedVertices[0], transformedVertices[1], 1);
		bresenham(transformedVertices[1], transformedVertices[2], 2);
		bresenham(transformedVertices[2], transformedVertices[0], 3);
	}


    // HomeWork: 2: 用edge-walking填充三角形内部到temp_buffer中
    int firstChangeLine = edge_walking(transformedVertices);
	//int firstChangeLine = 0;

	// 合并temp_buffer 到 render_buffer, 深度测试
	// 从firstChangeLine开始遍历，可以稍快
	for(int h = firstChangeLine; h < WindowSizeH ; h++){
		auto render_row = &render_buffer[h * WindowSizeW];
		auto temp_render_row = &temp_render_buffer[h * WindowSizeW];
		auto z_buffer_row = &z_buffer[h*WindowSizeW];
		auto temp_z_buffer_row = &temp_z_buffer[h*WindowSizeW];
		for (int i = 0 ; i < WindowSizeW ; i++){
			//std::cout << "z_buffer_row " << i << " : " << z_buffer_row[i] << " temp_z_buffer_row " << i << " : " << temp_z_buffer_row[i] << std::endl;
			if (z_buffer_row[i] < temp_z_buffer_row[i])
				continue;
			else
			{
				//std::cout << "sent to render_row" << std::endl;
				z_buffer_row[i] = temp_z_buffer_row[i];
				render_row[i] = temp_render_row[i];
			}
		}

	}
}


int MyGLWidget::edge_walking(FragmentAttr* transformedVertices) {
	// 存储每一行的边缘像素
	std::vector<std::vector<int>> edge_table(WindowSizeH);
	bool firstFilledLineFound = false;  
	int firstFilledLine = -1;  // 记录第一个填充的行

	for (int y = 0; y < WindowSizeH; ++y) {
		for (int x = 0; x < WindowSizeW; ++x) {
			int index = y * WindowSizeW + x;
			if (temp_render_buffer[index] != vec3(0.0, 0.0, 0.0)) {
				edge_table[y].push_back(x); 
			}
		}
		if (!firstFilledLineFound && edge_table[y].size() > 1) {
			firstFilledLine = y;
			firstFilledLineFound = true;
		}
	}

	// 填充三角形内部区域
	for (int y = 0; y < WindowSizeH; ++y) {
		if (edge_table[y].size() >= 2) {
			std::sort(edge_table[y].begin(), edge_table[y].end());
			for (size_t i = 0; i < edge_table[y].size(); i += 1) {
				if (i + 1 < edge_table[y].size()) {
					int x_start = edge_table[y][i];
					int x_end = edge_table[y][i + 1];

					float z_start = temp_z_buffer[y * WindowSizeW + x_start];
					float z_end = temp_z_buffer[y * WindowSizeW + x_end];

					for (int x = x_start + 1; x < x_end; ++x) { 
						int index = y * WindowSizeW + x;

						// 插值计算深度值
						float t = static_cast<float>(x - x_start) / (x_end - x_start);
						float interpolated_z = (1 - t) * z_start + t * z_end;

						// 插值计算颜色值
						switch (shadingMode) {
							case GOURAUD:
								GouraudShading(x, y, transformedVertices, index);
								break;
							case PHONG:
								PhongShading(x, y, transformedVertices, index);
								break;
							case BLINN_PHONG:
								BlinnPhongShading(x, y, transformedVertices, index);
								break;
						}
						//vec3 interpolated_color = vec3(1.0, 1.0, 1.0); // 暂时直接填充白色
						//temp_render_buffer[index] = interpolated_color;

						temp_z_buffer[index] = interpolated_z;
						
					}
				}
			}
		}
	}
	return firstFilledLine != -1 ? firstFilledLine : 0;
}


void MyGLWidget::bresenham(FragmentAttr& start, FragmentAttr& end, int id) {
	// 根据起点、终点，计算当前边在画布上的像素
	//（可以只考虑都在画布中。加分思考：在画布外怎么处理）
	//vec3 line_color = start.color;
	int x0 = start.x, y0 = start.y, x1 = end.x, y1 = end.y;
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int p0 = (dx > dy ? dx : -dy) / 2;

	float z0 = start.z, z1 = end.z;
	vec3 color0 = start.color, color1 = end.color;

	// 计算总步数，用于比例插值
	int totalSteps = dx > dy ? dx : dy;
	float t = 0.0f, tIncrement = 1.0f / totalSteps;

	while (x0 != x1 || y0 != y1) {
		if (x0 < 0 || x0 >= WindowSizeW || y0 < 0 || y0 >= WindowSizeH) {
			return;  // 在画布外的处理方式：直接忽略
		}

		// 插值计算颜色和深度
		vec3 interpolatedColor = (1 - t) * color0 + t * color1;
		float interpolatedZ = (1 - t) * z0 + t * z1;

		int bufferIndex = y0 * WindowSizeW + x0;
		if (temp_z_buffer[bufferIndex] > interpolatedZ) {
			temp_z_buffer[bufferIndex] = interpolatedZ;
			temp_render_buffer[bufferIndex] = interpolatedColor;
			//temp_render_buffer[bufferIndex] = lineColor;
		}

		int pk = p0;
		if (pk > -dx) {
			p0 -= dy;
			x0 += sx;
		}
		if (pk < dy) {
			p0 += dx;
			y0 += sy;
		}

		t += tIncrement;
	}
}

// ##############################################################
// ## 函数：DDA
// ## 函数描述：使用DDA算法绘制线段
// ## 参数描述：
// ##	FragmentAttr& start：线段起始点
// ##	FragmentAttr& end：线段结束点
// ##	int id：线段id（保留参数，暂未使用）
// ##############################################################
void MyGLWidget::DDA(FragmentAttr& start, FragmentAttr& end, int id) {
	float x0 = start.x, y0 = start.y;
	float x1 = end.x, y1 = end.y;

	float dx = x1 - x0;
	float dy = y1 - y0;

	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	float xIncrement = dx / steps;
	float yIncrement = dy / steps;

	float z0 = start.z, z1 = end.z;
	vec3 color0 = start.color, color1 = end.color;

	float zIncrement = (z1 - z0) / steps;
	vec3 colorIncrement = (color1 - color0) / static_cast<float>(steps);

	float x = x0;
	float y = y0;
	float z = z0;
	vec3 color = color0;

	for (int i = 0; i <= steps; i++) {
		int xi = round(x);
		int yi = round(y);

		if (xi >= 0 && xi < WindowSizeW && yi >= 0 && yi < WindowSizeH) {
			int bufferIndex = yi * WindowSizeW + xi;
			if (temp_z_buffer[bufferIndex] > z) {
				temp_z_buffer[bufferIndex] = z;
				temp_render_buffer[bufferIndex] = color;
				//temp_render_buffer[bufferIndex] = lineColor;
			}
		}

		x += xIncrement;
		y += yIncrement;
		z += zIncrement;
		color += colorIncrement;
	}
}


// ##############################################################
// ## 函数：GouraudShading
// ## 函数描述：使用Gouraud着色方法计算像素的颜色
// ## 参数描述：
// ##    int x：像素的x坐标
// ##    int y：像素的y坐标
// ##    FragmentAttr* v：顶点属性数组（包含三个顶点的信息）
// ##    int index：渲染缓冲区的索引
// ##############################################################
void MyGLWidget::GouraudShading(int x, int y, FragmentAttr* v, int index) {
	// 重心坐标
	float alpha = (((float)(y - v[1].y) * (v[2].x - v[1].x) - (float)(x - v[1].x) * (v[2].y - v[1].y)) /
				  ((float)(v[0].y - v[1].y) * (v[2].x - v[1].x) - (float)(v[0].x - v[1].x) * (v[2].y - v[1].y)));
	float beta  = ((float)(y - v[2].y) * (v[0].x - v[2].x) - (float)(x - v[2].x) * (v[0].y - v[2].y)) /
				  ((float)(v[1].y - v[2].y) * (v[0].x - v[2].x) - (float)(v[1].x - v[2].x) * (v[0].y - v[2].y));
	float gamma = 1 - alpha - beta;

	vec3 color = alpha * v[0].color + beta * v[1].color + gamma * v[2].color;
	temp_render_buffer[index] = color;
}

// ##############################################################
// ## 函数：PhongShading（版本1）
// ## 函数描述：计算给定像素的颜色值，使用Phong着色方法
// ## 参数描述：
// ##    FragmentAttr& nowPixelResult：当前像素的属性，包括法线、位置等
// ## 返回值：
// ##    vec3：计算得出的颜色值
// ##############################################################
vec3 MyGLWidget::PhongShading(FragmentAttr& nowPixelResult) {

	vec3 norm = glm::normalize(nowPixelResult.normal);
	vec3 lightDir = glm::normalize(lightPosition - nowPixelResult.pos_mv);
	vec3 viewDir = glm::normalize(camPosition - nowPixelResult.pos_mv);
	vec3 reflectDir = glm::reflect(-lightDir, norm);

	vec3 ambient = ambientStrength * lightColor;

	float diff = glm::max(glm::dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 16);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 color = (ambient + diffuse + specular) * objectColor;
	return color;
}

// ##############################################################
// ## 函数：PhongShading（版本2）
// ## 函数描述：对屏幕像素使用Phong着色方法并更新渲染缓冲区
// ## 参数描述：
// ##    int x：像素的x坐标
// ##    int y：像素的y坐标
// ##    FragmentAttr* v：顶点属性数组（包含三个顶点的信息）
// ##    int index：渲染缓冲区的索引
// ##############################################################
void MyGLWidget::PhongShading(int x, int y, FragmentAttr* v, int index) {
	// 重心坐标
	float alpha = (((float)(y - v[1].y) * (v[2].x - v[1].x) - (float)(x - v[1].x) * (v[2].y - v[1].y)) /
		((float)(v[0].y - v[1].y) * (v[2].x - v[1].x) - (float)(v[0].x - v[1].x) * (v[2].y - v[1].y)));
	float beta = ((float)(y - v[2].y) * (v[0].x - v[2].x) - (float)(x - v[2].x) * (v[0].y - v[2].y)) /
		((float)(v[1].y - v[2].y) * (v[0].x - v[2].x) - (float)(v[1].x - v[2].x) * (v[0].y - v[2].y));
	float gamma = 1 - alpha - beta;

	vec3 interpolatedNormal = alpha * v[0].normal + beta * v[1].normal + gamma * v[2].normal;
	vec3 interpolatedPosMV = alpha * vec3(v[0].pos_mv) +
							 beta  * vec3(v[1].pos_mv) +
							 gamma * vec3(v[2].pos_mv);

	FragmentAttr nowPixelResult(x, y, 0.0f, 0);
	nowPixelResult.normal = glm::normalize(interpolatedNormal);
	nowPixelResult.pos_mv = interpolatedPosMV;

	vec3 finalColor = PhongShading(nowPixelResult); 
	temp_render_buffer[index] = finalColor;
}

// ##############################################################
// ## 函数：BlinnPhong
// ## 函数描述：计算给定像素的颜色值，使用Blinn-Phong着色方法
// ## 参数描述：
// ##    FragmentAttr& nowPixelResult：当前像素的属性，包括法线、位置等
// ## 返回值：
// ##    vec3：计算得出的颜色值
// ##############################################################
vec3 MyGLWidget::BlinnPhong(FragmentAttr& nowPixelResult) {

	vec3 norm = glm::normalize(nowPixelResult.normal);
	vec3 lightDir = glm::normalize(lightPosition - nowPixelResult.pos_mv);
	vec3 viewDir = glm::normalize(camPosition - nowPixelResult.pos_mv);

	vec3 halfDir = glm::normalize(lightDir + viewDir);

	vec3 ambient = ambientStrength * lightColor;

	float diff = glm::max(glm::dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	float spec = glm::pow(glm::max(glm::dot(norm, halfDir), 0.0f), 16);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 color = (ambient + diffuse + specular) * objectColor;
	return color;
}

// ##############################################################
// ## 函数：BlinnPhongShading
// ## 函数描述：对屏幕像素使用Blinn-Phong着色方法并更新渲染缓冲区
// ## 参数描述：
// ##    int x：像素的x坐标
// ##    int y：像素的y坐标
// ##    FragmentAttr* v：顶点属性数组（包含三个顶点的信息）
// ##    int index：渲染缓冲区的索引
// ##############################################################
void MyGLWidget::BlinnPhongShading(int x, int y, FragmentAttr* v, int index) {
	// 重心坐标
	float alpha = (((float)(y - v[1].y) * (v[2].x - v[1].x) - (float)(x - v[1].x) * (v[2].y - v[1].y)) /
		((float)(v[0].y - v[1].y) * (v[2].x - v[1].x) - (float)(v[0].x - v[1].x) * (v[2].y - v[1].y)));
	float beta = ((float)(y - v[2].y) * (v[0].x - v[2].x) - (float)(x - v[2].x) * (v[0].y - v[2].y)) /
		((float)(v[1].y - v[2].y) * (v[0].x - v[2].x) - (float)(v[1].x - v[2].x) * (v[0].y - v[2].y));
	float gamma = 1 - alpha - beta;

	vec3 interpolatedNormal = alpha * v[0].normal + beta * v[1].normal + gamma * v[2].normal;
	vec3 interpolatedPosMV = alpha * vec3(v[0].pos_mv) +
							 beta  * vec3(v[1].pos_mv) +
							 gamma * vec3(v[2].pos_mv);

	FragmentAttr nowPixelResult(x, y, 0.0f, 0);  
	nowPixelResult.normal = glm::normalize(interpolatedNormal);
	nowPixelResult.pos_mv = interpolatedPosMV;

	vec3 finalColor = BlinnPhong(nowPixelResult); 
	temp_render_buffer[index] = finalColor;
}

