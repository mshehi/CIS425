/******************************************
*
* Official Name:  Megi Shehi
*
* Nickname:  Megi
*
* E-mail:  mshehi@syr.edu
*
* Assignment:  Assignment 3
*
* Environment/Compiler:  XCode
*
* Date:  February 27th, 2018
*
* References: http://avatar.se/molscript/doc/colour_names.html
*
* Interactions: "Interaction:"
    "Press the escape key to quit."
    "Press 'w' to flap the bug's wings, or to stop flapping them."
    "Press 'f' to see the bug fly. You cannot stop flapping the bug's wings while it is flying."
    "You can change your view! Press '1' to see the default view, '2' to see the scene from the left side, '3' to see the scene from the back, '4' to see the scene from the right, and '0' to see the scene from up above."
    "Press 'r' to return to the default settings."
    "Press the up and down arrow keys to speed up or slow down."
*
*******************************************/
//  Created by Megi Shehi on 2/26/18.
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

//Globals
#define FRONT 1;
#define LEFT 2;
#define BACK 3;
#define RIGHT 4;
#define TOP 0;

static GLsizei width, height;

bool flap = false, fly = false; //bools for flapping wings and flying
bool ortho = false;
float wa = 0.0, //wing angle
gx = 0.0, gy = -10.0, gz = 0.0; //bug x and y translations

//flower positions
float   f1x = -20.0, f1y = 10.0, f1z = -10.0,
        f2x = 20.0, f2y = 8.0, f2z = -15.0,
        f3x = -15.0, f3y = -20.0, f3z = -5.0,
        f4x = 30.0, f4y = -10.0, f4z = -13.0,
//increment to move bug toward flower
t = 0,
//increment amount for moving
ta = .2;

//where we actually want to move the bug because somehow these numbers look better on screen than the actual coordinates????
float   b1x = f1x, b1y = f1y, b1z = f1z,
        b2x = f2x, b2y = f2y, b2z = f2z,
        b3x = f3x, b3y = f3y, b3z = f3z,
        b4x = f4x, b4y = f4y, b4z = f4z;

//boolean values for whether a flower was visited
bool v1 = false, v2 = false, v3 = false, v4 = false;

static int animationPeriod = 100;
int direction = FRONT; //standard view is front

void drawBug(void){
    //cube the bug sits on
    glPushMatrix();
    glTranslatef(10.0, -7.0, -20.0);
    glColor3f(0.133333, 0.545098, 0.133333);
    glutSolidCube(5);
    glPopMatrix();
    
    glPushMatrix();
    //used to move bug
    glTranslatef(gx, gy, gz);
    //head
    glPushMatrix();
    glTranslatef(10.0, 10.0, -15.0);
    glScalef(1.0, 0.8, 1.0);
    glColor3f(0.7, 0.3, 0.5);
    glutSolidSphere(3.0, 40, 40);
    glPopMatrix();
    //right cone
    glPushMatrix();
    glTranslatef(11.0, 10.0, -12.0);
    glColor3f(1.0, 0.0, 0.5);
    glutSolidCone(0.3, 3.0, 30, 30);
    glPopMatrix();
    //left cone
    glPushMatrix();
    glTranslatef(9.0, 10.0, -12.0);
    glColor3f(1.0, 0.0, 0.5);
    glutSolidCone(0.3, 3.0, 30, 30);
    glPopMatrix();
    //body
    glPushMatrix();
    glTranslatef(10.0, 10.0, -20.0);
    glScalef(1.0, 0.8, 1.0);
    glColor3f(0.517647, 0.439216, 1.0);
    glutSolidSphere(6.0, 40, 40);
    glPopMatrix();
    //right wing
    glPushMatrix();
    glTranslatef(16.0, 10.0, -17.0);
    glRotatef(-wa, 0.0, 1.0, 1.0);
    glScalef(1.0, 0.15, 1.0);
    glColor3f(0.878431, 1.0, 1.0);
    glutSolidSphere(4.0, 20, 20);
    glPopMatrix();
    //left wing
    glPushMatrix();
    glTranslatef(4.0, 10.0, -17.0);
    glRotatef(wa, 0.0, 1.0, 1.0);
    glScalef(1.0, 0.15, 1.0);
    glColor3f(0.878431, 1.0, 1.0);
    glutSolidSphere(4.0, 20, 20);
    glPopMatrix();
    glPopMatrix();
}

