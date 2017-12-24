#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include <stdbool.h>

#define WIN_H 800
#define WIN_W 800

// ---------------------------
// -------- VARIABLES --------
// ---------------------------
typedef int OutCode;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Variable Menu
static int window;
static int menu_id;
static int submenu_id;
static int submenu_square_id;
static int value = 0;
static int colorValue = 0;

int isFirstClick = 0;
int isSecondClick = 0;
int isThirdClick = 0;

// Variable coordonnée click
int xA;
int yA;
int xB;
int yB;
int xC;
int yC;

int xmin = -200;
int xmax = 200;
int ymin = -200;
int ymax = 200;

// ---------------------------
// -------- FONCTIONS --------
// ---------------------------
OutCode ComputeOutCode(int x, int y)
{
    OutCode code;

    code = INSIDE;          // initialised as being inside of [[clip window]]

    if (x < xmin)           // to the left of clip window
        code |= LEFT;
    else if (x > xmax)      // to the right of clip window
        code |= RIGHT;
    if (y < ymin)           // below the clip window
        code |= BOTTOM;
    else if (y > ymax)      // above the clip window
        code |= TOP;

    return code;
}



void deleteCoord(){
    xA = 0;
    yA = 0;

    xB = 0;
    yB = 0;

    xC = 0;
    yC = 0;
}

// Récupère la valeur du menu
void menu(int num){
    value = num;
    glutPostRedisplay();
}

// Menu Glut
void createMenu(){

    submenu_square_id = glutCreateMenu(menu);
    glutAddMenuEntry("200x200", 3);
    glutAddMenuEntry("400x400", 4);
    glutAddMenuEntry("600,600", 5);


    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Segment", 2);
    glutAddSubMenu("Carre", submenu_square_id);
    glutAddMenuEntry("Rectangle",6);
    glutAddMenuEntry("Triangle", 7);
    glutAddMenuEntry("Cercle", 8);
    glutAddMenuEntry("Arc de cercle", 9);
    glutAddMenuEntry("Ellipse 1", 12);
    glutAddMenuEntry("Ellipse 2", 13);
    glutAddMenuEntry("Fenetrage simple (Cohen-Sutherland) : Segment", 10);
    glutAddMenuEntry("Fenetrage simple (Cohen-Sutherland) : Rectangle", 11);
    glutAddMenuEntry("Effacer", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//  Click Souris
void mouse(int btn, int state, int x, int y){


    // Coord pour Segment / Cercle /
    if(     value == 2 ||
            value == 6 ||
            value == 8 ||
            value == 9 ||
            value == 10||
            value == 11||
            value == 12||
            value == 13){
        if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            if(!isSecondClick){
                xA = x -= WIN_W/2;
                yA = y = -y + WIN_H/2;
                fprintf(stdout, "xA : %d, yA : %d  ", xA, yA);
                isSecondClick = 1;
            } else {
                xB = x -= WIN_W/2;
                yB = y = -y + WIN_H/2;
                fprintf(stdout, "xB : %d, yB : %d\n", xB, yB);
                isSecondClick = 0;
            }
        }
    }

    // Coord pour square
    if(value == 3 || value == 4 || value == 5){
        if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                xA = x -= WIN_W/2;
                yA = y = -y + WIN_H/2;
                fprintf(stdout, "xA : %d, yA : %d \n", xA, yA);
        }
    }

    // Coord pour triangle
    if(value == 7){
        if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            if(!isSecondClick){
                xA = x -= WIN_W/2;
                yA = y = -y + WIN_H/2;
                fprintf(stdout, "xA : %d, yA : %d ", xA, yA);
                isSecondClick = 1;
            } else if(isSecondClick && !isThirdClick){
                xB = x -= WIN_W/2;
                yB = y = -y + WIN_H/2;
                fprintf(stdout, "xB : %d, yB : %d ", xB, yB);
                isThirdClick = 1;
            } else if(isSecondClick && isThirdClick){
                xC = x -= WIN_W/2;
                yC = y = -y + WIN_H/2;
                fprintf(stdout, "xC : %d, yC : %d \n", xC, yC);
                isSecondClick = 0;
                isThirdClick = 0;
            }
        }
    }

   glutPostRedisplay();
}

