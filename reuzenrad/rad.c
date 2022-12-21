/* Reuzenrad made by Aaron Van Campenhout Start date: 7/12/22 */

#include <gl/glut.h>
#include <stdio.h>
#include <math.h>

// Tekenen
GLint winWidth = 1000, winHeight = 1000; /* 800*800 pixels */
int xmax = 5, ymax = 5, xmin = -5, ymin = -5;
float near = 0.5, far = 400;
GLdouble xc = 4, yc = 4, zc = 5;
GLdouble vlak[4] = { 0,1,0,1 };
char projectie = 'o';

float offsetX = 0, offsetY = 0, offsetZ = 0; // Offset dient om structuren te bewegen

int cabines = 6;
int rads = 1;
int draadmodel = 0;
int eenvoudig = 1;
int toonctp = 0;
int teken = 0;

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



void init(void)
{
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
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

void kuip(GLfloat x, GLfloat y, GLfloat z)
{
	// Bezier kuip
	glPushMatrix();

	glTranslatef(x + 3.1, y + 3.2 , z + 6.79); 
	glRotated(wiebelhoek, 0, 0, 1);
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
			glPointSize(10);
			glBegin(GL_POINTS);
			glColor3f(0, 1, 1);
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					glVertex3fv(ctrPt[j][k]);
				}
			}
			glEnd();
			glColor3f(0, 0, 1);
		}


		glScaled(-1, 1, 1); // halve kuip spiegelen
	}
	glDisable(GL_MAP2_VERTEX_3);
	glPopMatrix();
}


void bevestiging(GLfloat x, GLfloat y, GLfloat z)
{

	//Staven tekenen

	GLUquadricObj* staaf1;
	staaf1 = gluNewQuadric();

	glColor3f(1, 0, 0);

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
	gluCylinder(bevestiging, 0.02, 0.02, 0.6, 8, 8);
	glPopMatrix();
	gluDeleteQuadric(bevestiging);

}


void dak(void)
{
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
		glColor3b(0,0,1);
		
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
			glColor3f(0, 1, 1);
			glBegin(GL_POINTS);
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++) {
					glVertex3fv(bsplineDak[i][j]);
				}
			}
			glColor3f(0, 0, 1);
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
		glColor3f(0, 1, 1);
		glScalef(0.45, 0.45, 0.45);
		cabine(2.6 + x, 2.7 + y, 6.29 + z);
	glPopMatrix();
}


void steunbalken(float offsetZ)
{
	glColor3f(0, 0, 0);

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
	GLUquadricObj* as;
	as = gluNewQuadric();

	glPushMatrix();
	glColor3f(0, 0, 0);
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
	for (int i = 0; i < 2; i++)
	{
		glPushMatrix();
		glColor3f(0, 0, 0);
		glScalef(1, 1, 1);
		glTranslatef(3, 1.5, (i * 1.5) + 2.25 + Offsetz);
		glRotated(hoek, 0, 0, 1);
		if (draadmodel)
		{
			glutWireTorus(0.1, 1.65, 12, 12); // Inner radius, outer radius, aantal stukken in cross-richting, aantal stukken in radiale richting
		}
		else
		{
			glutSolidTorus(0.1, 1.65, 12, 12);
		}

		glPopMatrix();
	}
}

void spaken(float offsetz)
{
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
			gluCylinder(spaak, 0.05, 0.05, 1.6, 8, 8);
			glPopMatrix();
		}
	}
	gluDeleteQuadric(spaak);
}

void dwarsbalken(float offsetz)
{
	GLUquadricObj* dwarsbalk;
	dwarsbalk = gluNewQuadric();
	for (float i = -hoek; i < 360 - hoek; i += (360 / cabines))
	{
		glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(2.98 + 1.6 * sin(i * 3.14 / 180), 1.62 + 1.6 * cos(i * 3.14 / 180), 2.2 + offsetz);
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

		tekencabine(0.500000 + 3.599999 * sin(i * 3.14 / 180), -4.4 + 3.599999 * cos(i * 3.14 / 180), -6.399995 + (offsetz*2.2));
		printf("offset:%f\n", offsetX);
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
	case 't': teken = !teken; break;
	case 'l': draadmodel = !draadmodel; break;
	case 'k': toonctp = !toonctp; break; // y = 2.7 ; z = 6.29 ; x = 2.6
	case 'q': exit(0); break;

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

	// Elementen oproepen die getekend moeten worden
	assen();

	if (teken)
	{
		for (int i = 0; i > (-rads); i--)
		{
			as(4 * i);
			as_cylinder(4 * i);
			spaken(4 * i);
			steunbalken(4 * i);
			dwarsbalken(4 * i);
		}
	}
	
	glPushMatrix();//clipping plane
	glTranslatef(0.0, -0.5, 0.0);
	glClipPlane(GL_CLIP_PLANE0, vlak);
	glEnable(GL_CLIP_PLANE0);
	glPopMatrix();

	glutSwapBuffers(); // één buffer om te tekenen en andere doet berekeningen
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
