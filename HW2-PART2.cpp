/******************************************
*
* Official Name:  your full name as on university documents
*
* Call me: what you like to be called (optional)
*
* E-mail:  atmarkar@syr.edu
*
* Assignment:  Assignment 2
*
* Environment/Compiler:  Visual Studio Community 2019
*
* Date submitted:  February 26, 2024    (or when you submitted it)
*
* References:  list any references you used if none write NONE. (See instructions below.)
*
* Interactions:   Click the options on the GUI (left window) by left clicking within their selection field
*                 Each option has a keybind as stated below:
*                 Total: 't'
*                 Annualar: 'a'
*                 Partial: 'p'
*                 None: 'n'
*                 Scattered CLouds: 'c'
*                 Overcast: 'o'
*                 Balloon: 'b'
*                 Kite: 'k'
*                 Rocket: 'r'
*                 Some options are limited based on selected fields
*                 When r is pressed, and Partial is selected, it will default to None
*                 Right click in the display window for pop-up menu.
*
*******************************************/

#include <cmath>
#include <vector>
#include <cstdlib>
//for sin, cos
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159
#define INACTIVE 0
#define TOTAL 1
#define ANNULAR 2
#define PARTIAL 3
#define NONE 4

using namespace std;

// Globals.
static int window1, window2;

static float numVerticies = 20;
static float R1 = 10;
static float R2 = 6;
static float wx = 175.0, wy = 490, wz = -0.5;
static float text1X = 3.0;
static float text2X = 250;
static float pick1X = text1X + 100;
static float pick2X = text2X + 190;
static int primitive = INACTIVE;

static bool scattered = false;
static bool overcast = false;
static bool balloon = false;
static bool kite = false;
static bool rocket = false;

static int isAnimate = 0;
static float balloonY = 0;


void drawDivider() {

    glLineWidth(4.0);
    glColor3f(0.0, 0.4, 0.6);
    glBegin(GL_LINES);
    int i = wy;
    while (i >= 5)
    {
        glVertex3f(wx, i, wz);
        glVertex3f(wx, i - 25, wz);
        i = i - 50;
    }
    glEnd();

    glPointSize(4);
    glBegin(GL_POINTS);
    i = wy - 12.5;
    while (i >= 25)
    {
        glVertex3f(wx, i, wz);
        i = i - 25;
    }
    glEnd();

}

void drawTotalSelection() {

    if (primitive == TOTAL) {
        glLineWidth(3.0);
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_TRIANGLE_FAN);
        float t = 0;
        for (int i = 0; i <= numVerticies; ++i) {
            glVertex3f(pick1X + R2 * cos(t), 405 + R2 * sin(t), wz);
            t += 2 * PI / numVerticies;
        }
        glEnd();
    }



    glLineWidth(3.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    float t = 0;
    for (int i = 0; i <= numVerticies; ++i) {
        glVertex3f(pick1X + R1 * cos(t), 405 + R1 * sin(t), wz);
        t += 2 * PI / numVerticies;
    }
    glEnd();
}

void drawAnnularSelection() {

    glLineWidth(3.0);
    glColor3f(0.0, 0.0, 0.0);

    if (primitive == ANNULAR) {
        glBegin(GL_TRIANGLE_FAN);
        float t = 0;
        for (int i = 0; i <= numVerticies; ++i) {
            glVertex3f(pick1X + R2 * cos(t), 330 + R2 * sin(t), wz);
            t += 2 * PI / numVerticies;
        }
        glEnd();
    }

    glBegin(GL_LINE_STRIP);
    float t = 0;
    for (int i = 0; i <= numVerticies; ++i) {
        glVertex3f(pick1X + R1 * cos(t), 330 + R1 * sin(t), wz);
        t += 2 * PI / numVerticies;
    }
    glEnd();
}

