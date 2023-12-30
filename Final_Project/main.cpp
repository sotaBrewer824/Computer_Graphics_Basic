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

#define BMP_Header_Length 54  //ͼ���������ڴ���е�ƫ����

//�������������
GLuint texEarth;

//����������
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0); break;
		//������Ŵ�
	case 'w':
		//xyz[0] += 0.1;xyz[1] += 0.1;xyz[2] += 0.1;
		r += 10;
		glutPostRedisplay(); break;
		//��������С
	case 's':
		//xyz[0] -= 0.1;xyz[1] -= 0.1;xyz[2] -= 0.1;
		r -= 10;
		glutPostRedisplay(); break;
	}
}

//�������
void initLight()
{
	GLfloat	ambientLight[] = { 0.2,0.2,0.2,1 };
	GLfloat diffuseLight[] = { 0.8,.8,0.8,1 };
	GLfloat specularLight[] = { 0.5,0.5,0.5,1 };
	GLfloat posLight[] = { 400,240,1,1 };
	GLfloat	specref[] = { 1,1,1,1 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0,GL_POSITION,posLight); //ָ����Դλ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
}

// ����power_of_two�����ж�һ�������ǲ���2����������
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

/* ����load_texture
* ��ȡһ��BMP�ļ���Ϊ����
* ���ʧ�ܣ�����0������ɹ�������������
*/
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// ���ļ������ʧ�ܣ�����
	FILE* pFile;
	int err = fopen_s(&pFile, file_name, "rb");
	if (err != 0) { return 0; }

	// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// �����������ֽ��������ڴ�
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// ��ȡ��������
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// �Ծ;ɰ汾�ļ��ݣ����ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
	// ��ͼ���߳�����OpenGL�涨�����ֵ��Ҳ����
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// ����ÿ����Ҫ���ֽ��������ֽ���
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// �����ڴ�
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// ������������
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// ����һ���µ�������
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// ���µ������������������������
	// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ�������
	free(pixels);
	return texture_ID;
}


//����D��ģ���ղ���
void initRendering() {
	//���������������
	_earth = gluNewQuadric();

	//��ʼ������
	initLight();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);//��������
}

//ɾ����ģ
void deinitRendering()
{
	gluDeleteQuadric(_earth);
}

//���ڵ�������
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

//������
void drawEarth()
{
	static bool loaded = false;
	//����󶨵�Ŀ��
	glBindTexture(GL_TEXTURE_2D, texEarth);
	if (!loaded)
	{
		//���������Զ�����	
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		//����������������
		gluQuadricDrawStyle(_earth, GL_FILL);
		gluQuadricNormals(_earth, GLU_SMOOTH);
		gluQuadricTexture(_earth, GL_TRUE);

	}
	//��������
	glPushMatrix();
	{
		glEnable(GL_TEXTURE_2D);
		glRotatef(-90, 1, 0, 0);
		gluSphere(_earth, r, 100, 100);
		glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();

}

//����D����
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //�л�����ͼ͸��ͼ
	glLoadIdentity(); //���û�ͼ͸��ͼ

	glPushMatrix();
	//ʵ������
	glScalef(xyz[0], xyz[1], xyz[2]);
	//�˴���Ҫ�ѵ��������ƶ���ֹ�Ŵ��ʱ���м���ֺ�ɫԲȦ
	glTranslatef(400, 240, -200);
	//ʵ���϶���ת
	glRotatef(_rotX / 100, 0, 1, 0);
	glRotatef(-_rotY / 100, 1, 0, 0);
	//ʵ���Զ���ת
	glRotatef(_angle, 0, 1, 0);
	drawEarth();
	glPopMatrix();

	glutSwapBuffers();
}

//ʵ���Զ���תЧ��
void update(int value) {
	_angle += 2.0f;
	if (_angle > 360) {
		_angle -= 360;
	}

	//�ػ�
	glutPostRedisplay();

	//60FPS
	glutTimerFunc(16, update, 0);
}

//�����������ת�¼�
//��������������¼�
void handleMotion(int x, int y)
{
	int rx = x - oldX;
	int ry = 480 - y - oldY;
	_rotX += rx;
	_rotY += ry;
	//�ػ�
	glutPostRedisplay();
}

//����������¼�
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
	//��ʼ��
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 480);

	//��������
	glutCreateWindow("������");
	initRendering();

	//�����û�����
	texEarth = load_texture("D:\\CS\\CSDIY\\Computer_Graphics_XJTU\\earth.bmp");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMotionFunc(handleMotion);
	glutMouseFunc(handleMouse);

	//�˶�Ч��
	glutTimerFunc(16, update, 0);

	glutMainLoop();
	//�˳�ʱɾ����ģ
	deinitRendering();
	return 0;
}