/******************************************
 *
 * Official Name:  Megi Shehi
 *
 * Nickname:  Megi
 *
 * E-mail:  mshehi@syr.edu
 *
 * Assignment:  Assignment 1
 *
 * Environment/Compiler:  XCode
 *
 * Date:  February 5, 2018
 *
 * References: http://avatar.se/molscript/doc/colour_names.html for color codes
 *
 * Interactions:   Press the escape key to quit.
 *                 Press 's' to switch between sunset and space modes -
 *                 space mode also creates wired spheres.
 *                 Press 'p' to switch between parallel and perspective views.
 *
 *******************************************/

//  Created by Megi Shehi on 1/24/18.
//  Copyright © 2018 Megi Shehi. All rights reserved.

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
static float R = 4.0; // Radius of head.
static int v = 10; // Number of longitudinal (vertical) slices
static int h = 5; // Number of latitudinal (horizontal) slices
bool Sunset = true; //Background is sunset
bool Parallel = true; //Default is parallel view
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// on hemisphere.
// Removed global values to allow for more than one person to be drawn at once
static float cx, cy, cz; //center of sphere
static float bodylength;
static float armlength;
static float leglength;
static float armangle;
static float legangle;

// vertex array for sunset
static float vertices[] = {
    -50.0, 50.0, -15.0,
    50.0, 50.0, -15.0,
    50.0, 0.0, -15.0,
    -50.0, 0.0, -15.0
};
//color array for sunset
static float colors[] = {
    1.0, 0.843137, 0.0, //gold
    0.854902, 0.647059, 0.12549, //goldenrod
    1.0, 0.54902, 0.0, //dark orange
    1.0, 0.270588, 0 //orangered
};


