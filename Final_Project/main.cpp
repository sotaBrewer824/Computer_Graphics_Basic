#include <Windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace std;

static float _angle = 30.0f;
static float _cameraAngle = 0.0f;

static int oldX;
static int oldY;
static float _rotX = 0;
static float _rotY = 0;
static float xyz[3] = { 1,1,1 };
static float r = 200;

static GLUquadric* _earth;

#define BMP_Header_Length 54  //图像数据在内存块中的偏移量

//定义纹理对象编号
GLuint texEarth;

//按按键操作
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0); break;
		//按上则放大
	case 'w':
		//xyz[0] += 0.1;xyz[1] += 0.1;xyz[2] += 0.1;
		r += 10;
		glutPostRedisplay(); break;
		//按下则缩小
	case 's':
		//xyz[0] -= 0.1;xyz[1] -= 0.1;xyz[2] -= 0.1;
		r -= 10;
		glutPostRedisplay(); break;
	}
}

//定义光照
void initLight()
{
	GLfloat	ambientLight[] = { 0.2,0.2,0.2,1 };
	GLfloat diffuseLight[] = { 0.8,.8,0.8,1 };
	GLfloat specularLight[] = { 0.5,0.5,0.5,1 };
	GLfloat posLight[] = { 400,240,1,1 };
	GLfloat	specref[] = { 1,1,1,1 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0,GL_POSITION,posLight); //指定光源位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
}

// 函数power_of_two用于判断一个整数是不是2的整数次幂
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// 打开文件，如果失败，返回
	FILE* pFile;
	int err = fopen_s(&pFile, file_name, "rb");
	if (err != 0) { return 0; }

	// 读取文件中图象的宽度和高度
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// 根据总像素字节数分配内存
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// 读取像素数据
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
	// 若图像宽高超过了OpenGL规定的最大值，也缩放
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// 计算每行需要的字节数和总字节数
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// 分配内存
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// 进行像素缩放
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// 分配一个新的纹理编号
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
	free(pixels);
	return texture_ID;
}


//定义D建模光照材质
void initRendering() {
	//创建二次曲面对象
	_earth = gluNewQuadric();

	//初始化光照
	initLight();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);//镜面亮点
}

//删除建模
void deinitRendering()
{
	gluDeleteQuadric(_earth);
}

//窗口调整调用
void handleResize(int w, int h) {
	if (h == 0)
	{
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -200, 200);
	glMatrixMode(GL_MODELVIEW);
}

//画地球
void drawEarth()
{
	static bool loaded = false;
	//纹理绑定到目标
	glBindTexture(GL_TEXTURE_2D, texEarth);
	if (!loaded)
	{
		//纹理坐标自动生成	
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		//表面生成纹理坐标
		gluQuadricDrawStyle(_earth, GL_FILL);
		gluQuadricNormals(_earth, GLU_SMOOTH);
		gluQuadricTexture(_earth, GL_TRUE);

	}
	//生成球体
	glPushMatrix();
	{
		glEnable(GL_TEXTURE_2D);
		glRotatef(-90, 1, 0, 0);
		gluSphere(_earth, r, 100, 100);
		glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();

}

//绘制D场景
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //切换到绘图透视图
	glLoadIdentity(); //重置绘图透视图

	glPushMatrix();
	//实现缩放
	glScalef(xyz[0], xyz[1], xyz[2]);
	//此处需要把地球往后方移动防止放大的时候中间出现黑色圆圈
	glTranslatef(400, 240, -200);
	//实现拖动旋转
	glRotatef(_rotX / 100, 0, 1, 0);
	glRotatef(-_rotY / 100, 1, 0, 0);
	//实现自动旋转
	glRotatef(_angle, 0, 1, 0);
	drawEarth();
	glPopMatrix();

	glutSwapBuffers();
}

//实现自动旋转效果
void update(int value) {
	_angle += 2.0f;
	if (_angle > 360) {
		_angle -= 360;
	}

	//重画
	glutPostRedisplay();

	//60FPS
	glutTimerFunc(16, update, 0);
}

//处理鼠标点击旋转事件
//处理鼠标点击滑动事件
void handleMotion(int x, int y)
{
	int rx = x - oldX;
	int ry = 480 - y - oldY;
	_rotX += rx;
	_rotY += ry;
	//重画
	glutPostRedisplay();
}

//处理鼠标点击事件
void handleMouse(int button, int state, int x, int y)
{
	static bool done = false;
	if (button == GLUT_LEFT_BUTTON)
	{
		oldX = x;
		oldY = 480 - y;
	}
}

int main(int argc, char** argv) {
	//初始化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 480);

	//创建窗口
	glutCreateWindow("地球仪");
	initRendering();

	//设置用户操作
	texEarth = load_texture("D:\\CS\\CSDIY\\Computer_Graphics_XJTU\\earth.bmp");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMotionFunc(handleMotion);
	glutMouseFunc(handleMouse);

	//运动效果
	glutTimerFunc(16, update, 0);

	glutMainLoop();
	//退出时删除建模
	deinitRendering();
	return 0;
}