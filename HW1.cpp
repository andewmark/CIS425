/******************************************
*
* Official Name:  Andrew Markarian
*
* E-mail:  atmarkar@syr.edu
*
* Assignment:  Assignment 1
*
* Environment/Compiler:  Visual Studio Community 2019 (or whatever you use)
*
* Date submitted:  February 7, 2024
*
* References:  NONE
*
* Interactions:   
*                 Press the escape key to quit.
*                 Press P for a different perspective
*******************************************/

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265358979324

using namespace std;

// Globals.
     //Animal 1 (Cat)
     //The nose of the cat
static float wx = 5.0, wy = 20.0, wz = -2.0;

    //Animal 2 (Mouse)
    //The top of the nose of the mouse
    //This also will be used as the center of the face
static float bx = 0, by = 10.0, bz = -1.0; 

static float headHeight = 5; //head placement
static float bodyLength = 10;
static float bodyHeight = 5;
static float earLength = 2;
static float earHeight = 0.9;
static float R = 0.5; // Radius of circle1 & circle2.
static float R3 = 3;  // Radius of circle3 (Face of Mouse Size).
static float earSize = 2;
static float X1 = wx + 7; // X-coordinate of center of circle1.
static float Y1 = wy - (headHeight / 2) - 3.5; // Y-coordinate of center of circle1.
static float X2 = wx + 9; // X-coordinate of center of circle2.
static float Y2 = Y1; // Y-coordinate of center of circle2.
static int numVertices = 50; // Number of vertices on circle.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int perspective = 0;

