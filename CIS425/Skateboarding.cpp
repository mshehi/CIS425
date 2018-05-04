/*******************************************
 *
 * Official Name:  Megi Shehi
 *
 * Nickname:  Megi
 *
 * E-mail:  mshehi@syr.edu
 *
 * Assignment:  Final Project
 *
 * Environment/Compiler:  XCode
 *
 * Date:  May 3, 2018
 *
 * References: http://avatar.se/molscript/doc/colour_names.html
        Rocks Texture: https://www.google.com/imgres?imgurl=http%3A%2F%2Ffridayillustrated.com%2Fwp-content%2Fuploads%2F2015%2F04%2Ffghj.jpg&imgrefurl=http%3A%2F%2Ffridayillustrated.com%2Fpattern-library-cool-pattern-designs-free-use%2F&docid=em3ncXAnkjl-AM&tbnid=KC4KI4YJWhj_HM%3A&vet=10ahUKEwjKi9XX3-XaAhVFdt8KHU-_BlIQMwjwASgFMAU..i&w=700&h=600&bih=570&biw=1252&q=cool%20patterns&ved=0ahUKEwjKi9XX3-XaAhVFdt8KHU-_BlIQMwjwASgFMAU&iact=mrc&uact=8
        Swirl Texture: https://www.youtube.com/watch?v=CubMuBJFtlA
        Triangle Texture: https://www.wallartprints.com/cool-patterns/
 *
 * Interactions:  Press the up arrow key to move forward and the down arrow key to move backwards. Press the left arrow key to turn left, the right arrow key to turn right. Press 'y' to look down and 'Y' to look up. Press 'f' to toggle fog on or off in Space world. Press 's' to skate on a nearby object. Press '1', '2', or '3' to pick a texture for the skateboard. Conversely, click on one from the start screen. 1: Rocks, 2: Waves, 3: Triangles. Press '4', '5', or '6' to choose a world to skateboard in. Conversely, click on one from the choose screen. 4: Classic, 5: Space, 6: Water. Press the escape key to quit.
 *
 *******************************************/
//  Copyright © 2018 Megi Shehi. All rights reserved.

#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>

#define PI 3.14159265
#define CLASSIC 0
#define SPACE 1
#define WATER 2

static double randang = double(rand()%360);
#define RATIO 0.85 // Growth ratio = length of tree sub-branch to length of branch.
//for random angles that don't refresh!
#define ANGLE randang // Angle between the two tree sub-branches.

#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

//a whole lot of globals!!
static int animationPeriod = 100;

int world; //which world?
//to skate or not to skate
bool s = false,
//are we in the start screen?
init=true,
//are we in the choose world screen?
choose = false, collision = false,
//do we want fog in space?
fog = true;

int enter = 0; //track entry into skate motion loop
int pick = 0; //which texture for skateboard
float rot = 0.0; //rotate board
static float gx = 0.0, gy = 0.0, gz = 100.0, ylook = 0.0; //position coordinates
static float angle=180; //view angle
static float step=5.0;  //step size
static float turn=10.0; //degrees to turn
static float satrot = 45.0;
static unsigned int texture[3]; // Array of texture indices for board

static unsigned int closestName = 0; // Name of closest hit.
static int isSelecting = 0; // In selection mode?
static unsigned int buffer[1024]; // Hit buffer.
static int hits; // Number of entries in hit buffer.
static int highlightFrames = 10; // Number of frames to keep highlight.

//for tree fractal
class Source; // Make source class visible.
static int maxLevel = 5; // Recursion level.


//clipping plane for base cube
double clip0[] = {1,0,0,0};
//clipping planes for jupiter
double jup1[4] = {0.0, -1.0, 0.0, -40.0 }, jup2[4] = {0.0, 1.0, 0.0, 40.0}, jup3[4] = {0.0, -1.0, 0.0, -10.0}, jup4[4] = {0.0, 1.0, 0.0, 10}, jup5[4] = {0.0, -1.0, 0.0, 20}, jup6[4] = {0.0, 1.0, 0.0, 20};

//stars
float starsl[30][30][3], starsr[30][30][3], starsf[30][30][3], starsb[30][30][3];

//light stuff
float sun[] = {1.0, 1.0, 1.0, 1.0}, global[] = {0.7, 0.7, 0.7, 1.0}, globaldark[] = {0.3, 0.3, 0.3, 1.0}, lightPos[] = {0.0, 400.0, 0.0, 1.0}, reset[] = {1.0, 1.0, 1.0, 1.0}, glow[] = {1.0, 0.0, 0.0, 1.0}, unglow[] = {0.0, 0.0, 0.0, 1.0}, bubble[] = {1.0, 1.0, 1.0, 0.5};

//colors
float crimson[] = {0.862745, 0.0784314, 0.235294, 1.0}, gold[] = {1.0, 0.843137, 0.0, 1.0}, grey[] = {0.5, 0.5, 0.5, 1.0}, turquoise[] = {0.282353, 0.819608, 0.8, 1.0}, aquamarine[] = {0.498039, 1, 0.831373, 1.0}, pink[] = {1.0, 0.0784314, 0.576471,1.0}, black[]= {1.0, 1.0, 1.0, 1.0}, green[] = {0.133333, 0.545098, 0.133333, 1.0}, sky[] = {0.6, 0.6, 1.0, 1.0}, tree[] = {0.627451, 0.321569, 0.176471, 1.0}, jcolor1[] = {0.5, 0.2, 0.2, 1.0}, jcolor2[] = {1.0, 0.854902, 0.72549, 1.0}, jcolor3[] = {0.956863, 0.643137, 0.376471, 1.0}, sat1[] = {0.5, 1.0, 1.0, 1.0}, sat2[] = {1.0, 0.752941, 0.796078, 1.0};

//cylinder
GLUquadric* cyl = gluNewQuadric();

