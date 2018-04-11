/*******************************************
*
* Official Name:  Megi Shehi
*
* Nickname:  Megi
*
* E-mail:  mshehi@syr.edu
*
* Assignment:  Assignment 4
*
* Environment/Compiler:  XCode
*
* Date:  April 3, 2018
*
* References: http://avatar.se/molscript/doc/colour_names.html
*
* Interactions: Press the up arrow key to move forward and the down arrow key to move backwards. Press the left arrow key to turn left, the right arrow key to turn right. Press 'd' to open the double doors - you cannot close them. Press 'l' to turn the light on or off within the house. Press 'o' to turn the headlamp off and on - the headlamp must be on before you can change its colors. Press 'w' for a white headlamp light, 'r' for a red one, 'g' for a green one, 'b' for a blue one. Press 'k' to insert the key into the chest. Turn the key left with 'L' - this is the wrong way. Turn the key right with 'R' - this is the right way. Press the escape key to quit.
*******************************************/
//  Created by Megi Shehi on 3/19/18.
//  Copyright © 2018 Megi Shehi. All rights reserved.


#include <stdio.h>
#include <cmath>
#include <iostream>
#define PI 3.14159265

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

static GLsizei width, height;
static float gx = 0.0, gy = 0.0, gz = 100.0; //position coordinates
static float angle=180; //view angle
static float step=5.0;  //step size
static float turn=10.0; //degrees to turn

//for key
bool wrong = false;
bool rightkey = false;
bool bkey = false;
float kx = 0.0, ky = 0.0;

//for headlamp
float r = 1.0, g = 1.0, b = 1.0;
bool off = false;

static int isSelecting = 0; //for picking and selecting
static int hits; // Number of entries in hit buffer.
static unsigned int buffer[1024]; // Hit buffer.
static unsigned int closestName = 0; // Name of closest hit.
static int highlightFrames = 10;

bool outside = true, open = false; //track whether inside or outside
float leftd = 0.0, rightd = 0.0, lx = 0.0, lz = 0.0, rx = 0.0, rz = 0.0; //door variables
static int animationPeriod = 100;
float gamb[] = {0.2, 0.2, 0.3, 1.0}; //global ambient light
float lightamb[] = {1.0, 1.0, 1.0, 1.0};
float house[] = {0.5 , 0.2, 0.2, 1.0};
float door[] = {0.7, 0.0, 0.3, 1.0};
float outds[] = {0.4, 0.2, 0.2, 1.0};
float grass[] = {0.0, 0.5, 0.2, 1.0};
float on = 45.0; //angle of lightswitch inside house
double clipl[] = {-1.0, 0.0, 0.0, 0.0}, clipr[] = {1.0, 0.0, 0.0, 0.0};
float glow[] = {1.0, 1.0, 1.0, 1.0}, unglow[] = {0.0, 0.0, 0.0, 1.0}, black[] = {0.0, 0.0, 0.0, 1.0};

// Routine to draw a stroke character string.
void writeStrokeString(void *font, const char *string)
{
    const char *c;
    for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

void text(){
    glDisable(GL_LIGHTING);
    if (!rightkey){
    glPushMatrix();
    glTranslatef(-30.0, 10.0, 10.0);
    glScalef(0.015, 0.015, 0.015);
    writeStrokeString(GLUT_STROKE_MONO_ROMAN, "It's dark out. You should find shelter!");
    glPopMatrix();
    }
    glEnable(GL_LIGHTING);
}

void move(int n){
    //to open door
    if (open){
    if (leftd > -90) leftd -=10;
        if (lx > -5) {lx--; lz+=0.5;}
    if (rightd < 90) rightd +=10;
        if (rx < 5) {rx++; rz+=0.5;}
    }
    
    //to use key
    if(bkey){
        if(kx > -45) kx--;
        if(ky > -7) ky--;
    }
    
    glutTimerFunc(animationPeriod, move, 1);
    glutPostRedisplay();
}

//draw
void drawMoon(){
    glPushMatrix();
    //rotate moon, then move it
    glTranslatef(-300.0, 50.0, -40.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glClipPlane(GL_CLIP_PLANE0, clipl); //left clipping plane
    glClipPlane(GL_CLIP_PLANE1, clipr); //right clipping plane
    glEnable(GL_CLIP_PLANE0);
    glColor3f(0.2, 0.2, 0.2);
    if (rightkey) glColor3f(1, 0.843137, 0);
    glutSolidSphere(10, 60, 60); //darker half
    glDisable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    if (!rightkey) glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glow);
    glColor3f(0.8, 0.8, 0.8);
    if (rightkey) glColor3f(1, 0.843137, 0);
    glutSolidSphere(10, 60, 60); //lighter half
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, unglow);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();
    
    glPushMatrix();
    float lightPos[] = { -50.0, 40.0, -40.0, 1.0};
    float mamb[] = {1.0, 1.0, 1.0, 1.0};
    float mdir[] = {1.0, 0.0, 0.0};
    float spec[] = {0.5, 0.5, 0.5, 1.0};
    
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_AMBIENT_AND_DIFFUSE, mamb);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, mdir);
    glEnable(GL_LIGHT1);
    glPopMatrix();
    
}

