/* Reuzenrad made by Aaron Van Campenhout - Starting date: 7/12/22 */

#include <gl/glut.h>
#include <stdio.h>
#include <math.h>

// Textures

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define AANT 3
#define MAXL 80 
char beeldnaam[AANT][MAXL] = { "dak.jpg" , "kuip.jpg" , "vloer.jpg"};
static GLuint texName[AANT];
int textuur = 0;

// Tekenen
GLint winWidth = 1000, winHeight = 1000; /* 800*800 pixels */
int xmax = 5, ymax = 5, xmin = -5, ymin = -5;
float near = 0.5, far = 400;
GLdouble xc = 10, yc = 5, zc = 15;
GLdouble vlak[4] = { 0,0,0,0 };
char projectie = 'p';
GLdouble aspectratio;

float offsetX = 0, offsetY = 0, offsetZ = 0; // Offset dient om structuren te bewegen
int cabines = 10;
int rads = 1;
int draadmodel = 0;
int eenvoudig = 1;
int toonctp = 0;
int teken = 1;
int toonassen = 1;
int toonlightpunten = 0;
int flat = 0;
int grens;

// anitmatie
float draai = 0;
float draaisnelheid = 0.25;
float hoek = 0;
float wiebelhoek = 0;

int wiebelen = 0;
int richting = -1;
float tijd = 25; // 25 ms

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
const GLfloat KUIPJE_GRIJS_AMBI[] = { 0.22,0.22,0.22,1 };
const GLfloat KUIPJE_GRIJS_DIFF[] = { 0.33,0.33,0.33,1 };
const GLfloat KUIPJE_GRIJS_SPEC[] = { 0.11,0.11,0.11,1 };
const GLfloat KUIPJE_WIT_AMBI[] = { 0.66,0.66,0.66, 1 };
const GLfloat KUIPJE_WIT_DIFF[] = { 0.77,0.77,0.77, 1 };
const GLfloat KUIPJE_WIT_SPEC[] = { 0.55,0.55,0.55, 1 };

const GLfloat STANG_CHROOM_AMBI[] = { 0.46,0.58,0.35, 1 };
const GLfloat STANG_CHROOM_DIFF[] = { 0.23,0.29,0.17, 1 };
const GLfloat STANG_CHROOM_SPEC[] = { 0.69,0.87,0.52, 1 };
const GLfloat STANG_BRONS_AMBI[] = { 0.21,0.13,0.10, 1 };
const GLfloat STANG_BRONS_DIFF[] = { 0.39,0.27,0.17, 1 };
const GLfloat STANG_BRONS_SPEC[] = { 0.71,0.43,0.18, 1 };

const GLfloat DAK_GEEL_AMBI[] = { 0.65,0.55,0.15, 1 };
const GLfloat DAK_GEEL_DIFF[] = { 0.75,0.45,0.15, 1 };
const GLfloat DAK_GEEL_SPEC[] = { 0.85,0.35,0.15, 1 };
const GLfloat DAK_LILA_AMBI[] = { 0.45,0.15,0.75, 1 };
const GLfloat DAK_LILA_DIFF[] = { 0.55,0.15,0.65, 1 };
const GLfloat DAK_LILA_SPEC[] = { 0.35,0.15,0.85, 1 };



// Licht en kleuren

const GLfloat LICHTPOS1[] = { 0.0, 0.0, 0.0, 0.0 };	// Achteraan reuzenrad (oorsprong)
const GLfloat LICHTPOS2[] = { 4.0, 2, 4.0, 0.0 }; // Vooraan loodrecht op reuzenrad, achterkant van het rad = donker
const GLfloat LICHTPOS3[] = { 2, -1, 2, 0.0 }; // onderaan reuzenrad
GLfloat LICHTPOS4[] = { 3, 0.0, 5 }; // vooraaan beweegbare hoogte

GLfloat ZWART[] = { 0,0,0 };
GLfloat GRIJS[] = { 0.3,0.4,0.5,1.0 };
GLfloat WIT[] = { 1.0,1.0,1.0,1.0 };
GLfloat ROOD[] = { 0.9,0.1,0.0,1.0 };
GLfloat GEEL[] = { 1,1,0,1 };
GLfloat ROODBLAUW[] = { 0.9,0.0,0.9,1.0 };
GLfloat GROEN[] = { 0.0,1.0,0.0,1.0 };
GLfloat GRBLAUW[] = { 0.0,0.8,0.9,1.0 };
GLfloat ROGBRL[] = { 0.4,0.6,0.8,1.0 };
GLfloat oranje[] = {1, 0.9 ,0.4,1};
GLfloat mistkleur[4] = { 0.9, 0.9, 0.9, 1 };
GLfloat shini = 10;
GLfloat expo = 20.0;
GLfloat lichthoek = 45;
const GLfloat direc[] = { 0,0.0,-1};