//Dessine un pixel
void drawPixel(int x, int y){
    glBegin(GL_POINTS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2i(x, y);
    glEnd();
}

//Tracé du segment selon l'algo de Bresemham
void bresenham(int xA,int yA,int xB,int yB) {
    int dx,dy;
    int incr, x, y, xdir, ydir, i;

    x = xA;
    y = yA;

    dx = xB - xA;
    dy = yB - yA;

    //On repère où se positionne le deuxieme pixel cliqué pour indiquer la direction de x et de y
    xdir = (dx > 0) ? 1 : -1;
    ydir = (dy > 0) ? 1 : -1;

    //on s'assure que les variable largeur et longueur soient maintenant positive pour pouvoir les utiliser
    dx = abs(dx);
    dy = abs(dy);
    drawPixel(x,y);

    if(dx > dy) { // Si dx > dy alors notre droite est dans le premier octant
        incr = dx / 2;
        for ( i = 1; i <= dx; i++ ) { //On parcours la largeur
            x += xdir; // *en fonction de la direction* , on *incrémente/décrémente* x forcément car on est sur que notre droite sera dans les octants "horizontaux"
            incr += dy;
            if ( incr >= dx ) {
                y += ydir;
                incr -= dx;
            }
            drawPixel(x,y);
        }
    } else {
        incr = dy / 2;
        for ( i = 1; i <= dy; i++ ) {
            y += ydir; // On commence a incrémenter/décrémenter y (en fonction de la direction) car on est sur que notre droite sera dans les octants "verticaux"
            incr += dx;
            if ( incr >= dx ) {
                incr -= dy;
                x += xdir;
            }
            drawPixel(x,y);
        }
    }
}

void bresenhamArc(int xA, int yA, int xB, int yB){
    int x = 0;
    int R = (int) sqrt((xB - xA) * (xB - xA) + (yB - yA) * (yB - yA));
    int y = R;
    int dp = 5 - (4 * R);

    drawPixel(x, y);

    while(y > x){
        if(dp <= 0){
            dp = dp + 8 * x + 12;
        }else {
            dp = dp + 8 * (x - y) + 20;
            y--;
        }
        x++;
        drawPixel(x, y);
    }
}

void bresenhamCercle(int xA, int yA, int xB, int yB){
    int x = 0;
    int R = (int) sqrt((xB - xA) * (xB - xA) + (yB - yA) * (yB - yA));
    int y = R;
    int dp = 5 - (4 * R);

    drawPixel(x + xA, y + yA);
    drawPixel(y + xA, x + yA);
    drawPixel(x + xA, yA - y);
    drawPixel(xA - y, yA + x);

    while (y > x) {
        if (dp <= 0)
            dp += 8 * x + 12;
        else {
            dp += 8 * (x - y) + 20;
            y--;
        }
        x++;
        drawPixel(x + xA, y + yA);
        drawPixel(y + xA, x + yA);
        drawPixel(y + xA, yA - x);
        drawPixel(x + xA, yA - y);
        drawPixel(xA - x, yA - y);
        drawPixel(xA - y, yA - x);
        drawPixel(xA - y, yA + x);
        drawPixel(xA - x, yA + y);
    }
    glFlush();
}

void bresenhamEllipse(int xA, int yA, int xB, int yB){
    int xm = (xB + xA) / 2;
    int ym = (yB + yA) / 2;
    int w = abs(xB - xm);
    int h = abs(yA - ym);
    int x = 0;
    int y = h;
    double delta = (h * h) - (w * w * h) + (w * w / 4);

    drawPixel(xm + x, ym + y);
    drawPixel(xm - x, ym + y);
    drawPixel(xm + x, ym - y);
    drawPixel(xm - x, ym - y);

    while (w * w * (y - 0.5) > h * h * (x + 1)) {
        if (delta < 0) {
            delta += h * h * (2 * x + 3);
            x++;
        } else {
            delta += h * h * (2 * x + 3) + w * w * (-2 * y + 2);
            x++;
            y--;
        }
        drawPixel(xm + x, ym + y);
        drawPixel(xm - x, ym + y);
        drawPixel(xm + x, ym - y);
        drawPixel(xm - x, ym - y);
    }

    delta = h * h * (x + 0.5) + w * w * ((y - 1) * (y - 1)) - w * w * h * h;

    while (y > 0) {
        if (delta < 0) {
            delta += h * h * (2 * x + 2) + w * w * (-2 * y + 3);
            y--;
            x++;
        } else {
            delta += w * w * (-2 * y + 3);
            y--;
        }
        drawPixel(xm + x, ym + y);
        drawPixel(xm - x, ym + y);
        drawPixel(xm + x, ym - y);
        drawPixel(xm - x, ym - y);
    }
    glFlush();
}

void bresenhamEllipse2(int xA, int yA, int xB, int yB){
    int x,y ;
    double d1,d2 ;
    int a = xB - xA;
    int b = yB - yA;

    x = 0;
    y = b;
    d1 = b*b - a*a*b + a*a/4 ;
    drawPixel(x,y);
    drawPixel(x,-y);
    drawPixel(-x,-y);
    drawPixel(-x,y);
    while ( a*a*(y-.5) > b*b*(x+1) ) {
        if ( d1 < 0 ) {
            d1 += b*b*(2*x+3) ;
            x++ ; }
        else {
            d1 += b*b*(2*x+3) + a*a*(-2*y+2) ;
            x++ ;
            y-- ; }
        drawPixel(x,y) ;
        drawPixel(x,-y) ;
        drawPixel(-x,-y) ;
        drawPixel(-x,y); }
    d2 = b*b*(x+.5)*(x+.5) + a*a*(y-1)*(y-1) - a*a*b*b ;
    while ( y > 0 ) {
        if ( d2 < 0 ) {
            d2 += b*b*(2*x+2) + a*a*(-2*y+3) ;
            y-- ;
            x++ ; }
        else {
            d2 += a*a*(-2*y+3) ;
            y-- ; }
        drawPixel(x,y);
        drawPixel(x,-y);
        drawPixel(-x,-y);
        drawPixel(-x,y);
    }
}

void CohenSutherlandLineClipAndDraw(int xA, int yA, int xB, int yB) {
    // compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
    OutCode outcode0 = ComputeOutCode(xA, yA);
    OutCode outcode1 = ComputeOutCode(xB, yB);
    bool accept = false;

    while (true) {
        if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
            accept = true;
            break;
        } else if (outcode0 & outcode1) { // Bitwise AND is not 0. (implies both end points are in the same region outside the window). Reject and get out of loop
            break;
        } else {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            int x, y;

            // At least one endpoint is outside the clip rectangle; pick it.
            OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

            // Now find the intersection point;
            // use formulas:
            //   slope = (y1 - y0) / (x1 - x0)
            //   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
            //   y = y0 + slope * (xm - x0), where xm is xmin or xmax
            if (outcodeOut & TOP) {           // point is above the clip rectangle
                x = xA + (xB - xA) * (ymax - yA) / (yB - yA);
                y = ymax;
            } else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
                x = xA + (xB - xA) * (ymin - yA) / (yB - yA);
                y = ymin;
            } else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
                y = yA + (yB - yA) * (xmax - xA) / (xB - xA);
                x = xmax;
            } else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
                y = yA + (yB - yA) * (xmin - xA) / (xB - xA);
                x = xmin;
            }

            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (outcodeOut == outcode0) {
                xA = x;
                yA = y;
                outcode0 = ComputeOutCode(xA, yA);
            } else {
                xB = x;
                yB = y;
                outcode1 = ComputeOutCode(xB, yB);
            }
        }
    }
    if (accept) {
        // Following functions are left for implementation by user based on
        // their platform (OpenGL/graphics.h etc.)
        bresenham(xmin, ymin, xmax, ymin);
        bresenham(xmax, ymin, xmax, ymax);
        bresenham(xmax, ymax, xmin, ymax);
        bresenham(xmin, ymax, xmin, ymin);
        if(value == 10){
            if(!isSecondClick){
                bresenham(xA, yA, xB, yB);
            }
        } else if(value == 11){
            if(!isSecondClick){
                bresenham(xA, yA, xB, yA);
                bresenham(xB, yA, xB, yB);
                bresenham(xB, yB, xA, yB);
                bresenham(xA, yB, xA, yA);
            }
        }
    }
}