//little surprise
void drawStars(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glow);
    glColor3f(1.0, 1.0, 1.0);
    
    glPushMatrix();
    glTranslatef(-300, 0.0, 0.0);
    glScalef(3.0, 4.0, 3.0);
    
    
    for (int i = 0; i < 40; i ++){
    glPushMatrix();
    glTranslatef(0, rand()%256, -rand()%256);
    glutSolidIcosahedron();
    glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(0, -rand()%256, rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(0, rand()%256, rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(0, -rand()%256, -rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(300, 0.0, 0.0);
    glScalef(3.0, 4.0, 3.0);
    
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(0, rand()%256, -rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(0, -rand()%256, rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(0, rand()%256, rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(0, -rand()%256, -rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, -300);
    glScalef(3.0, 4.0, 3.0);
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(-rand()%256, rand()%256, 0.0);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(rand()%256, -rand()%256, 0.0);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(rand()%256, rand()%256, 0.0);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(-rand()%256, -rand()%256, 0.0);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, 300);
    glScalef(3.0, 4.0, 3.0);
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(-rand()%256, rand()%256, 0.0);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(rand()%256, -rand()%256, 0.0);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(rand()%256, rand()%256, 0.0);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(-rand()%256, -rand()%256, 0.0);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, 300.0, 0.0);
    glScalef(3.0, 4.0, 3.0);
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(-rand()%256, 0.0,  rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(rand()%256, 0.0, -rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(rand()%256, 0.0, rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    
    for (int i = 0; i < 40; i ++){
        glPushMatrix();
        glTranslatef(-rand()%256, 0.0, -rand()%256);
        glutSolidIcosahedron();
        glPopMatrix();
    }
    glPopMatrix();
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, unglow);
    
}

//ceiling lamp
void drawLamp(void){
    float lightPos[] = { 0.0, 40.0, -40.0, 1.0};
    
    //draw lamp
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0.9, 0.8, 0.8);
    //turn on light if lightswitch on
    if (on == 45.0) glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glow);
    glDisable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glTranslatef(0.0, 20.0, -40.0);
    glutSolidSphere(5.0, 30.0, 30.0);
    glPopMatrix();
    
    //place light, default values are ok for light0 so no need to change much
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90);
    
    //turn glow off for the rest of the materials
    glMaterialfv(GL_FRONT, GL_EMISSION, unglow);
    
    //turn off light if light switch turned off
    if (on == 0.0) glDisable(GL_LIGHT0);
    
}

//its a table with a book on it
void drawDesk(void){
    float desk[] = {0.803922, 0.521569, 0.247059, 1.0}, inbook[] = {1.0, 0.937255, 0.835294, 1.0}, outbook[] = {0.858824, 0.439216, 0.576471, 1.0};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, desk);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glPushMatrix();
    glTranslatef(30.0, -8.0, -50.0);
    glScalef(1.0, 0.2, 1.0);
    glutSolidSphere(15.0, 60.0, 60.0);
    
    glPushMatrix();
    glTranslatef(0.0, -40.0, 0.0);
    glScalef(0.05, 1.0, 0.05);
    glutSolidCube(40);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, inbook);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glow);
    glPushMatrix();
    glCullFace(GL_BACK);
    glTranslatef(3.0, 4.0, 0.0);
    glScalef(0.8, 0.5, 1.0);
    glutSolidCube(7.0);
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, unglow);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, outbook);
    glPushMatrix();
    glTranslatef(3.0, 6.0, 0.0);
    glScalef(0.8, 0.2, 1.0);
    glutSolidCube(8.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3.0, 3.0, 0.0);
    glScalef(0.8, 0.2, 1.0);
    glutSolidCube(8.0);
    glPopMatrix();
    
    glPopMatrix();
    glDisable(GL_CULL_FACE);
}

