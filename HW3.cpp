/////////////////////////////////          
// LegoLookAt.cpp
//
// LookAt a Lego scene
//
// Marjory Baruch
/////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define PI 3.14159

//globals
int direction = 0; // 0: North, 1: East, 2: South, 3: West
static float numVerticies = 20;
bool BLUEFLOWER = false;
bool PURPLEFLOWER = false;
bool REDFLOWER = false;
int currState = 0; // 0: start | 1: blue | 2: purple | 3: red
bool updateState = false;
int height, width;
bool selecting = false;
int xClick, yClick;  //coordinates of mouseclick

// for bee posistion
float startX = 6, startY = 9, startZ = -9; 
static float endX, endY, endZ;
static float Vx, Vy, Vz;
static float Mx = 0, My = 0, Mz = 0;
static int t = 0;
float beeRotationAngle = 0;

//for bee Wing Flaps
static float wingsRotation;
static float wingHeight;
static float beeLeg = 1;
static int e = 20;


//for cloud1 posistion
static float cloud1Px = -5, cloud1Py = 20, cloud1Pz = -10;
static float cloud1Qx = - 10, cloud1Qy = 20, cloud1Qz = -10;
int cloud1Leg = 1;
static int i = 0;
static float cloud1Vx, cloud1Vy, cloud1Vz;
static float cloud1Mx = 0, cloud1My = 0, cloud1Mz = 0;
static float cloud1StartX, cloud1StartY, cloud1StartZ;
static float cloud1EndX, cloud1EndY, cloud1EndZ;

//for cloud 2 posistion
static float cloud2Px = 12, cloud2Py = 24, cloud2Pz = -13;
static float cloud2Qx = 17, cloud2Qy = 24, cloud2Qz = -13;
int cloud2Leg = 1;
static int a = 0;
static float cloud2Vx, cloud2Vy, cloud2Vz;
static float cloud2Mx = 0, cloud2My = 0, cloud2Mz = 0;
static float cloud2StartX, cloud2StartY, cloud2StartZ;
static float cloud2EndX, cloud2EndY, cloud2EndZ;

//for cloud 3 posistion
static float cloud3Px = 3 , cloud3Py =  19, cloud3Pz = -20;
static float cloud3Qx = 8, cloud3Qy = 24, cloud3Qz = -20;
int cloud3Leg = 1;
static int b = 0;
static float cloud3Vx, cloud3Vy, cloud3Vz;
static float cloud3Mx = 0, cloud3My = 0, cloud3Mz = 0;
static float cloud3StartX, cloud3StartY, cloud3StartZ;
static float cloud3EndX, cloud3EndY, cloud3EndZ;

void assignEndpointMakeVector(
    float Px, float Py, float Pz,
    float Qx, float Qy, float Qz) 
{
    startX = Px;
    startY = Py;
    startZ = Pz;
    endX = Qx;
    endY = Qy;
    endZ = Qz;
    Vx = endX - startX;
    Vy = endY - startY;
    Vz = endZ - startZ;

    float dx = endX - startX;
    float dz = endZ - startZ;
    beeRotationAngle = atan2(dx, dz) * (180 / PI);
}

void assignEndpointMakeVectorcloud1(
    float Px, float Py, float Pz,
    float Qx, float Qy, float Qz)
{
    cloud1StartX = Px;
    cloud1StartY = Py;
    cloud1StartZ = Pz;
    cloud1EndX = Qx;
    cloud1EndY = Qy;
    cloud1EndZ = Qz;
    cloud1Vx = Qx - Px;
    cloud1Vy = Qy - Py;
    cloud1Vz = Qz - Pz;
}

void assignEndpointMakeVectorcloud2(
    float Px, float Py, float Pz,
    float Qx, float Qy, float Qz)
{
    cloud2StartX = Px;
    cloud2StartY = Py;
    cloud2StartZ = Pz;
    cloud2EndX = Qx;
    cloud2EndY = Qy;
    cloud2EndZ = Qz;
    cloud2Vx = Qx - Px;
    cloud2Vy = Qy - Py;
    cloud2Vz = Qz - Pz;
}

