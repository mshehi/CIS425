///////////////////////////////////////////////////////////////////////////////////////          
// person.cpp
// Author:  Baruch
//
// This program draws a person.
//
// Interaction:
// Press V/v to increase/decrease the number of longitudinal (verticle) slices.
// Press H/h to increase/decrease the number of latitudinal (horizontal) slices.
//
// 
/////////////////////////////////////////////////////////////////////////////////////// 

#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static float R = 1.0; // Radius of head.
static int v = 6; // Number of longitudinal (verticle) slices.
static int h = 4; // Number of latitudinal (horizontal) slices 
//on hemisphere.
static float cx=2.0, cy=3.0, cz=-10.0;  //center of sphere
static float bodylength=4.0;
static float armlength=4.0;
static float leglength=6.0;
static float armangle=PI/10.0;
//static float legangle=-(4.0*PI/10.0);
static float legangle=-(1.0*PI/10.0);

void drawHead()
{
  int i, j;
  // Based on hemisphere.cpp
  // cx, cy,cz is the center of the sphere, R is the radius.
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(0.0, 0.0, 0.0);
  
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

void drawStickBody()
{
  glLineWidth(5.0);
  glColor3f(0.0,1.0,0.0);
  glBegin(GL_LINES);
  glVertex3f(cx,cy-R,cz);
  glVertex3f(cx,cy-R-bodylength,cz);
  glEnd();
  glLineWidth(1.0);
  glColor3f(0.0,0.0,0.0);
  
}

void drawStickArms()
{
  glLineWidth(5.0);
  glColor3f(0.3,0.3,1.0);
  glBegin(GL_LINES);
  glVertex3f(cx,cy-R-.2*bodylength,cz); //down 20% of body
  glVertex3f(cx+armlength*cos(armangle),
             cy-R-.2*bodylength+armlength*sin(armangle),cz); 
  glVertex3f(cx,cy-R-.2*bodylength,cz); //down 20% of body
  glVertex3f(cx-armlength*cos(armangle),
             cy-R-.2*bodylength+armlength*sin(armangle),cz); 
  glEnd();
  glLineWidth(1.0);
  glColor3f(0.0,0.0,0.0);
}

void drawStickLegs()
{
  glLineWidth(5.0);
  glColor3f(1.0,0.3,0.7);
  glBegin(GL_LINES);
  glVertex3f(cx,cy-R-bodylength,cz); //down at bottom of body
  glVertex3f(cx+leglength*cos(legangle),
             //cy-R-.2*bodylength+leglength*sin(legangle),cz);
    cy-R-bodylength+leglength*sin(legangle),cz);//
  glVertex3f(cx,cy-R-bodylength,cz); //down at bottom of body
  glVertex3f(cx-leglength*cos(legangle),
             //cy-R-.2*bodylength+leglength*sin(legangle),cz);
   cy-R-bodylength+leglength*sin(legangle),cz);//
  glEnd();
  glLineWidth(1.0);
  glColor3f(0.0,0.0,0.0);
}


// Drawing routine.
void drawScene(void)
{
  glClear (GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  
  drawHead();
  drawStickBody();
  drawStickArms();
  drawStickLegs();
  
  
  glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
  //white background
  glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-20.0, 20.0, -20.0, 20.0, 0.0,20.0);
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
    case 'V':
      v += 1;
      glutPostRedisplay();
      break;
    case 'v':
      if (v > 3) v -= 1;
      glutPostRedisplay();
      break;
    case 'H':
      h += 1;
      glutPostRedisplay();
      break;
    case 'h':
      if (h > 3) h -= 1;
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
  cout << "Press V/v to increase/decrease the number of longitudinal slices." << endl
  << "Press H/h to increase/decrease the number of latitudinal slices." << endl;
  
}

// Main routine.
int main(int argc, char **argv) 
{
  printInteraction();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("A Person");
  setup(); 
  glutDisplayFunc(drawScene); 
  glutReshapeFunc(resize);  
  glutKeyboardFunc(keyInput);
  glutMainLoop(); 
  
  return 0;  
}

