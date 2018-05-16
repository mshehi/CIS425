//  Skateboard.cpp
//  CIS425
//
//  Created by Megi Shehi on 4/11/18.
//  Copyright © 2018 Megi Shehi. All rights reserved.
//

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>
#define PI 3.14159265
#define R 5

#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

double clipl[4] = {0.0, 1.0, 0.0, 1.0 }, clipr[4] = {0.0, -1.0, 0.0, 0.0};
static unsigned int texture[1]; // Array of texture indices
static int id = 0; // Currently displayed texture id.
static int p = 15; // Number of grid columns.
static int q = 12; // Number of grid rows
static float *vertices = NULL; // Vertex array of the mapped sample on the torus.
static float *textureCoordinates = NULL;

// Struct of bitmap file. 
struct BitMapFile
{
    int sizeX;
    int sizeY;
    unsigned char *data;
};

BitMapFile *getBMPData(string filename)
{
    BitMapFile *bmp = new BitMapFile;
    unsigned int size, offset, headerSize;
    
    // Read input file name.
    ifstream infile(filename.c_str(), ios::binary);
    
    // Get the starting point of the image data.
    infile.seekg(10);
    infile.read((char *) &offset, 4);
    
    // Get the header size of the bitmap.
    infile.read((char *) &headerSize,4);
    
    // Get width and height values in the bitmap header.
    infile.seekg(18);
    infile.read( (char *) &bmp->sizeX, 4);
    infile.read( (char *) &bmp->sizeY, 4);
    
    // Allocate buffer for the image.
    size = bmp->sizeX * bmp->sizeY * 24;
    bmp->data = new unsigned char[size];
    
    // Read bitmap data.
    infile.seekg(offset);
    infile.read((char *) bmp->data , size);
    
    // Reverse color from bgr to rgb.
    int temp;
    for (int i = 0; i < size; i += 3)
    {
        temp = bmp->data[i];
        bmp->data[i] = bmp->data[i+2];
        bmp->data[i+2] = temp;
    }
    
    return bmp;
}

void loadExternalTextures()
{
    // Local storage for bmp image data.
    BitMapFile *image[1];
    
    // Load the texture.
    image[0] = getBMPData("Textures/lightn.bmp");
    
    // Activate texture index texture[0].
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    
    // Create texture index array.
    glGenTextures(1, texture);
    
    // Load external textures.
    loadExternalTextures();
    
    // Turn on OpenGL texturing.
    glEnable(GL_TEXTURE_2D);
    
    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
}

void drawScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gluLookAt(0.0, 30.0, -15.0, 0.0, 15.0, -400.0, 0.0, 1.0, 0.0);

    glLoadIdentity();
   
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_2D);
    
    glPushMatrix();
    glTranslatef(0.0, -40.0, -10.0);
    glColor3f(1.0, 0.0, 0.0);
    glScalef(1.0, 1.0, 2.0);
    
    
    glutSolidSphere(20, 40, 40);
    glPopMatrix();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glutSwapBuffers();
}


void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(110,1,1,500);
    
    glMatrixMode(GL_MODELVIEW);
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("Skateboard");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutMainLoop();
    
    return 0;
}
