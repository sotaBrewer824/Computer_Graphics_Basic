//ʹ�� glTranslatef()������ʵ��2Dͼ��ƽ��
#include <Windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

static int day = 200; // day�ı仯����0��359
static int day0 = 0; // day�ı仯����0��359
static GLfloat height1 = 0.4f;
static GLfloat height2 = -0.4f;
static GLfloat step = -0.00001f;
// ƽ��
void myDisplayTranslate(void)
{
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0.00001, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(-0.3f, 0.2f, -0.1f, 0.4f);
    glFlush();
    glutSwapBuffers();
}

void myIdleTranslate(void)
{
    /* �µĺ������ڿ���ʱ���ã������ǰ�day��1�����»��ƣ��ﵽ����Ч�� */
    ++day;
    if (day >= 360)
        day = 0;
    myDisplayTranslate();
}

// ��ת
void myDisplayRotate(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(0.005, 0, 0, -1);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.4, -0.7);
    glVertex2f(-0.8, 0.0);
    glVertex2f(-0.4, 0.7);
    glVertex2f(0.4, 0.7);
    glVertex2f(0.8, 0.0);
    glVertex2f(0.4, -0.7);
    glEnd();
    glFlush();
    glutSwapBuffers();
}
void myIdleRotate(void)
{
    /* �µĺ������ڿ���ʱ���ã������ǰ�day��1�����»��ƣ��ﵽ����Ч�� */
    ++day;
    if (day >= 360)
        day = 0;
    myDisplayRotate();
}
// ����
void myDisplayScale(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glScalef(0.99999f, 0.99999f, 0.99999f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.4, -0.7);
    glVertex2f(-0.8, 0.0);
    glVertex2f(-0.4, 0.7);
    glVertex2f(0.4, 0.7);
    glVertex2f(0.8, 0.0);
    glVertex2f(0.4, -0.7);
    glEnd();
    glFlush();
    glutSwapBuffers();
}

void myIdleScale(void)
{
    /* �µĺ������ڿ���ʱ���ã������ǰ�day��1�����»��ƣ��ﵽ����Ч�� */
    ++day;
    if (day >= 360)
        day = 0;
    myDisplayScale();
}
// �ۺ�Ӧ��
void myDisplay(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPointSize(3);//һ����ռ����������
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);//��ʼ��ͼ
    glVertex2f(0.0f, height1 += -step);
    glVertex2f(0.0f, height2 += -step);
    glEnd();

    glTranslatef(0.0f, step, 0.0f);
    glRotatef(0.005, 0, -1, 0);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.35f, 0.2f);//���������������
    glVertex2f(-0.35f, 0.4f);
    glVertex2f(0.0f, 0.4f);
    glVertex2f(0.0f, 0.2f);
    glEnd();

    glFlush();
    glutSwapBuffers();
}

void myIdle(void)
{
    /* �µĺ������ڿ���ʱ���ã������ǰ�day��1�����»��ƣ��ﵽ����Ч�� */
    ++day0;
    if (day0 >= 60000)
        day0 = 0, step = -step;
    myDisplay();
}



int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // �޸��˲���ΪGLUT_DOUBLE
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);

    glutCreateWindow("ʵ��1");   
    // ƽ��
    glutDisplayFunc(&myDisplayTranslate);
    glutIdleFunc(&myIdleTranslate);
    // ��ת
    glutDisplayFunc(&myDisplayRotate);
    glutIdleFunc(&myIdleRotate);
    // ����
    glutDisplayFunc(&myDisplayScale);
    glutIdleFunc(&myIdleScale);
    // �ۺ�Ӧ��
    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);
    glutMainLoop();
    return 0;
}