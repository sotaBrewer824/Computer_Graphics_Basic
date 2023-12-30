//�������������
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
    //���еĴ����ÿ��Ƶ㶨��Bezier���溯��
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3); //��������溯��
    glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0); //����ƽ��ͶӰ���� ������ͶӰ�ռ�
}

void display(void)
{
    int i, j;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();//������ջ
    glRotatef(30.0, 1.0, 1.0, 1.0);//��תһ�±��ڲ鿴
    for (j = 0; j <= 20; j++)
    {
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 20; i++)
            glEvalCoord2f((GLfloat)i / 20.0, (GLfloat)j / 20.0); //������ֵ��
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 20; i++)
            glEvalCoord2f((GLfloat)j / 20.0, (GLfloat)i / 20.0); //������ֵ��
        glEnd();
    }
    glPopMatrix();//�����ջ
    glFlush();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    if (key >= 'a' && key <= 'p')
    {
        ctrlpoint = key;
        printf("��ǰ�ɸı����%c���%c����\n������a-p��ȷ��Ҫ�ı��\n����x��y��z��ȷ��Ҫ�ı��Ǹ�����\n������+��-�����ӻ���������ֵ\n\n", ctrlpoint, coordinate);
    }
    else if (key >= 'x' && key <= 'z')
    {
        coordinate = key;
        printf("��ǰ�ɸı����%c���%c����\n������a-p��ȷ��Ҫ�ı��\n����x��y��z��ȷ��Ҫ�ı��Ǹ�����\n������+��-�����ӻ���������ֵ\n\n", ctrlpoint, coordinate);
    }
    else if (key == '-' || key == '+') {
        int pointNum = ctrlpoint - 'a';
        int cooNum = coordinate - 'x';
        float add = (key == '-') ? (-0.5) : (0.5);
        if (ctrlpoints[pointNum / 4][pointNum % 4][cooNum] + add > 5.0 || ctrlpoints[pointNum / 4][pointNum % 4][cooNum] + add < -5.0) {
            printf("Խ��\n\n");
        }
        else {
            ctrlpoints[pointNum / 4][pointNum % 4][cooNum] += add;
            printf("��ǰ�ɸı����%c���%c����\n������a-p��ȷ��Ҫ�ı��\n����x��y��z��ȷ��Ҫ�ı��Ǹ�����\n������+��-�����ӻ���������ֵ\n\n", ctrlpoint, coordinate);
            glClearColor(1.0, 1.0, 1.0, 0.0);
            //���еĴ����ÿ��Ƶ㶨��Bezier���溯��
            glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
            glEnable(GL_MAP2_VERTEX_3); //��������溯��
            glutPostRedisplay();
        }
    }
    else { printf("��������\n\n"); }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("����������Ļ���");
    init();
    glutKeyboardFunc(key);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}