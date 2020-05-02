//-------------------------------------
// Exercício: Space Invaders - CG - 2020/1
// Dupla: Larissa Magistrali e Nikolas Lacerda
//-------------------------------------
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;
#ifdef WIN32
#include <windows.h>
#include <glut.h>
static DWORD last_idle_time;
#else
#include <sys/time.h>
static struct timeval last_idle_time;
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <glut.h>
#endif
#include "ImageClass.h"
#include "SOIL/SOIL.h"
#include <fstream>
#include <dirent.h>
#include <string>

//------------------------JOGO---------------------------------
//structs
typedef struct{
    float r, g, b, alpha; //alpha = transparencia
} Cor;

typedef struct{
    int altura;
    int largura;
    Cor cores[100][100];
} ModeloDeObjeto;

//variaveis
int numeroDeVidas=3;
float PosDisparadorX = 50;
ImageClass Image;
ModeloDeObjeto modeloDisparador, modeloNave1, modeloNave2, modeloNave3, modeloNave4;

//objetos
void LeArquivoModelo(ModeloDeObjeto &modelo, char *path){
    std::ifstream infile(path);
    int numeroDeCores, indiceCor = 0;
    infile >> numeroDeCores;
    Cor ListaCores[numeroDeCores] = {};
    while (indiceCor < numeroDeCores){
        Cor c;
        infile >> indiceCor >> c.r >> c.g >> c.b >> c.alpha;
        ListaCores[indiceCor - 1] = c;
    }
    int largura, altura,i, j, cor = 0;
    infile >> largura >> altura;
    modelo.altura = altura;
    modelo.largura = largura;
    Cor Matriz[altura][largura] = {};
    for (i = 0; i < altura; i++) {
        for (j = 0; j < largura; j++){
            infile >> cor;
            Matriz[i][j] = ListaCores[cor - 1];
        }
    }
    for (i = 0; i < altura; i++){
        for (j = 0; j < largura; j++) {
            modelo.cores[i][j] = Matriz[i][j];
        }
    }
    infile.close();
}

void DesenhaNaves(){}

void DesenhaDisparador(){}


//projetil
void DesenhaProjetil(){
    glPushMatrix();
        glLoadIdentity();
        glColor3f(1,0.2,0.2);
        glTranslatef(10, 10, 0);
        glBegin(GL_QUADS); //meio = (0,0)
            glVertex2f(1,0);
            glVertex2f(0,0);
            glVertex2f(0,1);
            glVertex2f(1,1);
        glEnd();
    glPopMatrix();
}

//mostra vidas


//chão
void DesenhaChao(){
    glPushMatrix();
        glLoadIdentity();
        glColor3f(0.7,0,0.3);
        glTranslatef(1, 1, 0);
        glBegin(GL_QUADS);
            glVertex2f(98,0);
            glVertex2f(0,0);
            glVertex2f(0,2);
            glVertex2f(98,2);
        glEnd();
    glPopMatrix();
}


//outras funcoes
void DesenhaEixos(){
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2d(0, 25);
    glVertex2d(100, 25);
    glVertex2d(50, 0);
    glVertex2d(50, 50);
    glEnd();
}


//comandos
void arrow_keys(int a_keys, int x, int y){
    switch (a_keys){
    case GLUT_KEY_RIGHT:
        PosDisparadorX++;
        break;
    case GLUT_KEY_LEFT:
        PosDisparadorX--;
        break;
    default:
        break;
    }
}

void keyboard(unsigned char key, int x, int y){
    switch (key){
    case 27:     // Termina o programa qdo
        exit(0); // a tecla ESC for pressionada
        break;
    case ' ':
        printf("barra de espaço = atirar"); //teste
    default:
        break;
    }
}


//padrao
void init(void){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
    int r;
    string nome = "./JogoImagens/cenario.png";
    r = Image.Load(nome.c_str());
    if (!r) exit(1); // Erro na carga da imagem

    //carrega modelos
    LeArquivoModelo(modeloDisparador,"./JogoArquivos/Disparador.txt");
    LeArquivoModelo(modeloNave1,"./JogoArquivos/NaveAzul.txt");
    LeArquivoModelo(modeloNave2,"./JogoArquivos/NaveRosa.txt");
    LeArquivoModelo(modeloNave3,"./JogoArquivos/NaveRoxa.txt");
    LeArquivoModelo(modeloNave4,"./JogoArquivos/NaveVermelha.txt");
}

void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);// Reset the coordinate system before modifying
    glLoadIdentity();
    glViewport(0, 0, w, h);// Define a area a ser ocupada pela �rea OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);// Define os limites l�gicos da �rea OpenGL dentro da Janela
    glLoadIdentity();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT); // Limpa a tela coma cor de fundo
    glMatrixMode(GL_PROJECTION);// Define os limites logicos da area OpenGL dentro da Janela
    glLoadIdentity();
    glOrtho(0, 100, 0, 50, -1, 1);// Define os limites logicos da area OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH))/(double)Image.SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(double)Image.SizeY();
    Image.SetZoomH(zoomH);
    Image.SetZoomV(zoomV);
    Image.SetPos(0, 0);
    Image.Display();

    DesenhaChao();
    DesenhaProjetil(); //test

    glutSwapBuffers();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 450);
    glutCreateWindow("SpaceInvaders-Larissa-Nikolas");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);
    glutMainLoop();
    return 0;
}