void drawPartialSelection() {

    glLineWidth(3.0);
    glColor3f(0.0, 0.0, 0.0);

    if (primitive == PARTIAL) {
        glBegin(GL_TRIANGLE_FAN);
        float t = 0;
        for (int i = 0; i <= numVerticies; ++i) {
            glVertex3f(pick1X + R2 * cos(t), 255 + R2 * sin(t), wz);
            t += 2 * PI / numVerticies;
        }
        glEnd();

        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_LINES);
        glVertex3f(text2X - 10, 105, wz);
        glVertex3f(pick2X - 40, 105, wz);
        glEnd();
    }

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    float t = 0;
    for (int i = 0; i <= numVerticies; ++i) {
        glVertex3f(pick1X + R1 * cos(t), 255 + R1 * sin(t), wz);
        t += 2 * PI / numVerticies;
    }
    glEnd();
}

void drawNoneSelection() {

    glLineWidth(3.0);
    glColor3f(0.0, 0.0, 0.0);

    if (primitive == NONE) {
        glBegin(GL_TRIANGLE_FAN);
        float t = 0;
        for (int i = 0; i <= numVerticies; ++i) {
            glVertex3f(pick1X + R2 * cos(t), 180 + R2 * sin(t), wz);
            t += 2 * PI / numVerticies;
        }
        glEnd();
    }

    glBegin(GL_LINE_STRIP);
    float t = 0;
    for (int i = 0; i <= numVerticies; ++i) {
        glVertex3f(pick1X + R1 * cos(t), 180 + R1 * sin(t), wz);
        t += 2 * PI / numVerticies;
    }
    glEnd();
}

void drawScatteredSelection() {

    if (scattered) {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        //X
        glVertex3f(pick2X - R1, 405 - R1, wz);
        glVertex3f(pick2X + R1, 405 + R1, wz);
        glVertex3f(pick2X + R1, 405 - R1, wz);
        glVertex3f(pick2X - R1, 405 + R1, wz);

        //CROSS LINE THROUGH OVERCAST
        glColor3f(0.5, 0.5, 0.5);
        glVertex3f(text2X - 10, 330, wz);
        glVertex3f(pick2X - 40, 330, wz);
        glEnd();
    }



    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glVertex3f(pick2X - R1, 405 - R1, wz);
    glVertex3f(pick2X - R1, 405 + R1, wz);
    glVertex3f(pick2X + R1, 405 + R1, wz);
    glVertex3f(pick2X + R1, 405 - R1, wz);
    glEnd();
    glPopMatrix();
}

void drawOvercastSelection() {

    if (overcast) {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(pick2X - R1, 330 - R1, wz);
        glVertex3f(pick2X + R1, 330 + R1, wz);
        glVertex3f(pick2X + R1, 330 - R1, wz);
        glVertex3f(pick2X - R1, 330 + R1, wz);

        //CROSS LINE THROUGH SCATTERED
        glColor3f(0.5, 0.5, 0.5);
        glVertex3f(text2X - 10, 405, wz);
        glVertex3f(pick2X - 40, 405, wz);

        //CROSS LINE THROUGH ROCKET
        glVertex3f(text2X - 10, 105, wz);
        glVertex3f(pick2X - 40, 105, wz);
        glEnd();
    }

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glVertex3f(pick2X - R1, 330 - R1, wz);
    glVertex3f(pick2X - R1, 330 + R1, wz);
    glVertex3f(pick2X + R1, 330 + R1, wz);
    glVertex3f(pick2X + R1, 330 - R1, wz);
    glEnd();
    glPopMatrix();
}

void drawBalloonSelection() {

    if (balloon) {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(pick2X - R1, 255 - R1, wz);
        glVertex3f(pick2X + R1, 255 + R1, wz);
        glVertex3f(pick2X + R1, 255 - R1, wz);
        glVertex3f(pick2X - R1, 255 + R1, wz);
        glEnd();
    }

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glVertex3f(pick2X - R1, 255 - R1, wz);
    glVertex3f(pick2X - R1, 255 + R1, wz);
    glVertex3f(pick2X + R1, 255 + R1, wz);
    glVertex3f(pick2X + R1, 255 - R1, wz);
    glEnd();
    glPopMatrix();
}

