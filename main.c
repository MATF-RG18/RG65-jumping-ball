#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

#define TIMER_ID 0
#define TIMER_ID1 1
#define TIMER_INTERVAL 20

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void skok_loptice(int value);
static void on_display(void);


void crtanje_loptice(void);
void crtanje_postolja(void);
void crtanje_lestvica(void);

static double koordinata_x_loptice = 0, koordinata_y_loptice = -0.8, koordinata_z_loptice = 1000.5;
static double koordinata_x_lestvice[1000], koordinata_z_lestvice[1000];
static double koordinata_y_lestvice = -1;
static double brzina[1000];
static double brzina_loptice = 0.015;
static double brzina_z, brzina_y;

static double dubina;

static int window_width, window_height;
static int kretanje_lestvica, kretanje_loptice;
static int nivo;
static int loptica_na_lestvici;

int main(int argc, char **argv){
    
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    
    srand(time(NULL));
    
    nivo = 0;
    dubina = 1007;    
    double pomeraj = 1.5;
    unsigned int i = 0;
    koordinata_z_lestvice[0] = 999;
    for(i = 0; i<1000; i++){
        if(i != 0)
            koordinata_z_lestvice[i] = koordinata_z_lestvice[i-1] - pomeraj;
        double randomBroj = rand()/(float)RAND_MAX;
        int pozicija_x_koordinate;
        brzina[i] = 0.03;
        if(randomBroj >=0.5)
            pozicija_x_koordinate = 1;
        else
            pozicija_x_koordinate = -1;
        koordinata_x_lestvice[i] = 2*pozicija_x_koordinate*rand()/(float)RAND_MAX;
    }
    kretanje_lestvica = 0;
    kretanje_loptice = 0;
    loptica_na_lestvici = 0;
    
    brzina_y = 0.10;
    brzina_z = -0.1;
    
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

static void on_keyboard(unsigned char key, int x, int y){
    
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;
    case 'g':
    case 'G':
        /* Pokrece se animacija. */
        if (!kretanje_lestvica) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            kretanje_lestvica = 1;
        }
        break;
    case 'a':
    case 'A':
        koordinata_x_loptice -= brzina_loptice;
        glutPostRedisplay();
        break;
    case 'd':
    case 'D':
        koordinata_x_loptice += brzina_loptice;
        glutPostRedisplay();
        break;
    case 'w':
    case 'W':
        if(!kretanje_loptice) {
            loptica_na_lestvici = 0;
            glutTimerFunc(TIMER_INTERVAL, skok_loptice, TIMER_ID1);
            kretanje_loptice = 1;
        }
        break;
    }
}

static void on_reshape(int width, int height){
    window_width = width;
    window_height = height;
}

static void skok_loptice(int value){
    
    if (value != TIMER_ID1)
        return;
    dubina -= 0.1;
    if(koordinata_z_loptice - koordinata_z_lestvice[nivo] > 0.75){
        koordinata_y_loptice += brzina_y - 0.001;
        koordinata_z_loptice += brzina_z;
    }
    else{
        koordinata_y_loptice -= brzina_y - 0.001;
        koordinata_z_loptice += brzina_z;
    }
    
    if (koordinata_z_loptice - koordinata_z_lestvice[nivo] < 0.01) {
        kretanje_loptice = 0;
        
        koordinata_y_loptice -= brzina_y;
        if(!(koordinata_x_loptice <= koordinata_x_lestvice[nivo] + 0.6 && 
           koordinata_x_loptice >= koordinata_x_lestvice[nivo] - 0.6)){
            nivo = 0;
            koordinata_x_loptice = 0;
            koordinata_y_loptice = -0.8; 
            koordinata_z_loptice = 1000.5;
            dubina = 1007;
            loptica_na_lestvici = 0;
            glutPostRedisplay();
        }
        else{
            nivo += 1;
            loptica_na_lestvici = 1;
            glutPostRedisplay();
        }
    }
    else{
        glutPostRedisplay();
        glutTimerFunc(TIMER_INTERVAL, skok_loptice, TIMER_ID1);
    }
    
}