//points for Bezier surfaces
//standardRamp
float controlPoints[6][4][3] =
{
    {{-40.0, 0.0, -60.0}, {-40.0, 0.0, -30.0}, {-40.0, 0.0, 30.0}, {-40.0, 0.0, 60.0}},
    {{-20.0, -20.0, -60.0}, {-20.0, -20.0, -30.0}, {-20.0, -20.0, 30.0}, {-20.0, -20.0, 60.0}},
    {{0.0, -40.0, -60.0}, {0.0, -40.0, -30.0}, {0.0, -40.0, 30.0}, {0.0, -40.0, 60.0}},
    {{0.0, -40.0, -60.0}, {0.0, -40.0, -30.0}, {0.0, -40.0, 30.0}, {0.0, -40.0, 60.0}},
    {{20.0, -20.0, -60.0}, {20.0, -20.0, -30.0}, {20.0, -20.0, 30.0}, {20.0, -20.0, 60.0}},
    {{40.0, 0.0, -60.0}, {40.0, 0.0, -30.0}, {40.0, 0.0, 30.0}, {40.0, 0.0, 60.0}}
};

//wavyRamp
float wavePoints[12][4][3] =
{
    {{-40.0, 0.0, 240.0}, {-40.0, 0.0, 220.0}, {-40.0, 0.0, 200.0}, {-40.0, 0.0, 180.0}},
    {{-20.0, -20.0, 240.0}, {-20.0, -20.0, 220.0}, {-20.0, -20.0, 200.0}, {-20.0, -20.0, 180.0}},
    {{0.0, -40.0, 240.0}, {0.0, -40.0, 220.0}, {0.0, -40.0, 200.0}, {0.0, -40.0, 180.0}},
    {{20.0, -20.0, 240.0}, {20.0, -20.0, 220.0}, {20.0, -20.0, 200.0}, {20.0, -20.0, 180.0}},
    {{40.0, 0.0, 240.0}, {40.0, 0.0, 220.0}, {40.0, 0.0, 200.0}, {40.0, 0.0, 180.0}},
    {{60.0, -20.0, 240.0}, {60.0, -20.0, 220.0}, {60.0, -20.0, 200.0}, {60.0, -20.0, 180.0}},
    {{80.0, -40.0, 240.0}, {80.0, -40.0, 220.0}, {80.0, -40.0, 200.0}, {80.0, -40.0, 180.0}},
    {{100.0, -20.0, 240.0}, {100.0, -20.0, 220.0}, {100.0, -20.0, 200.0}, {100.0, -20.0, 180.0}},
    {{120.0, 0.0, 240.0}, {120.0, 0.0, 220.0}, {120.0, 0.0, 200.0}, {120.0, 0.0, 180.0}},
    {{140.0, -20.0, 240.0}, {140.0, -20.0, 220.0}, {140.0, -20.0, 200.0}, {140.0, -20.0, 180.0}},
    {{160.0, -40.0, 240.0}, {160.0, -40.0, 220.0}, {160.0, -40.0, 200.0}, {160.0, -40.0, 180.0}},
    {{180.0, -60.0, 240.0}, {180.0, -60.0, 220.0}, {180.0, -60.0, 200.0}, {180.0, -60.0, 180.0}}
};

//simple
static float rampPoints[6][4][3] =
{
    {{-280.0, -40.0, -60.0}, {-280.0, -40.0, -30.0}, {-280.0, -40.0, 30.0}, {-280.0, -40.0, 60.0}},
    {{-320.0, -40.0, -60.0}, {-320.0, -40.0, -30.0}, {-320.0, -40.0, 30.0}, {-320.0, -40.0, 60.0}},
    {{-340.0, -30.0, -60.0}, {-340.0, -30.0, -30.0}, {-340.0, -30.0, 30.0}, {-340.0, -30.0, 60.0}},
    {{-360.0, -20.0, -60.0}, {-360.0, -20.0, -30.0}, {-360.0, -20.0, 30.0}, {-360.0, -20.0, 60.0}},
    {{-380.0, -10.0, -60.0}, {-380.0, -10.0, -30.0}, {-380.0, -10.0, 30.0}, {-380.0, -10.0, 60.0}},
    {{-400.0, 0.0, -60.0}, {-400.0, 0.0, -30.0}, {-400.0, 0.0, 30.0}, {-400.0, 0.0, 60.0}}
};

//for fractal stuff
class Sequel
{
public:
    Sequel() { coords.clear(); v.clear(); }
    
    void drawTree(); // Routine to draw Sequel object in case it represents a tree.
    
    friend class Source;
    
private:
    vector<float> coords; // Vector of x, y co-ordinates of points specifying sequel object.
    vector<Source> v; // Vector of associated source objects to be produced recursively.
};

void Sequel::drawTree()
{
    glBegin(GL_LINE_STRIP);
    for (int i=0; i < 3; i++)
        glVertex2f(coords[2*i], coords[2*i+1]);
    glEnd();
}

class Source
{
public:
    Source(){ }
    Source(float coordsVal[4])
    {
        for (int i=0; i < 4; i++) coords[i] = coordsVal[i];
    }
    
    void drawT(); // Routine to draw source line segment.
    Sequel sourceToSequelTree(); // Routine to generate Sequel object in case of tree.
    void produceTree(int level); // Recursive routine to produce tree.
    
    friend class Sequel;
    
private:
    float coords[4]; // x, y co-ordinates of a line segment endpoints.
};

