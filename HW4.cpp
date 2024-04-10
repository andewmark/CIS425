
/******************************************
*
* Official Name:  Andrew Markarian
*
* E-mail:  atmarkar@syr.edu
*
* Assignment:  Assignment 4
*
* Environment/Compiler:  Visual Studio Community 2021
*
* Date submitted:  April 4, 2024
*
* References: NONE
*
* Interactions:     Press esc to quit
*                   Press up/down to move forward/backwards 
*                   left/right to look left/right
*                   Press h to turn on your headlamp
*                   Press s to turn on Main room Light
*                   Press d to open/close door 
*                   Press w to open/close Window Blinds
*                   Press c to make paper rise from the container
*                   Press r (once arisen) to rotate paper
*                   Press f to ignite the fire
*                   Press G/g to increase/decrease ambient light
*
*******************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159
#define ONE_BY_ROOT_THREE 0.57735

int setRandom() {
    srand((unsigned)time(NULL));
    int random = rand() % 2 + 1;
    return random;
}

int randomNumber = setRandom();

using namespace std;
int direction = 5; // 0: North, 1: East, 2: South, 3: West, 4: Above, 5: POV 
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 6, zVal = 22; // Co-ordinates of the camera.
static int isCollision = 0; 
static float xLight = 21, yLight = 2.0, zLight = -59.0;
static float xDoor1 = 6, xDoor2 = 6; //variables to open door on button press

static float blindsScaledY = 15;
static float blindsY = -7.5;
static int blindsLeg = 2;

static float cardY = 2;
static float cardAngle = 0;
static int cardAngLeg = 1;
static int cardLeg = 1;
static bool cardRotated = false;
static bool celebrateDone = false;

static int seeCone = 1;
static bool headLamp = false;
static bool mainRoomLight = false;
static int leg = 1;
static bool button1Clicked;
static bool blindsClicked;
static bool withinContainerRadius;
static bool cardClicked;

static bool fireIgnited = false;
static float fireAngle = 0;

unsigned char button1IDColor[3] = { 255,128,0 }; 
unsigned char blindsIDColor[3] = { 255, 255, 0 };
unsigned char cardIDColor[3] = { 0, 0, 255 };
unsigned char fireIDColor[3] = { 255, 0, 255 };
bool isPicking = false;

int xClick, yClick;  //coordinates of mouseclick

//wall coordinates

//hallway walls
static float hBackX = -18, hBackZ = 30,
             hLeftX = -15, hLeftZ = 30,
             hRightX = 30, hRightZ = 30;

//Main room walls
static float mLeftX = -15, mLeftZ = -19,
             mFrontX = -15, mFrontZ = -59,
             mRightX = 40, mRightZ = -19;


//global amb lighting vector
float globAmb[] = { 0.0, 0.0, 0.0, 1.0 };

//materials
float matAmbAndDifWalls[] = { 0.4, 0.4, 0.7, 1.0 };
float matAmbAndDifFloor[] = { 0.4, 0.2, 0.0, 1.0 };
float matAmbAndDifDoors[] = { 1.0, 1.0, 1.0, 1.0 };
float matAmbAndDifCeiling[] = { 0.4, 0.4, 0.7, 1.0 };
float matAmbAndDiffMainLight[] = { 1.0,1.0,1.0,1.0 };

float emmissionMainLight[] = { 1.0,1.0,1.0,1.0 };
float emmissionDefault[] = { 0.01, 0.01 , 0.01 , 1.0 };

void setProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-4, 4, -4, 4, 2, 100);

    glMatrixMode(GL_MODELVIEW);

}

void setLook() {

    //Set View

    //from the north
    if (direction == 0) {
        gluLookAt(6, 10, -25,
            6, 5, -9,
            0, 1, 0);
    }

    //from the east
    if (direction == 1) {
        gluLookAt(23.5, 10, -9,
            6, 5, -9,
            0, 1, 0);
    }

    //from the south
    if (direction == 2) {
        gluLookAt(6, 10, 7,
            6, 5, -9,
            0, 1, 0);
    }

    //from the west
    if (direction == 3) {
        gluLookAt(-11.5, 10, -9,
            6, 5, -9,
            0, 1, 0);
    }

    //from above (birds eye view)
    if (direction == 4) {
        gluLookAt(6, 20, -9,
            6, 0, -9,
            0, 0, -1);
    }

    if (direction == 5) {
        gluLookAt(xVal,
            5.0,
            zVal,
            xVal - sin((PI / 180.0) * angle),
            5.0,
            zVal - cos((PI / 180.0) * angle),
            0.0,
            1.0,
            0.0);
    }
}

bool personCollision(float x, float z, float a) {
    const float bufferDistance = 3;
    
    // This will not return anything, but instead trigger annimation
    if ((z > -50) && (z < -25)) {
        if (x > 22) {
            withinContainerRadius = true;
        }
        else
            withinContainerRadius = false;
    }
    else
        withinContainerRadius = false;

    // check collision with table
    if (x + 1 > 27) {
        if ((z + 1 < -27) && (z + 1 > -46)) {
            cout << "table" << endl;
            return true;
        }
    }

    // check collision with back hallway wall
    if (z + bufferDistance >= hBackZ) {
        cout << "back hallway wall" << endl;
        return true;
    }

    //check collision with left walls
    if (x - bufferDistance <= hLeftX) {
        cout << "left wall" << endl;
        return true;
    }

    //check collision with right hallway wall
    if (zVal >= -20) {
        if (x + bufferDistance >= hRightX) {
            cout << "right hallway wall" << endl;
            return true;
        }
    }

    //check collision with front facing main room wall
    if (z - bufferDistance <= mFrontZ) {
        cout << "front facing main wall" << endl;
        return true;
    }

    //check collision with right main wall
    if (x + bufferDistance >= mRightX) {
        cout << "right main wall" << endl;
        return true;
    }

    //check collision with back wall in main area
    if (x >= hRightX - 7) {
        if (z + bufferDistance >= mLeftZ) {
            cout << "back main wall" << endl;
            return true;
        }
    }

    //check collision when the door is closed
    //program will not work if you shut door on yourself --> may need to do some edge cases
    if (leg == 1) {
        if (zVal > -20) {
            if (z - bufferDistance <= -20) {
                cout << "door" << endl;
                return true;
            }
        }
        else if (zVal < -20) {
            if (z + bufferDistance >= -20) {
                cout << "door" << endl;
                return true;
            }
        }
    }

    return false;
}

void drawWalls() {
    
    //back hallway wall
    glPushMatrix();
    glTranslated(hBackX, 0, hBackZ);
    double n = 10.0;  //experiment with different values
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 300; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 500; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();


    //left hallway wall
    glPushMatrix();
    glTranslated(hLeftX, 0, hLeftZ);
    glRotated(90, 0, 1, 0);
    n = 10.0;  //experiment with different values
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 300; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 500; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //right hallway wall
    glPushMatrix();
    glTranslated(hRightX, 0, hLeftZ);
    glRotated(90, 0, 1, 0);
    n = 10.0;  //experiment with different values
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 300; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 500; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //left main room w/ window wall
    
    //first full wall 
    glPushMatrix();
    glTranslated(mLeftX, 0, mLeftZ);
    glRotated(90, 0, 1, 0);
    n = 10.0;  //experiment with different values
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 300; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 100; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //bottom part of window
    glPushMatrix();
    glTranslated(mLeftX, 0, -29);
    glRotated(90, 0, 1, 0);
    n = 10.0;  //experiment with different values
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 30; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 200; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //top part of window
    glPushMatrix();
    glTranslated(mLeftX, 17, -29);
    glRotated(90, 0, 1, 0);
    n = 10.0;  //experiment with different values
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 30; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 200; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //rest of left main wall
    glPushMatrix();
    glTranslated(mLeftX, 0, -49);
    glRotated(90, 0, 1, 0);
    n = 10.0;  //experiment with different values
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 300; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 100; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //front facing wall in main room
    glPushMatrix();
    glTranslated(mFrontX, 0, mFrontZ);
    n = 10.0;  //experiment with different values
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 300; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 550; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //right wall of main room
    glPushMatrix();
    glTranslated(mRightX, 0, mRightZ);
    glRotated(90, 0, 1, 0);
    n = 10.0;  //experiment with different values
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 300; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 400; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();
}

void drawDoor() {

    //door on the right
    glPushMatrix();
    glTranslated(xDoor1, 0, -20);
    double n = 10.0;  
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 300; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 350; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //door on the left
    glPushMatrix();
    glTranslated(xDoor2, 0, -20);
    glRotated(180, 0, 1, 0);
    n = 10.0;
    glNormal3f(0.0, 0.0, -1.0);
    for (int r = 0; r < 300; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 225; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //
}

void drawOutside() {
    float matAmbAndDiffgrass[] = { 0.2, 0.5, 0.2, 1.0 };
    float matAmbAndDiffTrunk[] = { 0.3, 0.1, 0.0, 1.0 };
    float matAmbAndDiffLeaves[] = { 0.1, 0.3, 0.1, 1.0 };

    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);


    // grass plane
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffgrass);
    glPushMatrix();
    glNormal3f(0.0, 1.0, 0.0);
    glTranslated(-13, 0, -80);
    glScaled(5, 1, 5);
    for (int i = 0; i < 20; i++) {
        glTranslated(-1, 0, 20);
        for (int j = 0; j < 20; j++) {
            glTranslated(0, 0, -1);
            glutSolidCube(1);
        }
    }
    glPopMatrix();

    // trees

    glPushMatrix();
    glTranslated(-35, 0, -55);

    // leaves
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffLeaves);
    glPushMatrix();
    glTranslated(0, 5, 0);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(5, 10, 10, 10);
    glPopMatrix();

    // trunk
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffTrunk);
    glRotated(-90, 1, 0, 0);
    gluCylinder(quad, 2, 2, 5, 10, 10);

    glPopMatrix();

    glPushMatrix();
    glTranslated(-55, 0, -23);

    // leaves
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffLeaves);
    glPushMatrix();
    glTranslated(0, 5, 0);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(5, 10, 10, 10);
    glPopMatrix();

    // trunk
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffTrunk);
    glRotated(-90, 1, 0, 0);
    gluCylinder(quad, 2, 2, 5, 10, 10);

    glPopMatrix();

    glPushMatrix();
    glTranslated(-25, 0, -30);

    // leaves
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffLeaves);
    glPushMatrix();
    glTranslated(0, 5, 0);
    glRotated(-90, 1, 0, 0);
    glutSolidCone(5, 10, 10, 10);
    glPopMatrix();

    // trunk
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffTrunk);
    glRotated(-90, 1, 0, 0);
    gluCylinder(quad, 2, 2, 5, 10, 10);

    glPopMatrix();


    // Campfire

}



// Initialization routine.
void setup(void)
{
    glClearColor(0.2, 0.6, 0.8, 0.0);
    glEnable(GL_DEPTH_TEST); // Enable depth testing.

    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);

    // Light property vectors.

    // spotlight
    float lightAmb0[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDifAndSpec0[] = { 1.0, 1.0, 1.0, 1.0 };  //MJB

    //main room hanging light
    float lightAmb1[] = { 0.5, 0.5, 0.5, 1.0 };
    float lightDifAndSpec1[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

    // spotlight properties.   **LIGHTING**
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);

    //main room hanging light
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);

    glEnable(GL_LIGHT0); // Enable particular light source.
    glEnable(GL_LIGHT1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.


}

void updateVariables() {

    // For door
    if (button1Clicked) {
        if (leg == 1) {
            if (xDoor2 >= -10) {
                xDoor2--;
                xDoor1++;
            }
            else {
                button1Clicked = false;
                leg = 2;
            }
        }
        else if (leg == 2) {
            if (xDoor2 <= 6) {
                xDoor2++;
                xDoor1--;
            }
            else {
                button1Clicked = false;
                leg = 1;
            }
        }
    }

    // for card -> only when within radius (or c was pressed)
    if (withinContainerRadius) {
        if (cardLeg == 1) {
            if (cardY < 5) {
                cardY+= 0.5;
            }
            else {
                cardLeg = 2;
            }
        }
    }
    else if (!withinContainerRadius && cardLeg == 2) {
        if (cardY > 2) {
            cardY-=0.5;
            if (cardAngle > 0)
                cardAngle -= 15;
        }
        else {
            cardLeg = 1;
            cardAngLeg = 1;
        }
    }

    // for card when clicked
    if (cardClicked && withinContainerRadius) {
        if (cardAngLeg == 1) {
            if (cardAngle < 90) {
                cardAngle+=5;
            }
            else {
                cardClicked = false;
                cardRotated = true;
                cardAngLeg = 2;
            }
        }
        else if (cardAngLeg == 2) {
            if (cardAngle > 0) {
                cardAngle -= 5;
            }
            else {
                cardClicked = false;
                cardAngLeg = 1;
            }
        }
    }

    if (cardRotated) {
        if (!celebrateDone) {
            if (randomNumber == 1) {
                if (matAmbAndDifWalls[0] < 0.7)
                    matAmbAndDifWalls[0] += 0.1;
                else
                    celebrateDone = true;
                if (matAmbAndDifWalls[1] > 0.2)
                    matAmbAndDifWalls[1] -= 0.1;
                else
                    celebrateDone = true;
                if (matAmbAndDifWalls[2] > 0.2)
                    matAmbAndDifWalls[2] -= 0.1;
                else 
                    celebrateDone = true;
            }
            else if (randomNumber == 2) {
                if (matAmbAndDifWalls[1] > 0.15)
                    matAmbAndDifWalls[1] -= 0.1;
                else
                    celebrateDone = true;
                if (matAmbAndDifWalls[2] > 0.5)
                    matAmbAndDifWalls[2] -= 0.1;
                else
                    celebrateDone = true;
            }
 
        }
    }

    // For blinds -> update global amb light
    if (blindsClicked) {
        //close blinds
        if (blindsLeg == 1) {
            if (blindsScaledY <= 15) {
                blindsScaledY++;
                blindsY -= 0.5;

                globAmb[0] -= 0.025;
                globAmb[1] -= 0.025;
                globAmb[2] -= 0.025;
            }
            else {
                blindsClicked = false;
                blindsLeg = 2;
            }
        }
        //open blinds
        else if (blindsLeg == 2) {
            if (blindsScaledY >= 2) {
                blindsScaledY--;
                blindsY += 0.5;

                globAmb[0] += 0.025;
                globAmb[1] += 0.025;
                globAmb[2] += 0.025;
            }
            else {
                blindsClicked = false;
                blindsLeg = 1;
            }
        }
    }

    if (fireIgnited) {
        fireAngle += 20;
        if (fireAngle == 360)
            fireAngle = 0;
    }


    glutPostRedisplay();
}


void annimate(int someValue)
{
    updateVariables();
    glutTimerFunc(100, annimate, 1);
}

// Routine to draw a stroke character string.
void writeStrokeString(void* font, const char* string)
{
    const char* c;
    for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

void drawShapes() {

    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);

    float matAmbAndDiffButton1[] = { 0.0, 0.9, 0.0, 1.0 };
    float matAmbAndDiffBlinds[] = { 1.0, 1.0, 1.0, 1.0 };
    float matAmbAndDiffFire[] = { 0.8, 0.13, 0.13 };
    float matAmbAndDiffLogs[] = { 0.3, 0.1, 0.1, 1.0 };

    float emmissionButton1[] = { 0.0, 1.0 , 0.0 , 1.0 };
    float emmissionFire[] = { 1.0, 0.13, 0.13, 1.0 };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

    // button on the left side to open door

    glPushMatrix();
    glTranslated(-13, 5, -20);
    if (isPicking) //mouse was clicked
    {
        glDisable(GL_LIGHTING);
        glColor3ubv(button1IDColor);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffButton1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmissionButton1);
    }
    //then build it.
    glScaled(2, 2, 2);
    glutSolidCube(1);
    //end cube
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmissionDefault);
    glPopMatrix();

    //blinds of the left side of the main room

    glPushMatrix();
    glTranslated(mLeftX+0.2, 17, -28);
    if (isPicking) //mouse was clicked
    {
        glDisable(GL_LIGHTING);
        glColor3ubv(blindsIDColor); 
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffBlinds);
    }
    //then build it.
    glTranslated(1, blindsY, 1);
    glScaled(1, blindsScaledY, 1);
    for (int i = 0; i < 21; i++) {
        glTranslated(0, 0, -1);
        glutSolidCube(1);
    }
    //end cube
    glPopMatrix();


    // card inside container
    glPushMatrix();
    glTranslated(33, cardY, -37);
    if (isPicking) //mouse was clicked
    {
        glDisable(GL_LIGHTING);
        glColor3ubv(cardIDColor);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffBlinds);
    }
    //then build it.
    glPushMatrix();
    glRotated(cardAngle, 0, 0, 1);
    glScaled(3.5, 0.5, 3);
    glutSolidCube(1);
    glPopMatrix();


    // text
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 0.0);
    glTranslated(-1, 0.2, -1.2);
    glScaled(0.003, 0.003, 0.003);
    glRotated(cardAngle, 0, 0, 1);
    glRotated(-90, 0, 1, 0);
    glRotated(-90, 1, 0, 0);

    if (randomNumber == 1) 
        writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)"Appa");
    else if (randomNumber == 2)
        writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)"Momo");
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPopMatrix();

    // Logs outside 
    glPushMatrix();
    glTranslated(-41, 0.5, -41);
    if (isPicking) {
        glDisable(GL_LIGHTING);
        glColor3ubv(fireIDColor);
    }
    else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffLogs);
    }
    glRotated(45, 0, 1, 0);
    gluCylinder(quad, 1, 1 , 8, 10, 10);
    glTranslated(-3, 0, 3);
    glRotated(90, 0, 1, 0);
    gluCylinder(quad, 1, 1, 8, 10, 10);
    glPopMatrix();


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffFire);
    glPushMatrix();
    if (fireIgnited)
        glTranslated(-41, 1, -39);
    else
        glTranslated(-41, -5, -41);
    glRotated(90, 0, 0, 1);
    glRotated(fireAngle, 1, 1, 1);
    glScaled(1, 4, 1);


    glutSolidTeapot(1);

    glTranslated(-1, 0, 0);
    glRotated(180, 0, 1, 0);
    glutSolidTeapot(1);
    glPopMatrix();

}

void drawFurniture() {
    //materials
    float matAmbAndDiffTable[] = { 0.7, 0.4, 0.1, 1.0 };
    float matAmbAndDiffTabletop[] = { 1.0, 1.0, 1.0, 1.0 };
    float matAmbAndDiffTeapot[] = { 0.55, 0.47, 0.14 };
    float matAmbAndDiffContainer[] = { 0.85 , 0.5, 0.1 };



    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffTable);

    //table
    glPushMatrix();
    glTranslated(29, 0, -45);
    glPushMatrix();
    glScaled(0.5, 3, 0.5);
    glutSolidCube(1);
    glTranslated(0,0,30);
    glutSolidCube(1);

    glTranslated(20, 0, 0);
    glutSolidCube(1);

    glTranslated(0, 0, -30);
    glutSolidCube(1);
    glPopMatrix();

    // teapot on table next to container
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffTeapot);
    glPushMatrix();
    glEnable(GL_NORMALIZE);
    glTranslated(3, 2.5, 2);
    glutSolidTeapot(1);
    glPopMatrix();

    // container -> letter will be drawn in drawShapes
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffContainer);
    glPushMatrix();
    glTranslated(2, 1.8, 10);
    glScaled(4, 2, 4);
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(-1, 0, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 1, -1);
    glVertex3f(0, 0, -1);

    glNormal3f(0, 0, -1);
    glVertex3f(1, 1, -1);
    glVertex3f(1, 0, -1);

    glNormal3f(1, 0, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(1, 0, 0);

    glNormal3f(0, 0, 1);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 0);

    glNormal3f(-1, 0, 0);
    glVertex3f(0, 0, -1);
    glVertex3f(1, 0, 0);

    glNormal3f(0, -1, 0);
    glVertex3f(1, 0, -1);
    glEnd();
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffTabletop);
    
    glTranslated(5, 1.5, -1);
    glScaled(11, 0.5, 1);
    for (int i = 0; i < 16; i++) {
        glTranslated(0, 0, 1);
        glutSolidCube(1);
    }

    glPopMatrix();

    
}

void getID(int x, int y)
{
    unsigned char pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    
    if (pixel[0] == 255 && pixel[1] == 128 && pixel[2] == 0)//orange
        button1Clicked = true;
    else if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 0)
        blindsClicked = true;
    else if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 255)
        cardClicked = true;
    else if (pixel[0] == 255 && pixel[1] == 0 && pixel[2] == 255)
        fireIgnited = !fireIgnited;
    else
        button1Clicked = false;

    glutPostRedisplay();
}


// Drawing routine.
void drawScene(void)
{

    float matSpec[] = { 0.5, 0.5, 0.5, 1.0 };
    float matShine[] = { 500.0 };

    // **LIGHTING**
    float lightPos0[] = { 1, 3, 0, 1.0 }; // Spotlight position.
    float spotDirection[] = { -sin((PI / 180.0) * angle), -.1, -cos((PI / 180.0) * angle) }; // Spotlight direction.
    float lightDifAndSpec0[] = { 1.0, 1.0, 1.0, 1.0 };  //MJB

    float lightPos1[] = {13, 20, -41};
    float mainDirection[] = { 0, -1, 0 };
    float lightDifAndSpec1[] = { 1.0, 1.0, 1.0, 1.0 };

    float lightPosSun[] = { 0, 50, -41 };
    float lightDiffAndSpecSun[] = { 1.0, 1.0, 1.0, 1.0 };

    //toggle the headlamp
    if (!headLamp) {
        lightDifAndSpec0[0] = 0.0;
        lightDifAndSpec0[1] = 0.0;
        lightDifAndSpec0[2] = 0.0;
    }

    //toggle the main room light
    if (!mainRoomLight) {
        lightDifAndSpec1[0] = 0.0;
        lightDifAndSpec1[1] = 0.0;
        lightDifAndSpec1[2] = 0.0;
    }

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);//MJB
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);//MJB

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffAndSpecSun);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffAndSpecSun);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_BUFFER_BIT);

    //Set Projection
    setProjection();

    //Clear Modeview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightfv(GL_LIGHT2, GL_POSITION, lightPosSun);

    setLook();


    glPushMatrix();
    glTranslatef(xVal, 6, zVal); // Move the spotlight.
    //glTranslatef(xLight, yLight, zLight);

    // Draw the spotlight cone in wireframe after disabling lighting
   //MJB    
    if (seeCone) {
        glPushMatrix();
        glDisable(GL_LIGHTING);
        glRotated(angle, 0, 1, 0);
        glColor3f(1.0, 1.0, 1.0);
        glutWireSphere(1, 5, 5);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    // Spotlight properties including position.   **LIGHTING**
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(13, 20, -41); // Move the main room Light.

    // Draw the spotlight cone in wireframe after disabling lighting
   //MJB    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDiffMainLight);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmissionMainLight);

    glPushMatrix();
    if (!mainRoomLight) {
        emmissionMainLight[0] = 0;
        emmissionMainLight[1] = 0;
        emmissionMainLight[2] = 0;
        matAmbAndDifCeiling[0] = 0.2;
        matAmbAndDifCeiling[1] = 0.2;
        matAmbAndDifCeiling[2] = 0.2;
    }
    else {
        emmissionMainLight[0] = 1;
        emmissionMainLight[1] = 1;
        emmissionMainLight[2] = 1;
        matAmbAndDifCeiling[0] = 1.;
        matAmbAndDifCeiling[1] = 1;
        matAmbAndDifCeiling[2] = 1;
    }
    glutSolidSphere(3, 10, 10);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmissionDefault);

    // main room Light properties including position.   **LIGHTING**
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDifDoors);

    drawDoor();

    //Material properties of the walls
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDifWalls);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

    drawWalls();


    //draw ceiling
    glPushMatrix();
    glTranslatef(-18.0, 20, -60); //move to the left
    glRotated(90, 1, 0, 0);

    //n rows and n columns of triangles
    double n = 10.0;  //experiment with different values
    glNormal3f(0.0, -1.0, 0.0);
    for (int r = 0; r < 950; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 600; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    drawOutside();
    drawFurniture();

    // Material properties of the flat plane (only the front is ever seen). -- Floor
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDifFloor);
    

    //multitriangle square
    glEnable(GL_NORMALIZE);

    glPushMatrix();
    glTranslatef(-15.0, 0, -60); //move to the left
    glRotated(90, 1, 0, 0);

    //n rows and n columns of triangles
    n = 10.0;  //experiment with different values
    glNormal3f(0.0, 1.0, 0.0);
    for (int r = 0; r < 950; r++)  //replace n by 1 to see one row
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c <= 600; c++)
        {
            glVertex3f(c / n, r / n, 0.0);
            glVertex3f(c / n, (r + 1) / n, 0.0);
        }
        glEnd();
    }
    glPopMatrix();

    //if mouse was clicked figure out what object
    //was selected
    if (isPicking)
    {

        drawShapes();
        getID(xClick, yClick);
        isPicking = false;
        glEnable(GL_LIGHTING);
    }
    else
    {
        //Real drawing, with current status.
        drawShapes(); //real colors
        glutSwapBuffers();
    }

}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    setProjection();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'P':
        direction = 5;
        glutPostRedisplay();
        break;
    case 'h':
        headLamp = !headLamp;
        glutPostRedisplay();
        break;
    case 's':
        mainRoomLight = !mainRoomLight;
        glutPostRedisplay();
        break;
    case 'd':
        button1Clicked = true;
        glutPostRedisplay();
        break;
    case 'w':
        blindsClicked = true;
        glutPostRedisplay();
        break;
    case 'c':
        withinContainerRadius = !withinContainerRadius;
        glutPostRedisplay();
        break;
    case 'r':
        cardClicked = true;
        glutPostRedisplay();
        break;
    case 'f':
        fireIgnited = !fireIgnited;
        glutPostRedisplay();
        break;
    case 'G':
        if (globAmb[0] < 1) {
            globAmb[0] += 0.025;
            globAmb[1] += 0.025;
            globAmb[2] += 0.025;
        }
        glutPostRedisplay();
        break;
    case 'g':
        if (globAmb[0] > 0) {
            globAmb[0] -= 0.025;
            globAmb[1] -= 0.025;
            globAmb[2] -= 0.025;
        }
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void mouseControl(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN && button == GLUT_LEFT)
    {
        isPicking = true;
        xClick = x;
        yClick = 500 - y; //for mouse vs window coordinates

        glutPostRedisplay();
    }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

    // Compute next position.
    if (key == GLUT_KEY_LEFT) tempAngle = angle + 5.0;
    if (key == GLUT_KEY_RIGHT) tempAngle = angle - 5.0;
    if (key == GLUT_KEY_UP)
    {
        tempxVal = xVal - sin(angle * PI / 180.0);
        tempzVal = zVal - cos(angle * PI / 180.0);
    }
    if (key == GLUT_KEY_DOWN)
    {
        tempxVal = xVal + sin(angle * PI / 180.0);
        tempzVal = zVal + cos(angle * PI / 180.0);
    }

    // Angle correction.
    if (tempAngle > 360.0) tempAngle -= 360.0;
    else if (tempAngle < 0.0) tempAngle += 360.0;

    // Move spacecraft to next position only if there will not be collision with an asteroid.
    if (!personCollision(tempxVal, tempzVal, tempAngle))
    {
        isCollision = 0;
        xVal = tempxVal;
        zVal = tempzVal;
        angle = tempAngle;
    }
    else isCollision = 1;

    cout << "(" << xVal << "," << zVal << ")" << endl;
    cout << "angle: " << angle << endl;
    

    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press esc to quit" << endl;
    cout << "Press up/down to move forward/backwards, left/right to look left/right" << endl;
    cout << "Press h to turn on your headlamp" << endl;
    cout << "Press s to turn on Main room Light" << endl;
    cout << "Press d to open/close door OR click on green button on the left" << endl;
    cout << "Press w to open/close Window Blinds OR click on the blinds" << endl;
    cout << "Press c or get close to the container on the table to make paper rise from the container" << endl;
    cout << "Press r or press on paper (once arisen) to rotate paper" << endl;
    cout << "Press f or press on the logs outside the window to ignite the fire" << endl;
    cout << "Press G/g to increase/decrease ambient light" << endl;
    
}

void top_menu(int id) {
    if (id == 1) exit(0);
}

void winner_menu(int id) {
    if (id == 2)
        randomNumber = 1;
    if (id == 3)
        randomNumber = 2;
    glutPostRedisplay();
}

void makeMenu() {
    int sub_menu;
    sub_menu = glutCreateMenu(winner_menu);
    glutAddMenuEntry("Appa", 2);
    glutAddMenuEntry("Momo", 3);

    glutCreateMenu(top_menu);
    glutAddSubMenu("Rig Election", sub_menu);
    glutAddMenuEntry("Quit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Main routine.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Lego Scene with gluLookAt");
    setup();
    setRandom();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouseControl);
    glutSpecialFunc(specialKeyInput);
    glutTimerFunc(100, annimate, 1);
    printInteraction();
    makeMenu();
    glutMainLoop();

    return 0;
}

