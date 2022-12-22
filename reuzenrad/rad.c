/* Reuzenrad made by Aaron Van Campenhout - Starting date: 7/12/22 */

#include <gl/glut.h>
#include <stdio.h>
#include <math.h>

// Tekenen
GLint winWidth = 1000, winHeight = 1000; /* 800*800 pixels */
int xmax = 5, ymax = 5, xmin = -5, ymin = -5;
float near = 0.5, far = 400;
GLdouble xc = 4, yc = 4, zc = 5;
GLdouble vlak[4] = { 0,1,0,1 };
char projectie = 'p';

float offsetX = 0, offsetY = 0, offsetZ = 0; // Offset dient om structuren te bewegen

int cabines = 6;
int rads = 1;
int draadmodel = 0;
int eenvoudig = 1;
int toonctp = 0;
int teken = 0;
int toonassen = 1;
int toonlightpunten = 0;
int flat = 0;

// anitmatie
float draai = 0;
float draaisnelheid = 0.25;
float hoek = 0;
float wiebelhoek = 0;

// Afmetingen
GLfloat ctrPt[6][4][3] =
{
   { {0.0, 0.1, 0.00}, {0.1, 0.1, 0.00}, {0.08, 0.1, 0.00}, {0.12, 0.1, 0.00}},
   { {0.0, 0.0, 0.1}, {0.1, 0.0, 0.1}, {0.08, 0.0, 0.1}, {0.12, 0.1, 0.1}},
   { {0.0, 0.0, 0.08}, {0.1, 0.0, 0.08}, {0.08, 0.0, 0.08}, {0.12, 0.1, 0.08}},
   { {0.0, 0.0, 0.12}, {0.1, 0.0, 0.12}, {0.08, 0.0, 0.12}, {0.12, 0.1, 0.12}},
   { {0.0, 0.0, 0.18}, {0.1, 0.0, 0.18}, {0.08, 0.0, 0.18}, {0.12, 0.1, 0.18}},
   { {0.0, 0.1, 0.20}, {0.1, 0.1, 0.20}, {0.08, 0.1, 0.20}, {0.12, 0.1, 0.20}}
};

GLUnurbsObj* dakelement;
GLfloat bsplineDak[4][4][3];    //b-spline

// Materiaal
const GLfloat KUIPJE_GRIJS_AMBI[] = { 0.22,0.22,0.22 };
const GLfloat KUIPJE_GRIJS_DIFF[] = { 0.33,0.33,0.33 };
const GLfloat KUIPJE_GRIJS_SPEC[] = { 0.11,0.11,0.11 };
const GLfloat KUIPJE_WIT_AMBI[] = { 0.66,0.66,0.66 };
const GLfloat KUIPJE_WIT_DIFF[] = { 0.77,0.77,0.77 };
const GLfloat KUIPJE_WIT_SPEC[] = { 0.55,0.55,0.55 };

const GLfloat STANG_CHROOM_AMBI[] = { 0.46,0.58,0.35 };
const GLfloat STANG_CHROOM_DIFF[] = { 0.23,0.29,0.17 };
const GLfloat STANG_CHROOM_SPEC[] = { 0.69,0.87,0.52 };
const GLfloat STANG_BRONS_AMBI[] = { 0.21,0.13,0.10 };
const GLfloat STANG_BRONS_DIFF[] = { 0.39,0.27,0.17 };
const GLfloat STANG_BRONS_SPEC[] = { 0.71,0.43,0.18 };

const GLfloat DAK_GEEL_AMBI[] = { 0.65,0.55,0.15 };
const GLfloat DAK_GEEL_DIFF[] = { 0.75,0.45,0.15 };
const GLfloat DAK_GEEL_SPEC[] = { 0.85,0.35,0.15 };
const GLfloat DAK_LILA_AMBI[] = { 0.45,0.15,0.75 };
const GLfloat DAK_LILA_DIFF[] = { 0.55,0.15,0.65 };
const GLfloat DAK_LILA_SPEC[] = { 0.35,0.15,0.85 };

// Licht en kleuren