// Routine to generate Sequel object in case of tree.
Sequel Source::sourceToSequelTree()
{
    float x0, y0, x1, y1, z, coordsVal[10], coordsVal1[4];
    int i, j;
    Source s;
    Sequel seq = *new Sequel();
    
    x0 = coords[0]; y0 = coords[1]; x1 = coords[2]; y1 = coords[3];
    
    // From the co-ordinates of the two segment endpoints calculate the co-ordinates of
    // the 3 vertices of the sub-branch V-shape.
    coordsVal[0] = x1 + RATIO*cos((PI/180.0)*ANGLE/2.0)*(x1-x0) - RATIO*sin((PI/180.0)*ANGLE/2.0)*(y1-y0);
    coordsVal[1] = y1 + RATIO*cos((PI/180.0)*ANGLE/2.0)*(y1-y0) + RATIO*sin((PI/180.0)*ANGLE/2.0)*(x1-x0);
    coordsVal[2] = x1;
    coordsVal[3] = y1;
    coordsVal[4] = x1 + RATIO*cos((PI/180.0)*ANGLE/2.0)*(x1-x0) + RATIO*sin((PI/180.0)*ANGLE/2.0)*(y1-y0);
    coordsVal[5] = y1 + RATIO*cos((PI/180.0)*ANGLE/2.0)*(y1-y0) - RATIO*sin((PI/180.0)*ANGLE/2.0)*(x1-x0);
    
    
    // Enter V-shape vertices into sequel object.
    for (i=0; i < 6; i++) seq.coords.push_back(coordsVal[i]);
    
    // Specify both sub-branch segments of the V-shape for recursive production.
    for (i=0; i < 2; i++)
    {
        coordsVal1[0] = coordsVal[2];
        coordsVal1[1] = coordsVal[3];
        for (j=2; j < 4; j++)
        {
            coordsVal1[j] = coordsVal[4*i+j-2];
        }
        s = *new Source(coordsVal1);
        seq.v.push_back(s);
    }
    return seq;
}

// Routine to draw leaf called by the following routine to produce tree.
void drawLeaf(float x, float y)
{
    //changed for lighting
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    //changed so leaves won't keep randomizing
    glRotatef(randang, 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 2.0);
    glVertex2f(0.0, 4.0);
    glVertex2f(-1.0, 2.0);
    glEnd();
    glPopMatrix();
}

// Recursive routine to produce tree.
void Source::produceTree(int level)
{
    //glColor3f(0.4, 0.5, 0.5);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tree);
    
    // Branches are thinner up the tree.
    glLineWidth(2*(maxLevel - level));
    
    // Source and sequels at all prior levels are drawn (different from Kock and Koch variant).
    if (maxLevel == 0) this->drawT();
    else if (maxLevel == 1) {this->drawT(); this->sourceToSequelTree().drawTree();}
    else if (level < maxLevel)
    {
        if (level == 0) this->drawT();
        this->sourceToSequelTree().drawTree();
        for (int i=0; i < 2; i++) this->sourceToSequelTree().v[i].produceTree(level+1);
    }
    
    // Embellish with leaves.
    if (level == maxLevel-1)
    {
        drawLeaf(this->sourceToSequelTree().coords[0], this->sourceToSequelTree().coords[1]);
        drawLeaf(this->sourceToSequelTree().coords[4], this->sourceToSequelTree().coords[5]);
    }
    
    // Restore line width.
    glLineWidth(1.0);
}

// Routine to draw source line segment.
void Source::drawT()
{
    //changed to make trunk 3d, looks weird but i kinda like it??
    glPushMatrix();
    glTranslatef(coords[0], coords[1], 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(cyl, 5.0, 10.0, coords[3], 30.0, 30.0);
    glPopMatrix();
}

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

void writeStrokeString(void *font, const char *string)
{
    const char *c;
    for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

void loadExternalTextures()
{
    // Local storage for bmp image data.
    BitMapFile *image[3];
    
    // Load the texture.
    image[0] = getBMPData("Textures/rocks.bmp");
    image[1] = getBMPData("Textures/swirl.bmp");
    image[2] = getBMPData("Textures/triangles.bmp");
    
    // Activate texture index texture[0].
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
    
    // Activate texture index texture[1].
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);
    
    // Activate texture index texture[2].
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2]->sizeX, image[2]->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image[2]->data);
    
}

// Process hit buffer to find record with smallest min-z value.
void findClosestHit(int hits, unsigned int buffer[])
{
    unsigned int *ptr, minZ;
    
    minZ= 0xffffffff; // 2^32 - 1
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

//clip cube for base of ramp
void baseCube(void){
    glClipPlane(GL_CLIP_PLANE0, clip0);
    glPushMatrix();
    glEnable(GL_CLIP_PLANE0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glutSolidCube(100);
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
}

//draw skateboard with chosen texture
void skateboard(void){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, reset);
    glColor3f(0.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[pick]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    
    glPushMatrix();
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_2D);
    
    //move with eye, but below the eye
    glTranslatef(gx + sin(angle*PI/180), gy-35.0, gz + cos(angle*PI/180));
    if (!s) glRotatef(rot, 1.0, 0.0, 0.0);
    if (world != WATER) glRotatef(angle, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glScalef(1.5, 1.0, 2.5);
    
    glutSolidSphere(20, 40, 40);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glutPostRedisplay();
}

void pipe(void){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
    glPushMatrix();
    glTranslatef(130.0, -16.0, -250.0);
    gluCylinder(cyl, 2.0, 2.0, 100.0, 10, 16);
    
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(cyl, 2.0, 2.0, 30.0, 10, 16);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, 100.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluCylinder(cyl, 2.0, 2.0, 30.0, 10, 16);
    glPopMatrix();
    
    glPopMatrix();
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
}

void standardRamp(void){
    int i, j;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pink);
    glBegin(GL_POINTS);
    for (i = 0; i < 6; i++)
        for (j = 0; j < 4; j++)
            glVertex3fv(controlPoints[i][j]);
    glEnd();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (i = 0; i < 5; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j < 4; j++)
        {
            glVertex3fv(controlPoints[i][j]);
            glVertex3fv(controlPoints[i+1][j]);
        }
        glEnd();
    }
    
    // Specify and enable the Bezier surface.
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, controlPoints[0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    
    // Draw the Bezier surface using a mesh approximation.
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, crimson);
    //glColor3f(0.862745, 0.0784314, 0.235294);
    glPushMatrix();
    glTranslatef(10.0, -30.0, 0.0);
    glRotatef(-45.0, 0.0, 0.0, 1.0);
    glScalef(1.5, 0.6, 1.2);
    baseCube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-10.0, -30.0, 0.0);
    glRotatef(225.0, 0.0, 0.0, 1.0);
    glScalef(1.5, 0.6, 1.2);
    baseCube();
    glPopMatrix();
}

