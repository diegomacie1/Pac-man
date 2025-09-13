#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <mmsystem.h>

#define COLUNAS 28
#define LINHAS 22
#define PACMAN 'C'
#define PAREDE '#'
#define VAZIO ' '
#define PONTO '-'
#define ESPECIAL '*'
#define FANTASMA 'M'

void iniciarMapa(char mapa[LINHAS][COLUNAS]);

void moverFantasma (char mapa[LINHAS][COLUNAS], int *fantasmaX, int *fantasmaY, int *pacmanX, int *pacmanY, bool *fantasmaParado, clock_t *tempoParado);

void printarMapa (char mapa[LINHAS][COLUNAS], int pontuacao, int vidas);

char telaGameOver();

void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY,  char direcao, int *pPontuacao, int *pPontosdenivel);

void resetarJogo (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *pontos, int *pontosdenivel, char *direcao, int *fantasmaX2, int *fantasmaY2);

void posicionarPersonagens(int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *fantasmaX2, int *fantasmaY2);

char mapaDinamico[LINHAS][COLUNAS];
bool medoFantasma = false;
clock_t tempoMedo;
const int DURACAO_MEDO = 15; 
bool fantasmaParado1 = false;
clock_t tempoParado1;
bool fantasmaParado2 = false;
clock_t tempoParado2;
const int DURACAO_ESPERA = 5; 

int main (){
    char mapa[LINHAS][COLUNAS];
    int pacmanX, pacmanY, pontos = 0, fantasmaX, fantasmaY, pacmanOldX, pacmanOldY, fantasmaOldX, fantasmaOldY, vidas, pontosdenivel, fantasmaX2, fantasmaY2, fantasmaOldX2, fantasmaOldY2;
    char direcao;

    srand(time(NULL));

    while(true) {
        vidas = 3;

        resetarJogo(mapa, &pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &pontos, &pontosdenivel, &direcao, &fantasmaX2, &fantasmaY2);
        
        while (vidas > 0)
        {
            printarMapa(mapa, pontos, vidas);

            if (kbhit())
            {
                direcao = getch();
            }
            if(direcao == 'q'){
                vidas = 0;
            }
            
            if (pontosdenivel <= 0)
            {
                Sleep(2000);
                resetarJogo(mapa, &pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &pontos, &pontosdenivel, &direcao, &fantasmaX2, &fantasmaY2);
                vidas++;
            }
            pacmanOldX = pacmanX;
            pacmanOldY = pacmanY;
            fantasmaOldX = fantasmaX;
            fantasmaOldY = fantasmaY;
            fantasmaOldX2 = fantasmaX2;
            fantasmaOldY2 = fantasmaY2;

            moverPacman(mapa, &pacmanX, &pacmanY, direcao, &pontos, &pontosdenivel);
            moverFantasma(mapa, &fantasmaX, &fantasmaY, &pacmanX, &pacmanY, &fantasmaParado1, &tempoParado1);
            moverFantasma(mapa, &fantasmaX2, &fantasmaY2, &pacmanX, &pacmanY, &fantasmaParado2, &tempoParado2);

            Sleep(150); 
            if ((pacmanX == fantasmaX && pacmanY == fantasmaY) || (pacmanX == fantasmaOldX && pacmanY == fantasmaOldY && pacmanOldX == fantasmaX && pacmanOldY == fantasmaY)){
                if (medoFantasma) {
                    pontos += 200; 
                    mapaDinamico[fantasmaX][fantasmaY] = VAZIO;
                    fantasmaX = 10;
                    fantasmaY = 13;
                    fantasmaParado1 = true;
                    tempoParado1 = clock();
        
                    mapaDinamico[fantasmaX][fantasmaY] = FANTASMA;
                } else {
                    vidas--;

                    mapaDinamico[pacmanX][pacmanY] = VAZIO;
                    mapaDinamico[fantasmaX][fantasmaY] = VAZIO;
                    mapaDinamico[fantasmaX2][fantasmaY2] = VAZIO;
                    PlaySound("die.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
                    Sleep(2500);
                    posicionarPersonagens(&pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &fantasmaX2, &fantasmaY2);
                    direcao = 'd';
                    continue; 
                }
            }
            if ((pacmanX == fantasmaX2 && pacmanY == fantasmaY2) || (pacmanX == fantasmaOldX2 && pacmanY == fantasmaOldY2 && pacmanOldX == fantasmaX2 && pacmanOldY == fantasmaY2)) {
                if (medoFantasma) {
                    pontos += 200;
                    mapaDinamico[fantasmaX2][fantasmaY2] = VAZIO;
                    fantasmaX2 = 10;
                    fantasmaY2 = 14;
                    fantasmaParado2 = true;
                    tempoParado2 = clock();

                    mapaDinamico[fantasmaX2][fantasmaY2] = FANTASMA;
                } else {
                    vidas--;
                    mapaDinamico[pacmanX][pacmanY] = VAZIO;
                    mapaDinamico[fantasmaX][fantasmaY] = VAZIO;
                    mapaDinamico[fantasmaX2][fantasmaY2] = VAZIO;
                    PlaySound("die.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
                    Sleep(2500);
                    posicionarPersonagens(&pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &fantasmaX2, &fantasmaY2);
                    direcao = 'd';
                    continue;
                }
            }
        }
        char escolha = telaGameOver();
        if (escolha == '2'){
            break;
        } else{
            pontos = 0;
        }
    }
    return 0;
}
void iniciarMapa(char mapa[LINHAS][COLUNAS]){

    char mapaInicial[LINHAS][COLUNAS] = {

        "### #################### ###",
        "#----#-------##-------#----#",
        "#-##-#-#####-##-#####-#-##-#",
        "#-##-#-#####-##-#####-#-##-#",
        "#*##---##----------##---##*#",
        "#-####----########----####-#",
        "#------##----##----##------#",
        "######-#####-##-#####-####-#",
        "##-------           -------#",
        "######-##  ###  ### ##-#####",
        "---------           --------",
        "#-#######  ###  ### ######-#",
        "#---#####           ####---#",
        "###-#####-########-#####-###",
        "#------------##------------#",
        "#-####-#####-##-#####-####-#",
        "#---##------ ##-------##---#",
        "#-#######-########-#######-#",
        "#*-----##----------##-----*#",
        "#-####----########----####-#",
        "#------##----------##------#",
        "### #################### ###",
    };
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            mapa[i][j] = mapaInicial[i][j];
            mapaDinamico[i][j] = VAZIO; 
        }
    }
}
void printarMapa (char mapa[LINHAS][COLUNAS], int pontos, int vidas){
    
    CONSOLE_CURSOR_INFO cursorInfo; 
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    COORD pos; 
    pos.X = 0; 
    pos.Y = 0; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 

    printf("Pontos: %d\n", pontos);
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            if (mapaDinamico[i][j] == PACMAN){
                printf("%c", mapaDinamico[i][j]);
            } else if (mapaDinamico[i][j] == FANTASMA) {
                if (medoFantasma) {
                    printf("w"); 
                } else {
                    printf("%c", mapaDinamico[i][j]); 
                }
            } else {
                printf("%c", mapa[i][j]);
            }
        }   
        printf("\n");
    }

    printf("\n");

    for (int i = 0; i < vidas; i++) 
    {
        printf("C ");
    }

    for (int i = 0; i < 3 - vidas; i++)
    {
        printf("  ");
    }
    printf("\n\n\n");
}

