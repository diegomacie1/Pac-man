#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>

#define COLUNAS 15
#define LINHAS 15
#define PACMAN 'C'
#define PAREDE '#'
#define VAZIO ' '
#define PONTO '-'
#define ESPECIAL '*'

void iniciarMapa(char mapa[LINHAS][COLUNAS]){

    char mapaInicial[LINHAS][COLUNAS] = {

        "#############",
        "#C----#-----#",
        "#-##--#--##-#",
        "#-##--#--##-#",
        "#-##--#--##-#",
        "#-----#-----#",
        "####-----####",
        "####-----####",
        "####-----####",
        "#-----#-----#",
        "#-##--#--##-#",
        "#-##--#--##-#",
        "#-##--#--##-#",
        "#-----#-----#",
        "#############",

    };
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            mapa[i][j] = mapaInicial[i][j];
        }
    }
}

void printarMapa (char mapa[LINHAS][COLUNAS]){
    system("cls");
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++){
            printf("%c", mapa[i][j]);

        }   
        printf("\n");
    }
}

void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY,  char direcao ){
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

    if (novoX >= 0 && novoX < LINHAS && novoY >= 0 && novoY < COLUNAS && mapa[novoX][novoY] != PAREDE)
    {
        mapa[*pacmanX][*pacmanY] = VAZIO;
        *pacmanX = novoX;
        *pacmanY = novoY;
        mapa[*pacmanX][*pacmanY] = PACMAN;
    }
    
         
}
int main (){
    char mapa[LINHAS][COLUNAS];
    int pacmanX, pacmanY;
    char direcao;

    iniciarMapa(mapa);

    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
        if (mapa[i][j] == PACMAN){
            pacmanX = i;
            pacmanY = j;
            break;
        }
    }
}

while (true)
{
    printarMapa(mapa);

    direcao = getch();

    moverPacman(mapa, &pacmanX, &pacmanY, direcao);
}

return 0;
}
