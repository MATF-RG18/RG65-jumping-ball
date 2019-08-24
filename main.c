#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include "image.h"


/*makro za sliku*/
#define FILENAME0 "more.bmp"

/*makro za razlikovanje ID kod timer funkcija i broj milisekundi na koliko se pozivaju takve funkcije*/
#define TIMER_ID 0
#define TIMER_ID1 1
#define TIMER_ID2 2
#define TIMER_INTERVAL 20

/*makro za maksimalan broj karaktera*/
#define MAX_BROJ_KARAKTERA 256

/*makro za gravitaciju*/
#define GRAVITACIJA 0.0001;



/* Deklaracije callback funkcija*/
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void skok_loptice(int value);
static void pad_loptice(int value);
static void on_display(void);

static void inicijalizacija(void); /*deklaracija pocetnih vrednosti*/

static void postavi_teksture(void); /*funkcija za dodavanje slike vode*/
void dodajTekst(void); /*funckija za dodavanje teksta*/
/*funckije za crtanje objekata na sceni*/
void crtanje_loptice(void);
void crtanje_postolja(void);
void crtanje_lestvica(void);

/* Identifikatori tekstura. */
static GLuint names[1];

/*globalne promenljive za poziciju i brzinu loptice i lestvica*/
static double koordinata_x_loptice, koordinata_y_loptice, koordinata_z_loptice;
static double koordinata_x_lestvice[1000], koordinata_z_lestvice[1000];
static double koordinata_y_lestvice;
static double brzina[1000];
static double brzina_lestvice;

static double brzina_loptice;
static double brzina_z, brzina_y;
static double sirina;

/*poziciju kamere na z osi*/
static double dubina;

/*sirina i visina ekrana*/
static int window_width, window_height;

/*promenljive za trenutni nivo i najbolji rezultat*/
static int nivo;
static int najbolji_rezultat;

/*identifikatorske promenljive*/
static int kretanje_lestvica, kretanje_loptice;
static int izgubio;
static int loptica_na_lestvici;
static int pad_sa_lestvice;

int main(int argc, char **argv){
    
    /* Inicijalizuje se GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor */
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    
    
    inicijalizacija();
    /* Registruju se callback funkcije */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija */
    glClearColor(0.74902, 0.847059, 0.847059, 0);
    glEnable(GL_DEPTH_TEST);
    
    postavi_teksture();

    /* Program ulazi u glavnu petlju */
    glutMainLoop();

    return 0;
}