//double doors
void drawDoor(void){
    //front door
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, door);
    
    if (isSelecting) glLoadName(1);
    if ((highlightFrames > 0) && (closestName == 1)) {
        open = true;
        glutPostRedisplay();
    }
    
    //left
    glPushMatrix();
    //to open
    glTranslatef(-5.0+lx, -10.0, lz);
    glRotatef(leftd, 0.0, 1.0, 0.0);
    
    glScalef(0.5, 1.4, 0.1);
    glutSolidCube(20);
    glPopMatrix();
    
    //right
    glPushMatrix();
    //to open
    glTranslatef(5.0+rx, -10.0, rz);
    glRotatef(rightd, 0.0, 1.0, 0.0);
    
    glScalef(0.5, 1.4, 0.1);
    glutSolidCube(20);
    glPopMatrix();
}

//draw box that is chest
void drawChest(void){
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, house);
    glPushMatrix();
    glTranslatef(-30.0, -18.0, -50.0);
    glutSolidCube(10.0);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
    glPushMatrix();
    glTranslatef(-25.0, -15, -50.0);
    glutSolidCube(0.5);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-25.0, -14, -50.0);
    glutSolidCube(0.5);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-25.0, -14.5, -51.0);
    glutSolidCube(0.5);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-25.0, -14.5, -49.0);
    glutSolidCube(0.5);
    glPopMatrix();
}

