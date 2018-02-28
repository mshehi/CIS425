/******************************************
 *
 * Official Name:  Megi Shehi
 *
 * Nickname:  Megi
 *
 * E-mail:  mshehi@syr.edu
 *
 * Assignment:  Assignment 2
 *
 * Environment/Compiler:  XCode
 *
 * Date:  February 19, 2018
 *
 * References: Once again used colors from http://avatar.se/molscript/doc/colour_names.html
 *
 * Interactions: Press the escape key or select 'Quit' from the menu to quit.
                Select a lunar phase from the control panel. Alternatively, press 'n' for 'New', 'c' for 'Crescent', 'h' for 'Half', 'g' for 'Gibbous', or 'f' for 'Full.'
                Select a checkbox to see the corresponding stellar item. Alternatively, press 'j' for 'Jupiter', 'p' for 'Scorpius,' 't' for 'Saturn', and 's' for 'Sun.'
                Press 'R' while 'Saturn' is active to rotate its ring.
                Switch between 'Space' and 'Pink' background colors in the menu.
                Make the moon bigger or smaller in the menu.
                Rotate the entire sky left or right using the menu, or alternatively by pressing the left or right arrow keys. You can also reset the rotation of the sky in the menu.
 *******************************************/
//
//  Copyright © 2018 Megi Shehi. All rights reserved.
//

#include <cmath>
#include <iostream>
#define PI 3.14159265

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Globals.
#define NEW 0
#define CRESCENT 1
#define HALF 2
#define GIBBOUS 3
#define FULL 4
static long font;
static GLsizei cwidth, cheight, dwidth, dheight; //height and width for each window
//stellar booleans, and Pink background
bool Jupiter = false, Scorpius = false, Sun = false, Saturn = false, Pink = false;
int phase = HALF, y = 0;  // default is half moon
//scale factor for moon
double s = 1.0,
//angle of rotation for sky
rot = 0.0,
//x coordinate to translate moon
cx = 0.0,
//angle of rotation for saturn's ring
satrot = 110.0;
//clipping planes for moon
double clipl[4] = {-1.0, 0.0, 0.0, 0.0 }, clipr[4] = {1.0, 0.0, 0.0, 0.0};
//clipping planes for jupiter
double jup1[4] = {0.0, -1.0, 0.0, -40.0 }, jup2[4] = {0.0, 1.0, 0.0, 40.0}, jup3[4] = {0.0, -1.0, 0.0, -10.0}, jup4[4] = {0.0, 1.0, 0.0, 10}, jup5[4] = {0.0, -1.0, 0.0, 20}, jup6[4] = {0.0, 1.0, 0.0, 20};

float t = 0; //angle parameter for drawing circles
static int controlid, displayid; // Window identifiers.

void writeBitmapString(void *font, char *string)
{
    char *c;
    
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

//draw radio button
void drawCircle(double x, double y){
    glPolygonMode(GL_FRONT, GL_LINE);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 80; ++i){
        glVertex3f(x + cos(t)*2, y + sin(t)*2, 0.0);
        t +=  PI / 20;
    }
    glEnd();
}

//select radio button corresponding to phase
void fillCircle(){
    int x = 9.0, y;
    
    if (phase == 0) y = 45.0;
    else if (phase == 1) y = 40.0;
    else if (phase == 2) y = 35.0;
    else if (phase == 3) y = 30.0;
    else y = 25.0;
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 80; ++i){
        glVertex3f(x + cos(t)*1.5, y + sin(t)*1.5, 0.0);
        t +=  PI / 20;
    }
    glEnd();
}

//select checkbox item for stellar objects
void checkBox(double x, double y, bool z){
    if (z){
        glColor3f(0.6, 0.196078, 0.8);
        glLineWidth(2.0);
        glBegin(GL_LINES);
        glVertex3f(x, y, 0.0);
        glVertex3f(x+4, y-4, 0.0);
        glVertex3f(x+4, y, 0.0);
        glVertex3f(x, y-4, 0.0);
        glEnd();
        glLineWidth(1.0);
    }
}