// to print bitmap strings
void writeBitmapString(void *font, char *string)
{
    char *c;
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void drawHead()
{
    int i, j;
    // Based on hemisphere.cpp
    // cx, cy,cz is the center of the sphere, R is the radius.
    if (Sunset) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //changed to fill in head
        else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wired view in space mode
    
    for(j = 0; j < h; j++)
    {
        // One latitudinal triangle strip. top half
        glBegin(GL_TRIANGLE_STRIP);
        for(i = 0; i <= v; i++)
        {
            glVertex3f( R * cos( (float)(j+1)/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI )+cx,
                       R * sin( (float)(j+1)/h * PI/2.0 )+cy,
                       R * cos( (float)(j+1)/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+cz );
            glVertex3f( R * cos( (float)j/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI  )+cx,
                       R * sin( (float)j/h * PI/2.0  )+cy,
                       R * cos( (float)j/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI)+cz );
        }
        glEnd();
        // One latitudinal triangle strip. bottom half
        glBegin(GL_TRIANGLE_STRIP);
        for(i = 0; i <= v; i++)
        {
            glVertex3f( R * cos( (float)(j+1)/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI )+cx,
                       -1*R * sin( (float)(j+1)/h * PI/2.0 )+cy,
                       R * cos( (float)(j+1)/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI )+cz );
            glVertex3f( R * cos( (float)j/h * PI/2.0 ) * cos( 2.0 * (float)i/v * PI  )+cx,
                       -1*R * sin( (float)j/h * PI/2.0  )+cy,
                       R * cos( (float)j/h * PI/2.0 ) * sin( 2.0 * (float)i/v * PI)+cz );
        }
        glEnd();
    }
}

void drawEyes()
{
    float t = 0; // Angle parameter.
    
    //uses center coordinates to draw on a generic spherical head
    
    glBegin(GL_POLYGON);
    for(int i = 0; i < 10; ++i){
        glVertex3f(cx + (R/2) + cos(t), cy + sin(t), cz); //right eye
        t += 2 * PI / 10;
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    for(int i = 0; i < 10; ++i){
        glVertex3f(cx - (R/2) - cos(t), cy - sin(t), cz); //left eye
        t -= 2 * PI / 10;
    }
    glEnd();
}

void drawStickBody()
{
    if (Sunset) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < 10; i++){
        glVertex3f(cx+ cos(i+1*PI/4), cy-R-1 +sin(i+1*PI/4), cz + i);
        glVertex3f(cx+ cos(i+1*PI/4), cy-R-bodylength +sin(i+1*PI/4), cz + i);
    }
 glEnd();
}

void drawStickArms()
{
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glVertex3f(cx,cy-R-.2*bodylength,cz); //down 20% of body
    glVertex3f(cx+armlength*cos(armangle),
               cy-R-.2*bodylength+armlength*sin(armangle),cz);
    glVertex3f(cx,cy-R-.2*bodylength,cz); //down 20% of body
    glVertex3f(cx-armlength*cos(armangle),
               cy-R-.2*bodylength+armlength*sin(armangle),cz);
    glEnd();
    glLineWidth(1.0);
}

void drawSkirt()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    float t = 0; //angle
    
    glBegin(GL_TRIANGLE_FAN);
    //position at end of body
    glVertex3f( cx, cy-R-(bodylength*1.5), cz);
    for(int i = 0; i <= 20; ++i) //only make half the circle
    {
        t = 2 * PI * i / 40;
        glVertex3f(cx + cos(t) * R, cy-R-(bodylength*1.5) + sin(t) * R, cz);
    }
    glEnd();
}

void drawStickLegs()
{
    glLineWidth(5.0);
    glBegin(GL_LINES);
        glVertex3f(cx,cy-R-bodylength,cz); //down at bottom of body
        glVertex3f(cx+leglength*cos(legangle),
                   cy-R-bodylength+leglength*sin(legangle),cz);
        glVertex3f(cx,cy-R-bodylength,cz); //down at bottom of body
        glVertex3f(cx-leglength*cos(legangle),
                   cy-R-bodylength+leglength*sin(legangle),cz);
    glEnd();
    glLineWidth(1.0);
}

void sunset()
{
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    //draw sunset background
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 4; i++) glArrayElement(i%4);
    glEnd();
    
    //draw red setting sun
    float t;
    
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    //position at end of background
    glVertex3f( -25.0, 0.0, -15.0);
    for(int i = 0; i <= 40; ++i)
    {
        t = 2 * PI * i / 40;
        glVertex3f(-25.0 + cos(t) * 10, sin(t) * 10, -10.0);
    }
    glEnd();

    
    //draw ground
    glBegin(GL_POLYGON);
    glColor3f(0.419608, 0.556863, 0.137255); //olivedrab for top half
    glVertex3f(-50.0, 0.0, -10.0);
    glVertex3f(50.0 ,0.0, -10.0);
    glColor3f(0.603922, 0.803922, 0.196078); //yellowgreen for lower half
    glVertex3f(50.0, -50.0, -10.0);
    glVertex3f(-50.0, -50.0, -10.0);
    glEnd();
    glDisable(GL_DEPTH_TEST); // Disable depth testing.
    
    //output text
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-30.0, -30.0, 5.0);
    writeBitmapString((void*)font, "Look at this sunset!");
    
}