const GLfloat LICHTPOS1[] = { 0.0, 0.0, 0.0, 0.0 };	// Achteraan reuzenrad (oorsprong)
const GLfloat LICHTPOS2[] = { 5.0, 2, 5.0, 0.0 }; // Vooraan loodrecht op reuzenrad, achterkant van het rad = donker
const GLfloat LICHTPOS3[] = { 2.5, 0.0, 2.5, 0.0 }; // onderaan reuzenrad
GLfloat LICHTPOS4[] = { 5, 0.0, 5, 0.0 }; // vooraaan beweegbare hoogte

GLfloat ZWART[] = { 0,0,0 };
GLfloat GRIJS[] = { 0.3,0.4,0.5,1.0 };
GLfloat WIT[] = { 1.0,1.0,1.0,1.0 };
GLfloat ROOD[] = { 0.9,0.1,0.0,1.0 };
GLfloat GEEL[] = { 1,1,0,1 };
GLfloat ROODBLAUW[] = { 0.9,0.0,0.9,1.0 };
GLfloat GROEN[] = { 0.0,1.0,0.0,1.0 };
GLfloat GRBLAUW[] = { 0.0,0.8,0.9,1.0 };
GLfloat ROGBRL[] = { 0.4,0.6,0.8,1.0 };
GLfloat direc[] = { 0.0,-1.0,0.0 };
GLfloat shini = 60.0;
GLfloat expo = 5.0;
GLfloat lichthoek = 35;

int materiaalstang = 0; // Kiezen tussen brons en chroom
int materiaalkuip = 0;
int materiaaldak = 0;

void materiaal_menu(int id)
{
	switch (id)
	{
	case 1: break;
	case 2:	materiaalkuip = 1;    break;
	case 3:	materiaalkuip = 0;    break;
	case 4: break;
	case 5:	materiaalstang = 1;   break;
	case 6:	materiaalstang = 0;   break;
	case 7: break;
	case 8:	materiaaldak = 1;     break;
	case 9:	materiaaldak = 0;     break;
	case 10: exit(0);        			break;
	default: printf("default"); break;
		
	}
	glutPostRedisplay();
}


void init(void)
{
	glClearColor(0.8, 0.8, 0.8, 0);
	glEnable(GL_DEPTH_TEST);

	// Materiaal menu
	GLint kuipMenu = glutCreateMenu(materiaal_menu);
	glutAddMenuEntry("grijs", 2);
	glutAddMenuEntry("witachtig", 3);
	GLint stangMenu = glutCreateMenu(materiaal_menu);
	glutAddMenuEntry("chroom", 5);
	glutAddMenuEntry("brons", 6);
	GLint dakMenu = glutCreateMenu(materiaal_menu);
	glutAddMenuEntry("geel", 8);
	glutAddMenuEntry("lila", 9);
	glutCreateMenu(materiaal_menu);
	glutAddSubMenu("kuip", kuipMenu);
	glutAddSubMenu("Stangen", stangMenu);
	glutAddSubMenu("Dak", dakMenu);
	glutAddMenuEntry("quit", 10);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	// Licht eigenschappen
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ZWART);

	glLightfv(GL_LIGHT0, GL_SPECULAR, ZWART);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ZWART);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ZWART);

	glLightfv(GL_LIGHT1, GL_SPECULAR, ZWART);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, ZWART);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ZWART);

	glLightfv(GL_LIGHT2, GL_SPECULAR, ZWART);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, ZWART);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ZWART);

	glLightfv(GL_LIGHT3, GL_SPECULAR, ZWART);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, ZWART);
	glLightfv(GL_LIGHT3, GL_AMBIENT, ZWART);

	glLightfv(GL_LIGHT4, GL_SPECULAR, ZWART);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, ZWART);
	glLightfv(GL_LIGHT4, GL_AMBIENT, ZWART);

	glMaterialf(GL_FRONT, GL_SHININESS, 127.0);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_DST_ALPHA);

	glDepthFunc(GL_LESS);

	glEnable(GL_NORMALIZE); // Normaliseren van vectoren

}
void raam(GLint n_w, GLint n_h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	winWidth = n_w;
	winHeight = n_h;

	switch (projectie)
	{
	case 'p':
		gluPerspective(45, (GLdouble)n_w / (GLdouble)n_h, near, far); printf("Perspective\n"); break;
	case 'o':
		glOrtho(xmin, xmax, ymin, ymax, near, far);  printf("Ortho\n"); break;
	case 'f':
		glFrustum(xmin, xmax, ymin, ymax, near, far);  printf("Frustum\n"); break;
	default: break;
	}

	printf("herschaald\n");
	glViewport(0, 0, n_w, n_h);
}

