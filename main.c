#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);


void crtanje_loptice(void);
void crtanje_postolja(void);
void crtanje_lestvica(void);

static double koordinata_x_loptice = 0, koordinata_y_loptice = -0.8, koordinata_z_loptice = 0;
static double koordinata_x_lestvice = 0, koordinata_y_lestvice = -0.4, koordinata_z_lestvice = 0;

int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;
    }
}

static void on_reshape(int width, int height)
{
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (float) width / height, 1, 10);
}

static void on_display(void)
{
    /* Podesava se vidna tacka. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 7, 
              0, 0, 0, 
              0, 1, 0);
    
    crtanje_loptice();
    crtanje_postolja();
    crtanje_lestvica();
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}

void crtanje_loptice(){
    /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat pozicija_svetla[] = { 1, 1, 1, 0 };

    /* Ambijentalna boja svetla. */
    GLfloat ambijentalno_svetlo[] = { 0.35, 0.35, 0.35, 1 };

    /* Difuzna boja svetla. */
    GLfloat difuzno_svetlo[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla. */
    GLfloat spekularno_svetlo[] = { 0.9, 0.9, 0.9, 1 };

    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambijentalna_refleksija[] = { 1.0, 1.0, 0.1, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat difuzna_refleksija[] = { 1.0, 1.0, 0.0, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat spekularna_refleksija[] = { 1, 1, 1, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 20;

    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pozicija_svetla);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambijentalno_svetlo);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuzno_svetlo);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spekularno_svetlo);

    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentalna_refleksija);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuzna_refleksija);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularna_refleksija);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    glPushMatrix();
        glTranslatef(koordinata_x_loptice, koordinata_y_loptice, koordinata_z_loptice);
        glutSolidSphere(0.15, 500, 500);
    glPopMatrix();
        
}


void crtanje_postolja(){
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glBegin(GL_QUADS);
        glColor3f(0.0, 0.9, 0.0);
        glVertex2f(-2.0, -7.0);
        glVertex2f(2.0, -7.0);
        glVertex2f(2.0, -0.9);
        glVertex2f(-2.0, -0.9);
        glEnd();    
    glPopMatrix();
}

void crtanje_lestvica(){    
    srand(time(NULL));

    double pomeraj = 0.5;
    unsigned i;
    for(i = 0; i<1000; i++){
        double randomBroj = rand()/(float)RAND_MAX;
        int pozicija_x_koordinate;
        if(randomBroj >=0.5)
            pozicija_x_koordinate = 1;
        else
            pozicija_x_koordinate = -1;
        glPushMatrix();
            glColor3f(0.5, 0.35, 0.05);
            glTranslatef(koordinata_x_lestvice+2*pozicija_x_koordinate*rand()/(float)RAND_MAX,
                         koordinata_y_lestvice, koordinata_z_lestvice);
            glScalef(6, 0.5, 0);
            glutSolidCube(0.2);
        glPopMatrix();
        koordinata_y_lestvice += pomeraj;
    }
}