void drawKiteSelection() {

    if (kite) {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(pick2X - R1, 180 - R1, wz);
        glVertex3f(pick2X + R1, 180 + R1, wz);
        glVertex3f(pick2X + R1, 180 - R1, wz);
        glVertex3f(pick2X - R1, 180 + R1, wz);
        glEnd();
    }

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glVertex3f(pick2X - R1, 180 - R1, wz);
    glVertex3f(pick2X - R1, 180 + R1, wz);
    glVertex3f(pick2X + R1, 180 + R1, wz);
    glVertex3f(pick2X + R1, 180 - R1, wz);
    glEnd();
    glPopMatrix();
}

void drawRocketSelection() {

    if (rocket) {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(pick2X - R1, 105 - R1, wz);
        glVertex3f(pick2X + R1, 105 + R1, wz);
        glVertex3f(pick2X + R1, 105 - R1, wz);
        glVertex3f(pick2X - R1, 105 + R1, wz);
        glEnd();


        //Draw line through overcast
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_LINES);
        glColor3f(0.5, 0.5, 0.5);
        glVertex3f(text2X - 10, 330, wz);
        glVertex3f(pick2X - 40, 330, wz);

        //Draw line through Partial
        glVertex3f(text1X, 255, wz);
        glVertex3f(pick1X - 15, 255, wz);
        glEnd();
    }

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glVertex3f(pick2X - R1, 105 - R1, wz);
    glVertex3f(pick2X - R1, 105 + R1, wz);
    glVertex3f(pick2X + R1, 105 + R1, wz);
    glVertex3f(pick2X + R1, 105 - R1, wz);
    glEnd();
    glPopMatrix();
}

void drawSun() {
    //Variably changing posistions based on what type of eclipse
    glPushMatrix();
    if (primitive == NONE || primitive == INACTIVE)
        glTranslated(0.0, 0.0, -122.5);
    if (primitive == TOTAL)
        glTranslated(0.0, 0.0, -500.0);
    if (primitive == ANNULAR)
        glTranslated(0.0, 0.0, -122.0);
    if (primitive == PARTIAL)
        glTranslated(0.0, 0.0, -122.5);

    glColor3f(1.0, 1.0, 0.2);
    glutSolidSphere(120, 20, 20);
    glPopMatrix();
}

void drawMoon() {
    //Moon only changes posistion if it is partial
    glPushMatrix();
    glTranslated(0.0, 0.0, -51);
    //if partial draw an Annulus to cover rest of moon behind the same colored background
    if (primitive == PARTIAL)
        //move moon to the side
        glTranslated(9, 0.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(50, 20, 20);
    glPopMatrix();
}

void drawClouds(int x, int y) {

    float t = 0;

    if (primitive == NONE || primitive == INACTIVE)
        glColor3f(0.9, 0.9, 0.9);
    if (primitive == PARTIAL)
        glColor3f(0.5, 0.5, 0.5);
    if (primitive == ANNULAR)
        glColor3f(0.25, 0.25, 0.25);
    if (primitive == TOTAL)
        glColor3f(0.15, 0.15, 0.15);

    glLineWidth(15.0);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numVerticies / 2; ++i) {
        glVertex3f(x + 3 * cos(t), y + 1 + 2 * sin(t), -1.0);
        t += 2 * PI / numVerticies;
    }
    glEnd();

    t = 0;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numVerticies / 2; ++i) {
        glVertex3f(x + 5 + 3 * cos(t), y + 2 + 2 * sin(t), -1.0);
        t += 2 * PI / numVerticies;
    }
    glEnd();

    t = PI / 2;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numVerticies / 2; ++i) {
        glVertex3f(x - 3 + 2 * cos(t), y + 2 * sin(t), -1.0);
        t += 2 * PI / numVerticies;
    }
    glEnd();

    t = (3 * PI) / 2;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numVerticies / 2; ++i) {
        glVertex3f(x + 10 + 2 * cos(t), y + 2 * sin(t), -1.0);
        t += 2 * PI / numVerticies;
    }
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(x - 3, y + 2, -1);
    glVertex3f(x - 3, y - 2, -1);
    glVertex3f(x + 10, y - 2, -1);
    glVertex3f(x + 10, y + 2, -1);
    glEnd();

}