void lichten(void) 
{
	glLightfv(GL_LIGHT1, GL_AMBIENT, WIT);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, GRBLAUW);
	glLightfv(GL_LIGHT3, GL_SPECULAR, ROOD);
	glLightfv(GL_LIGHT4, GL_AMBIENT, GEEL);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, GEEL);
	glLightfv(GL_LIGHT4, GL_SPECULAR, GEEL);

	glLightfv(GL_LIGHT1, GL_POSITION, LICHTPOS1);
	glLightfv(GL_LIGHT2, GL_POSITION, LICHTPOS2);
	glLightfv(GL_LIGHT3, GL_POSITION, LICHTPOS3);
	glLightfv(GL_LIGHT4, GL_POSITION, LICHTPOS4);

	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, lichthoek);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, expo);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, direc);

}

void kuip(GLfloat x, GLfloat y, GLfloat z)
{

	// Bezier kuip
	glPushMatrix();
	glTranslatef(x + 3.1, y + 3.2 , z + 6.79); 
	glRotated(wiebelhoek, 0, 0, 1); // Wiebel animatie
	glRotated(90, 0, 1, 0);
	glScaled(5, 5, 5);

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, &ctrPt[0][0][0]); // laat toe onze Bezier curve te evalueren
	glEnable(GL_MAP2_VERTEX_3); // Genereren bezier curve

	glMapGrid2f(20, 0, 1, 20, 0, 1);

	for (int i = 0; i < 2; i++)
	{
		if (draadmodel)
		{
			glEvalMesh2(GL_LINE, 0, 20, 0, 20);
		}
		else
		{
			glEvalMesh2(GL_FILL, 0, 20, 0, 20);
		}
		if (toonctp)
		{
			glPointSize(8);
			glBegin(GL_POINTS);
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					glVertex3fv(ctrPt[j][k]);
				}
			}
			glEnd();
		}


		glScaled(-1, 1, 1); // halve kuip spiegelen
	}
	glDisable(GL_MAP2_VERTEX_3);
	glPopMatrix();
	
}


