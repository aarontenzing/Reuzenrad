/* Reuzenrad made by Aaron Van Campenhout Start date: 7/12/22 */

#include <gl/glut.h>
#include <stdio.h>
#include <math.h>

GLint winWidth = 800, winHeight = 800; /* 800*800 pixels */
int xmax = 5, ymax = 5, xmin = -5, ymin = -5;

float near = 0.5, far = 400;
GLdouble xc = 4, yc = 4, zc = 5;
GLdouble vlak[4] = { 0,1,0,1 };

float offsetX = 0, offsetY = 0, offsetZ = 0; // Offset dient om structuren te bewegen
char projectie = 'o';
float draai = 0, draaisnelheid = 0.25, hoek = 0, wiebelhoek = 0;

int cabines = 6;
int rads = 1;
int draadmodel = 0;

GLfloat ctrPt[6][4][3] =
{
   { {0.0, 0.5, 0.00}, {0.04, 0.5, 0.00}, {0.08, 0.5, 0.00}, {0.12, 0.5, 0.00}},
   { {0.0, 0.0, 0.04}, {0.04, 0.0, 0.04}, {0.08, 0.0, 0.04}, {0.12, 0.5, 0.04}},
   { {0.0, 0.0, 0.08}, {0.04, 0.0, 0.08}, {0.08, 0.0, 0.08}, {0.12, 0.5, 0.08}},
   { {0.0, 0.0, 0.12}, {0.04, 0.0, 0.12}, {0.08, 0.0, 0.12}, {0.12, 0.5, 0.12}},
   { {0.0, 0.0, 0.18}, {0.04, 0.0, 0.18}, {0.08, 0.0, 0.18}, {0.12, 0.5, 0.18}},
   { {0.0, 0.5, 0.20}, {0.04, 0.5, 0.20}, {0.08, 0.5, 0.20}, {0.12, 0.5, 0.20}}
};

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

void kuipje(GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();
	glColor3f(1, 0, 0);
		glTranslatef(x + 0.4 * sin(wiebelhoek * 3.14 / 180), y - 0.4 * cos(wiebelhoek * 3.14 / 180), 2.85); 
		glRotated(wiebelhoek, 0, 0, 1);
		glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, &ctrPt[0][0][0]); // 
		glEnable(GL_MAP2_VERTEX_3);
		glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
		glEvalMesh2(GL_LINE, 0, 20, 0, 20);
		glScaled(-2, 2, 2);
		
		glDisable(GL_MAP2_VERTEX_3);
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
		//kuipje(2.5 + 1.6 * sin(i * 3.14 / 180), 1.2 + 1.6 * cos(i * 3.14 / 180), 0.95);
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

	case '1': offsetX = offsetX + 0.01; printf("offset=%f\n", offsetX); break;
	case '4': offsetX = offsetX - 0.01; printf("offset=%f\n", offsetX); break;
	case '2': offsetY = offsetY + 0.01; printf("offset=%f\n", offsetY); break;
	case '5': offsetY = offsetY - 0.01; printf("offset=%f\n", offsetY); break;
	case '3': offsetZ = offsetZ + 0.01; printf("offset=%f\n", offsetZ); break;
	case '6': offsetZ = offsetZ - 0.01; printf("offset=%f\n", offsetZ); break;

	case 'n': rads++; break;
	case 'g': draai = !draai; if (draai) { glutIdleFunc(rotate_cylinder); }
			else { glutIdleFunc(NULL); } break;
	case 'l': draadmodel = !draadmodel; break;
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
	kuipje(2, 2, 4);
	

	for (int i = 0; i > (-rads); i--)
	{
		as(4 * i);
		as_cylinder(4 * i);
		spaken(4 * i);
		steunbalken(4 * i);
		dwarsbalken(4 * i);
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
	if (argc == 2 ) {
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