static void inicijalizacija(){
    /*postavljanje pocetnih vrednosti promenljivima*/
    srand(time(NULL));
    
    koordinata_x_loptice = 0;
    koordinata_y_loptice = -0.8;
    koordinata_z_loptice = 1000.5;
    koordinata_y_lestvice = -1;
    brzina_lestvice = 0.03;

    brzina_loptice = 0.015;
    izgubio = 0;
    nivo = 0;
    dubina = 1007;    
    double pomeraj = 1.5;
    sirina = 1.2;
    unsigned int i = 0;
    koordinata_z_lestvice[0] = 999;
    
    for(i = 0; i<1000; i++){
        /*postavljanje vrednosti na z osi lestvicama*/
        if(i != 0)
            koordinata_z_lestvice[i] = koordinata_z_lestvice[i-1] - pomeraj;
        double random_broj = rand()/(float)RAND_MAX;
        /*na slucajan nacin se bira x koordinata lestvicama i pravac kretanja*/
        int pozicija_x_koordinate;
        if(random_broj >=0.5)
            pozicija_x_koordinate = 1;
        else
            pozicija_x_koordinate = -1;
        double random_smer = rand()/(float)RAND_MAX;
        if(random_smer >= 0.5)
            brzina[i] = brzina_lestvice;
        else
            brzina[i] = -brzina_lestvice;
        /*postavljanje brzine kretanja lestvica*/
        if(i != 0 && (i % 10) == 0){
            brzina_lestvice += 0.005;
            sirina -= 0.001;
        }

        koordinata_x_lestvice[i] = 2*pozicija_x_koordinate*rand()/(float)RAND_MAX;
    }
    
    kretanje_lestvica = 0;
    kretanje_loptice = 0;
    loptica_na_lestvici = 0;
    pad_sa_lestvice = 0;
    
    brzina_y = 0.06;
    brzina_z = -0.1;
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
        /*kretanje loptice u levu stranu, obrada slucaja ako loptica sklizne sa lestvice*/
        koordinata_x_loptice -= brzina_loptice;
        if(loptica_na_lestvici && koordinata_x_loptice < koordinata_x_lestvice[nivo-1] - sirina/2 - 0.1 
            && !pad_sa_lestvice){
            izgubio = 1;
            pad_sa_lestvice = 1;
            glutTimerFunc(TIMER_INTERVAL, pad_loptice, TIMER_ID2);
        }
        if(!pad_sa_lestvice){
            glutPostRedisplay();
        }
        break;
    case 'd':
    case 'D':
        /*kretanje loptice u desnu stranu, obrada slucaja ako loptica sklizne sa lestvice*/
        koordinata_x_loptice += brzina_loptice;
        if(loptica_na_lestvici && koordinata_x_loptice > koordinata_x_lestvice[nivo-1] + sirina/2 + 0.1
            && !pad_sa_lestvice){
            izgubio = 1;
            pad_sa_lestvice = 1;
            glutTimerFunc(TIMER_INTERVAL, pad_loptice, TIMER_ID2);
        }
        if(!pad_sa_lestvice){
            glutPostRedisplay();
        }
        break;
    case 'w':
    case 'W':
        /*skok loptice*/
        if(!kretanje_loptice && kretanje_lestvica) {
            loptica_na_lestvici = 0;
            glutTimerFunc(TIMER_INTERVAL, skok_loptice, TIMER_ID1);
            kretanje_loptice = 1;
        }
        break;
    case 'r':
    case 'R':
        /*igra krece ispocetka*/
        inicijalizacija();
        glutPostRedisplay();
        break;
    case 'p':
    case 'P':
        /*igra se pauzira*/
        kretanje_lestvica = 0;
        kretanje_loptice = 0;
        glutPostRedisplay();
        break;
    }
}

static void on_reshape(int width, int height){
    window_width = width;
    window_height = height;
}


static void on_display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* Podesava se vidna tacka. */
    glViewport(0, 0, window_width, window_height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (float) window_width / window_height, 1, 1000);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, dubina, 
              0, 0, 0, 
              0, 1, 0);
    
    glEnable(GL_DEPTH_TEST);
    
    /*pozivanje funkcija za crtanje objekata*/
    crtanje_loptice();
    crtanje_postolja();
    
    /*postavljanje slike vode*/
    unsigned int i = 1000;
    while(i>= 20){
            
        glBindTexture(GL_TEXTURE_2D, names[0]);
        glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-2, -1, i);

            glTexCoord2f(1, 0);
            glVertex3f(2, -1, i);

            glTexCoord2f(1, 1);
            glVertex3f(2, -1, i-20);

            glTexCoord2f(0, 1);
            glVertex3f(-2, -1, i-20);
        glEnd();
        i = i - 20;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    
    crtanje_lestvica();
    dodajTekst();
    
    
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}

/*funkcije za kretanje lestvica*/
static void on_timer(int value){
    
    if (value != TIMER_ID)
        return;
    unsigned int i = 0;
    /*ukoliko lestvica dodje do ivice ekrana, menja smer kretanja*/
    for(i = 0; i<1000; i++){
        koordinata_x_lestvice[i] += brzina[i];
        
        if(koordinata_x_lestvice[i] + sirina/2 >= 1.95 || koordinata_x_lestvice[i] - sirina/2 <= -1.95)
            brzina[i] *= -1;
    }
    
    /*ako je loptica na lestvici, loptica se krece zajedno sa lestvicom*/
    if(loptica_na_lestvici){
        koordinata_x_loptice += brzina[nivo-1];
    }
    glutPostRedisplay();
    
    if (kretanje_lestvica) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}