void bevestiging(GLfloat x, GLfloat y, GLfloat z)
{
	if (materiaalkuip) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, KUIPJE_GRIJS_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, KUIPJE_GRIJS_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, KUIPJE_GRIJS_SPEC);
	}
	else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, KUIPJE_WIT_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, KUIPJE_WIT_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, KUIPJE_WIT_SPEC);
	}

	//Staven tekenen

	GLUquadricObj* staaf1;
	staaf1 = gluNewQuadric();

	glPushMatrix();
	glTranslatef(x+0.5 +2.6 ,y+1+2.7,z-0.075+6.29); // y = 2.7 ; z = 6.29 ; x = 2.6
	glRotated(90, -1, 0, 0);
	if (draadmodel)
	{
		gluQuadricDrawStyle(staaf1, GLU_LINE);
	}
	else 
	{
		gluQuadricDrawStyle(staaf1, GLU_FILL);
	}
	gluCylinder(staaf1, 0.02, 0.02, 0.6, 8, 8);
	glPopMatrix();
	gluDeleteQuadric(staaf1);

	GLUquadricObj* staaf2;
	staaf2 = gluNewQuadric();

	glPushMatrix();
	glTranslatef(x+1.475+2.6,y+1+2.7,z-0.075+6.29);  // y = 2.7 ; z = 6.29 ; x = 2.6
	glRotated(90, -1, 0, 0);
	if (draadmodel)
	{
		gluQuadricDrawStyle(staaf2, GLU_LINE);
	}
	else
	{
		gluQuadricDrawStyle(staaf2, GLU_FILL);
	}
	gluCylinder(staaf2, 0.02, 0.02, 0.6, 8, 8);
	glPopMatrix();
	gluDeleteQuadric(staaf2);

	GLUquadricObj* staaf3;
	staaf3 = gluNewQuadric();

	glPushMatrix();
	glTranslatef(x+1.475+2.6,y+1+2.7,z+1.075+6.29); // y = 2.7 ; z = 6.29 ; x = 2.6 
	glRotated(90, -1, 0, 0);
	if (draadmodel)
	{
		gluQuadricDrawStyle(staaf3, GLU_LINE);
	}
	else
	{
		gluQuadricDrawStyle(staaf3, GLU_FILL);
	}
	gluCylinder(staaf3, 0.02, 0.02, 0.6, 8, 8);
	glPopMatrix();
	gluDeleteQuadric(staaf3);

	GLUquadricObj* staaf4;
	staaf4 = gluNewQuadric();

	glPushMatrix();
	glTranslatef(x+0.5+2.6,y+1+2.7, z+1.075+6.29); // y = 2.7 ; z = 6.29 ; x = 2.6
	glRotated(90, -1, 0, 0);
	if (draadmodel)
	{
		gluQuadricDrawStyle(staaf4, GLU_LINE);
	}
	else
	{
		gluQuadricDrawStyle(staaf4, GLU_FILL);
	}
	gluCylinder(staaf4, 0.02, 0.02, 0.6, 8, 8);
	glPopMatrix();
	gluDeleteQuadric(staaf4);

	if (materiaalstang) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_CHROOM_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_CHROOM_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_CHROOM_SPEC);
	}
	else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_BRONS_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_BRONS_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_BRONS_SPEC);
	}

	GLUquadricObj* bevestiging;
	bevestiging = gluNewQuadric();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(x + 0.99 + 2.6, y + 2.08 +2.7, z + 0.49+6.29); // y = 2.7 ; z = 6.29 ; x = 2.6
	glRotated(90, -1, 0, 0);
	if (draadmodel)
	{
		gluQuadricDrawStyle(bevestiging, GLU_LINE);
	}
	else
	{
		gluQuadricDrawStyle(bevestiging, GLU_FILL);
	}
	gluCylinder(bevestiging, 0.03, 0.03, 0.6, 8, 8);
	glPopMatrix();
	gluDeleteQuadric(bevestiging);

}


void dak(void)
{
	if (materiaaldak) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DAK_GEEL_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DAK_GEEL_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, DAK_GEEL_SPEC);
	}
	else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DAK_LILA_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DAK_LILA_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, DAK_LILA_SPEC);
	}

	// Dak genereren B-spline
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) {
			bsplineDak[j][i][0] = i * (cos((30 * j) * 3.14 / 180)) * 0.075; 
			bsplineDak[j][i][1] = i * 0.8 * 0.075;  
			bsplineDak[j][i][2] = i * (sin((30 * j) * 3.14 / 180))* 0.075; 
		}
	}


	GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
	for (int k= 0; k < 4; k++)
	{
		glPushMatrix();
		glScalef(3.8, 3, 3.8);
		glTranslatef(0.26,0.7, 0.13); 
		dakelement = gluNewNurbsRenderer();
		glRotatef(45, 0, 1, 0);
		glRotatef(180, 0, 0, 1);
		glRotated(wiebelhoek, 0, 0, 1);
		glRotatef(k * 90, 0., 1., 0.);

		if (toonctp)
		{
			glPointSize(8.0);
			glColor3f(0, 0, 1); //blauw
			glBegin(GL_POINTS);
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++) {
					glVertex3fv(bsplineDak[i][j]);
				}
			}
			glEnd();
		}
		gluBeginSurface(dakelement);
		gluNurbsSurface(dakelement, 8, knots, 8, knots, 4 * 3, 3, &bsplineDak[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(dakelement);

		gluDeleteNurbsRenderer(dakelement);

		glPopMatrix();
	}

}