void drawHead() 
{
    //Cat
    glLineWidth(1.0);
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(wx, wy-headHeight/2, wz);                    //v1
    glVertex3f(wx + 2, wy - (headHeight / 2) + 1, wz);      //v2
    glVertex3f(wx + 4, wy - (headHeight / 2) + 1, wz);      //v3
    glVertex3f(wx + 5, wy - (headHeight / 2), wz);          //v4
    glVertex3f(wx + 3.5, wy - (headHeight / 2) - 1, wz);    //v5 
    glVertex3f(wx + 2, wy - (headHeight / 2) - 1, wz);      //v6
    glEnd();

    //Mouse
    glLineWidth(5.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    float t = 0;
    for (int i = 0; i <= numVertices; ++i) {
        glVertex3f(bx + R3 * cos(t), by + R3 * sin(t), bz);
        t += 2 * PI / numVertices;
    }
    glEnd();
}

void drawWhiskers() {
    //Cat
    glLineWidth(1.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(wx + 0.5, wy - (headHeight / 2) - 0.1, wz);  //v1
    glVertex3f(wx + 1, wy - (headHeight / 2) - 1, wz);      //v2
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(wx + 0.5, wy - (headHeight / 2) + 0.1, wz);  //v1
    glVertex3f(wx + 1, wy - (headHeight / 2) + 1, wz);      //v2
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(wx + 0.6, wy - (headHeight / 2), wz);        //v1
    glVertex3f(wx + 1.5, wy - (headHeight / 2) - 0.1, wz);  //v2
    glEnd();

    //Mouse
    
    //top right
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(bx + 0.5, by, bz);
    glVertex3f(bx + 5, by + 2, bz);
    glEnd();

    //middle right
    glBegin(GL_LINES);
    glVertex3f(bx + 0.9, by - 0.4, bz);
    glVertex3f(bx + 5.5, by - 0.4, bz);
    glEnd();

    //bottom right
    glBegin(GL_LINES);
    glVertex3f(bx + 0.6, by - 0.7, bz);
    glVertex3f(bx + 5, by - 2, bz);
    glEnd();

    //top left
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(bx - 0.5, by, bz);
    glVertex3f(bx - 5, by + 2, bz);
    glEnd();

    //middle left
    glBegin(GL_LINES);
    glVertex3f(bx - 0.9, by - 0.4, bz);
    glVertex3f(bx - 5.5, by - 0.4, bz);
    glEnd();

    //bottom left
    glBegin(GL_LINES);
    glVertex3f(bx - 0.6, by - 0.7, bz);
    glVertex3f(bx - 5, by - 2, bz);
    glEnd();
}

void drawEars()
{
    //Cat
    glLineWidth(1.0);
    glColor3f(1.0, 0.4, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex3f(wx + 2, wy - (headHeight / 2) + 0.9, wz);                  //v1
    glVertex3f(wx + 1.8 + earLength, wy - (headHeight / 2) + 0.9, wz);    //v2
    glVertex3f(wx + 3, wy - (headHeight / 2) + 1 + earHeight, wz);      //v3
    glEnd();

    //Mouse

    glLineWidth(5.0);
    glColor3f(0.0, 0.0, 0.0);
    float t = 0; //angle param

    //Ear 1
    glBegin(GL_LINE_STRIP);
    t = 0.0;
    for (int i = 0; i <= numVertices; ++i) {
        glVertex3f((bx + 2.5) + earSize * cos(t), (by + 2.5) + earSize * sin(t), bz);
        t += 2 * PI / numVertices;
    }
    glEnd();

    //Ear 2
    glBegin(GL_LINE_STRIP);
    t = 0.0;
    for (int i = 0; i <= numVertices; ++i) {
        glVertex3f((bx - 2.5) + earSize * cos(t), (by + 2.5) + earSize * sin(t), bz);
        t += 2 * PI / numVertices;
    }
    glEnd();

    //Fill in Face so mouse ears don't show
    glColor3f(1.0, 1.0, 1.0);
    //glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    t = 0;
    for (int i = 0; i <= numVertices; ++i) {
        glVertex3f(bx + R3 * cos(t), by + R3 * sin(t), bz);
        t += 2 * PI / numVertices;
    }
    glEnd();
}

void drawNose() {
    glLineWidth(1.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(bx, by, bz);
    glVertex3f(bx - 0.5, by - 0.5, bz);
    glVertex3f(bx + 0.5, by - 0.5, bz);
    glEnd();
}

void drawEyes() {
    glLineWidth(5.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(bx - 0.5, by + 1, bz);
    glVertex3f(bx - 0.5, by + 2, bz);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(bx + 0.5, by + 1, bz);
    glVertex3f(bx + 0.5, by + 2, bz);
    glEnd();
}

void drawBody()
{
    //Cat
    glLineWidth(1.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(wx + 4, wy - (headHeight / 2) + 0.5, wz);            //v1
    glColor3f(1.0, 0.4, 0.0);
    glVertex3f(wx + 5, wy - (headHeight / 2) - 0.9, wz);            //v2
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(wx + 6, wy - (headHeight / 2) + 0.5, wz);            //v3
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(wx + 7, wy - (headHeight / 2) - 0.9, wz);            //v4
    glColor3f(1.0, 0.4, 0.0);
    glVertex3f(wx + 7, wy - (headHeight / 2) + 0.5, wz);            //v5
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(wx + 8, wy - (headHeight / 2) + 0.5, wz);            //v6
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(wx + 8.3, wy - (headHeight / 2) - 0.9, wz);          //v7
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(wx + 7, wy - (headHeight / 2) - 0.9, wz);            //v8
    glEnd();

    //Mouse
    glColor3f(1, 0, 1);
    glLineWidth(1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numVertices; ++i) {
        float angle = (2 * PI * i) / numVertices;
        float x = R3 * cos(angle);
        float z = -4 + R3 * sin(angle);

        glVertex3f(x, bodyHeight + 3, z);  // Top vertex
        glVertex3f(x, -bodyHeight + 6, z); // Bottom vertex
    }
    glEnd();


    glColor3f(0, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numVertices; ++i) {
        float angle = (2 * PI * i) / numVertices;
        float x = R3 * cos(angle);
        float z = -4 + R3 * sin(angle);

        glVertex3f(x, bodyHeight + 3, z);  // Top vertex
        glVertex3f(x, -bodyHeight + 6, z); // Bottom vertex
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawFrontLegs() {
    //Cat
    glLineWidth(5);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(wx + 5, wy - (headHeight / 2) - 0.8, wz);    //v1
    glVertex3f(wx + 4, wy - (headHeight / 2) - 3, wz);      //v2
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(wx + 5, wy - (headHeight / 2) - 0.8, wz);    //v1
    glVertex3f(wx + 6, wy - (headHeight / 2) - 3, wz);      //v2
    glEnd();
}

void drawFrontPaws() {
    //Cat
    glLineWidth(1);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(wx + 3.5, wy - (headHeight / 2) - 3, wz);    //v1
    glVertex3f(wx + 4.5, wy - (headHeight / 2) - 3, wz);    //v2
    glVertex3f(wx + 4.5, wy - (headHeight / 2) - 4, wz);    //v3
    glVertex3f(wx + 3.5, wy - (headHeight / 2) - 4, wz);    //v4
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(wx + 5.5, wy - (headHeight / 2) - 3, wz);    //v1
    glVertex3f(wx + 6.5, wy - (headHeight / 2) - 3, wz);    //v2
    glVertex3f(wx + 6.5, wy - (headHeight / 2) - 4, wz);    //v3
    glVertex3f(wx + 5.5, wy - (headHeight / 2) - 4, wz);    //v4
    glEnd();
}

void drawBackLegs() {
    //Cat
    glLineWidth(5);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(wx + 8, wy - (headHeight / 2) - 0.8, wz);    //v1
    glVertex3f(wx + 7, wy - (headHeight / 2) - 3, wz);      //v2
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(wx + 8, wy - (headHeight / 2) - 0.8, wz);    //v1
    glVertex3f(wx + 9, wy - (headHeight / 2) - 3, wz);      //v2
    glEnd();
}

void drawBackPaws() {

    //Cat

    float t = 0;

    glLineWidth(4.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    t = 0;
    for (int i = 0; i <= numVertices; ++i) {
        glVertex3f(X1 + R * cos(t), Y1 + R * sin(t), wz);
        t += 2 * PI / numVertices;
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    t = 0;
    for (int i = 0; i <= numVertices; ++i) {
        glVertex3f(X2 + R * cos(t), Y2 + R * sin(t), wz);
        t += 2 * PI / numVertices;
    }
    glEnd();
}


void drawTail() {

    //Cat
    glLineWidth(7);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(wx + 7.5, wy - (headHeight / 2) - 0.5, wz);        //v1
    glVertex3f(wx + 9, wy - (headHeight / 2) + 1, wz);            //v2
    glEnd();
}

void writeBitmapString(void* font, const char* string)
{
    const char* c;

    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Drawing routine.
void drawScene(void)
{
    //fill background
    glClear(GL_COLOR_BUFFER_BIT);

    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    */


    //return to fresh model view before assembling drawing
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (perspective == 0)
        glOrtho(-20.0, 20.0, -20.0, 20.0, 1.0, 100.0);
    else if (perspective == 1)
        //glFrustum(0.0, 25.0, 0.0, 25.0, 1.0, 100.0);
        glFrustum(-20.0, 20.0, -20.0, 20.0, 1.0, 100.0);
    else if (perspective == 2)
        glOrtho(-10.0, 10.0, -5.0, 15.0, 1.0, 20.0);
    else
        glOrtho(3, 17, 10, 25, 1, 20);

    drawTail();
    drawBody();
    drawEars();
    drawEyes();
    drawNose();
    drawHead();
    drawWhiskers();
    drawFrontLegs();
    drawBackLegs();
    drawBackPaws();
    drawFrontPaws();

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(15, 15, -2);
    writeBitmapString((void*)font, "Cat");
    glRasterPos3f(-1.5, -1, -1.0);
    writeBitmapString((void*)font, "Mouse");
    

    glFlush(); //send drawing to the screen
}

// Initialization routine.
void setup(void)
{
    // Set background (or clearing) color.
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    // Set viewport size to be entire OpenGL window.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // Set matrix mode to projection.
    glMatrixMode(GL_PROJECTION);

    // Clear current projection matrix to identity.
    glLoadIdentity();

    // Specify the orthographic (or perpendicular) projection, 
    // i.e., define the viewing box.
    

    // Set matrix mode to modelview.
    glMatrixMode(GL_MODELVIEW);

    // Clear current modelview matrix to identity.
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        // Press escape to exit.
    case 27:
        exit(0);
        break;
    case 'p':
        if (perspective == 3)
            perspective = 0;
        else
            perspective++;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press p to change the perspective of the drawing." << endl;
}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char** argv)
{
    // Initialize GLUT.
    printInteraction();
    glutInit(&argc, argv);

    // Set display mode as single-buffered and RGB color.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    // Set OpenGL window size.
    glutInitWindowSize(500, 500);

    // Set position of OpenGL window upper-left corner.
    glutInitWindowPosition(100, 100);

    // Create OpenGL window with title.
    glutCreateWindow("HW1.cpp");

    // Initialize.
    setup();

    // Register display routine.
    glutDisplayFunc(drawScene);

    // Register reshape routine.
    glutReshapeFunc(resize);

    // Register keyboard routine.
    glutKeyboardFunc(keyInput);

    // Begin processing.
    glutMainLoop();

    return 0;
}