void drawOvercast() {
    int x;
    int y;
    for (y = 30; y > -18; y = y - 3) {
        for (x = 30; x > -18; x = x - 3) {
            drawClouds(x, y);
        }
    }
}

void drawBalloon() {

    float t = 0;

    glColor3f(1.0, 0.5, 0.0);
    if (primitive == PARTIAL)
        glColor3f(0.8, 0.3, 0.0);
    if (primitive == ANNULAR)
        glColor3f(0.6, 0.2, 0.0);
    if (primitive == TOTAL)
        glColor3f(0.4, 0.2, 0.0);

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numVerticies; ++i) {
        glVertex3f(10 + 3 * cos(t), 10 + balloonY + 4 * sin(t), -1.0);
        t += 2 * PI / numVerticies;
    }
    glEnd();


    glColor3f(1.0, 1.0, 1.0);
    if (primitive == PARTIAL)
        glColor3f(0.9, 0.9, 0.9);
    if (primitive == ANNULAR)
        glColor3f(0.7, 0.7, 0.7);
    if (primitive == TOTAL)
        glColor3f(0.6, 0.6, 0.6);

    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(10, 10 + balloonY, -1);
    glVertex3f(10, 5 + balloonY, -1);
    glVertex3f(10.3, 4.5 + balloonY, -1);
    glVertex3f(10.5, 4.3 + balloonY, -1);
    glVertex3f(10.8, 4 + balloonY, -1);
    glVertex3f(10.9, 3.7 + balloonY, -1);
    glVertex3f(10.9, 3 + balloonY, -1);
    glVertex3f(10.7, 2.8 + balloonY, -1);
    glVertex3f(10.3, 2.3 + balloonY, -1);
    glVertex3f(9.8, 2.3 + balloonY, -1);
    glVertex3f(9.5, 2.0 + balloonY, -1);
    glVertex3f(9.3, 1.5 + balloonY, -1);

    glEnd();
}

void increaseY(void) {
    if (overcast)
        balloonY += 0.03;
    else
        balloonY += 0.005;
    if (balloonY > 30)
        balloonY = -50;
    glutPostRedisplay();
}

void drawKite() {

    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(-8.5, 4, -1);
    glVertex3f(-13, 3, -1);
    glVertex3f(-12, -6, -1);
    glVertex3f(-6, 1, -1);
    glVertex3f(-13, 3, -1);
    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glColor3f(1.0, 0.0, 1.0);
    if (primitive == PARTIAL)
        glColor3f(0.9, 0.0, 0.9);
    if (primitive == ANNULAR)
        glColor3f(0.6, 0.0, 0.6);
    if (primitive == TOTAL)
        glColor3f(0.5, 0.0, 0.5);

    glBegin(GL_POLYGON);
    glVertex3f(-8.5, 4, -1);
    glVertex3f(-13, 3, -1);
    glVertex3f(-12, -6, -1);
    glVertex3f(-6, 1, -1);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    if (primitive == PARTIAL)
        glColor3f(0.9, 0.9, 0.9);
    if (primitive == ANNULAR)
        glColor3f(0.7, 0.7, 0.7);
    if (primitive == TOTAL)
        glColor3f(0.6, 0.6, 0.6);

    glBegin(GL_LINES);
    glVertex3f(-12, -6, -1);
    glVertex3f(-20, -20, -1);
    glEnd();
}