//draw weird geometric blue key
void drawKey(void){
    float blue[] = {0.0, 0.0, 1.0, 1.0};
    
    glPushMatrix();
    glTranslatef(kx, ky, 0.0);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
    glPushMatrix();
    glTranslatef(20.0, -8.0, -50.0);
    glScalef(1.0, 0.05, 0.05);
    glutSolidCube(10.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(20.0, -7.0, -50.0);
    glScalef(1.0, 0.05, 0.05);
    glutSolidCube(10.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(20.0, -7.5, -51.0);
    glScalef(1.0, 0.05, 0.05);
    glutSolidCube(10.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(20.0, -7.5, -49.0);
    glScalef(1.0, 0.05, 0.05);
    glutSolidCube(10.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(25.0, -7.0, -50.0);
    glScalef(1.0, 1.0, 1.0);
    glutSolidCube(2.0);
    glPopMatrix();
    glPopMatrix();
}

void drawHouse(void){
    //arrays for ambient/diffused light materials
    float front[] = {0.0, 0.7, 0.7, 1.0}, left[] = {0.8, 0.7, 0.2, 1.0},
          right[] = {0.5, 0.6, 0.8, 1.0}, back[] = {1.0, 0.8, 0.8, 1.0};
    
    float ls[] = {0.8, 0.75, 0.75, 1.0}, s[] = {0.6, 0.6, 0.6, 1.0};
    
    //front wall
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, house);
    glPushMatrix();
    glTranslatef(-30.0, 0.0, 1.0);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    glScalef(1.0, 1.0, 0.05);
    glutSolidCube(40.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(30.0, 0.0, 1.0);
    glScalef(1.0, 1.0, 0.05);
    glutSolidCube(40.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 12.0, 1.0);
    glScalef(1.0, 0.8, 0.1);
    glutSolidCube(20.0);
    glPopMatrix();
    
    //inner
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front);
    glPushMatrix();
    glTranslatef(-30.0, 0.0, 0.0);
    glScalef(1.0, 1.0, 0.05);
    glutSolidCube(40.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(30.0, 0.0, 0.0);
    glScalef(1.0, 1.0, 0.05);
    glutSolidCube(40.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 12.0, 0.0);
    glScalef(1.0, 0.8, 0.1);
    glutSolidCube(20.0);
    glPopMatrix();
    
    //left wall with window
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, house);
    glPushMatrix();
    glTranslatef(-50.0, 0.0, -13.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(1.0, 1.4, 0.06);
    glutSolidCube(30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-50.0, 0.0, -65.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(1.0, 1.4, 0.06);
    glutSolidCube(30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-50.0, -15.0, -39.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(2.2, 1.0, 0.2);
    glutSolidCube(10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-50.0, 15.0, -39.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(2.2, 1.0, 0.2);
    glutSolidCube(10);
    glPopMatrix();
    
    //window bar thing
    glPushMatrix();
    glTranslatef(-50.0, 0.0, -39.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(1.1, 0.1, 0.2);
    glutSolidCube(20);
    glPopMatrix();
    
    //light switch
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ls);
    glPushMatrix();
    glTranslatef(-48.0, 0.0, -55.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(0.7, 1.0, 0.2);
    glutSolidCube(4);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, s);
    glPushMatrix();
    glTranslatef(-48.0, 0.0, -55.0);
    glRotatef(on, 0.0, 0.0, 1.0);
    glutSolidCube(1.5);
    glPopMatrix();
    
    //inner
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, left);
    glPushMatrix();
    glTranslatef(-49.0, 0.0, -13.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(0.95, 1.4, 0.06);
    glutSolidCube(30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-49.0, 0.0, -65.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(0.95, 1.4, 0.06);
    glutSolidCube(30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-49.0, -15.0, -39.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(2.4, 1.0, 0.2);
    glutSolidCube(10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-49.0, 15.0, -39.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(2.4, 1.0, 0.2);
    glutSolidCube(10);
    glPopMatrix();
    
    //right wall
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, house);
    glPushMatrix();
    glTranslatef(50.0, 0.0, -38.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glScalef(1.0, 0.5, 0.05);
    glutSolidCube(80);
    glPopMatrix();
    
    //inner
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, right);
    glPushMatrix();
    glTranslatef(49.0, 0.0, -38.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glScalef(0.95, 0.5, 0.05);
    glutSolidCube(80);
    glPopMatrix();
    
    //back wall
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, house);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, -78.0);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    glScalef(1.0, 0.4, 0.04);
    glutSolidCube(100);
    glPopMatrix();
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
    
    //inner
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, back);
    glPushMatrix();
    glTranslatef(0.0, 0.0, -77.0);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    glScalef(0.95, 0.4, 0.04);
    glutSolidCube(100);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, door);
    //ceiling
    glPushMatrix();
    glTranslatef(0.0, 22.0 , -40.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glScalef(1.0, 0.9, 0.04);
    glutSolidCube(100);
    glPopMatrix();
    
    //floor
    glPushMatrix();
    glTranslatef(0.0, -21.0 , -40.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glScalef(1.0, 0.8, 0.04);
    glutSolidCube(100);
    glPopMatrix();
    
    drawDoor();
    drawDesk();
    drawChest();
}

void background(void){
    //ground
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grass);
    glPushMatrix();
    glTranslatef(0.0, -170.0, 0.0);
    glutSolidCube(300);
    glPopMatrix();
    
    glEnable(GL_COLOR_MATERIAL);
    //sky
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glPopMatrix();
    drawMoon();
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, unglow);
}

void draw(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    //arrays for headlamps
    float headlamp[] = {r, g, b, 1.0}, hamb[] = {0.0, 0.0, 0.0, 1.0}, hspec[] = {1.0, 1.0, 1.0, 1.0}, hpos[] = {gx, gy, gz, 1.0}, hdir[] = { static_cast<float>(sin(angle*PI/180)), 0.0, static_cast<float>(cos(angle*PI/180))};
    
    //move around
    gluLookAt(gx, gy, gz,
              gx + sin(angle*PI/180), 0.0, gz + cos(angle*PI/180),
              0.0, 1.0, 0.0);
    
    //headlamp setup
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    if (!rightkey) glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gamb);
    else glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightamb);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
    glPushMatrix();
    glLightfv(GL_LIGHT2, GL_AMBIENT, hamb);
    glLightfv(GL_LIGHT2, GL_SPECULAR, headlamp);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, headlamp);
    glLightfv(GL_LIGHT2, GL_POSITION, hpos);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, hdir);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 5.0);
    glEnable(GL_LIGHT2);
    glPopMatrix();
    
    if (off) glDisable(GL_LIGHT2);
    
    if (!wrong){
    drawHouse();
    drawKey();
    drawLamp();
    background();
    text();
    }
    
    else if (wrong){
        drawStars();
        glDisable(GL_LIGHTING);
        
        glPushMatrix();
        glTranslatef(-30.0, 10.0, -100.0);
        glScalef(0.015, 0.015, 0.015);
        writeStrokeString(GLUT_STROKE_MONO_ROMAN, "You lose, so you're stuck in space now!");
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }
    
    if (rightkey){
        glClearColor(0.529412, 0.807843, 0.980392, 0.0);
        glPushMatrix();
        glTranslatef(-30.0, 10.0, -100.0);
        glScalef(0.015, 0.015, 0.015);
        writeStrokeString(GLUT_STROKE_MONO_ROMAN, "You've survived another night - it's light out now!");
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }
    
    glEnable(GL_NORMALIZE);
    glDisable(GL_DEPTH_TEST);
    glutPostRedisplay();
}

void drawScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    isSelecting = 0;
    draw();
    glutSwapBuffers();
}