//Função para mover o pacman
void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, char direcao, int *pPontuacao, int *pPontosdenivel){ 
    int novoX = *pacmanX;
    int novoY = *pacmanY;

    switch(direcao){
        case 'w':
            novoX = *pacmanX - 1;
            break;
        case 's':
            novoX = *pacmanX + 1;
            break;
        case 'a':
            novoY = *pacmanY - 1;
            break;
        case 'd':
            novoY = *pacmanY + 1;
            break;
    }
    if (novoY < 0) { 
        if (novoX == 10) {
                novoY = COLUNAS - 1;
            }
        } else if (novoY >= COLUNAS) {
            if (novoX ==10) {
            novoY = 0;
            }
        }
    if (novoX < 0) { 
        if (novoY == 3 || novoY == 24) {
            novoX = LINHAS - 1;
        }
    } else if (novoX >= LINHAS) { 
        if (novoY == 3 || novoY == 24) {
            novoX = 0;
        }
    }
    if (novoX >= 0 && novoX < LINHAS && novoY >= 0 && novoY < COLUNAS && mapa[novoX][novoY] != PAREDE)
    {
        if (mapa[novoX][novoY] == PONTO)
        {
            *pPontuacao += 10;

            *pPontosdenivel-=1; 

            mapa[novoX][novoY] = VAZIO; 

            PlaySound("pellet.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);

        }else if (mapa[novoX][novoY] == ESPECIAL) { 
            *pPontuacao += 100;
            PlaySound("specialT.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
            medoFantasma = true; 
            tempoMedo = clock(); 
            mapa[novoX][novoY] = VAZIO;
        }
        mapaDinamico[*pacmanX][*pacmanY] = VAZIO; 
        *pacmanX = novoX;
        *pacmanY = novoY;
        mapaDinamico[*pacmanX][*pacmanY] = PACMAN;       
    }
}

void moverFantasma (char mapa[LINHAS][COLUNAS], int *fantasmaX, int *fantasmaY, int *pacmanX, int *pacmanY, bool *fantasmaParado, clock_t *tempoParado){
    if (*fantasmaParado && (clock() - *tempoParado) / CLOCKS_PER_SEC < DURACAO_ESPERA) {
        return;
    } else if (*fantasmaParado && (clock() - *tempoParado) / CLOCKS_PER_SEC >= DURACAO_ESPERA) {
        *fantasmaParado = false;
    }
    if (medoFantasma && (clock() - tempoMedo) / CLOCKS_PER_SEC >= DURACAO_MEDO) {
        medoFantasma = false;
    }
    
    int novoX = *fantasmaX; 
    int novoY = *fantasmaY; 

    int distX = *pacmanX - *fantasmaX;
    int distY = *pacmanY - *fantasmaY;

    if (medoFantasma) { 
        if (abs(distX) > abs(distY)) {
            if (distX > 0) { 
                novoX = *fantasmaX - 1;
            } else { 
                novoX = *fantasmaX + 1;
            }
        } else {
            if (distY > 0) { 
                novoY = *fantasmaY - 1;
            } else { 
                novoY = *fantasmaY + 1;
            }
        }
    } else { 
        if (abs(distX) > abs(distY)) {
            if (distX > 0) {
                novoX = *fantasmaX + 1;
            } else {
                novoX = *fantasmaX - 1; 
            }
        } else {
            if (distY > 0) {
                novoY = *fantasmaY + 1;
            } else {
                novoY = *fantasmaY - 1; 
            }
        }
    }
    if (novoX < 0 || novoX >= LINHAS || novoY < 0 || novoY >= COLUNAS || mapa[novoX][novoY] == PAREDE) {
        novoX = *fantasmaX; 
        novoY = *fantasmaY; 
        
        if (abs(distX) > abs(distY)) { 
            if (distY > 0) { 
                novoY = *fantasmaY + 1;
            } else {
                novoY = *fantasmaY - 1; 
            }
        } else {
            if (distX > 0) {
                novoX = *fantasmaX + 1;
            } else {
                novoX = *fantasmaX - 1; 
            }
        }
    }
    if (novoX < 0 || novoX >= LINHAS || novoY < 0 || novoY >= COLUNAS || mapa[novoX][novoY] == PAREDE) {
        int direcao = rand() % 4;
        
        switch(direcao){
            case 0: novoX = *fantasmaX - 1; break;
            case 1: novoX = *fantasmaX + 1; break;
            case 2: novoY = *fantasmaY - 1; break;
            case 3: novoY = *fantasmaY + 1; break;
        }
    }
    if (novoY < 0) { 
        if (novoX == 10) {
                novoY = COLUNAS - 1;
            }
        } else if (novoY >= COLUNAS) { 
            if (novoX == 10) {
            novoY = 0;
            }
        }
    if (novoX < 0) { 
        if (novoY == 3 || novoY == 24) {
            novoX = LINHAS - 1;
        }
    } else if (novoX >= LINHAS) { 
        if (novoY == 3 || novoY == 24) {
            novoX = 0;
        }
    }
    if (novoX >= 0 && novoX < LINHAS && novoY >= 0 && novoY < COLUNAS && mapa[novoX][novoY] != PAREDE) {
        mapaDinamico[*fantasmaX][*fantasmaY] = VAZIO; 
        *fantasmaX = novoX;
        *fantasmaY = novoY;
        mapaDinamico[*fantasmaX][*fantasmaY] = FANTASMA;
    }
}

char telaGameOver(){

    system("cls");
    printf("\n\n\n\n");
    printf("        ###################\n");
    printf("        #                 #\n");
    printf("        #    GAME OVER    #\n");
    printf("        #                 #\n");
    printf("        ###################\n");
    printf("\n\n");
    printf("\n\n      Selecione: \n\n");
    printf("      1 - Tentar Novamente\n");
    printf("\n      2 - Sair do Jogo\n");

    Sleep(1000); 

    while (kbhit()) {
        getch();
    }

    return getch();
}

void resetarJogo (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *pontos, int *pontosdenivel, char *direcao, int *fantasmaX2, int *fantasmaY2){

    iniciarMapa(mapa);

    if (*pontos == 0) {
        *pontos = 0;
        posicionarPersonagens(pacmanX, pacmanY, fantasmaX, fantasmaY, fantasmaX2, fantasmaY2);
        printarMapa(mapa, *pontos, 3);
        PlaySound("begin.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
        Sleep(1300);

    }
    *pontosdenivel = 0;
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (mapa[i][j] == PONTO) {
                (*pontosdenivel)++;
            }
        }
    }

    *direcao = 'd';

    posicionarPersonagens(pacmanX, pacmanY, fantasmaX, fantasmaY, fantasmaX2, fantasmaY2);

}
void posicionarPersonagens(int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *fantasmaX2, int *fantasmaY2){
    
    *pacmanX = 16;
    *pacmanY = 12;
    *fantasmaX = 10;
    *fantasmaY = 13;
    *fantasmaX2 = 10;
    *fantasmaY2 = 14; 
    
    mapaDinamico[*pacmanX][*pacmanY] = PACMAN;
    mapaDinamico[*fantasmaX][*fantasmaY] = FANTASMA;
    mapaDinamico[*fantasmaX2][*fantasmaY2] = FANTASMA;
}