// ------------------------------------
// -------- FONCTIONS DE FORME --------
// ------------------------------------

void segment(){
    if(!isSecondClick){
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        bresenham(xA, yA, xB, yB);
        glutSwapBuffers();
        deleteCoord();
    }
}

void square(){

    if(value == 3){
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        bresenham(xA, yA, (xA+200), yA);
        bresenham((xA+200), yA, (xA+200), (yA+200));
        bresenham((xA+200), (yA+200), xA, (yA+200));
        bresenham(xA, (yA+200), xA, yA);
        deleteCoord();
    }

    if(value == 4){
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        bresenham(xA, yA, (xA+400), yA);
        bresenham((xA+400), yA, (xA+400), (yA+400));
        bresenham((xA+400), (yA+400), xA, (yA+400));
        bresenham(xA, (yA+400), xA, yA);
        deleteCoord();
    }

    if(value == 5){
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        bresenham(xA, yA, (xA+600), yA);
        bresenham((xA+600), yA, (xA+600), (yA+600));
        bresenham((xA+600), (yA+600), xA, (yA+600));
        bresenham(xA, (yA+600), xA, yA);
        deleteCoord();
    }
}

void rectangle(){
    if(!isSecondClick){
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bresenham(xA, yA, xB, yA);
        bresenham(xB, yA, xB, yB);
        bresenham(xB, yB, xA, yB);
        bresenham(xA, yB, xA, yA);
        deleteCoord();
    }
}

