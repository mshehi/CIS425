////////////////////////////////////////////////////          
// squareV2.cpp
//
// Stripped down OpenGL program that draws a square.
// 
// Original square.cpp from Sumanta Guha.
// Modification by Marjory Baruch
//
//  Modifications:
//     Part 0:  original black square from last class
//              can change from polygon to lines etc
//     Part 1:  draw small green rectangle too (Keep part 0)
//     Part 4:  Change pointsize and width (add to 3)
//     Part YYY:  Triangle sticking out of viewing box
//     Part ZZZ:  Rectangle offscreen
//     Part 2:  Polygon with different color vertices
//     Part 3:  Two triangle strips, front and back, one fill, one line
//     Part 5:  Triangle fan, first black fill, then red line
//               Switch from FRONT_AND_BACK to two statements, one FRONT, one BACK
//     Part 6:  Convexity Example. black fill and red line. 
//                 Note confusion of what "fill" means!
//
////////////////////////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Drawing (display) routine.
void drawScene(void)
{
   // Clear screen to background color.
   glClear(GL_COLOR_BUFFER_BIT);

  //*
  //Part 0
  //Original square  **********
  // Draw a polygon with specified vertices.
  
  //glBegin(GL_LINES);
  glColor3f(0.0,0.0,0.0);
  glBegin(GL_POLYGON);
  glVertex3f(20.0, 20.0, 0.0);
  glVertex3f(80.0, 20.0, 0.0);
  glVertex3f(80.0, 80.0, 0.0);
  glVertex3f(20.0, 80.0, 0.0);
  glEnd();
  //End Original square  **********
  //*/
  
  
  /*
  // Part 1
  //Draw small green rectangle
  glColor3f(0.0,1.0,0.0);
  glBegin(GL_POLYGON);
  glVertex3f(40.0, 30.0, 0.0);
  glVertex3f(97.0, 30.0, 0.0);
  glVertex3f(97.0, 60.0, 0.0);
  glVertex3f(40.0, 60.0, 0.0);
  glEnd();
  //End small rectangle
  */
  
  /*
  // Part 2
  // Draw a polygon with specified vertices********
  // and colors
  
   glBegin(GL_POLYGON);
   glColor3f(1.0,0.0,0.0);
   glVertex3f(20.0, 20.0, 0.0);
   glColor3f(0.0,0.0,1.0);
   glVertex3f(80.0, 20.0, 0.0);
   glColor3f(0.0,1.0,0.0);
   glVertex3f(80.0, 80.0, 0.0);
   glColor3f(1.0,1.0,0.0);
   glVertex3f(20.0, 80.0, 0.0);
   glEnd();
   // end part 2 ***********
  */
  
  
  
  /*
   // Part 4
   // Change pointsize and width
   glPointSize(3.0);
   glLineWidth(5.0);
   
  */
  
  
  //Several triangles, different ways
  
  // Part 3
  
  /*
  
   
   // front and back, triangle strips, one fill, one line ********
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glBegin(GL_TRIANGLE_STRIP);
   glVertex3f(10.0, 90.0, 0.0);
   glVertex3f(10.0, 10.0, 0.0);
   glVertex3f(35.0, 75.0, 0.0);
   glVertex3f(30.0, 20.0, 0.0);
   glVertex3f(90.0, 90.0, 0.0);
   glVertex3f(80.0, 40.0, 0.0);
   glEnd();
   
  glColor3f(1.0,0.0,0.0);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3f(10.0, 90.0, 0.0);
  glVertex3f(10.0, 10.0, 0.0);
  glVertex3f(35.0, 75.0, 0.0);
  glVertex3f(30.0, 20.0, 0.0);
  glVertex3f(90.0, 90.0, 0.0);
  glVertex3f(80.0, 40.0, 0.0);
  glEnd();
  //end of front and back triangle strips
 
  
  
   */
  
  /*
  // Part 5
  // Triangle fan, first black fill, then red line*********
   glColor3f(0.0,0.0,0.0);
   glLineWidth(8.0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  //glPolygonMode(GL_FRONT,GL_FILL);
  //glPolygonMode(GL_BACK,GL_LINE);
  
  
   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(10.0, 10.0, 0.0);
   glVertex3f(15.0, 90.0, 0.0);
   glVertex3f(55.0, 75.0, 0.0);
   glVertex3f(80.0, 30.0, 0.0);
   glVertex3f(90.0, 10.0, 0.0);
   glEnd();
   
   
   glLineWidth(3.0);
   glColor3f(1.0,0.0,0.0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(10.0, 10.0, 0.0);
   glVertex3f(15.0, 90.0, 0.0);
   glVertex3f(55.0, 75.0, 0.0);
   glVertex3f(80.0, 30.0, 0.0);
   glVertex3f(90.0, 10.0, 0.0);
   glEnd();
    
   //end of triangle fans************
   */
  
  
  
  /*
  // Part YYY
  //Triangle sticking out of viewing box ***
  glColor3f(0.0,0.0,0.0);
  glBegin(GL_POLYGON);
  glVertex3f(20.0, 20.0,10.0);
  glVertex3f(80.0, 20.0, 0.0);
  glVertex3f(80.0, 80.0, 0.0);
  glEnd();
  */
  //end triangle ******
 
  
  // Part ZZZ
  // rectangle off screen ********
  /*
  glColor3f(0.0,0.0,0.0);
  glBegin(GL_POLYGON);
  glVertex3f(-20.0, -20.0, 0.0);
  glVertex3f(80.0, 20.0, 0.0);
  glVertex3f(120.0, 120.0, 0.0);
  glVertex3f(20.0, 80.0, 0.0);
  glEnd();
   */
  // End rectangle off screen *******
  
  
  
  
  // Part 6
  //Convexity Example
  // black fill and red line
  // Note confusion of what "fill" means!
 /*
  glColor3f(0.0,0.0,0.0); 
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_POLYGON);
  glVertex3f(80.0, 20.0, 0.0);
  glVertex3f(40.0, 40.0, 0.0);
  glVertex3f(20.0, 80.0, 0.0);
  glVertex3f(20.0, 20.0, 0.0);
  glEnd();
  
  glColor3f(1.0,0.0,0.0);
  glLineWidth(3.0);
  glBegin(GL_LINE_LOOP);
  glVertex3f(80.0, 20.0, 0.0);
  glVertex3f(40.0, 40.0, 0.0);
  glVertex3f(20.0, 80.0, 0.0);
  glVertex3f(20.0, 20.0, 0.0);
  glEnd();
  */
  
  
  // Flush created objects to the screen, i.e., force rendering.
   //glFlush();
    glutSwapBuffers();  //*******!!!!!!!!!!!
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
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);

   // Set matrix mode to modelview.
   glMatrixMode(GL_MODELVIEW);

   // Clear current modelview matrix to identity.
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
	  // Press escape to exit.
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char **argv) 
{  
   // Initialize GLUT.
   glutInit(&argc, argv);
 
   // Set display mode as single-buffered and RGB color.
   //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//****!!!!
   
   // Set OpenGL window size.
   glutInitWindowSize(500, 500);

   // Set position of OpenGL window upper-left corner.
   glutInitWindowPosition(100, 100); 
  
   
   // Create OpenGL window with title.
   glutCreateWindow("squareV2.cpp");
   
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