void cabine(GLfloat x, GLfloat y, GLfloat z) // samenstellen van cabine met verschillende onderdelen
{
	if (materiaalkuip) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, KUIPJE_GRIJS_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, KUIPJE_GRIJS_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, KUIPJE_GRIJS_SPEC);
	}
	else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, KUIPJE_WIT_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, KUIPJE_WIT_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, KUIPJE_WIT_SPEC);
	}

	kuip(x,y,z);
	bevestiging(x, y, z);
	glPushMatrix();
		glTranslatef(2.6+x,2.7+y,6.29+z); // y = 2.7 ; z = 6.29 ; x = 2.6
		dak();
	glPopMatrix();
}

void tekencabine(GLfloat x, GLfloat y, GLfloat z) // Scale/translate cabine naar juiste formaat 
{

	glPushMatrix();
		glScalef(0.45, 0.45, 0.45);
		cabine(2.6 + x, 2.8 + y , 6.29 + z);
	glPopMatrix();
}


void steunbalken(float offsetZ)
{
	if (materiaalstang) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_CHROOM_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_CHROOM_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_CHROOM_SPEC);
	}
	else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_BRONS_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_BRONS_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_BRONS_SPEC);
	}
	
	// Steunbalk 1 
	glPushMatrix();
	glTranslatef(4, 0.0, 4 + offsetZ);
	glRotatef(30, 0, 0, 1);
	glScalef(0.1, 4, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	// Steunbalk 2 
	glPushMatrix();
	glTranslatef(2, 0.0, 4 + offsetZ);
	glRotatef(30, 0, 0, -1);
	glScalef(0.1, 4, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	// Steunbalk 3 
	glPushMatrix();
	glTranslatef(4, 0.0, 2 + offsetZ);
	glRotatef(30, 0, 0, 1);
	glScalef(0.1, 4, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	// Steunbalk 4 
	glPushMatrix();
	glTranslatef(2, 0.0, 2 + offsetZ);
	glRotatef(30, 0, 0, -1);
	glScalef(0.1, 4, 0.1);
	glutSolidCube(1);
	glPopMatrix();


}

void as(float offsetZ)
{
	if (materiaalstang) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_CHROOM_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_CHROOM_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_CHROOM_SPEC);
	}
	else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_BRONS_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_BRONS_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_BRONS_SPEC);
	}

	GLUquadricObj* as;
	as = gluNewQuadric();

	glPushMatrix();
	
	glTranslatef(3, 1.65, 2 + offsetZ);
	if (draadmodel)
	{
		gluQuadricDrawStyle(as, GLU_LINE);
	}
	else
	{
		gluQuadricDrawStyle(as, GLU_FILL);
	}
	gluCylinder(as, 0.1, 0.1, 2, 15, 15);
	glPopMatrix();
	gluDeleteQuadric(as);
}

void as_cylinder(float Offsetz)
{
	if (materiaalstang) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_CHROOM_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_CHROOM_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_CHROOM_SPEC);
	}
	else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_BRONS_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_BRONS_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_BRONS_SPEC);
	}

	for (int i = 0; i < 2; i++)
	{
		glPushMatrix();
		glColor3f(0, 0, 0);
		glScalef(1, 1, 1);
		glTranslatef(3, 1.6, (i * 1.5) + 2.25 + Offsetz);
		glRotated(hoek, 0, 0, 1);
		if (draadmodel)
		{
			glutWireTorus(0.1, 1.68, 12, 12); // Inner radius, outer radius, aantal stukken in cross-richting, aantal stukken in radiale richting
		}
		else
		{
			glutSolidTorus(0.1, 1.68, 12, 12);
		}

		glPopMatrix();
	}
}