void wavyRamp(void){
    int i,j;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, aquamarine);
    glBegin(GL_POINTS);
    for (i = 0; i < 12; i++)
        for (j = 0; j < 4; j++)
            glVertex3fv(wavePoints[i][j]);
    glEnd();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (i = 0; i < 11; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j < 4; j++)
        {
            glVertex3fv(wavePoints[i][j]);
            glVertex3fv(wavePoints[i+1][j]);
        }
        glEnd();
    }
    
    // Specify and enable the Bezier surface.
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 12, wavePoints[0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    
    // Draw the Bezier surface using a mesh approximation.
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, turquoise);
    //glColor3f(0.282353, 0.819608, 0.8);
    glPushMatrix();
    glTranslatef(10.0, -30.0, 210.0);
    glRotatef(-45.0, 0.0, 0.0, 1.0);
    glScalef(1.2, 0.6, 0.6);
    baseCube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(90.0, -30.0, 210.0);
    glRotatef(-45.0, 0.0, 0.0, 1.0);
    glScalef(1.2, 0.6, 0.6);
    baseCube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-10.0, -30.0, 210.0);
    glRotatef(225.0, 0.0, 0.0, 1.0);
    glScalef(1.2, 0.6, 0.6);
    baseCube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(70.0, -30.0, 210.0);
    glRotatef(225.0, 0.0, 0.0, 1.0);
    glScalef(1.2, 0.6, 0.6);
    baseCube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(150.0, -30.0, 210.0);
    glRotatef(225.0, 0.0, 0.0, 1.0);
    glScalef(1.2, 0.6, 0.6);
    baseCube();
    glPopMatrix();
    
}

void simple(void){
    int i, j;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, turquoise);
    //glColor3f(0.282353, 0.819608, 0.8);
    glBegin(GL_POINTS);
    for (i = 0; i < 6; i++)
        for (j = 0; j < 4; j++)
            glVertex3fv(rampPoints[i][j]);
    glEnd();
    
    // Specify and enable the Bezier surface.
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, controlPoints[0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    
    // Draw the Bezier surface using a mesh approximation.
    // glColor3f(0.5, 0.5, 0.5);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    for (i = 0; i < 5; i++)
    {
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j < 4; j++)
        {
            glVertex3fv(rampPoints[i][j]);
            glVertex3fv(rampPoints[i+1][j]);
        }
        glEnd();
    }
    
    glPushMatrix();
    glTranslatef(-350.0, -25.0, 0.0);
    glRotatef(245.0, 0.0, 0.0, 1.0);
    glScalef(2.0, 0.6, 1.0);
    baseCube();
    glPopMatrix();
}

//haha get it??
void satTURN(int n){
    satrot +=5;
    
    glutTimerFunc(animationPeriod, satTURN, 1);
    glutPostRedisplay();
}

//classic skate
void skate(int n){
    if (s && world == CLASSIC){
        //pipe
        if (gz <= -100 && gz >= -300 && gx <= 180 && gx >= 80){
            gy = 20.0;
            gx = 130.0;
            rot = -60.0;
            gz+=5;
            ylook = gy;
        }
        //standardRamp
        else if (gz <= 100 && gz >= -80 && gx <= 80 && gx >= -80) {
            if (enter == 0) {
                gy = 60;
                gx = -40;
                gz = 65;
                rot = 120.0;
                enter++;
            }
            
            if (gx >= 0 && gy <= 40 && gz <= 0){
                gy++;
            }
            else gy--;
            
            gx++;
            gz--;
            
        }
        //wavyRamp
        else if (gz <= 280 && gz >= 140 && gx <= 200 && gx >= -60) {
            if (enter == 0) {
                gy = 40;
                gx = -40;
                gz = 250;
                rot = 60.0;
                ylook=20;
                enter++;
            }
            
            if (gx >= 0 && gy <= 50){
                gy++;
                gz++;
            }
            else if (gx >= 80 && gy <= 50 ){
                gy++;
                gz++;
            }
            else if (gx >= 160 && gy <= 50){
                gy++;
                gz++;
            }
            else gy--;
            
            gx++;
            gz--;
        }
        //simple
        else if (gz <= 80 && gz >= -80 && gx <= -240 && gx >= -420) {
            //ylook = gy;
            if (enter == 0) {
                gy = 0.0;
                gx = -280;
                gz = -30;
                rot = 60.0;
                enter++;
                ylook=gy;
            }
            if ( gx >= -400 && gy <= 30){
                gy+=5;
                gx-=5;
            }
            else {
                gy-=5;
                gx+=5;
            }
            
            gx--;
            gz+=5;
        }
        else s = !s;
    }
    else if (!s) {
        gy = 0.0;
        rot = 0.0;
        enter = 0.0;
        ylook = 0.0;
    }
   
    glutTimerFunc(animationPeriod, skate, 1);
    glutPostRedisplay();
}