//flaps bug's wings by changing angle of them
void flapWings(int n){
    if (flap){
        if (wa >= 40) wa =-40;
        wa+=10;
    }
    glutTimerFunc(animationPeriod, flapWings, 1);
    glutPostRedisplay();
}

//fly to some point xyz
void flyTo(float x, float y, float z){
    if (t < 1) t+= ta; //increment movement unless you've reached your destination
    else t = 0;
        if (x != gx) gx = (1-t)*gx + t*x;
        if (y != gy) gy = (1-t)*gy + t*y;
        if (z != gz) gz = (1-t)*gz + t*z;
    
    //mark off if the flower has been visited
    if (t == 1){
        if (gx == b1x) v1 = true;
        else if (gx == b2x) v2 = true;
        else if (gx == b3x) v3 = true;
        else if (gx == b4x) v4 = true;
    }
}

//fly bug fly!
void flight(int n){
    if (fly){
        //force the wings to flap
        flap = true;
        
        //figure out where we're flying to
        if ( gx != b1x && !v1) flyTo(b1x, b1y, b1z);
        else if (gx != b2x && !v2) flyTo(b2x, b2y, b2z);
        else if (gx != b3x && !v3) flyTo(b3x, b3y, b3z);
        else if (gx != b4x && !v4) flyTo(b4x, b4y, b4z);
        
        //clear everything once the flight is done
        if (v4){
            v1 = false;
            v2 = false;
            v3 = false;
            v4 = false;
            
            gx = 0.0, gy = -10.0, gz = 0.0;
            
            fly = false;
            flap = false;
        }
        
    }
    glutTimerFunc(animationPeriod, flight, 1);
    glutPostRedisplay();
}

void flower1(void){
    glPushMatrix();
    //move whole flower
    glTranslatef(f1x, f1y, f1z);
    
    //stem
    glPushMatrix();
    glScalef(0.1, 1.0 ,0.1);
    glColor3f(0.443137, 0.776471, 0.443137);
    glutSolidCube(5);
    glPopMatrix();
    
    //grass cube thing that holds it
    glPushMatrix();
    glTranslatef(0.0, -5.0, 0.0);
    glColor3f(0.133333, 0.545098, 0.133333);
    glutSolidCube(5);
    glPopMatrix();
    
    //core
    glPushMatrix();
    glTranslatef(0.0, 3.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidSphere(1.0, 30, 30);
    
    //petals
    glColor3f(1.0, 0.6, 0.796078);
    
    glPushMatrix();
    glTranslatef(1.0, 1.0, -1.0);
    glRotatef(45.0, 1.0, 0.0, 0.0);
    glScalef(2.5, 1.5, 1.5);
    glutSolidIcosahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1.5, 1.0, -1.0);
    glRotatef(45.0, 1.0, 0.0, 0.0);
    glScalef(2.5, 1.5, 1.5);
    glutSolidIcosahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1.5, -1.0, 1.0);
    glRotatef(-45.0, 1.0, 0.0, 0.0);
    glScalef(2.5, 1.5, 1.5);
    glutSolidIcosahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.0, -1.0, 1.0);
    glRotatef(-45.0, 1.0, 0.0, 0.0);
    glScalef(2.5, 1.5, 1.5);
    glutSolidIcosahedron();
    glPopMatrix();
    
    glPopMatrix();
    
    glPopMatrix();
}

