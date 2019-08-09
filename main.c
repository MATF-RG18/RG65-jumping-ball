#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);


void crtanje_loptice(void);
void crtanje_postolja(void);
void crtanje_lestvica(void);

static double koordinata_x_loptice = 0, koordinata_y_loptice = -0.8, koordinata_z_loptice = 0;
static double koordinata_x_lestvice[1000], koordinata_y_lestvice[1000];
static double koordinata_z_lestvice = 0;
static double brzina[1000];
static int window_width, window_height;
static int animacija;
int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
        srand(time(NULL));

    double pomeraj = 0.5;
    unsigned int i = 0;
    koordinata_y_lestvice[0] = -0.4;
    for(i = 0; i<1000; i++){
        if(i != 0)
            koordinata_y_lestvice[i] = koordinata_y_lestvice[i-1] + pomeraj;
        double randomBroj = rand()/(float)RAND_MAX;
        int pozicija_x_koordinate;
        brzina[i] = 0.03;
        if(randomBroj >=0.5)
            pozicija_x_koordinate = 1;
        else
            pozicija_x_koordinate = -1;
        koordinata_x_lestvice[i] = 2*pozicija_x_koordinate*rand()/(float)RAND_MAX;
    }
    animacija = 0;
    
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
    case 'g':
    case 'G':
        /* Pokrece se animacija. */
        if (!animacija) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animacija = 1;
        }
        break;
    }
}

static void on_reshape(int width, int height){
    window_width = width;
    window_height = height;
}


static void on_timer(int value)
{
    if (value != TIMER_ID)
        return;
    unsigned int i = 0;
    for(i = 0; i<1000; i++){
        koordinata_x_lestvice[i] += brzina[i];
        double sirina = 1.2;
        
        if(koordinata_x_lestvice[i] + sirina/2 >= 2 || koordinata_x_lestvice[i] - sirina/2 <= -2)
            brzina[i] *= -1;

        
    }
    /* Forsira se ponovno iscrtavanje prozora. */
        glutPostRedisplay();

        /* Po potrebi se ponovo postavlja tajmer. */
        if (animacija) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
        }
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* Podesava se vidna tacka. */
    glViewport(0, 0, window_width, window_height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (float) window_width / window_height, 1, 10);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 7, 
              0, 0, 0, 
              0, 1, 0);
    glEnable(GL_DEPTH_TEST);
    crtanje_loptice();
    crtanje_lestvica();

    crtanje_postolja();
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
    glDisable(GL_LIGHTING);
    unsigned int i = 0;
    for(i=0; i<1000; i++){
        glPushMatrix();
            glColor3f(0.5, 0.35, 0.05);
            glTranslatef(koordinata_x_lestvice[i], koordinata_y_lestvice[i], koordinata_z_lestvice);
            glScalef(6, 0.5, 0);
            glutSolidCube(0.2);
        glPopMatrix();
    }
}