void spaken(float offsetz)
{
	if (materiaalstang) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_CHROOM_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_CHROOM_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_CHROOM_SPEC);
	}
	else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_BRONS_AMBI);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_BRONS_DIFF);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_BRONS_SPEC);
	}
	GLUquadricObj* spaak;
	spaak = gluNewQuadric();

	for (float i = 0.0; i < 360.0; i += (360.0 / cabines)) {
		for (int j = 0; j < 2; j++) // 2 spaken tekenen
		{
			glPushMatrix();
			glTranslatef(3, 1.65, (j * 1.5) + 2.25 + offsetz);
			glRotatef(-90, 1.0, 0.0, 0.0); // Rotate spaken zodat deze rechtop staan (3de uitgevoerd)
			glRotatef(i, 0.0, 1.0, 0.0); // Rotate zodat deze in een cirkel komen te staan (2de uitgevoerd)
			glRotated(-hoek, 0, 1, 0); // Rotate beweging (Eerst uitgevoerd)
			if (draadmodel)
			{
				gluQuadricDrawStyle(spaak, GLU_LINE);
			}
			else
			{
				gluQuadricDrawStyle(spaak, GLU_FILL);
			}
			gluCylinder(spaak, 0.05, 0.05, 1.7, 8, 8);
			glPopMatrix();
		}
	}
	gluDeleteQuadric(spaak);
}

void dwarsbalken_cabines(float offsetz)
{
	GLUquadricObj* dwarsbalk;
	dwarsbalk = gluNewQuadric();
	for (float i = -hoek; i < 360 - hoek; i += (360 / cabines))
	{
		if (materiaalstang) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_CHROOM_AMBI);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_CHROOM_DIFF);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_CHROOM_SPEC);
		}
		else {
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, STANG_BRONS_AMBI);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, STANG_BRONS_DIFF);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, STANG_BRONS_SPEC);
		}
		glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(2.98 + 1.7 * sin(i * 3.14 / 180), 1.62 + 1.7 *cos(i * 3.14 / 180), 2.2 + offsetz);
		glRotatef(0.0, 1.0, 0.0, 0.0);
		if (draadmodel)
		{
			gluQuadricDrawStyle(dwarsbalk, GLU_LINE);
		}
		else
		{
			gluQuadricDrawStyle(dwarsbalk, GLU_FILL);
		}
		gluCylinder(dwarsbalk, 0.05, 0.05, 1.6, 8, 8);
		glPopMatrix();

		tekencabine(0.400000 + 3.799999 * sin(i * 3.14 / 180),-4.5 + 3.799999 * cos(i * 3.14 / 180), -6.399995 + (offsetz*2.2));
		
	}
	gluDeleteQuadric(dwarsbalk);

}