void flower2(void){
    glPushMatrix();
    //move whole flower
    glTranslatef(f2x, f2y, f2z);
    
    //stem
    glPushMatrix();
    glScalef(0.1, 1.0 ,0.1);
    glColor3f(0.443137, 0.776471, 0.443137);
    glutSolidCube(5);
    glPopMatrix();
    
    //grass cube thing that holds it
    glPushMatrix();
    glTranslatef(0.0, -5.0, 0.0);
    glColor3f(0.133333, 0.545098, 0.133333);
    glutSolidCube(5);
    glPopMatrix();
    
    //core
    glPushMatrix();
    glTranslatef(0.0, 3.0, 0.0);
    glColor3f(0.0, 0.5, 1.0);
    glutSolidSphere(1.5, 30, 30);
    
    //petals
    glColor3f(0.580392, 0.0, 0.827451);
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidCone(3.0, 8.0, 30, 30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, -1.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidCone(3.0, 8.0, 30, 30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.0, 0.0, -1.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidCone(3.0, 8.0, 30, 30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1.0, 0.0, -1.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glScalef(0.5, 0.5, 0.5);
    glutSolidCone(3.0, 8.0, 30, 30);
    glPopMatrix();
    
    glPopMatrix();
    glPopMatrix();
}

void flower3(void){
    glPushMatrix();
    //move whole flower
    glTranslatef(f3x, f3y, f3z);
    
    //stem
    glPushMatrix();
    glScalef(0.1, 1.0 ,0.1);
    glColor3f(0.443137, 0.776471, 0.443137);
    glutSolidCube(5);
    glPopMatrix();
    
    //grass cube thing that holds it
    glPushMatrix();
    glTranslatef(0.0, -5.0, 0.0);
    glColor3f(0.133333, 0.545098, 0.133333);
    glutSolidCube(5);
    glPopMatrix();
    
    //core
    glPushMatrix();
    glTranslatef(0.0, 3.0, 0.0);
    glColor3f(0.545098, 0.270588, 0.0745098);
    glutSolidSphere(1.5, 30, 30);
    
    //petals
    glColor3f(1.0, 1.0, 0.0);
    
    glPushMatrix();
    glTranslatef(2.0, 0.0, 0.0);
    glScalef(1.5, 0.5, 1.0);
    glutSolidSphere(2.0, 30, 30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-2.0, 0.0, 0.0);
    glScalef(1.5, 0.5, 1.0);
    glutSolidSphere(2.0, 30, 30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);
    glScalef(1.5, 0.5, 1.0);
    glutSolidSphere(2.0, 30, 30);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, -1.0);
    glScalef(1.5, 0.5, 1.0);
    glutSolidSphere(2.0, 30, 30);
    glPopMatrix();
    
    glPopMatrix();
    glPopMatrix();
}

void flower4(void){
    glPushMatrix();
    //move whole flower
    glTranslatef(f4x, f4y, f4z);
    
    //stem
    glPushMatrix();
    glScalef(0.1, 1.0 ,0.1);
    glColor3f(0.443137, 0.776471, 0.443137);
    glutSolidCube(5);
    glPopMatrix();
    
    //grass cube thing that holds it
    glPushMatrix();
    glTranslatef(0.0, -5.0, 0.0);
    glColor3f(0.133333, 0.545098, 0.133333);
    glutSolidCube(5);
    glPopMatrix();
    
    //core petals
    glColor3f(1.0, 0.0, 0.2);
    glPushMatrix();
    glTranslatef(0.0, 3.0, 0.0);
    
    glPushMatrix();
    glScalef(2.0, 2.5, 2.0);
    glutSolidTetrahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1.0, 0.0, 0.0);
    glScalef(2.0, 2.5, 2.0);
    glutSolidTetrahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, 1.0, -1.0);
    glScalef(2.0, 2.5, 2.0);
    glutSolidTetrahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.0, 1.0, -1.0);
    glScalef(2.0, 2.5, 2.0);
    glutSolidTetrahedron();
    glPopMatrix();
    
    //outer petals
    glColor3f(1.0, 0.0, 0.5);
    
    glPushMatrix();
    glTranslatef(0.0, -0.5, 1.0);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    glScalef(2.0, 2.5, 2.0);
    glutSolidTetrahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-3.0, -0.5, 1.0);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    glScalef(2.0, 2.5, 2.0);
    glutSolidTetrahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1.0, 0.0, -1.5);
    glRotatef(-180.0, 0.0, 1.0, 0.0);
    glScalef(2.0, 2.5, 2.0);
    glutSolidTetrahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3.0, 0.0, -1.5);
    glRotatef(-180.0, 0.0, 1.0, 0.0);
    glScalef(2.0, 2.5, 2.0);
    glutSolidTetrahedron();
    glPopMatrix();
    
    glPopMatrix();
    glPopMatrix();
}

void drawScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //need this in case we're switching back from ortho
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10.0, 10.0, -10.0, 10.0, 5.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    
    //change your viewpoint, and also alter where the bug is flying to because that is never uniform for some reason across all the views
    if (direction == 1){
        gluLookAt(0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        b1x = f1x*2, b1y = f1y*2, b1z = f1z*2,
        b2x = f2x*1.5, b2y = f2y*2, b2z = f2z*2,
        b3x = f3x*2.5, b3y = f3y*2, b3z = f3z*2,
        b4x = f4x*1.5, b4y = f4y*2, b4z = f4z*2;
    }
    else if (direction == 2){
        gluLookAt(-40.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        b1x = f1x, b1y = f1y*2, b1z = f1z,
        b2x = f2x*2, b2y = f2y*2, b2z = f2z*2,
        b3x = f3x, b3y = f3y*2, b3z = f3z,
        b4x = f4x*2, b4y = f4y*2, b4z = f4z*2;
    }
    else if (direction == 3){
        gluLookAt(0.0, 0.0, -50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        b1x = -f1x, b1y = f1y, b1z = -f1z,
        b2x = -f2x*2, b2y = f2y, b2z = -f2z,
        b3x = -f3x*1.5, b3y = f3y, b3z = -f3z*2,
        b4x = -f4x, b4y = f4y*2.5, b4z = -f4z*2;
    }
    else if (direction == 4){
        gluLookAt(40.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        b1x = f1x*2, b1y = f1y*2, b1z = f1z,
        b2x = f2x*0.3, b2y = f2y*2, b2z = f2z,
        b3x = f3x*3, b3y = f3y*1.5, b3z = f3z,
        b4x = f4x, b4y = f4y*2, b4z = f4z;
    }
    else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-50, 50, -50, 50, 0.0, 100.0);
        
        glMatrixMode(GL_MODELVIEW);
        ortho = true;
        gluLookAt(0.0, 40.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        b1x = f1x*1.5, b1y = f1y, b1z = -f1z*1.5,
        b2x = f2x*.5, b2y = f2y, b2z = -f2z*.5,
        b3x = f3x*1.5, b3y = f3y*1.5, b3z = -f3z*1.5,
        b4x = f4x*0.5, b4y = f4y, b4z = -f4z*0.5;
    }
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    drawBug();
    flower1();
    flower2();
    flower3();
    flower4();
    glDisable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

void setup(void)
{
    glClearColor(0.529412, 0.807843, 0.980392, 0.0);
}

void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    width = w;
    height = h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10.0, 10.0, -10.0, 10.0, 5.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 'w':
            flap = !flap;
            glutPostRedisplay();
            break;
        case 'f':
            fly = !fly;
            glutPostRedisplay();
            break;
        case '1':
            direction = FRONT;
            glutPostRedisplay();
            break;
        case '2':
            direction = LEFT;
            glutPostRedisplay();
            break;
        case '3':
            direction = BACK;
            glutPostRedisplay();
            break;
        case '4':
            direction = RIGHT;
            glutPostRedisplay();
            break;
        case '0':
            direction = TOP;
            glutPostRedisplay();
            break;
        case 'r':
            direction = FRONT;
            fly = false;
            flap = false;
            v1 = false, v2 = false, v3 = false, v4 = false;
            gx = 0.0, gy = -10.0, gz = 0.0;
            f1x = -20.0, f1y = 10.0, f1z = -10.0,
            f2x = 20.0, f2y = 8.0, f2z = -15.0,
            f3x = -15.0, f3y = -20.0, f3z = -5.0,
            f4x = 30.0, f4y = -10.0, f4z = -13.0;
            ta = .2;
            wa = 0;
            t = 0;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

void specialKeyInput(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) {
        if (ta < .8) ta +=.2;
    }
    if(key == GLUT_KEY_DOWN){
        if (ta > .2) ta -=.2;
    }
    glutPostRedisplay();
}

// Update the location of the current point as the mouse moves with button pressed in display window
void mouseMotion(int x, int y)
{
    if (x == f1x){
            f1x = x;
            f1y = (height/2)-y;
    }
    glutPostRedisplay();
}

//mouse method for Display Window
void mouseDisplay(int button, int state, int x, int y)
{
    if (ortho){
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
        //check if click is within window
            if ( x < 0 || x > width || y < 0 || y > height ) ;
            else mouseMotion(x, y);
        }
    }
}

void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press the escape key to quit." << endl;
    cout << "Press 'w' to flap the bug's wings, or stop flapping them." << endl;
    cout << "Press 'f' to see the bug fly. You cannot stop flapping the bug's wings while it is flying." << endl;
     cout << "You can change your view! Press '1' to see the default view, '2' to see the scene from the left side, '3' to see the scene from the back, '4' to see the scene from the right, and '0' to see the scene from up above." << endl;
    cout << "Press 'r' to return to the default settings." << endl;
    cout << "Press the up and down arrow keys to speed up or slow down." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("Flying Bug");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouseDisplay);
    glutMotionFunc(mouseMotion);
    glutSpecialFunc(specialKeyInput);
    glutTimerFunc(5, flapWings, 1);
    glutTimerFunc(5, flight, 1);
    glutMainLoop();
    
    return 0;
}