void drawRocket() {

    glLineWidth(1.0);
    glPushMatrix();
    glTranslatef(-65.0, 47.0, -10.0);
    glColor3f(1.0, 0.0, 0.0);
    glRotated(270, 1, 0, 0);
    glRotated(300, 0, 1, 0);
    glScaled(3, 1.1, 1.1);
    glutWireCone(3, 7, 10, 10);

    glColor3f(1.0, 0.5, 0.0);
    glutSolidCone(3, 7, 10, 10);
    glPopMatrix();

    glColor3f(0.0, 0.0, 1.0);
    if (primitive == PARTIAL)
        glColor3f(0.0, 0.0, 0.9);
    if (primitive == ANNULAR)
        glColor3f(0.0, 0.0, 0.7);
    if (primitive == TOTAL)
        glColor3f(0.0, 0.0, 0.6);
    glBegin(GL_POLYGON);
    glVertex3f(-15.5, 9, -1);
    glVertex3f(-11, 5.5, -1);
    glVertex3f(-8.5, 9, -1);
    glVertex3f(-13.1, 12.2, -1);
    glEnd();

    glColor3f(0.3, 0.3, 0.3);
    if (primitive == PARTIAL)
        glColor3f(0.25, 0.25, 0.25);
    if (primitive == ANNULAR)
        glColor3f(0.15, 0.15, 0.15);
    if (primitive == TOTAL)
        glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_TRIANGLES);
    glVertex3f(-10.75, 8.25, -1);
    glVertex3f(-10, 4.5, -1);
    glVertex3f(-7.5, 8, -1);
    glEnd();

    glColor3f(1.0, 0.0, 0.0);
    if (primitive == PARTIAL)
        glColor3f(0.8, 0.0, 0.0);
    if (primitive == ANNULAR)
        glColor3f(0.6, 0.0, 0.0);
    if (primitive == TOTAL)
        glColor3f(0.4, 0.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(-16.3, 12.6, -1);
    glVertex3f(-15.5, 9, -1);
    glVertex3f(-13.1, 12.2, -1);
    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(5);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex3f(-16.3, 12.6, -1);
    glVertex3f(-15.5, 9, -1);
    glVertex3f(-13.1, 12.2, -1);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-15.5, 9, -1);
    glVertex3f(-11, 5.5, -1);
    glVertex3f(-8.5, 9, -1);
    glVertex3f(-13.1, 12.2, -1);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}

void writeBitmapString(void* font, char* string)
{
    char* c;
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Drawing routine.
void drawScene1(void)
{
    glutSetWindow(window1);

    //fill background
    glClear(GL_COLOR_BUFFER_BIT);

    //return to fresh model view before assembling drawing
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawDivider();
    drawTotalSelection();
    drawAnnularSelection();
    drawPartialSelection();
    drawNoneSelection();

    drawScatteredSelection();
    drawOvercastSelection();
    drawKiteSelection();
    drawBalloonSelection();
    drawRocketSelection();

    glColor3f(0.0, 0.4, 0.6);
    glRasterPos3f(text1X + 10, wy - 30, wz);
    writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (char*)"Type of Eclipse");

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(text1X + 30, 400, wz);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*)"total(t)");

    glRasterPos3f(text1X + 6, 325, wz);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*)"annular(a)");

    if (rocket)
        glColor3f(0.5, 0.5, 0.5);
    glRasterPos3f(text1X + 13, 250, wz);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*)"partial(p)");

    glRasterPos3f(text1X + 20, 175, wz);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*)"none(n)");

    /******************************************************************/

    glColor3f(0.0, 0.4, 0.6);
    glRasterPos3f(text2X + 25, wy - 30, wz);
    writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (char*)"Other Items");

    glColor3f(0.0, 0.0, 0.0);
    if (overcast)
        glColor3f(0.5, 0.5, 0.5);
    glRasterPos3f(text2X, 400, wz);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*)"scattered clouds(c)");

    glColor3f(0.0, 0.0, 0.0);
    if (scattered || rocket)
        glColor3f(0.5, 0.5, 0.5);
    glRasterPos3f(text2X + 65, 325, wz);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*)"overcast(o)");

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(text2X + 77, 250, wz);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*)"balloon(b)");

    glRasterPos3f(text2X + 110, 175, wz);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*)"kite(k)");
    if (overcast || primitive == PARTIAL)
        glColor3f(0.5, 0.5, 0.5);
    glRasterPos3f(text2X + 92, 100, wz);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*)"rocket(r)");

    glutSwapBuffers(); //send drawing to the screen
}