void assen(void)
{
	// assen tekenen
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(xmax, 0.0, 0.0);
	glVertex3f(.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, ymax, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}

void teken_lichtbronnen(void)
{
	glColor3f(0, 1, 1);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex3fv(LICHTPOS1);
	glVertex3fv(LICHTPOS2);
	glVertex3fv(LICHTPOS3);
	glVertex3fv(LICHTPOS4);
	glEnd();
	glColor3f(0, 0, 0);
}

void rotate_cylinder(void) {
	if (draai) {
		hoek += draaisnelheid;
		if (hoek > 360.0)
			hoek -= 360.0;
		else if (hoek < -360.0)
			hoek += 360.0;
	}
	glutPostRedisplay();
}

void toetsen(unsigned char key, int x, int y)
{
	switch (key)
	{
	// Coördinaten camera
	case 'x': xc++; break;
	case 'y': yc++; break;
	case 'z': zc++; break;
	case 'X': xc--; break;
	case 'Y': yc--; break;
	case 'Z': zc--; break;

	case '1': offsetX = offsetX + 0.1; printf("offset=%f\n", offsetX); break;
	case '4': offsetX = offsetX - 0.1; printf("offset=%f\n", offsetX); break;
	case '2': offsetY = offsetY + 0.1; printf("offset=%f\n", offsetY); break;
	case '5': offsetY = offsetY - 0.1; printf("offset=%f\n", offsetY); break;
	case '3': offsetZ = offsetZ + 0.1; printf("offset=%f\n", offsetZ); break;
	case '6': offsetZ = offsetZ - 0.1; printf("offset=%f\n", offsetZ); break;

	case 'n': rads++; break;
	case 'g': draai = !draai; if (draai) { glutIdleFunc(rotate_cylinder); }
			else { glutIdleFunc(NULL); } break;
	case 'j': toonassen = !toonassen; break;
	case 't': teken = !teken; break;
	case 'l': draadmodel = !draadmodel; break;
	case 'k': toonctp = !toonctp; break; // y = 2.7 ; z = 6.29 ; x = 2.6

	case 'q': exit(0); break;

	// Lichten

	case 'a': glEnable(GL_LIGHT0); break;
	case 'A': glDisable(GL_LIGHT0); break;
	case 'b': glEnable(GL_LIGHT1); break;
	case 'B': glDisable(GL_LIGHT1); break;
	case 'c': glEnable(GL_LIGHT2); break;
	case 'C': glDisable(GL_LIGHT2); break;
	case 'd': glEnable(GL_LIGHT3); break;
	case 'D': glDisable(GL_LIGHT3); break;

	case 'h': LICHTPOS4[1] += 0.25;      printf("zet eerst 'v' en 'w' juist!");	printf("lichtpositie: %f\n", LICHTPOS4[1]);  break;
	case 'H': LICHTPOS4[1] -= 0.25;      printf("zet eerst 'v' en 'w' juist!");	printf("lichtpositie: %f\n", LICHTPOS4[1]);  break;

	case 'v': lichthoek -= 5.0;     printf("lichthoek: %f\n", lichthoek);    printf("probeer eens rond 60\n");
		if (lichthoek > 90.0 && lichthoek < 180.0) lichthoek = 90.0; break;
	case 'V': lichthoek += 5.0;     printf("lichthoek: %f\n", lichthoek);    printf("probeer eens rond 60\n");
		if (lichthoek > 90.0) lichthoek = 180.0;                       break;
	case 'w': expo += 5;     printf("exponent= %f\n", expo); printf("probeer eens rond 5\n"); break;
	case 'W': expo -= 5;     printf("exponent= %f\n", expo); printf("probeer eens rond 5\n"); break;

	case 's': flat = 1; break;
	case 'S': flat = 0; break;

	case 'L': toonlightpunten = !toonlightpunten; break;

	}
	printf("oog %lf %lf %lf\n", xc, yc, zc);
	glutPostRedisplay();

}


void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(xc, yc, zc, 0, 0, 0, 0, 1, 0); // Eerst coördinaten waar het oog staat, waar het kijkt en wat boven is.

	glEnable(GL_LIGHTING); 
	lichten(); 

	// Elementen oproepen die getekend moeten worden

	glMaterialf(GL_FRONT, GL_SHININESS, shini);
	glShadeModel(flat ? GL_FLAT : GL_SMOOTH);

	if (toonlightpunten)
	{
		teken_lichtbronnen();
	}

	if (toonassen)
	{
		assen(); 
	}

	if (teken)
	{
		for (int i = 0; i > (-rads); i--)
		{
			as(4 * i);
			as_cylinder(4 * i);
			spaken(4 * i);
			steunbalken(4 * i);
			dwarsbalken_cabines(4 * i);
		}
	}
	
	glPushMatrix();//clipping plane
	glTranslatef(0.0, -0.5, 0.0);
	glClipPlane(GL_CLIP_PLANE0, vlak);
	glEnable(GL_CLIP_PLANE0);
	glPopMatrix();


	glutSwapBuffers(); // één buffer om te tekenen en andere doet berekeningen
	glEnable(GL_LIGHTING);
	glFlush();
}

int main(int argc, char** argv)
{
	if (argc == 2) {
		projectie = argv[1][0];
	}
	if (argc == 3) {
		projectie = argv[1][0];
		cabines = argv[2][0];
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("reuzenrad");
	init();
	glutDisplayFunc(displayFcn);
	glutKeyboardFunc(toetsen);
	glutReshapeFunc(raam);
	glutIdleFunc(rotate_cylinder);
	glutMainLoop();
	return 0;
}
