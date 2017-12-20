#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

void affichage(){

    float xA,yA,xB,yB,m,p,y;

    xA = 100;
    yA = 60;
    xB = 200;
    yB = 90;
    m = (yB - yA)/(xB - xA);
    p = yA -m * xA;
    glClear(GL_COLOR_BUFFER_BIT);
    while (xA < xB)
    {
        y = floor(m * xA + p);
        glBegin(GL_POINTS);
        glColor3f(1.0,0.0,0.0);
        glVertex2f(xA, y);
        glEnd();
        xA = xA + 1;
    }
    glFlush();
}

/* Programme principal */
int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("Drawing window");

    gluOrtho2D(-400.0,400.0,-400.0,400.0);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);

    glPointSize(2.0);
    glutDisplayFunc(affichage);
    glutMainLoop();
    return 0;
}