void drawScene2(void)
{
    float angle = 0;
    int i;

    // Choose Window 1
    glutSetWindow(window2);

    //background color
    if (primitive == NONE || primitive == INACTIVE)
        glClearColor(0.0, 0.7, 1.0, 0.0);

    if (primitive == TOTAL)
        glClearColor(0.0, 0.1, 0.3, 0.0);

    if (primitive == ANNULAR)
        glClearColor(0.2, 0.2, 0.65, 0.0);

    if (primitive == PARTIAL)
        glClearColor(0.0, 0.4, 0.9, 0.0);
    //fill background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //return to fresh model view before assembling drawing
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawSun();

    if (balloon)
        drawBalloon();

    if (kite)
        drawKite();

    if (primitive == TOTAL || primitive == ANNULAR || primitive == PARTIAL)
        drawMoon();

    if (scattered) {
        drawClouds(-10, -10);
        drawClouds(3, -1);
        drawClouds(-4, 9);
    }

    if (overcast)
        drawOvercast();

    if (rocket)
        drawRocket();

    if (primitive == PARTIAL) {
        glColor3f(0.0, 0.4, 0.9);
        glBegin(GL_TRIANGLE_STRIP);
        for (i = 0; i <= numVerticies; ++i)
        {
            angle = 2 * PI * i / numVerticies;
            glVertex3f(cos(angle) * 4.8, sin(angle) * 4.8, -1.0);
            glVertex3f(cos(angle) * 15.0, sin(angle) * 15.0, -1.0);
        }

        glEnd();
    }

    glutSwapBuffers(); //send drawing to the screen
}


void mouseControl(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        //click inside left X selection
        if (x > pick1X - R1 && x < pick1X + R1) {
            //click in total(t)
            if (y > 500 - 415 && y < 500 - 355)

                primitive = TOTAL;

            //click in annular(a)
            if (y > 500 - 340 && y < 500 - 320)
                primitive = ANNULAR;

            //click in partial(p)
            //make sure rocket is not selected
            if (!rocket) {
                if (y > 500 - 265 && y < 500 - 245)
                    primitive = PARTIAL;
            }

            //click in none(n)
            if (y > 500 - 190 && y < 500 - 170)
                primitive = NONE;
        }

        if (x > pick2X - R1 && x < pick2X + R1) {
            //Make sure overcast is not selected
            if (!overcast) {
                //click in scatterd
                if (y > 500 - (405 + R1) && y < 500 - (405 - R1))
                    scattered = !scattered;
            }
            //make sure scattered nor rocket is not selected
            if (!scattered && !rocket) {
                //click in overcast
                if (y > 500 - (330 + R1) && y < 500 - (330 - R1))
                    overcast = !overcast;
            }
            //click in balloon
            if (y > 500 - (255 + R1) && y < 500 - (255 - R1))
                balloon = !balloon;
            //clcik in kite
            if (y > 500 - (180 + R1) && y < 500 - (180 - R1))
                kite = !kite;
            //click in rocket
            //Make sure neither overcast nor Partial is selected
            if (!overcast && (primitive != PARTIAL)) {
                if (y > 500 - (105 + R1) && y < 500 - (105 - R1))
                    rocket = !rocket;
            }
        }

        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
    }

}