/*funkcija za skok loptice*/
static void skok_loptice(int value){
    
    if (value != TIMER_ID1)
        return;
    
    if(!izgubio){
        /*pomeranje loptice po y i z osi* i priblizivanje kamere*/
        dubina -= 0.1;
        if(koordinata_z_loptice - koordinata_z_lestvice[nivo] > 0.75){
            koordinata_y_loptice += brzina_y - GRAVITACIJA;
            koordinata_z_loptice += brzina_z;
        }
        else{
            koordinata_y_loptice -= brzina_y - GRAVITACIJA;
            koordinata_z_loptice += brzina_z;
        }
    }
    
    /*detekcija sudara loptice i lestvice*/
    if (koordinata_z_loptice - koordinata_z_lestvice[nivo] < 0.01) {
        kretanje_loptice = 0;
        
        koordinata_y_loptice -= brzina_y;
        if(!(koordinata_x_loptice <= koordinata_x_lestvice[nivo] + sirina/2 && 
           koordinata_x_loptice >= koordinata_x_lestvice[nivo] - sirina/2)){
            if(koordinata_y_lestvice >= -1){
                koordinata_y_loptice -= 0.05;
                izgubio = 1;
                if(najbolji_rezultat <= nivo){
                    najbolji_rezultat = nivo;
                }
                glutTimerFunc(TIMER_INTERVAL, skok_loptice, TIMER_ID1);
            }
            else{
                inicijalizacija();
                glutPostRedisplay();
            }
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


/*funkcija za obradu pada loptice sa lestvice*/
static void pad_loptice(int value){
    if (value != TIMER_ID2)
        return;
    /*propadanje loptice u vodu*/
    if(koordinata_y_loptice >= -2){
        koordinata_y_loptice -= 0.05;
        glutTimerFunc(TIMER_INTERVAL, pad_loptice, TIMER_ID2);
    } else {
        /*azuriranje najboljeg rezultata*/
        if(najbolji_rezultat <= nivo){
            najbolji_rezultat = nivo;
        }
        inicijalizacija();
        glutPostRedisplay();
    }
    
}

/*funckije za crtanje objekata na sceni*/

/*funckija za crtanje loptice*/
void crtanje_loptice(){
    /* Pozicija svetla */
    GLfloat pozicija_svetla[] = { 1, 1, 1007, 0 };

    /* Ambijentalna boja svetla */
    GLfloat ambijentalno_svetlo[] = { 0.35, 0.35, 0.35, 1 };

    /* Difuzna boja svetla */
    GLfloat difuzno_svetlo[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla */
    GLfloat spekularno_svetlo[] = { 0.9, 0.9, 0.9, 1 };

    /* Koeficijenti ambijentalne refleksije materijala */
    GLfloat ambijentalna_refleksija[] = { 1.0, 1.0, 0.1, 1 };

    /* Koeficijenti difuzne refleksije materijala */
    GLfloat difuzna_refleksija[] = { 1.0, 1.0, 0.0, 1 };

    /* Koeficijenti spekularne refleksije materijala */
    GLfloat spekularna_refleksija[] = { 1, 1, 1, 1 };

    /* Koeficijent glatkosti materijala */
    GLfloat shininess = 20;

    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pozicija_svetla);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambijentalno_svetlo);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuzno_svetlo);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spekularno_svetlo);

    /* Podesavaju se parametri materijala */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijentalna_refleksija);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuzna_refleksija);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularna_refleksija);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    /*obrada slucaja kad loptica dotakne ivicu ekrana*/
    if(koordinata_x_loptice + 0.15 >= 1.9 || koordinata_x_loptice - 0.15 <= -1.9){
        if(najbolji_rezultat <= nivo){
            najbolji_rezultat = nivo;
        }
        izgubio = 1;
        inicijalizacija();
        glutPostRedisplay();
    }
    /*crtanje loptice*/
    glPushMatrix();
        glTranslatef(koordinata_x_loptice, koordinata_y_loptice, koordinata_z_loptice);
        glutSolidSphere(0.15, 500, 500);
    glPopMatrix();
        
}