void spaceSkate(int n){
    
    if (s && world == SPACE){
        //pipe
        if (gz <= -100 && gz >= -300 && gx <= 180 && gx >= 80){
            gy = 20.0;
            gx = 130.0;
            rot = -60.0;
            gz+=5;
            ylook = gy;
        }
        //standardRamp
        else if (gz <= 100 && gz >= -80 && gx <= 80 && gx >= -80) {
            if (enter == 0) {
                gy = 60;
                gx = -40;
                gz = 65;
                rot = 120.0;
                enter++;
                ylook = 20.0;
            }
            
            if (gx >= 0 && gy <= 40 && gz <= 0){
                gy++;
            }
            else gy--;
            
            gx++;
            gz--;
            
        }
        //wavyRamp
        else if (gz <= 280 && gz >= 140 && gx <= 200 && gx >= -60) {
            if (enter == 0) {
                gy = 40;
                gx = -40;
                gz = 250;
                rot = 60.0;
                ylook=20;
                enter++;
            }
            
            if (gx >= 0 && gy <= 50){
                gz++;
            }
            else if (gx >= 80 && gy <= 50 ){
                gz++;
            }
            else if (gx >= 160 && gy <= 50){
                gz++;
            }
            gy++;
            gx++;
            gz--;
        }
        //simple
        else if (gz <= 80 && gz >= -80 && gx <= -240 && gx >= -420) {
            //ylook = gy;
            if (enter == 0) {
                gy = 0.0;
                gx = -280;
                gz = -30;
                rot = 60.0;
                enter++;
                ylook=gy;
            }
            if ( gx >= -400 && gy <= 30){
                gx-=5;
            }
            else {
                gx+=5;
            }
            
            gy+=5;
            gx--;
            gz+=5;
        }
        else s = !s;
    }
    else if (!s) {
        gy = 0.0;
        rot = 0.0;
        enter = 0.0;
        ylook = 0.0;
    }
    
    glutTimerFunc(animationPeriod, spaceSkate, 1);
    glutPostRedisplay();
    
}

//slower skateboarding in water
void waterskate(int n){
    if (s && world == WATER){
        //pipe
        if (gz <= -100 && gz >= -300 && gx <= 180 && gx >= 80){
            gy = 20.0;
            gx = 130.0;
            rot = -60.0;
            gz++;
            ylook = gy;
        }
        //standardRamp
        else if (gz <= 100 && gz >= -80 && gx <= 80 && gx >= -80) {
            if (enter == 0) {
                gy = 60;
                gx = -40;
                gz = 65;
                rot = 120.0;
                enter++;
            }
            
            if (gx >= 0 && gy <= 40 && gz <= 0){
                gy+=0.5;
            }
            else gy-=0.5;
            
            gx+=0.5;
            gz-=0.5;
            
        }
        //wavyRamp
        else if (gz <= 280 && gz >= 140 && gx <= 200 && gx >= -60) {
            if (enter == 0) {
                gy = 40;
                gx = -40;
                gz = 250;
                rot = 60.0;
                enter++;
                ylook = 20.0;
            }
            
            if (gx >= 0 && gy <= 50){
                gy+=0.5;
                gz+=0.5;
            }
            else if (gx >= 80 && gy <= 50 ){
                gy+=0.5;
                gz+=0.5;
            }
            else if (gx >= 160 && gy <= 50){
                gy+=0.5;
                gz+=0.5;
            }
            else gy-=0.5;
            
            gx+=0.5;
            gz-=0.5;
        }
        //simple
        else if (gz <= 80 && gz >= -80 && gx <= -240 && gx >= -420) {
            if (enter == 0) {
                gy = 0.0;
                gx = -280;
                gz = -60;
                rot = 60.0;
                enter++;
            }
            if ( gy <= 40){
                gy+=0.5;
                gx-=0.5;
            }
            else {
                gy-=0.5;
                gx+=0.5;
            }
            gz+=0.5;
        }
        else s = !s;
    }
    else if (!s) {
        gy = 0.0;
        rot = 0.0;
        enter = 0.0;
    }
    
    glutTimerFunc(animationPeriod, waterskate, 1);
    glutPostRedisplay();
}

//tried to do collision
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
                             float x2, float y2, float z2, float r2){
    return ( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) <= (r1+r2)*(r1+r2) );
}

bool collide(float x, float y, float z){
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 4; j++ ){
            if (checkSpheresIntersection(x, y, z, 10.0, controlPoints[i][j][0], controlPoints[i][j][1], controlPoints[i][j][2], 30.0)) return true;
        }
    }
    
    for (int i = 0; i < 12; i++){
        for (int j = 0; j < 4; j++ ){
            if (checkSpheresIntersection(x, y, z, 10.0, wavePoints[i][j][0], wavePoints[i][j][1], wavePoints[i][j][2], 20.0)) return true;
        }
    }
    
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 4; j++ ){
            if (checkSpheresIntersection(x, y, z, 10.0, rampPoints[i][j][0], rampPoints[i][j][1], rampPoints[i][j][2],20.0)) return true;
        }
    }
    
    //these don't work at all for some reason, they force me to stop in the middle of parts of the cube
    if (checkSpheresIntersection(x, y, z, 10.0, -600, 0.0, -600, 35.0)) return true;
    if (checkSpheresIntersection(x, y, z, 10.0, 600, 0.0, -600, 35.0)) return true;
    if (checkSpheresIntersection(x, y, z, 10.0, -600, 0.0, 600, 35.0)) return true;
    if (checkSpheresIntersection(x, y, z, 10.0, 600, 0.0, 600, 35.0)) return true;
    return false;
}