void findClosestHit(int hits, unsigned int buffer[])
{
    unsigned int *ptr, minZ;
    
    minZ = 0xffffffff; // 2^32 - 1
    ptr = buffer;
    closestName = 0;
    for (int i = 0; i < hits; i++)
    {
        ptr++;
        if (*ptr < minZ)
        {
            minZ = *ptr;
            ptr += 2;
            closestName = *ptr;
            ptr++;
        }
        else ptr += 3;
    }
    if (closestName != 0) highlightFrames = 10;
}
void pickFunction(int button, int state, int x, int y) //also could be selectHits
{
    int viewport[4]; // Viewport data.
    
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return; // Don't react unless left button is pressed.
    
    glGetIntegerv(GL_VIEWPORT, viewport); // Get viewport data.
    
    glSelectBuffer(1024, buffer); // Specify buffer to write hit records in selection mode
    (void)glRenderMode(GL_SELECT); // Enter selection mode.
    
    // Save the viewing volume defined in the resize routine.
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    // Define a viewing volume corresponding to selecting in 3 x 3 region around the cursor.
    glLoadIdentity();
    gluPickMatrix((float)x, (float)(viewport[3] - y), 3.0, 3.0, viewport);
    gluPerspective(70,1,1,500);
    
    glMatrixMode(GL_MODELVIEW); // Return to modelview mode before drawing.
    glLoadIdentity();
    
    glInitNames(); // Initializes the name stack to empty.
    glPushName(0); // Puts name 0 on top of stack.
    
    isSelecting = 1;
    draw();
    
    hits = glRenderMode(GL_RENDER); // Return to rendering mode, returning number of hits.
    
    // Restore viewing volume of the resize routine and return to modelview mode.
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    // Determine closest of the hit objects (if any).
    findClosestHit(hits, buffer);
    
    glutPostRedisplay();
}

void setup(void)
{
    glClearColor(0.0, 0.0, 0.5, 0.0);
}

void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    width = w;
    height = h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70,1,1,500);
    
    glMatrixMode(GL_MODELVIEW);
}

void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 'd':
            open = !open;
            break;
        case 'l':
            if (on == 45.0)on = 0;
            else on = 45.0;
            break;
        case 'r':
            r = 1.0;
            g = 0.0;
            b = 0.0;
            break;
        case 'b':
            r = 0.0;
            g = 0.0;
            b = 1.0;
            break;
        case 'g':
            r = 0.0;
            g = 1.0;
            b = 1.0;
            break;
        case 'w':
            r = 1.0;
            g = 1.0;
            b = 1.0;
            break;
        case 'L':
            wrong = true;
            break;
        case 'R':
            rightkey = true;
            break;
        case 'k':
            bkey = true;
            break;
        case 'o':
            off = !off;
            break;
    }
    glutPostRedisplay();
}
void specialKeyInput(int key, int x, int y)
{
    switch(key){
        case GLUT_KEY_UP: //forward
                gz+=step*cos(angle*PI/180);
                gx+=step*sin(angle*PI/180);
            break;
        case GLUT_KEY_DOWN: //back
            gz-=step*cos(angle*PI/180);
            gx-=step*sin(angle*PI/180);
            break;
        case GLUT_KEY_RIGHT: //turn right
            angle -=turn;
            break;
        case GLUT_KEY_LEFT: //turn left
            angle +=turn;
            break;
    }//end switch
    glutPostRedisplay();
}

void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press the up arrow key to move forward and the down arrow key to move backwards." << endl;
    cout << "Press the left arrow key to turn left, the right arrow key to turn right." << endl;
    cout << "Press 'd' to open the double doors - you cannot close them." << endl;
    cout << "Press 'l' to turn the light on or off within the house." << endl;
    cout << "Press 'o' to turn the headlamp off and on - the headlamp must be on before you can change its colors. Press 'w' for a white headlamp light, 'r' for a red one, 'g' for a green one, 'b' for a blue one." << endl;
    cout << "Press 'k' to insert the key into the chest. Turn the key left with 'L' - this is the wrong way. Turn the key right with 'R' - this is the right way." << endl;
    cout << "Press the escape key to quit." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("Magic");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glutTimerFunc(5, move, 1);
    glutMainLoop();
    
    return 0;
}

