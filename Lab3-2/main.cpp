//贝塞尔曲面绘制
#include <Windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

static unsigned char ctrlpoint = 'a';
static unsigned char coordinate = 'x';
static GLfloat ctrlpoints[4][4][3] = {
{{ -3, -3, 4.0 },{ -1, -3, 4.0 },{ 1, -3, -2.0 },{ 3, -3, 4.0 }},
{{ -3, -1, 2.0 },{ -1, 3, 4.0 },{ 1, 1, 2},{3,-1,-2}},
{{ -3, 1, 4.0 },{ -1, 1, 2.0 },{ 1,1, 6},{3,-3,3} },
{{ -3,3, -4},{ -1,3,-4.0},{1,1,2},{3,3,-2}}
};

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    //下行的代码用控制点定义Bezier曲面函数
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3); //激活该曲面函数
    glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0); //构造平行投影矩阵 设置正投影空间
}

void display(void)
{
    int i, j;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();//矩阵入栈
    glRotatef(30.0, 1.0, 1.0, 1.0);//旋转一下便于查看
    for (j = 0; j <= 20; j++)
    {
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 20; i++)
            glEvalCoord2f((GLfloat)i / 20.0, (GLfloat)j / 20.0); //调用求值器
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 20; i++)
            glEvalCoord2f((GLfloat)j / 20.0, (GLfloat)i / 20.0); //调用求值器
        glEnd();
    }
    glPopMatrix();//矩阵出栈
    glFlush();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    if (key >= 'a' && key <= 'p')
    {
        ctrlpoint = key;
        printf("当前可改变的是%c点的%c坐标\n请输入a-p以确定要改变点\n输入x，y，z以确定要改变那个坐标\n请输入+或-以增加或减少坐标的值\n\n", ctrlpoint, coordinate);
    }
    else if (key >= 'x' && key <= 'z')
    {
        coordinate = key;
        printf("当前可改变的是%c点的%c坐标\n请输入a-p以确定要改变点\n输入x，y，z以确定要改变那个坐标\n请输入+或-以增加或减少坐标的值\n\n", ctrlpoint, coordinate);
    }
    else if (key == '-' || key == '+') {
        int pointNum = ctrlpoint - 'a';
        int cooNum = coordinate - 'x';
        float add = (key == '-') ? (-0.5) : (0.5);
        if (ctrlpoints[pointNum / 4][pointNum % 4][cooNum] + add > 5.0 || ctrlpoints[pointNum / 4][pointNum % 4][cooNum] + add < -5.0) {
            printf("越界\n\n");
        }
        else {
            ctrlpoints[pointNum / 4][pointNum % 4][cooNum] += add;
            printf("当前可改变的是%c点的%c坐标\n请输入a-p以确定要改变点\n输入x，y，z以确定要改变那个坐标\n请输入+或-以增加或减少坐标的值\n\n", ctrlpoint, coordinate);
            glClearColor(1.0, 1.0, 1.0, 0.0);
            //下行的代码用控制点定义Bezier曲面函数
            glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
            glEnable(GL_MAP2_VERTEX_3); //激活该曲面函数
            glutPostRedisplay();
        }
    }
    else { printf("错误输入\n\n"); }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("贝塞尔曲面的绘制");
    init();
    glutKeyboardFunc(key);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}