static void on_timer(int value){
    
    if (value != TIMER_ID)
        return;
    unsigned int i = 0;
    for(i = 0; i<1000; i++){
        koordinata_x_lestvice[i] += brzina[i];
        double sirina = 1.2;
        
        if(koordinata_x_lestvice[i] + sirina/2 >= 2 || koordinata_x_lestvice[i] - sirina/2 <= -2)
            brzina[i] *= -1;

        
    }
    if(loptica_na_lestvici){
        koordinata_x_loptice += brzina[nivo-1];
    }
    glutPostRedisplay();
    
    if (kretanje_lestvica) {
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
    gluPerspective(30, (float) window_width / window_height, 1, 20);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, dubina, 
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
    GLfloat pozicija_svetla[] = { 1, 1, 1007, 0 };

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
    
    if(koordinata_x_loptice + 0.15 >= 1.9 || koordinata_x_loptice - 0.15 <= -1.9){
        koordinata_x_loptice = 0;
        koordinata_y_loptice = -0.8;
        koordinata_z_loptice = 1000.5;
        loptica_na_lestvici = 0;
        dubina = 1007;
        nivo = 0;
    }
    
    glPushMatrix();
        glTranslatef(koordinata_x_loptice, koordinata_y_loptice, koordinata_z_loptice);
        glutSolidSphere(0.15, 500, 500);
    glPopMatrix();
        
}


void crtanje_postolja(){
    glDisable(GL_LIGHTING);
    GLfloat pozicija_svetla[] = { 1, 200, 1007, 1 };

    /* Ambijentalna boja svetla. */
    GLfloat ambijentalno_svetlo[] = { 0.35, 0.35, 0.35, 1 };

    /* Difuzna boja svetla. */
    GLfloat difuzno_svetlo[] = { 0.75, 0.75, 0.75, 1 };

    /* Spekularna boja svetla. */
    GLfloat spekularno_svetlo[] = { 0.9, 0.9, 0.9, 1 };

    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambijentalna_refleksija[] = { 0.0, 1.0, 0.0, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat difuzna_refleksija[] = { 0.0, 0.6, 0.0, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat spekularna_refleksija[] = { 1, 1, 1, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 40;

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
            glTranslatef(0, -1.45, 1000.2);
            glScalef(3.6, 1, 1.2);
            glutSolidCube(1);
    glPopMatrix();
}

void crtanje_lestvica(){    
    glDisable(GL_LIGHTING);
    unsigned int i = 0;
    for(i=0; i<1000; i++){
        GLfloat pozicija_svetla[] = { 1, 100, 1007, 0 };
        GLfloat ambijentalno_svetlo[] = { 0.35, 0.35, 0.35, 1 };
        GLfloat difuzno_svetlo[] = { 0.7, 0.7, 0.7, 1 };
        GLfloat spekularno_svetlo[] = { 0.9, 0.9, 0.9, 1 };
        
        GLfloat ambijentalna_refleksija[] = { 0.647059, 0.364706, 0.164706, 1 };
        GLfloat difuzna_refleksija[] = { 0.7, 0.55, 0.25, 1 };
        GLfloat spekularna_refleksija[] = { 1, 1, 1, 1 };

        GLfloat shininess = 20;
        
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, pozicija_svetla);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambijentalno_svetlo);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, difuzno_svetlo);
        glLightfv(GL_LIGHT0, GL_SPECULAR, spekularno_svetlo);

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentalna_refleksija);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, difuzna_refleksija);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spekularna_refleksija);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        
        glPushMatrix();
            glTranslatef(koordinata_x_lestvice[i], koordinata_y_lestvice, koordinata_z_lestvice[i]);
            glScalef(6, 0.5, 3);
            glutSolidCube(0.2);
        glPopMatrix();
    }
}