void triangle(){
    if(!isSecondClick && !isThirdClick){
        bresenham(xA,yA, xB,yB);
        bresenham(xB,yB, xC, yC);
        bresenham(xC, yC, xA, yA);
        deleteCoord();
    }
}

void cercle(){
    if(!isSecondClick){
        bresenhamCercle(xA, yA, xB, yB);
        deleteCoord();
    }
}

void arc(){
    if(!isSecondClick){
        bresenhamArc(xA, yA, xB, yB);
        deleteCoord();
    }
}

void ellipse1(){
    if(!isSecondClick){
        bresenhamEllipse(xA, yA, xB, yB);
        deleteCoord();
    }
}

void ellipse2(){
    if(!isSecondClick){
        bresenhamEllipse2(xA, yA, xB, yB);
    }
}

void display(){
    //glClear(GL_COLOR_BUFFER_BIT);
    // Value = 1 -> Clear Screen

    switch(value){
        case 0:
            glClear(GL_COLOR_BUFFER_BIT);
            break;
        case 1:
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();
            break;
        case 2:
            segment();
            break;
        case 3:
        case 4:
        case 5:
            square();
            break;
        case 6:
            //glClear(GL_COLOR_BUFFER_BIT);
            rectangle();
            break;
        case 7:
            triangle();
            break;
        case 8:
            cercle();
            break;
        case 9:
            arc(); // Arc dans le deuxieme octant
            break;
        case 10:
        case 11:
            CohenSutherlandLineClipAndDraw(xA, yA, xB, yB);
            break;
        case 12:
            ellipse1();
            break;
        case 13:
            ellipse2();
            break;
    }

    glFlush();
}

// ---------------------------
// ---------- MAIN -----------
// ---------------------------

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_H, WIN_W);
    glutInitWindowPosition (500, 200);
    glutCreateWindow("Drawing window");

    // Paramètre du plan orthonormé
    gluOrtho2D(-WIN_W/2,WIN_W/2,-WIN_W/2,WIN_W/2);
    //Couleur de la fenêtre
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);

    //Définit la taille d'un point sur le plan
    glPointSize(2.0);

    createMenu();
    glutMouseFunc(mouse);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