//for space
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
    //glColor3f(0.5, 0.2, 0.2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, jcolor1);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
    //glColor3f(1.0, 0.854902, 0.72549);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, jcolor2);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    //this happened by accident but i like the texture it creates
    glEnable(GL_CLIP_PLANE2);
    //glColor3f(0.956863, 0.643137, 0.376471);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, jcolor3);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE2);
    glEnable(GL_CLIP_PLANE3);
    glEnable(GL_CLIP_PLANE4);
    glColor3f(0.5, 0.2, 0.2);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE3);
    glDisable(GL_CLIP_PLANE4);
    glEnable(GL_CLIP_PLANE4);
    //glColor3f(0.956863, 0.643137, 0.376471);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE4);
    glEnable(GL_CLIP_PLANE5);
    //glColor3f(0.956863, 0.643137, 0.376471);
    glutSolidSphere(80.0, 40, 40);
    glDisable(GL_CLIP_PLANE5);
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();
}
//for space
void drawSaturn(){
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    glTranslatef(300.0, 200.0, 100.0);
    //glColor3f(0.5, 1.0, 1.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sat1);
    glutSolidSphere(40.0, 30, 30);
    glPopMatrix();
    glPushMatrix();
    //glColor3f(1.0, 0.752941, 0.796078);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sat2);
    glTranslatef(300.0, 200.0, 100.0);
    glRotatef(satrot, 1.0, 0.0, 0.0);
    glutWireTorus(10.0, 65.0, 15, 15);
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
}

//space world
void space(void){
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globaldark);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    //move normally
    step = 5.0, turn = 10.0;
    
    //move around
    gluLookAt(gx, gy, gz,
              gx + sin(angle*PI/180), ylook, gz + cos(angle*PI/180),
              0.0, 1.0, 0.0);
    
    if (fog) glEnable(GL_FOG);
    glHint(GL_FOG_HINT, GL_NICEST);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_DENSITY, 0.5);
    glFogf(GL_FOG_START, -100.0);
    glFogf(GL_FOG_END, 200.0);
    glFogfv(GL_FOG_COLOR, grey);
    
    glPushMatrix();
    //glColor3f(0.2, 0.2, 0.2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glTranslatef(0.0, -500.0, 0.0);
    glutSolidCube(900);
    glPopMatrix();
    
    drawJupiter();
    drawSaturn();
    pipe();
    simple();
    standardRamp();
    wavyRamp();
    skateboard();
    glDisable(GL_FOG);
   
}

//water world
void water(void){
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globaldark);
    glEnable(GL_BLEND); // Enable blending.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
    glColor3f(1.0, 1.0, 1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    //move slower in water
    step = 1.0, turn = 1.0;
    
    //move around
    gluLookAt(gx, gy, gz,
              gx + sin(angle*PI/180), ylook, gz + cos(angle*PI/180),
              0.0, 1.0, 0.0);
    
    //aqumarine fog for an underwater effect
    glEnable(GL_FOG);
    glHint(GL_FOG_HINT, GL_NICEST);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_DENSITY, 0.02);
    glFogf(GL_FOG_START, -100.0);
    glFogf(GL_FOG_END, 200.0);
    glFogfv(GL_FOG_COLOR, aquamarine);
    
    glPushMatrix();
    glColor3f(0.2, 0.2, 0.2);
    glTranslatef(0.0, -500.0, 0.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, turquoise);
    glutSolidCube(900);
    glPopMatrix();

    glutSolidCube(900.0);
    
    skateboard();
    pipe();
    simple();
    standardRamp();
    wavyRamp();
    
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 0.1);
    glPushMatrix();
    glTranslatef(0.0, -45.0, 0.0);
    glScalef(1.0, 0.0, 1.0);
    pipe();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-20.0, -49.0, 10.0);
    glScalef(1.0, 0.0, 1.0);
    standardRamp();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-20.0, -49.0, 10.0);
    glScalef(1.0, 0.0, 1.0);
    wavyRamp();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-10.0, -49.0, 10.0);
    glScalef(1.0, 0.0, 1.0);
    simple();
    glPopMatrix();
    glDisable(GL_FOG);
    
    glutPostRedisplay();
    
}

//classic world
void classic(void){
    glLoadIdentity();
    //bunch of lighting things
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global);
    
    //reset colors
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, reset);
    glColor3f(1.0, 1.0, 1.0);
   
    //move normally
    step = 5.0, turn = 10.0;
    
    //move around
    gluLookAt(gx, gy, gz,
              gx + sin(angle*PI/180), ylook, gz + cos(angle*PI/180),
              0.0, 1.0, 0.0);
    
    float tree1[] = {-150.0, -10.0, -150.0, 30.0}, tree2[] = {300.0, -10, 300.0, 30.0}, tree3[] = {-200, -10.0, -200.0, 30.0};
    Source src1 = *new Source(tree1);
    Source src2 = *new Source(tree2);
    Source src3 = *new Source(tree3);
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, -300.0);
    src1.produceTree(0);
    glPopMatrix();
    
    src2.produceTree(0);
    
    glPushMatrix();
    glTranslatef(0.0, 0.0, 200.0);
    src3.produceTree(0);
    glPopMatrix();
    
    pipe();
    
    skateboard();
   
    //halfpipe ramp
    standardRamp();
    
    //wavy ramp
    wavyRamp();
    
    //simple ramp
    simple();
    
    //disable lighting for shadows
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0, -45.0, 0.0);
    glScalef(1.0, 0.0, 1.0);
    pipe();
    glPopMatrix();
    
    glColor3f(0.527451, 0.221569, 0.076471);
    glPushMatrix();
    glTranslatef(0.0, -50.0, -300.0);
    glScalef(1.0, 0.0, 1.0);
    src1.produceTree(0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, -50.0, 0.0);
    glScalef(1.0, 0.0, 1.0);
    src2.produceTree(0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, -50.0, 200.0);
    glScalef(1.0, 0.0, 1.0);
    src3.produceTree(0);
    glPopMatrix();
    
    glColor3f(0.815686, 0.12549, 0.564706);
    glPushMatrix();
    glTranslatef(-20.0, -49.0, 10.0);
    glScalef(1.0, 0.0, 1.0);
    standardRamp();
    glPopMatrix();
    
    glColor3f(0.27451, 0.509804, 0.705882);
    glPushMatrix();
    glTranslatef(-20.0, -49.0, 10.0);
    glScalef(1.0, 0.0, 1.0);
    wavyRamp();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-10.0, -49.0, 10.0);
    glScalef(1.0, 0.0, 1.0);
    simple();
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
    
    //grass
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
    glPushMatrix();
    glTranslatef(0.0, -500.0, 0.0);
    glutSolidCube(900);
    glPopMatrix();
    
    //sky
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sky);
    glPushMatrix();
    glutSolidSphere(600, 200, 200);
    glPopMatrix();
    
    //sun
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, sun);
    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glow);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gold);
    glTranslatef(0.0, 400.0, 0.0);
    glutSolidSphere(50.0, 20.0, 20.0);
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, unglow);
    glDisable(GL_DEPTH_TEST);

    //reset material colors
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, reset);
    glutPostRedisplay();
}