//set phase based on mouse y coordinate
void setPhase(int y){
    if (y < cheight*.27 && y > cheight*.20) phase = FULL;
    else if (y < cheight*.32 && y > cheight*.28) phase = GIBBOUS;
    else if (y < cheight*.37 && y > cheight*.33) phase = HALF;
    else if (y < cheight*.42 && y > cheight*.38) phase = CRESCENT;
    else if (y < cheight*.47 && y > cheight*.43)phase = NEW;
}

//draw moon with specific phase
void drawMoon(){
    //determine rotating angle based on phase
    if (phase == NEW) y = 270;
    else if (phase == CRESCENT) y = 320;
    else if (phase == HALF) y = 0;
    else if (phase == GIBBOUS) y = 30;
    else y = 90;
    
    glPushMatrix();
    
    //rotate moon, then move it
    glTranslatef(cx, 0.0, 5.0);
    glRotatef(y, 0.0, 1.0, 0.0);
    glScalef(s, s, s);
    
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glClipPlane(GL_CLIP_PLANE0, clipl); //left clipping plane
    glClipPlane(GL_CLIP_PLANE1, clipr); //right clipping plane
    glEnable(GL_CLIP_PLANE0);
    glColor3f(0.2, 0.2, 0.2);
    glutSolidSphere(80, 60, 60); //darker half
    glDisable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glColor3f(0.8, 0.8, 0.8);
    glutSolidSphere(80, 60, 60); //lighter half
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();
    
}

//draw control panel
void drawControl(void){
    glutSetWindow(controlid);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Stellar Objects heading
    font = (long)GLUT_BITMAP_HELVETICA_18;
    glColor3f(0.6, 0.196078, 0.8);
    glRasterPos3f(3.0,95.0,0.0);
    writeBitmapString((void*)font, "Stellar Objects");
    
    //draw first checkbox for Jupiter
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glVertex3f(7.0, 92.0, 0.0);
    glVertex3f(11.0, 92.0, 0.0);
    glVertex3f(11.0, 88.0, 0.0);
    glVertex3f(7.0, 88.0, 0.0);
    glEnd();
    
    //Jupiter label
    font = (long)GLUT_BITMAP_HELVETICA_12;
    glRasterPos3f(13.0,89.0,0.0);
    writeBitmapString((void*)font, "Jupiter");
    
    //second checkbox for Scorpius constellation
    glBegin(GL_POLYGON);
    glVertex3f(7.0, 86.0, 0.0);
    glVertex3f(11.0, 86.0, 0.0);
    glVertex3f(11.0, 82.0, 0.0);
    glVertex3f(7.0, 82.0, 0.0);
    glEnd();
    
    //Scorpius label
    glRasterPos3f(13.0,83.0,0.0);
    writeBitmapString((void*)font, "Scorpius");
    
    //Sun checkbox
    glBegin(GL_POLYGON);
    glVertex3f(7.0, 80.0, 0.0);
    glVertex3f(11.0, 80.0, 0.0);
    glVertex3f(11.0, 76.0, 0.0);
    glVertex3f(7.0, 76.0, 0.0);
    glEnd();
    
    //Sun label
    glRasterPos3f(13.0,77.0,0.0);
    writeBitmapString((void*)font, "Sun");
    
    //Saturn checkbox
    glBegin(GL_POLYGON);
    glVertex3f(7.0, 74.0, 0.0);
    glVertex3f(11.0, 74.0, 0.0);
    glVertex3f(11.0, 70.0, 0.0);
    glVertex3f(7.0, 70.0, 0.0);
    glEnd();
    
    //Saturn label
    glRasterPos3f(13.0,71.0,0.0);
    writeBitmapString((void*)font, "Saturn");
    
    //Lunar Phases heading
    font = (long)GLUT_BITMAP_HELVETICA_18;
    glRasterPos3f(3.0,50.0,0.0);
    writeBitmapString((void*)font, "Lunar Phases");
    
    //Lunar phases radio buttons and labels
    font = (long)GLUT_BITMAP_HELVETICA_12;
    drawCircle(9.0,45.0);
    glRasterPos3f(13.0,44.0,0.0);
    writeBitmapString((void*)font, "New");
    drawCircle(9.0,40.0);
    glRasterPos3f(13.0,39.0,0.0);
    writeBitmapString((void*)font, "Crescent");
    drawCircle(9.0,35.0);
    glRasterPos3f(13.0,34.0,0.0);
    writeBitmapString((void*)font, "Half");
    drawCircle(9.0,30.0);
    glRasterPos3f(13.0,29.0,0.0);
    writeBitmapString((void*)font, "Gibbous");
    drawCircle(9.0,25.0);
    glRasterPos3f(13.0,24.0,0.0);
    writeBitmapString((void*)font, "Full");
    
    //current phase
    fillCircle();
    
    //check boxes that need to be checked
    checkBox(7.0, 92.0, Jupiter);
    checkBox(7.0, 86.0, Scorpius);
    checkBox(7.0, 80.0, Sun);
    checkBox(7.0, 74.0, Saturn);
    
    glutSwapBuffers();
}