int materiaalstang = 0; // Kiezen tussen brons en chroom
int materiaalkuip = 0;
int materiaaldak = 0;
int doorzichtig = 0;
int mist = 0;
int grond_textuur = 1;



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
	glClearColor(0.9, 0.9, 0.9, 1);
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

	glMaterialf(GL_FRONT, GL_SHININESS, 127);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_DST_ALPHA);

	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE); // Normaliseren van vectoren

	dakelement = gluNewNurbsRenderer();
	gluNurbsProperty(dakelement, GLU_DISPLAY_MODE, GLU_FILL);


	glGenTextures(AANT, texName);
	for (int i = 0; i < AANT; i++)
	{
		int x, y, n;
		stbi_uc* data = stbi_load(beeldnaam[i], &x, &y, &n, 0);
		if (data == NULL)
		{
			printf("%s niet gevonden\n", beeldnaam[i]);
			exit(0);
		}
		printf("size X is %d en size Y is %d en n %d\n", x, y, n);
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		GLint format;
		switch (n) {
		case 3: format = GL_RGB;
			break;
		case 4: format = GL_RGBA;
			break;
		default: printf("Afbeelding niet genoeg componenten per pixel\n");
			exit(0);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}


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
	case 'f':  grens = near * tan(3.14 * (45.0 / 2.0) / 180.0);
		glFrustum(-0.5, 0.5, -0.5, 0.5, near, far * 9);  printf("Frustum\n"); break;
	default: break;
	}

	printf("herschaald\n");
	glViewport(0, 0, n_w, n_h);
}

void lichten(void) 
{
	// Positie lichtbronnen
	glLightfv(GL_LIGHT1, GL_POSITION, LICHTPOS1);
	glLightfv(GL_LIGHT2, GL_POSITION, LICHTPOS2);
	glLightfv(GL_LIGHT3, GL_POSITION, LICHTPOS3);
	glLightfv(GL_LIGHT4, GL_POSITION, LICHTPOS4);

	// Eigenschappen lichtbronnen
	glLightfv(GL_LIGHT1, GL_AMBIENT, WIT);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, GRBLAUW);
	glLightfv(GL_LIGHT3, GL_SPECULAR, ROOD);
	glLightfv(GL_LIGHT4, GL_AMBIENT, GEEL);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, GEEL);
	glLightfv(GL_LIGHT4, GL_SPECULAR, GEEL);


	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, lichthoek);  // CUTOFF openingshoek kegel
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, expo); // de intensiteit in het midden maximaal en verzwakt naar randen volgens factor cos^a(??); a 0 - 128 ingesteld worden
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, direc); // Richting licht

}
void mistfunctie(void)
{
	glEnable(GL_FOG);
	if (mist >= 3)
		glFogi(GL_FOG_MODE, GL_EXP2);
	if (mist == 2)
		glFogi(GL_FOG_MODE, GL_EXP);
	if (mist == 1)
		glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, mistkleur);
	glFogf(GL_FOG_DENSITY, 0.25);
	glFogf(GL_FOG_START, 0.0); // afstand van oog
	glFogf(GL_FOG_END, 15);
}


void kuip(GLfloat x, GLfloat y, GLfloat z)
{
	// Bezier kuip
	glPushMatrix();

	glTranslatef(x + 3.1 + 2 * sin(wiebelhoek * 3.14 / 180), y + 3.2 - 0.5 * sin(wiebelhoek * 3.14 / 180), z + 6.79);
	glRotated(90, 0, 1, 0);
	glScaled(5, 5, 5);
	glRotated(wiebelhoek, -1, 0, 0); // Wiebel animatie

	if (textuur) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName[1]);//1=kuip.jpg
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 3, 4, 0, 1, 12, 6, &ctrPt[0][0][0]);
	}

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
	if (textuur)
	{
		glDisable(GL_TEXTURE_2D);
	}

	glDisable(GL_MAP2_VERTEX_3);
	glPopMatrix();
	
}