void chooseWorld(void){
    //pink box
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(1.0, 0.713725, 0.756863);
    glBegin(GL_POLYGON);
    glVertex3f(-80.0, 60.0, -50.0);
    glVertex3f(80.0, 60.0, -50.0);
    glVertex3f(80.0, 40.0, -50.0);
    glVertex3f(-80.0, 40.0, -50.0);
    glEnd();
    
    //start screen text
    glLineWidth(2.0);
    glColor3f(0.545098, 0.0, 0.545098);
    glPushMatrix();
    glTranslatef(-28.0, 20.0, -20.0);
    glScalef(0.015, 0.015, 0.015);
    writeStrokeString(GLUT_STROKE_MONO_ROMAN, "Where would you like to skateboard?");
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-6.0, 3.0, -20.0);
    glScalef(0.015, 0.015, 0.015);
    writeStrokeString(GLUT_STROKE_MONO_ROMAN, "Classic");
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-4.0, -9.0, -20.0);
    glScalef(0.015, 0.015, 0.015);
    writeStrokeString(GLUT_STROKE_MONO_ROMAN, "Space");
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-4.0, -21.0, -20.0);
    glScalef(0.015, 0.015, 0.015);
    writeStrokeString(GLUT_STROKE_MONO_ROMAN, "Water");
    glPopMatrix();
    
    //choices
    glColor3f(0.98, 0.9, 0.878431);
    if (isSelecting) glLoadName(4);
    glBegin(GL_POLYGON);
    glVertex3f(-40.0, 20.0, -50.0);
    glVertex3f(40.0, 20.0, -50.0);
    glVertex3f(40.0, 0.0, -50.0);
    glVertex3f(-40.0, 0.0, -50.0);
    glEnd();
    
    if (isSelecting) glLoadName(5);
    glBegin(GL_POLYGON);
    glVertex3f(-40.0, -10.0, -50.0);
    glVertex3f(40.0, -10.0, -50.0);
    glVertex3f(40.0, -30.0, -50.0);
    glVertex3f(-40.0, -30.0, -50.0);
    glEnd();
    
    if (isSelecting) glLoadName(6);
    glBegin(GL_POLYGON);
    glVertex3f(-40.0, -40.0, -50.0);
    glVertex3f(40.0, -40.0, -50.0);
    glVertex3f(40.0, -60.0, -50.0);
    glVertex3f(-40.0, -60.0, -50.0);
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
    glutPostRedisplay();
}

void start(void){
    glDisable(GL_LIGHTING);
    //pink box
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(1.0, 0.713725, 0.756863);
    glBegin(GL_POLYGON);
    glVertex3f(-80.0, 60.0, -50.0);
    glVertex3f(80.0, 60.0, -50.0);
    glVertex3f(80.0, 40.0, -50.0);
    glVertex3f(-80.0, 40.0, -50.0);
    glEnd();
    
    //start screen text
    glLineWidth(2.0);
    glColor3f(0.545098, 0.0, 0.545098);
    glPushMatrix();
    glTranslatef(-25.0, 20.0, -20.0);
    glScalef(0.015, 0.015, 0.015);
    writeStrokeString(GLUT_STROKE_MONO_ROMAN, "To begin, please pick a board.");
    glPopMatrix();
    glColor3f(1.0, 1.0, 1.0);
    
    //boards with different textures
    glPushMatrix();
    glTranslatef(-40.0, -20.0, -50.0);
    glScalef(0.5, 2.0, 0.5);
    
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_2D);
    if (isSelecting) glLoadName(1);
    glutSolidSphere(20, 40, 40);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    
    glPushMatrix();
    glTranslatef(0.0, -20.0, -50.0);
    glScalef(0.5, 2.0, 0.5);
    if (isSelecting) glLoadName(2);
    glutSolidSphere(20, 40, 40);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    
    glPushMatrix();
    glTranslatef(40.0, -20.0, -50.0);
    glScalef(0.5, 2.0, 0.5);
    if (isSelecting) glLoadName(3);
    glutSolidSphere(20, 40, 40);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    glutPostRedisplay();
}

void draw(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    if (!choose && init) start();
    else if (choose) chooseWorld();
    
    if (!choose && !init){
    if (world == 0) classic();
    else if (world == 1) space();
    else if (world == 2) water();
    }
    glutSwapBuffers();
}

void drawScene(void){
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    isSelecting = 0;
    draw();
}