//just a bunch of solid octahedrons
void drawStars(){
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(150.0, 200.0, -70.0);
    glScalef(10.0, 10.0, 10.0);
    glutSolidOctahedron();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-250.0, 150.0, -70.0);
    glScalef(10.0, 10.0, 10.0);
    glutSolidOctahedron();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(90.0, -300.0, -70.0);
    glScalef(10.0, 10.0, 10.0);
    glutSolidOctahedron();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-30.0, 400.0, -70.0);
    glScalef(10.0, 10.0, 10.0);
    glutSolidOctahedron();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-150.0, 500.0, -70.0);
    glScalef(10.0, 10.0, 10.0);
    glutSolidOctahedron();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-300.0, -200.0, -70.0);
    glScalef(10.0, 10.0, 10.0);
    glutSolidOctahedron();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(400.0, -300.0, -70.0);
    glScalef(10.0, 10.0, 10.0);
    glutSolidOctahedron();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-550.0, 500.0, -70.0);
    glScalef(10.0, 10.0, 10.0);
    glutSolidOctahedron();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-500.0, 50.0, -70.0);
    glScalef(10.0, 10.0, 10.0);
    glutSolidOctahedron();
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
}

//draw jupiter with stripes and weird clipping side effects
void drawJupiter(){
    
    glPushMatrix();
    glEnable(GL_DEPTH_TEST);
    glTranslatef(-250.0, 300.0, -20.0);
    glClipPlane(GL_CLIP_PLANE0, jup1);
    glClipPlane(GL_CLIP_PLANE1, jup2);
    glClipPlane(GL_CLIP_PLANE2, jup3);
    glClipPlane(GL_CLIP_PLANE3, jup4);
    glClipPlane(GL_CLIP_PLANE4, jup5);
    glClipPlane(GL_CLIP_PLANE5, jup6);
    glEnable(GL_CLIP_PLANE0);
    glColor3f(0.5, 0.2, 0.2);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
    glColor3f(1.0, 0.854902, 0.72549);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    //this happened by accident but i like the texture it creates
    glEnable(GL_CLIP_PLANE2);
    glColor3f(0.956863, 0.643137, 0.376471);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE2);
    glEnable(GL_CLIP_PLANE3);
    glEnable(GL_CLIP_PLANE4);
    glColor3f(0.5, 0.2, 0.2);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE3);
    glDisable(GL_CLIP_PLANE4);
    glEnable(GL_CLIP_PLANE4);
    glColor3f(0.956863, 0.643137, 0.376471);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE4);
    glEnable(GL_CLIP_PLANE5);
    glColor3f(0.956863, 0.643137, 0.376471);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE5);
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();
}

//draw saturn with a wired torus as its ring
void drawSaturn(){
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    glTranslatef(-300.0, -400.0, -60.0);
    glColor3f(0.5, 1.0, 1.0);
    glutSolidSphere(40.0, 30, 30);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1.0, 0.752941, 0.796078);
    glTranslatef(-300.0, -400.0, -60.0);
    glRotatef(satrot, 1.0, 0.0, 0.0);
    glutWireTorus(10.0, 65.0, 15, 15);
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
}