// Initialization routine.
void setup1(void)
{
    //white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void setup2(void)
{
    //white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize1(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 500.0, 0.0, 500.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void resize2(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-15.0, 15.0, -15.0, 15.0, 0.9, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 't':
        primitive = TOTAL;
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    case 'a':
        primitive = ANNULAR;
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    case 'p':
        primitive = PARTIAL;
        rocket = false;
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    case 'n':
        primitive = NONE;
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    case 'c':
        scattered = !scattered;
        overcast = false;
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    case 'o':
        overcast = !overcast;
        scattered = false;
        rocket = false;
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    case 'b':
        balloon = !balloon;
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    case 'k':
        kite = !kite;
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    case 'r':
        rocket = !rocket;
        if (primitive == PARTIAL)
            primitive = NONE;
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    case ' ':
        if (isAnimate) {
            isAnimate = 0;
            glutIdleFunc(NULL);
        }
        else {
            isAnimate = 1;
            glutIdleFunc(increaseY);
        }
        glutSetWindow(window1);
        glutPostRedisplay();
        glutSetWindow(window2);
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void topMenu(int id) {
    if (id == 1)
        exit(0);
}

void otherMenu(int id) {
    if (id == 2)
        scattered = true;
    if (id == 3)
        overcast = true;
    if (id == 4)
        balloon = true;
    if (id == 5)
        kite = true;
    if (id == 6)
        rocket = true;
    glutSetWindow(window1);
    glutPostRedisplay();
    glutSetWindow(window2);
    glutPostRedisplay();
}

void eclipseMenu(int id) {
    if (id == 7)
        primitive = TOTAL;
    if (id == 8)
        primitive = ANNULAR;
    if (id == 9)
        primitive = PARTIAL;
    if (id == 10)
        primitive = NONE;
    glutSetWindow(window1);
    glutPostRedisplay();
    glutSetWindow(window2);
    glutPostRedisplay();
}

void makeMenu() {

    int eclipse_menu;
    int other_menu;
    eclipse_menu = glutCreateMenu(eclipseMenu);
    glutAddMenuEntry("Total", 7);
    glutAddMenuEntry("Annualar", 8);
    glutAddMenuEntry("Partial", 9);
    glutAddMenuEntry("None", 10);

    other_menu = glutCreateMenu(otherMenu);
    glutAddMenuEntry("Scattered Clouds", 2);
    glutAddMenuEntry("Overcast", 3);
    glutAddMenuEntry("Balloon", 4);
    glutAddMenuEntry("Kite", 5);
    glutAddMenuEntry("Rocket", 6);

    glutCreateMenu(topMenu);
    glutAddSubMenu("Eclipse", eclipse_menu);
    glutAddSubMenu("Other Options", other_menu);
    glutAddMenuEntry("Quit", 1);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Click the options on the GUI (left window) by left clicking within their selection field" << endl;
    cout << "Each option has a keybind as stated below:" << endl;
    cout << "Total: 't'" << endl;
    cout << "Annular: 'a'" << endl;
    cout << "Partial: 'p'" << endl;
    cout << "None: 'n'" << endl;
    cout << "Scattered Clouds: 'c'" << endl;
    cout << "Overcast: 'o'" << endl;
    cout << "Balloon: 'b'" << endl;
    cout << "Kite: 'k'" << endl;
    cout << "Rocket: 'r'" << endl;
    cout << "You may also right click on the Eclipse Window (Right Window) to open a menu filled with the the options" << endl;
    cout << "Some options are limited based on selected fields" << endl;
    cout << "When r is pressed, and Partial is selected, it will default to None" << endl;
}

// Main routine.
int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    window1 = glutCreateWindow("HW2 - GUI");
    setup1();
    glutDisplayFunc(drawScene1);
    glutReshapeFunc(resize1);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouseControl);

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(650, 100);
    window2 = glutCreateWindow("HW2 - Eclipses");
    setup2();
    glutDisplayFunc(drawScene2);
    glutReshapeFunc(resize2);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouseControl);
    makeMenu();

    glutMainLoop();

    return 0;
}