/*crtanje postolja na kom loptica na pocetku stoji*/
void crtanje_postolja(){
    /*Iskljucivanje osvetljena*/
    glDisable(GL_LIGHTING);
    
    GLfloat pozicija_svetla[] = { 1, 200, 1007, 1 };

    GLfloat ambijentalno_svetlo[] = { 0.35, 0.35, 0.35, 1 };
    GLfloat difuzno_svetlo[] = { 0.75, 0.75, 0.75, 1 };
    GLfloat spekularno_svetlo[] = { 0.9, 0.9, 0.9, 1 };
    
    GLfloat ambijentalna_refleksija[] = { 0.0, 1.0, 0.0, 1 };
    GLfloat difuzna_refleksija[] = { 0.0, 0.6, 0.0, 1 };
    GLfloat spekularna_refleksija[] = { 1, 1, 1, 1 };

    GLfloat shininess = 40;

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
    
    /*crtanje 3 kvadra*/
    glPushMatrix();
            glTranslatef(0, -1.45, 1000.2);
            glScalef(7, 1, 1.2);
            glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
            glTranslatef(-4, -1.45, 500);
            glScalef(2, 1, 1000);
            glutSolidCube(1);
    glPopMatrix();
        glPushMatrix();
            glTranslatef(4, -1.45, 500);
            glScalef(2, 1, 1000);
            glutSolidCube(1);
    glPopMatrix();
}


/*crtanje lestvica po kojima loptica skace*/
void crtanje_lestvica(){    
    glDisable(GL_LIGHTING);
    /*Podesavanje osvetljenja za sve loptice*/
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
        /*crtanje lestvica*/
        glPushMatrix();
            glTranslatef(koordinata_x_lestvice[i], koordinata_y_lestvice, koordinata_z_lestvice[i]);
            glScalef(6, 0.5, 3);
            glutSolidCube(0.2);
        glPopMatrix();
    }
}


/*funkcija za dodavanje teksta*/
void dodajTekst(void){
    glDisable(GL_LIGHTING);
    
    /*niska u kojima se cuva tekst*/
    char tekst1[MAX_BROJ_KARAKTERA], *p1;
    /*upisivanje teksta u promenljivu*/
    sprintf(tekst1, "Poeni:");
    
    char tekstZaNivo[MAX_BROJ_KARAKTERA], *p2;
    sprintf(tekstZaNivo, "%d", nivo);
    
    /*pisanje jednog po jednog teksta pomocu funkcije glutBitmapCharacter*/
    glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(-2, 0.9, dubina-7);
        glRasterPos3f(0.2, 0.7, 0);
        for(p1 = tekst1; *p1!= '\0'; p1++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p1);
        }
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(-1.7, 0.9, dubina-7);
        glRasterPos3f(0.2, 0.7, 0);
        for(p2 = tekstZaNivo; *p2!= '\0'; p2++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p2);
        }
    glPopMatrix();
    
    char tekst_za_najbolji_rez[MAX_BROJ_KARAKTERA], *p3;
    sprintf(tekst_za_najbolji_rez, "Najbolji rezultat:");
    
    glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(-2, 0.8, dubina-7);
        glRasterPos3f(0.2, 0.7, 0);
        for(p3 = tekst_za_najbolji_rez; *p3!= '\0'; p3++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p3);
        }
    glPopMatrix();
    
    char rez[MAX_BROJ_KARAKTERA], *p4;
    sprintf(rez, "%d", najbolji_rezultat);
    
    glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(-1.3, 0.8, dubina-7);
        glRasterPos3f(0.2, 0.7, 0);
        for(p4 = rez; *p4!= '\0'; p4++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p4);
        }
    glPopMatrix();
    
    
    char tekst_za_kraj[MAX_BROJ_KARAKTERA], *p5;
    sprintf(tekst_za_kraj, "IZGUBILI STE!");
    
    if(izgubio){
        glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(-0.5, 0, dubina-7);
        glRasterPos3f(0.2, 0.7, 0);
        for(p5 = tekst_za_kraj; *p5!= '\0'; p5++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p5);
        }
        glPopMatrix();
    
    }
    
}

static void postavi_teksture(void){
    /* Objekat koji predstavlja teskturu ucitanu iz fajla*/
    Image * image;
    
    /* Ukljucuju se teksture*/
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz fajla*/
    image = image_init(0, 0);

    /* Kreira se prva tekstura */
    image_read(image, FILENAME0);

    /* Generisu se identifikatori tekstura */
    glGenTextures(1, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    

    /* Iskljucuje se aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla*/
    image_done(image);
}