//simple sun
void drawSun(){
    glPushMatrix();
    glTranslatef(200.0, -200.0, -70.0);
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0, 0.843137, 0.0);
    glutSolidSphere(60.0, 40, 40);
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();
}

//draw Scorpius constellation
void drawScorpius(){
    glEnable(GL_DEPTH_TEST);
    //larger points and stippled lines to connect them
    glPointSize(3.0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0101);
    glColor3f(1.0, 1.0, 1.0);
    
    //drew points separately to make them stand out
    glBegin(GL_POINTS);
    glVertex3f(200.0, 450.0, -10.0);
    glVertex3f(250.0, 250.0, -10.0);
    glVertex3f(300.0, 400.0, -10.0);
    glVertex3f(350.0, 350.0, -10.0);
    glVertex3f(150.0, 350.0, -10.0);
    glVertex3f(250.0, 200.0, -10.0);
    glVertex3f(270.0, 150.0, -10.0);
    glVertex3f(350.0, 0.0, -10.0);
    glVertex3f(500.0, -100.0, -10.0);
    glVertex3f(480.0, -200.0, -10.0);
    glVertex3f(400.0, -250.0, -10.0);
    glVertex3f(300.0, -270.0, -10.0);
    glVertex3f(250.0, -230.0, -10.0);
    glVertex3f(300.0, -150.0, -10.0);
    glVertex3f(350.0, -170.0, -10.0);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(200.0, 450.0, -10.0);
    glVertex3f(250.0, 250.0, -10.0);
    
    glVertex3f(300.0, 400.0, -10.0);
    glVertex3f(250.0, 250.0, -10.0);
    
    glVertex3f(250.0, 250.0, -10.0);
    glVertex3f(350.0, 350.0, -10.0);
    
    glVertex3f(250.0, 250.0, -10.0);
    glVertex3f(150.0, 350.0, -10.0);
    
    glVertex3f(250.0, 250.0, -10.0);
    glVertex3f(250.0, 200.0, -10.0);
    
    glVertex3f(250.0, 200.0, -10.0);
    glVertex3f(270.0, 150.0, -10.0);
    
    glVertex3f(270.0, 150.0, -10.0);
    glVertex3f(350.0, 0.0, -10.0);
    
    glVertex3f(350.0, 0.0, -10.0);
    glVertex3f(500.0, -100.0, -10.0);
    
    glVertex3f(500.0, -100.0, -10.0);
    glVertex3f(480.0, -200.0, -10.0);
    
    glVertex3f(480.0, -200.0, -10.0);
    glVertex3f(400.0, -250.0, -10.0);
    
    glVertex3f(400.0, -250.0, -10.0);
    glVertex3f(300.0, -270.0, -10.0);
    
    glVertex3f(300.0, -270.0, -10.0);
    glVertex3f(250.0, -230.0, -10.0);
    
    glVertex3f(250.0, -230.0, -10.0);
    glVertex3f(300.0, -150.0, -10.0);
    
    glVertex3f(300.0, -150.0, -10.0);
    glVertex3f(350.0, -170.0, -10.0);
    
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_DEPTH_TEST);
}

//draw Display Window
void drawDisplay(void){
    glutSetWindow(displayid);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    //rotate everything
    glRotatef(rot, 0.0, 1.0, 0.0);
    
    //draw pink gradient background if activated
    if (Pink){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_POLYGON);
        glColor3f(0.580392, 0.0, 0.827451);
        glVertex3f(-600.0, 600.0, -90.0);
        glColor3f(1.0, 0.0784314, 0.576471);
        glVertex3f(600.0, 600.0, -90.0);
        glColor3f(0.545098, 0.0, 0.545098);
        glVertex3f(600.0, -600.0, -90.0);
        glColor3f(0.780392, 0.0823529, 0.521569);
        glVertex3f(-600.0, -600.0, -90.0);
        glEnd();
    }
    
    drawStars();
    drawMoon();
    
    if (Jupiter) drawJupiter();
    if (Scorpius) drawScorpius();
    if (Sun) drawSun();
    if (Saturn) drawSaturn();
    
    glutSwapBuffers();
}

