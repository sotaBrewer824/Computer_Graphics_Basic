//使用 glTranslatef()函数，实现2D图形平移
#include <Windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

static int day = 200; // day的变化：从0到359
static int day0 = 0; // day的变化：从0到359
static GLfloat height1 = 0.4f;
static GLfloat height2 = -0.4f;
static GLfloat step = -0.00001f;
// 平移
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
    /* 新的函数，在空闲时调用，作用是把day加1并重新绘制，达到动画效果 */
    ++day;
    if (day >= 360)
        day = 0;
    myDisplayTranslate();
}

// 旋转
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
    /* 新的函数，在空闲时调用，作用是把day加1并重新绘制，达到动画效果 */
    ++day;
    if (day >= 360)
        day = 0;
    myDisplayRotate();
}
// 缩放
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
    /* 新的函数，在空闲时调用，作用是把day加1并重新绘制，达到动画效果 */
    ++day;
    if (day >= 360)
        day = 0;
    myDisplayScale();
}
// 综合应用
void myDisplay(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPointSize(3);//一个点占据三个像素
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);//开始画图
    glVertex2f(0.0f, height1 += -step);
    glVertex2f(0.0f, height2 += -step);
    glEnd();

    glTranslatef(0.0f, step, 0.0f);
    glRotatef(0.005, 0, -1, 0);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.35f, 0.2f);//三角旗与矩形旗帜
    glVertex2f(-0.35f, 0.4f);
    glVertex2f(0.0f, 0.4f);
    glVertex2f(0.0f, 0.2f);
    glEnd();

    glFlush();
    glutSwapBuffers();
}

void myIdle(void)
{
    /* 新的函数，在空闲时调用，作用是把day加1并重新绘制，达到动画效果 */
    ++day0;
    if (day0 >= 60000)
        day0 = 0, step = -step;
    myDisplay();
}



int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // 修改了参数为GLUT_DOUBLE
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);

    glutCreateWindow("实验1");   
    // 平移
    glutDisplayFunc(&myDisplayTranslate);
    glutIdleFunc(&myIdleTranslate);
    // 旋转
    glutDisplayFunc(&myDisplayRotate);
    glutIdleFunc(&myIdleRotate);
    // 缩放
    glutDisplayFunc(&myDisplayScale);
    glutIdleFunc(&myIdleScale);
    // 综合应用
    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);
    glutMainLoop();
    return 0;
}