void assignEndpointMakeVectorcloud3(
    float Px, float Py, float Pz,
    float Qx, float Qy, float Qz)
{
    cloud3StartX = Px;
    cloud3StartY = Py;
    cloud3StartZ = Pz;
    cloud3EndX = Qx;
    cloud3EndY = Qy;
    cloud3EndZ = Qz;
    cloud3Vx = Qx - Px;
    cloud3Vy = Qy - Py;
    cloud3Vz = Qz - Pz;
}

void setProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-4, 4, -4, 4, 2, 30);
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
}

void drawFlowers() {

    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL); 


    //purple flower 128 0 128

    //stem
    glPushMatrix();
    glColor3f(0.2, 0.5, 0.2);
    glTranslated(-3, 6, -14);
    glRotated(90, 1, 0, 0);
    gluCylinder(quad, 1, 1, 6, 10, 10);
    glPopMatrix();

    //pedal
    glPushMatrix();
    glColor3f(0.5, 0.0, 0.5);
    glTranslated(-3, 6, -14);
    glScalef(2.0, 1.0, 2.0);

    glPushMatrix();
    glScalef(.8, 1.0, 1.2);
    glTranslated(0.0, 0.0, -1.0);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0.0, 0.0, -1.0);
    glScalef(.8, 1.0, 1.2);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0.0, 0.0, 1.0);
    glScalef(.8, 1.0, 1.2);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, 0.0, 1.0);
    glScalef(.8, 1.0, 1.2);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    //leaf
    glPushMatrix();
    glColor3f(0.2, 0.5, 0.2);
    glTranslated(-3, 2, -14);

    glPushMatrix();
    glTranslated(0, 0, -0.5);
    glRotated(160, 0, 1, 0);
    glRotated(-30, 1, 0, 0);
    glutSolidCone(1, 4, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(-20, 1, 0, 0);
    glutSolidCone(1, 7, 10, 10);
    glPopMatrix();

    glPopMatrix();

    //blue flower

    //stem
    glPushMatrix();
    glColor3f(0.2, 0.5, 0.2);
    glTranslated(6, 6, -3);
    glRotated(90, 1, 0, 0);
    gluCylinder(quad, 1, 1, 6, 10, 10);
    glPopMatrix();

    //pedal
    glPushMatrix();
    glColor3f(0.2, 0.2, 0.7);
    glTranslated(6, 6, -3);
    glScalef(2.0, 1.0, 2.0);

    glPushMatrix();
    glScalef(.8, 1.0, 1.2);
    glTranslated(0.0, 0.0, -1.0);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0.0, 0.0, -1.0);
    glScalef(.8, 1.0, 1.2);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0.0, 0.0, 1.0);
    glScalef(.8, 1.0, 1.2);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, 0.0, 1.0);
    glScalef(.8, 1.0, 1.2);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();
    
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    //leaf
    glPushMatrix();
    glColor3f(0.2, 0.5, 0.2);
    glTranslated(6, 2, -3);
    glRotated(45, 0, 1, 0);

    glPushMatrix();
    glTranslated(0, 0, -0.5);
    glRotated(160, 0, 1, 0);
    glRotated(-30, 1, 0, 0);
    glutSolidCone(1, 4, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(-20, 1, 0, 0);
    glutSolidCone(1, 7, 10, 10);
    glPopMatrix();

    glPopMatrix();


    //red flower

    //stem
    glPushMatrix();
    glColor3f(0.2, 0.5, 0.2);
    glTranslated(14, 6, -11);
    glRotated(90, 1, 0, 0);
    gluCylinder(quad, 1, 1, 6, 10, 10); 
    glPopMatrix();

    //pedal
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslated(14, 6, -11);
    glScalef(2.0, 1.0, 2.0);

    glPushMatrix();
    glScalef(.8, 1.0, 1.2);
    glTranslated(0.0, 0.0, -1.0);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0.0, 0.0, -1.0);
    glScalef(.8, 1.0, 1.2);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0.0, 0.0, 1.0);
    glScalef(.8, 1.0, 1.2);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, 0.0, 1.0);
    glScalef(.8, 1.0, 1.2);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    //leaf
    glPushMatrix();
    glColor3f(0.2, 0.5, 0.2);
    glTranslated(14, 2, -11);
    glRotated(80, 0, 1, 0);

    glPushMatrix();
    glTranslated(0, 0, -0.5);
    glRotated(160, 0, 1, 0);
    glRotated(-30, 1, 0, 0);
    glutSolidCone(1, 4, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotated(-20, 1, 0, 0);
    glutSolidCone(1, 7, 10, 10);
    glPopMatrix();

    glPopMatrix();

}