void space()
{
    glClearColor(0.0980392, 0.0980392, 0.439216, 0.0); //midnightblue
    glClear(GL_COLOR_BUFFER_BIT);
    
    float R = 5.0; // Radius of helix.
    
    float t; // Angle parameter.
    
    //this represents a spiral galaxy of sorts
    glBegin(GL_LINE_STRIP);
    for(t = -10 * PI; t <= 0 * PI; t += PI/15.0){
        glColor3ub(rand()%256, rand()%256, rand()%256); //randomizes colors
        glVertex3f(R * cos(t)-20, R * sin(t)-20, t);
        R-=.25; //creates a spiral, even in ortho
    }
    glEnd();
    
    glColor3f(1.0, 1.0, 1.0); //white
    t=0;
    //series of circles representing 'stars'
    glBegin(GL_POLYGON);
    for(int i = 0; i < 10; ++i){
    glVertex3f(-30 + cos(t), 30 + sin(t), 0.0);
    t += 2 * PI / 10;
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    for(int i = 0; i < 10; ++i){
        glVertex3f(20 + cos(t), 10 + sin(t), 0.0);
        t += 2 * PI / 10;
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    for(int i = 0; i < 10; ++i){
        glVertex3f(40 + cos(t), -25 + sin(t), 0.0);
        t += 2 * PI / 10;
    }
    glEnd();
    
    //output text
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(-30.0, 35.0, 5.0);
    writeBitmapString((void*)font, "Now we're in space!");
}


// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (Parallel) glOrtho(-50.0, 50.0, -50.0, 50.0, -30.0, 30.0);
    else glFrustum(-50.0, 50.0, -50.0, 50.0, 5.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    
    if (Sunset) {
        sunset();
        cx = 2.0;
        cy = 3.0;
        cz = -5.0;
        bodylength = 4.0;
        armlength=4.0;
        leglength=8.0;
        armangle=PI/20.0;
        legangle=-(1.0*PI/3.0);
        
        glColor3f(1.0, 0.894118, 0.768627); //color bisque for head and legs
        drawHead();
        drawStickLegs();
        
        glColor3f(1.0, 0.0784314, 0.576471); //deeppink color for clothes
        drawStickBody();
        drawStickArms();
        drawSkirt();
        
        glColor3f(0.4, 0.803922, 0.666667);//medium aquamarine for eyes
        drawEyes();
        
        cx = 20.0;
        cy = 10.0;
        cz = 5.0;
        bodylength = 5.0;
        armlength=5.0;
        leglength=7.0;
        armangle=PI/10.0;
        legangle=-(1.0*PI/5.0);
        
        glColor3f(1.0, 0.894118, 0.768627); //bisque for head
        drawHead();
        
        glColor3f(0.4, 0.803922, 0.666667); //medium aquamarine for everything else
        drawStickBody();
        drawStickLegs();
        drawStickArms();
        
        glColor3f(1.0, 0.0, 1.0);//purple for eyes
        drawEyes();
        
    }
    else {
        space();
        cx = 0.0;
        cy = -10.0;
        cz = -5.0;
        bodylength = 4.0;
        armlength=4.0;
        leglength=8.0;
        armangle=-PI/5.0;
        legangle=-(1.0*PI/10.0);
        glColor3f(1.0, 1.0, 1.0); //white color for all features
        
        drawHead();
        drawStickBody();
        drawStickLegs();
        drawStickArms();
        
        cx = 20.0;
        cy = 30.0;
        cz = 5.0;
        bodylength = 3.0;
        armlength= 3.0;
        leglength= 5.0;
        armangle= PI/20.0;
        legangle= (1.0*PI/20.0);
        
        drawHead();
        drawStickBody();
        drawStickLegs();
        drawStickArms();
        
    }
    
    glutSwapBuffers();
}

// Routine to output interaction instructions to the console.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press the escape key to quit." << endl;
    cout << "Press 's' to switch between sunset and space modes - space mode also creates wired spheres." << endl;
    cout << "Press 'p' to switch between parallel and perspective views." << endl;
}


// Initialization routine.
void setup(void)
{
    //enable vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    //enable color array
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, colors);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (Parallel) glOrtho(-50.0, 50.0, -50.0, 50.0, -30.0, 30.0);
    else glFrustum(-50.0, 50.0, -50.0, 50.0, 5.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 's': //change between sunset and space modes
            Sunset = !(Sunset);
            glutPostRedisplay();
            break;
        case 'p':
            Parallel = !(Parallel); //change betwen parallel and perspective views
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

// Call methods in main routine
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Assignment 1");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMainLoop();
    
    return 0;
}