//picking and selecting!!!
void pickFunction(int button, int state, int x, int y)
{
    int viewport[4]; // Viewport data.
    
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return; // Don't react unless left button is pressed.
    
    glGetIntegerv(GL_VIEWPORT, viewport); // Get viewport data.
    
    glSelectBuffer(1024, buffer); // Specify buffer to write hit records in selection mode
    (void) glRenderMode(GL_SELECT); // Enter selection mode.
    
    // Save the viewing volume defined in the resize routine.
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    // Define a viewing volume corresponding to selecting in 3 x 3 region around the cursor.
    glLoadIdentity();
    gluPickMatrix((float)x, (float)(viewport[3] - y), 3.0, 3.0, viewport);
    gluPerspective(110,1,5,1000); // Copied from the reshape routine.
    
    glMatrixMode(GL_MODELVIEW); // Return to modelview mode before drawing.
    glLoadIdentity();
    
    glInitNames(); // Initializes the name stack to empty.
    glPushName(0); // Puts name 0 on top of stack.
    
    // Determine hits by calling drawBallAndTorus() so that names are assigned.
    isSelecting = 1;
    draw();
    
    hits = glRenderMode(GL_RENDER); // Return to rendering mode, returning number of hits.
    
    // Restore viewing volume of the resize routine and return to modelview mode.
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    // Determine closest of the hit objects (if any).
    findClosestHit(hits, buffer);
    
    if (closestName == 1) {
        pick = 0;
        choose = true;
        init = false;
    }
    else if (closestName == 2) {
        pick = 1;
        choose = true;
        init = false;
    }
    else if (closestName == 3) {
        pick = 2;
        choose = true;
        init = false;
    }
    else if (closestName == 4) {
        world = 0;
        choose = false;
    }
    else if (closestName == 5) {
        world = 1;
        choose = false;
    }
    else if (closestName == 6) {
        world = 2;
        choose = false;
    }
    
    glutPostRedisplay();
}

void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    
    // Create texture index array.
    glGenTextures(3, texture);
    
    // Load external textures.
    loadExternalTextures();
    
    // Turn on OpenGL texturing.
    glEnable(GL_TEXTURE_2D);
    
    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
}

//change board texture
void boardMenu(int n){
    pick = n;
    glutPostRedisplay();
}

//change the size of the moon
void worldMenu(int n){
    if (n == 3) world = 0;
    else if (n == 4) world = 1;
    else if (n == 5) world = 2;
}

void top_menu(int n)
{
    if (n==1) exit(0);
    else if (n==2){
        init = true;
        choose = false;
    }
}

//menu on right click
void makeMenu(void)
{
    int sub_menu1;
    sub_menu1 = glutCreateMenu(boardMenu);
    glutAddMenuEntry("Rocks", 0);
    glutAddMenuEntry("Waves", 1);
    glutAddMenuEntry("Triangles", 2);
    
    int sub_menu2;
    sub_menu2 = glutCreateMenu(worldMenu);
    glutAddMenuEntry("Classic", 3);
    glutAddMenuEntry("Space", 4);
    glutAddMenuEntry("Water", 5);
    
    glutCreateMenu(top_menu);
    glutAddSubMenu("Change Board", sub_menu1);
    glutAddSubMenu("Change World", sub_menu2);
    glutAddMenuEntry("Quit",1);
    glutAddMenuEntry("Restart",2);
    
    // The menu is attached to a mouse button.
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(110,1,5,1000);
    
    glMatrixMode(GL_MODELVIEW);
}

void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 's':
            s = !s;
            break;
        case '1':
            pick = 0;
            init = false;
            choose = true;
            break;
        case '2':
            pick = 1;
            init = false;
            choose = true;
            break;
        case '3':
            pick = 2;
            init = false;
            choose = true;
            break;
        case '4':
            world = 0;
            choose = false;
            break;
        case '5':
            world = 1;
            choose = false;
            break;
        case '6':
            world = 2;
            choose = false;
            break;
        case 'y':
            if (world == WATER) ylook -=0.01;
            else ylook -= 0.5;
            break;
        case 'Y':
            if (world == WATER) ylook +=0.01;
            else ylook += 0.5;
            break;
        case 'f':
            fog = !fog;
            break;
    }
    glutPostRedisplay();
}

void specialKeyInput(int key, int x, int y)
{
    switch(key){
        case GLUT_KEY_UP: //forward
            //if (!collide((gz+10)+step*cos(angle*PI/180), 0.0, gx+step*sin(angle*PI/180))){
            gz+=step*cos(angle*PI/180);
            gx+=step*sin(angle*PI/180);
            //}
            break;
        case GLUT_KEY_DOWN: //back
            //if (!collide((gz+10)-step*cos(angle*PI/180), 0.0, gx-step*sin(angle*PI/180))){
            gz-=step*cos(angle*PI/180);
            gx-=step*sin(angle*PI/180);
            //}
            break;
        case GLUT_KEY_RIGHT: //turn right
            angle -=turn;
            gx--;
            break;
        case GLUT_KEY_LEFT: //turn left
            angle +=turn;
            gx++;
            break;
    }//end switch
    glutPostRedisplay();
}

void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press the up arrow key to move forward and the down arrow key to move backwards." << endl;
    cout << "Press the left arrow key to turn left, the right arrow key to turn right." << endl;
    cout << "Press 'y' to look down and 'Y' to look up." << endl;
    cout << "Press 'f' to toggle fog on or off in Space world." << endl;
     cout << "Press 's' to skate on a nearby object." << endl;
    cout << "Press '1', '2', or '3' to pick a texture for the skateboard. Conversely, click on one from the start screen. 1: Rocks, 2: Waves, 3: Triangles." << endl;
    cout << "Press '4', '5', or '6' to choose a world to skateboard in. Conversely, click on one from the choose screen. 4: Classic, 5: Space, 6: Water." << endl;
    cout << "Press the escape key to quit." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("Skateboarding");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(pickFunction);
    glutSpecialFunc(specialKeyInput);
    glutTimerFunc(5, skate, 1);
    glutTimerFunc(5, waterskate, 1);
    glutTimerFunc(5, satTURN, 1);
    glutTimerFunc(5, spaceSkate, 1);
    makeMenu();
    glutMainLoop();
    
    return 0;
}