void bevestiging(GLfloat x, GLfloat y, GLfloat z)
{
	// Staaf 
	GLUquadricObj* bevestiging;
	bevestiging = gluNewQuadric();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(x + 3.6, y + 5.18, z + 6.78); // y = 2.7 ; z = 6.29 ; x = 2.6
	glRotated(90, 1, 0, 0);
	glRotated(wiebelhoek, 0, 1, 0);
	if (draadmodel)
	{
		gluQuadricDrawStyle(bevestiging, GLU_LINE);
	}
	else
	{
		gluQuadricDrawStyle(bevestiging, GLU_FILL);
	}
	gluCylinder(bevestiging, 0.03, 0.03, 1.9, 8, 8);
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
	for (int k = 0; k < 4; k++)
	{
		glPushMatrix();
		glScalef(3.8, 3, 3.8);
		glTranslatef(0.26 + 0.1 * sin(wiebelhoek * 3.14 / 180), 0.8 - 0.1 * cos(wiebelhoek * 3.14 / 180), 0.13); //0.26 .7

		glRotatef(180, 0, 0, 1); // Juist richting dak 
		glRotated(wiebelhoek, 0, 0, 1);
		glRotatef(45, 0, 1, 0); // Draai naar juiste riching voor bevestiging
		glRotatef(k * 90, 0, 1, 0); // Dak vier keer geroteerd tekenen rond y-as

		if (textuur)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[0]);   //dak.jpg
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		}

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

		//gluBeginSurface(dakelement);
		gluNurbsSurface(dakelement, 8, knots, 8, knots, 4 * 3, 3, &bsplineDak[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(dakelement);
		if (textuur)
		{
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}

}

void cabine(GLfloat x, GLfloat y, GLfloat z) // samenstellen van cabine met verschillende onderdelen
{
	glPushMatrix();
		glTranslatef(2.6+x,2.7+y,6.29+z); // y = 2.7 ; z = 6.29 ; x = 2.6
		dak();
	glPopMatrix();

	// Materiaal kuip en bevestiging
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
	kuip(x,y, z);
	bevestiging(x, y, z);
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

void schijven(float Offsetz)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ZWART);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, oranje);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, GRBLAUW);
	
	for (int i = 0; i < 2; i++)
	{
		glPushMatrix();
		if (doorzichtig)
		{
			glEnable(GL_BLEND);
			glDepthMask(GL_FALSE);
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);//GL_ONE_MINUS_SRC_ALPHA);
		}
		glScalef(1, 1, 1);
		glTranslatef(3, 1.6, (i * 1.5) + 2.25 + Offsetz);
		glRotated(hoek, 0, 0, 1);
		if (draadmodel)
		{
			glutWireTorus(0.09, 1.8, 12, 12); // Inner radius, outer radius, aantal stukken in cross-richting, aantal stukken in radiale richting
		}
		else
		{
			glutSolidTorus(0.09, 1.8, 12, 12);
		}
		if (doorzichtig)
		{
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
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

void teken_assen_lichtbronnen(void)
{
	if (toonassen)
	{
		glDisable(GL_LIGHTING);
		
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
		glEnable(GL_LIGHTING);
	}

	if (toonlightpunten)
	{
		glDisable(GL_LIGHTING);
		glColor3f(0.0, 1.0, 0.0);
		// Punten lichtbronnen tekenen
		glPointSize(8);
		glBegin(GL_POINTS);
		glVertex3fv(LICHTPOS1);
		glVertex3fv(LICHTPOS2);
		glVertex3fv(LICHTPOS3);
		glVertex3fv(LICHTPOS4);
		glEnd();
		glEnable(GL_LIGHTING);
	}

}

void beweeg(void) {
	if (draai) {
		hoek += draaisnelheid;
		if (hoek > 360.0)
			hoek -= 360.0;
		else if (hoek < -360.0)
			hoek += 360.0;
	}
	glutPostRedisplay();
}

void wiebel(int delta)
{
	if (wiebelen) {
		wiebelhoek += (delta * richting ); // else wiebelhoek = 0 
		if (wiebelhoek > 15)
		{
			richting = -1;
		}
		else if (wiebelhoek < -15)
		{
			richting = 1;
		}
	}
	else
	{
		wiebelhoek = 0; // else wiebelhoek = 0 

	}

	glutTimerFunc(tijd, wiebel, 1);
	glutPostRedisplay();
}

void toetsen(unsigned char key, int x, int y)
{
	switch (key)
	{
	// Co??rdinaten camera
	case 'x': xc++; break;
	case 'y': yc++; break;
	case 'z': zc++; break;
	case 'X': xc--; break;
	case 'Y': yc--; break;
	case 'Z': zc--; break;

	case '1': xc = 8; yc = 4; zc = 8; break;
	case '4': xc = 5; yc = 8; zc = 5; break;
	case '2': xc = -10; yc = 0; zc = -12; break;
	case '5': xc = -15; yc = 3; zc = 3; break;
	case '3': offsetZ = offsetZ + 0.1; printf("offset=%f\n", offsetZ); break;
	case '6': offsetZ = offsetZ - 0.1; printf("offset=%f\n", offsetZ); break;
	case '9': xc = 8; yc = 1; zc = 3; break;

	// Lichten

	case 'a': glEnable(GL_LIGHT1); break;
	case 'A': glDisable(GL_LIGHT1); break;
	case 'b': glEnable(GL_LIGHT2); break;
	case 'B': glDisable(GL_LIGHT2); break;
	case 'c': glEnable(GL_LIGHT3); break;
	case 'C': glDisable(GL_LIGHT3); break;
	case 'd': glEnable(GL_LIGHT4); break;
	case 'D': glDisable(GL_LIGHT4); break;

	case 'e': shini += 5.0;      printf("shininess: %f\n", shini);  break;
	case 'E': shini -= 5.0;      printf("shininess: %f\n", shini);  break;
	case 'f': doorzichtig = !doorzichtig; printf("doorzichtig=%d", doorzichtig); break;
	case 'g': draai = !draai; if (draai) { glutIdleFunc(beweeg); }
			else { glutIdleFunc(NULL); } break;

	case 'G': wiebelen = !wiebelen; break;
	case 'h': LICHTPOS4[1] += 0.1;      printf("zet eerst 'v' en 'w' juist!");	printf("lichtpositie: %f\n", LICHTPOS4[1]);  break;
	case 'H': LICHTPOS4[1] -= 0.1;      printf("zet eerst 'v' en 'w' juist!");	printf("lichtpositie: %f\n", LICHTPOS4[1]);  break;
	case 'j': toonassen = !toonassen; break;
	case 'k': toonctp = !toonctp; break; // y = 2.7 ; z = 6.29 ; x = 2.6
	case 'l': draadmodel = !draadmodel; break;
	case 'n': rads++; break;

	case 'q': exit(0); break;
	case 's': flat = 1; break;
	case 'S': flat = 0; break;
	case 't': textuur = !textuur; break;
	case 'T': teken = !teken; break;
	case 'u': grond_textuur = !grond_textuur; break;
	case 'v': lichthoek -= 5.0;     printf("lichthoek: %f\n", lichthoek);  break;
	case 'V': lichthoek += 5.0;     printf("lichthoek: %f\n", lichthoek); break;
	case 'w': expo += 5;     printf("exponent= %f\n", expo);  break;
	case 'W': expo -= 5;     printf("exponent= %f\n", expo);  break;


	case 'L': toonlightpunten = !toonlightpunten; break;
	case 'm': mist = !mist; printf("doorzichtig=%d", mist); break;
	case 'M': mist += 1; printf("doorzichtig=%d", mist); break;

	}
	printf("oog %lf %lf %lf\n", xc, yc, zc);
	glutPostRedisplay();

}


void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(xc, yc, zc, 4, 2, 4, 0, 1, 0); // Eerst co??rdinaten waar het oog staat, waar het kijkt en wat boven is.

	glEnable(GL_LIGHTING);
	lichten();

	// Elementen oproepen die getekend moeten worden

	glMaterialf(GL_FRONT, GL_SHININESS, shini);
	glShadeModel(flat ? GL_FLAT : GL_SMOOTH);

	teken_assen_lichtbronnen();

	if (mist)
	{
		mistfunctie();
	}

	if (teken) // teken rad
	{
		for (int i = 0; i > (-rads); i--)
		{
			as(4 * i);
			schijven(4 * i);
			spaken(4 * i);
			steunbalken(4 * i);
			dwarsbalken_cabines(4 * i);
		}
	}
	
	glPushMatrix();//clipping plane
	glTranslatef(0.0, -1, 0.0);
	glClipPlane(GL_CLIP_PLANE0, vlak);
	glEnable(GL_CLIP_PLANE0);
	glPopMatrix();

	if (grond_textuur) {

		glPushMatrix();//vloer

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[2]);//2=vloer.jpg
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);	glVertex3d(-20.0, -0.99, 20.0);
			glTexCoord2f(0.0, 2.0);	glVertex3d(20.0, -0.99, 20.0);
			glTexCoord2f(2.0, 2.0);	glVertex3d(20.0, -0.99, -50.0);
			glTexCoord2f(2.0, 0.0);	glVertex3d(-20.0, -0.99, -50.0);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	if (mist)
	{
		glDisable(GL_FOG);
	}

	glutSwapBuffers(); // ????n buffer om te tekenen en andere doet berekeningen
	glDisable(GL_LIGHTING);
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
	glutIdleFunc(beweeg);
	glutTimerFunc(tijd, wiebel, 1);  // 25 ms to pass before calling the callback; wiebel is the timer callback function; 1 integer for callback func
	glutMainLoop();
	return 0;
}