void drawCloud1(int x, int y, int z, int r) {

    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(cloud1Mx, cloud1My, cloud1Mz);
    glTranslated(x, y, z);
    glRotated(r, 0, 1, 0);
    
    glPushMatrix();
    glScaled(3, 2, 6);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 1.5, 2);
    glScaled(3, 2, 4);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, -1, 2);
    glScaled(3, 2, 6);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();



    glPopMatrix();
}

void drawCloud2(int x, int y, int z, int r) {

    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(cloud2Mx, cloud2My, cloud2Mz);
    glTranslated(x, y, z);
    glRotated(r, 0, 1, 0);

    glPushMatrix();
    glScaled(3, 2, 6);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 1.5, 2);
    glScaled(3, 2, 4);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, -1, 2);
    glScaled(3, 2, 6);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();



    glPopMatrix();
}

void drawCloud3(int x, int y, int z, int r) {

    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(cloud3Mx, cloud3My, cloud3Mz);
    glTranslated(x, y, z);
    glRotated(r, 0, 1, 0);

    glPushMatrix();
    glScaled(3, 2, 6);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 1.5, 2);
    glScaled(3, 2, 4);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, -1, 2);
    glScaled(3, 2, 6);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();



    glPopMatrix();
}

void drawBee(int shadow) {

    glPushMatrix();
    glTranslated(Mx, My, Mz);
    glTranslated(startX, startY, startZ);

    glRotated(beeRotationAngle+90, 0, 1, 0);

    //body
    glPushMatrix();
    if (shadow) glColor3f(0.0, 0.0, 0.0);
    else glColor3f(0.7, 0.7, 0.0);
    glScaled(3, 1.5, 1.5);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();


    //stinger
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslated(2.2, 0, 0);
    glRotated(90, 0, 1, 0);
    glScaled(0.5, 1.0, 1.0);
    glutSolidCone(1,2,10,10);
    glPopMatrix();


    //nose
    glPushMatrix();
    glTranslated(-3.2, 0, 0);
    glScaled(0.5, 0.5, 0.5);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    //wings

    //wing 1
    glPushMatrix();
    glTranslated(0, wingHeight, 3);
    glRotated(wingsRotation, 1, 0, 0);
    glScaled(1, 0.1, 3);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();


    //wing 2
    glPushMatrix();
    glTranslated(0, wingHeight, -3);
    glRotated(180, 0, 1, 0);
    glRotated(wingsRotation, 1, 0, 0);
    glScaled(1, 0.1, 3);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

/*
Note: 

Used NaiveColorPickingDemo method for colorpicking. 
Was able to figure out my pixel colors by using the following:

    cout << "R: " << (int)pixel[0] << endl;
    cout << "G: " << (int)pixel[1] << endl;
    cout << "B: " << (int)pixel[2] << endl;
    cout << endl;

*/
void identifyColor(int x, int y)
{
    unsigned char pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    
    BLUEFLOWER = false;
    PURPLEFLOWER = false;
    REDFLOWER = false;

    Mx = 0;
    My = 0;
    Mz = 0;

    if ((int)pixel[0] == 51 && (int)pixel[1] == 51 && (int)pixel[2] == 178)
    {
        BLUEFLOWER = true;
        PURPLEFLOWER = false;
        REDFLOWER = false;
        updateState = true;
        cout << "Blue" << endl;
        if (currState == 0)
            assignEndpointMakeVector(startX, startY, startZ, 6, 8, -3);
        else if (currState == 1)
            assignEndpointMakeVector(6, 8, -3, 6, 8, -3);
        else if (currState == 2)
            assignEndpointMakeVector(-3, 8, -14, 6, 8, -3);
        else if (currState == 3)
            assignEndpointMakeVector(14, 8, -11, 6, 8, -3);
        currState = 1;

    }
    else if ((int)pixel[0] == 128 && (int)pixel[1] == 0 && (int)pixel[2] == 128)
    {
        PURPLEFLOWER = true;
        BLUEFLOWER = false;
        REDFLOWER = false;
        updateState = true;
        cout << "Purple" << endl;
        if (currState == 0)
            assignEndpointMakeVector(startX, startY, startZ, -3, 8, -14);
        else if (currState == 1)
            assignEndpointMakeVector(6, 8, -3, - 3, 8, -14);
        else if (currState == 2)
            assignEndpointMakeVector(-3, 8, -14, -3, 8, -14);
        else if (currState == 3)
            assignEndpointMakeVector(14, 8, -11, -3, 8, -14);

        currState = 2;
    }
    else if ((int)pixel[0] == 255 && (int)pixel[1] == 0 && (int)pixel[2] == 0)
    {
        REDFLOWER = true;
        BLUEFLOWER = false;
        PURPLEFLOWER = false;
        updateState = true;
        cout << "red" << endl;
        if (currState == 0)
            assignEndpointMakeVector(startX, startY, startZ, 14, 8, -11);
        else if (currState == 1)
        assignEndpointMakeVector(6, 8, -3, 14, 8, -11);
        else if (currState == 2)
            assignEndpointMakeVector(-3, 8, -14, 14, 8, -11);
        else if (currState == 3)
            assignEndpointMakeVector(14, 8, -11, 14, 8, -11);

        currState = 3;
    }

    selecting = false;
    glutPostRedisplay();
}

void increaseCloudVector() {

    //cloud 1 
    if (i == 30 && cloud1Leg == 1) //go back
    {
        i = 0;
        cloud1Leg = 2;
        assignEndpointMakeVectorcloud1(cloud1Qx, cloud1Qy, cloud1Qz,
                                       cloud1Px, cloud1Py, cloud1Pz);

    }
    else if (i == 30 && cloud1Leg == 2) //go forward
    {
        i = 0;
        cloud1Leg = 1;
        assignEndpointMakeVectorcloud1(cloud1Px, cloud1Py, cloud1Pz,
                                       cloud1Qx, cloud1Qy, cloud1Qz);
    }

    //cloud 2
    if (a == 30 && cloud2Leg == 1) //go back
    {
        a = 0;
        cloud2Leg = 2;
        assignEndpointMakeVectorcloud2(cloud2Qx, cloud2Qy, cloud2Qz,
                                       cloud2Px, cloud2Py, cloud2Pz);

    }
    else if (a == 30 && cloud2Leg == 2) //go forward
    {
        a = 0;
        cloud2Leg = 1;
        assignEndpointMakeVectorcloud2(cloud2Px, cloud2Py, cloud2Pz,
                                 cloud2Qx, cloud2Qy, cloud2Qz);
    }

    //cloud 3
    if (b == 30 && cloud3Leg == 1) //go back
    {
        b = 0;
        cloud3Leg = 2;
        assignEndpointMakeVectorcloud3(cloud3Qx, cloud3Qy, cloud3Qz,
                                 cloud3Px, cloud3Py, cloud3Pz);

    }
    else if (b == 30 && cloud3Leg == 2) //go forward
    {
        b = 0;
        cloud3Leg = 1;
        assignEndpointMakeVectorcloud3(cloud3Px, cloud3Py, cloud3Pz,
                                 cloud3Qx, cloud3Qy, cloud3Qz);
    }

    //update partial vector
    i++;
    cloud1Mx = i / 30.0 * cloud1Vx;
    cloud1My = i / 30.0 * cloud1Vy;
    cloud1Mz = i / 30.0 * cloud1Vz;

    a++;
    cloud2Mx = a / 30.0 * cloud2Vx;
    cloud2My = a / 30.0 * cloud2Vy;
    cloud2Mz = a / 30.0 * cloud2Vz;

    b++;
    cloud3Mx = b / 30.0 * cloud3Vx;
    cloud3My = b / 30.0 * cloud3Vy;
    cloud3Mz = b / 30.0 * cloud3Vz;

    glutPostRedisplay();
}

void increaseVector() {

    //rotate bee

    if (t < 20 && updateState) {
        // Rotate the bee gradually towards the next flower
        if (beeRotationAngle > 0)
            beeRotationAngle -= 1.0; 
        else if (beeRotationAngle < 0)
            beeRotationAngle += 1.0; 

        // Ensure beeRotationAngle stays within [-180, 180] degrees
        if (beeRotationAngle > 180)
            beeRotationAngle -= 360;
        else if (beeRotationAngle < -180)
            beeRotationAngle += 360;
    }

    //update partial vector
    if (t < 20) {
        t++;
        Mx = t / 20.0 * Vx;
        My = t / 20.0 * Vy;
        Mz = t / 20.0 * Vz;
    }

    //flap wings
    if (e < 20) {
        e++;
        if (beeLeg == 1) {
            wingsRotation += 15;
            wingHeight -= 0.2;

            if (wingsRotation == 30) {
                beeLeg = 2;
            }
        }
        else if (beeLeg == 2) {
            wingsRotation -= 15;
            wingHeight += 0.2;

            if (wingsRotation == -30) {
                beeLeg = 1;
            }
        }
    }
    else {
        wingsRotation = 0;
    }

    if (updateState) {
        t = 0;
        e = 0;
        updateState = false;
    }

    glutPostRedisplay();
}

void animateBee(int someValue)
{
    increaseVector();
    glutTimerFunc(100, animateBee, 1);
}

void animateClouds(int someValue) {
    increaseCloudVector();
    glutTimerFunc(100, animateClouds, 0.1);
}

// Drawing routine.
void drawScene(void)
{
    //Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Set Projection
    setProjection();

    //Clear Modeview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setLook();

    glEnable(GL_DEPTH_TEST);

    //green flat piece
    glColor3f(.3, 0.8, .3);
    glPushMatrix();
    glTranslated(6, -.5, -9.01);
    glScaled(25, 1, 25);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(.3, 0.8, .3);
    glPushMatrix();
    glTranslated(6, -1, -9.01);
    glScaled(1000, 1, 1000);
    glutSolidCube(1);
    glPopMatrix();

    drawFlowers();
    drawCloud1(cloud1StartX, cloud1StartY, cloud1StartZ, 0);
    drawCloud2(cloud2StartX, cloud2StartY, cloud2StartZ, 0);
    drawCloud3(cloud3StartX, cloud3StartY, cloud3StartZ, 120);
    
    //shadow
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glScalef(1.0, 0.0, 1.0);
    drawBee(1);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);

    //actual bee
    drawBee(0);

    if (selecting)
        identifyColor(xClick, yClick);
    //draw everything, possibly with results of clicking
    else
        glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
    glClearColor(0.0, 0.7, 1.0, 0.0);
    assignEndpointMakeVectorcloud1(cloud1Px, cloud1Py, cloud1Pz, cloud1Qx, cloud1Qy, cloud1Pz);
    assignEndpointMakeVectorcloud2(cloud2Px, cloud2Py, cloud2Pz, cloud2Qx, cloud2Qy, cloud2Pz);
    assignEndpointMakeVectorcloud3(cloud3Px, cloud3Py, cloud3Pz, cloud3Qx, cloud3Qy, cloud3Pz);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    setProjection();
    height = h;
    width = w;
}

void mouseControl(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN && button == GLUT_LEFT)
    {
        selecting = true;
        xClick = x;
        yClick = height - y; //for mouse vs window coordinates
        glutPostRedisplay();
    }
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'N':
        direction = 0;
        glutPostRedisplay();
        break;
    case 'E':
        direction = 1;
        glutPostRedisplay();
        break;
    case 'S':
        direction = 2;
        glutPostRedisplay();
        break;
    case 'W':
        direction = 3;
        glutPostRedisplay();
        break;
    case 'A':
        direction = 4;
        glutPostRedisplay();
        break;
    case ' ':
        increaseCloudVector();
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
    cout << "Press esc to quit" << endl;
    cout << "Press N to look from the North" << endl;
    cout << "Press S to look from the South" << endl;
    cout << "Press E to look from the East" << endl;
    cout << "Press W to look from the West" << endl;
    cout << "Press A to look from Above" << endl;
}

// Main routine.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("HW3");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouseControl);
    glutTimerFunc(5, animateBee, 1);
    glutTimerFunc(5, animateClouds, 1);
    printInteraction();
    glutMainLoop();

    return 0;
}