void setupControl(void){
    //gold color
    glClearColor(1.0, 0.8, 0.0, 0.0);
}

void setupDisplay(void){
    //dark purplish blue color
    glClearColor(0.2, 0.0, 0.5, 0.0);
}

// Reshape routine for control window.
void resizeControl(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    cwidth = w;
    cheight = h;
    
    glOrtho(0.0, 65.0, 0.0, 100.0, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
}

// Reshape routine for display window.
void resizeDisplay(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    dwidth = w;
    dheight = h;
    
    glOrtho(-600.0, 600.0, -600.0, 600.0, -5.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

//cases for keyboard input, applied to both windows
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'c':
            phase = CRESCENT;
            glutSetWindow(controlid);
            glutPostRedisplay();
            glutSetWindow(displayid);
            glutPostRedisplay();
            break;
        case 'n':
            phase = NEW;
            glutSetWindow(controlid);
            glutPostRedisplay();
            glutSetWindow(displayid);
            glutPostRedisplay();
            break;
        case 'h':
            phase = HALF;
            glutSetWindow(controlid);
            glutPostRedisplay();
            glutSetWindow(displayid);
            glutPostRedisplay();
            break;
        case 'f':
            phase = FULL;
            glutSetWindow(controlid);
            glutPostRedisplay();
            glutSetWindow(displayid);
            glutPostRedisplay();
            break;
        case 'g':
            phase = GIBBOUS;
            glutSetWindow(controlid);
            glutPostRedisplay();
            glutSetWindow(displayid);
            glutPostRedisplay();
            break;
        case 't':
            Saturn = !Saturn;
            glutSetWindow(controlid);
            glutPostRedisplay();
            glutSetWindow(displayid);
            glutPostRedisplay();
            break;
        case 'R':
            if (Saturn){
            satrot +=10;
            glutSetWindow(displayid);
            glutPostRedisplay();
            }
            break;
        case 's':
            Sun = !Sun;
            glutSetWindow(controlid);
            glutPostRedisplay();
            glutSetWindow(displayid);
            glutPostRedisplay();
            break;
        case 'j':
            Jupiter = !Jupiter;
            glutSetWindow(controlid);
            glutPostRedisplay();
            glutSetWindow(displayid);
            glutPostRedisplay();
            break;
        case 'p':
            Scorpius = !Scorpius;
            glutSetWindow(controlid);
            glutPostRedisplay();
            glutSetWindow(displayid);
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

//rotate sky with left and right arrows
void specialKeyInput(int key, int x, int y)
{
    glutSetWindow(displayid);
    if(key == GLUT_KEY_LEFT) rot-=5;
    if(key == GLUT_KEY_RIGHT) rot+=5;
    glutPostRedisplay();
}

//mouse method for Control Window
void mouseControl(int button, int state, int x, int y)
{
    glutSetWindow(controlid);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //adjust y coordinate to match drawing coordinates
        y = cheight - y;
        
        //check if click is within window
        if ( x < 0 || x > cwidth || y < 0 || y > cheight ) ;
        
        //check if selecting lunar phase
        else if ( y < cheight*.5) setPhase(y);
        
        else if ( y < cheight *.74 && y > cheight*.7) {
            Saturn = !Saturn;
        }
        
        else if ( y <cheight*.8 && y >cheight*.76){
            Sun = !Sun;
        }
        else if ( y < cheight*.86 && y > cheight*.82 ){
            Scorpius = !Scorpius;
        }
        
        else if ( y < cheight *.92 && y > cheight*.88) {
            Jupiter = !Jupiter;
        }
    }
    glutSetWindow(controlid);
    glutPostRedisplay();
    glutSetWindow(displayid);
    glutPostRedisplay();
}

// Update the location of the current point as the mouse moves with button pressed in display window
void mouseMotion(int x, int y)
{
    glutSetWindow(displayid);
    if (!(x <= 0) && !(x >= dwidth)) cx = x-(dheight/2);
    glutPostRedisplay();
}

//mouse method for Display Window
void mouseDisplay(int button, int state, int x, int y)
{
    glutSetWindow(displayid);
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //check if click is within window
        if ( x < 0 || x > dwidth || y < 0 || y > dheight ) ;
        else mouseMotion(x, y);
    }
}

//quit
void top_menu(int n)
{
    if (n==1) exit(0);
}

//change display window background
void colorMenu(int n){
    if (n == 0) Pink = false;
    else Pink = true;
    glutPostRedisplay();
}

//change the size of the moon
void sizeMenu(int n){
    //make sure moon does not disappear
    if (n == 2 && s > 0.5) s-=.2;
    //do not allow moon to get too big
    else if (n==3 && s < 1.5) s+=.2;
    glutPostRedisplay();
}

//rotates the whole sky
void turnMenu(int n){
    if (n == 4) rot -=5;
    else if (n == 5 ) rot +=5;
    else rot = 0;
    glutPostRedisplay();
}

//menu on right click
void makeMenu(void)
{
    glutSetWindow(displayid);
    int sub_menu1;
    sub_menu1 = glutCreateMenu(colorMenu);
    glutAddMenuEntry("Space", 0);
    glutAddMenuEntry("Pink", 1);
    
    int sub_menu2;
    sub_menu2 = glutCreateMenu(sizeMenu);
    glutAddMenuEntry("Smaller", 2);
    glutAddMenuEntry("Bigger", 3);
   
    int sub_menu3;
    sub_menu3 = glutCreateMenu(turnMenu);
    glutAddMenuEntry("Turn Left", 4);
    glutAddMenuEntry("Turn Right", 5);
    glutAddMenuEntry("Reset", 6);
    
    glutCreateMenu(top_menu);
    glutAddSubMenu("Color", sub_menu1);
    glutAddSubMenu("Moon Size", sub_menu2);
    glutAddSubMenu("Rotate Sky", sub_menu3);
    glutAddMenuEntry("Quit",1);
    
    // The menu is attached to a mouse button.
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press the escape key or select 'Quit' from the menu to quit." << endl;
    cout << "Select a lunar phase from the control panel. Alternatively, press 'n' for 'New', 'c' for 'Crescent', 'h' for 'Half', 'g' for 'Gibbous', or 'f' for 'Full.' " << endl;
    cout << "Select a checkbox to see the corresponding stellar item. Alternatively, press 'j' for 'Jupiter', 'p' for 'Scorpius,' 't' for 'Saturn', and 's' for 'Sun.' " << endl;
    cout << "Press 'R' while 'Saturn' is active to rotate its ring." << endl;
    cout << "Switch between 'Space' and 'Pink' background colors in the menu." << endl;
     cout << "Make the moon bigger or smaller in the menu." << endl;
     cout << "Rotate the entire sky left or right using the menu, or alternatively by pressing the left or right arrow keys. You may also reset the rotation of the sky in the menu." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // First top-level window definition.
    glutInitWindowSize(250,400);
    glutInitWindowPosition(100, 100);
    
    // Create the first window and return id.
    controlid = glutCreateWindow("Control Window");
    
    // Initialization, display, and other routines of the first window.
    setupControl();
    glutDisplayFunc(drawControl);
    glutReshapeFunc(resizeControl);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glutMouseFunc(mouseControl);
   
    // Second top-level window definition.
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(400, 400);
    
    // Create the second window and return id.
    displayid = glutCreateWindow("Display Window");
    
    // Initialization, display, and other routines of the second window.
    setupDisplay();
    glutDisplayFunc(drawDisplay);
    glutReshapeFunc(resizeDisplay);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glutMouseFunc(mouseDisplay);
    glutMotionFunc(mouseMotion);
    makeMenu();
    
    glutMainLoop();
    
    return 0;
}
