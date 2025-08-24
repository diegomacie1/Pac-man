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

void iniciarMapa(char mapa[LINHAS][COLUNAS]);

void printarMapa (char mapa[LINHAS][COLUNAS], int pontuacao);

void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY,  char direcao, int *pPontuacao);
//*pPontuacao é o que a função moverPacman usa para ter acesso ao endereço de memoria da pontuacao, assim a função pode modificar o valor da variavel pontuacao

int main (){
    char mapa[LINHAS][COLUNAS];
    int pacmanX, pacmanY, pontuacao = 0;
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
    printarMapa(mapa, pontuacao);

    direcao = getch();

    moverPacman(mapa, &pacmanX, &pacmanY, direcao, &pontuacao);
}

return 0;
}
void iniciarMapa(char mapa[LINHAS][COLUNAS]){

    char mapaInicial[LINHAS][COLUNAS] = {

        "#############",
        "#C----#-----#",
        "#-##-----##-#",
        "#-----#-----#",
        "#-##--#--##-#",
        "#-##--------#",
        "#-----#--##-#",
        "#-##--#-----#",
        "#-##--#--##-#",
        "#-----------#",
        "#-##--#--##-#",
        "#-##-----##-#",
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

void printarMapa (char mapa[LINHAS][COLUNAS], int pontuacao){
    system("cls");
    printf("\n=============\n");
    printf("\n  PONTOS:\n");
    printf("  %d\n", pontuacao); //espaços para uma melhor visualização
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++){
            printf("%c", mapa[i][j]);

        }   
        printf("\n");
    }
}

void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, char direcao, int *pPontuacao){
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
        if (mapa[novoX][novoY] == PONTO) //está verificando a próxima posição para onde o Pac-Man vai se mover. Ela faz uma pergunta: "A próxima casa é um ponto (-)?"
        {
            *pPontuacao = *pPontuacao + 1;
            //pega o valor atual da pontuação (*pPontuacao) e soma 1 a esse valor
            //e entçao atualiza a variável original da pontuacao na função main com o novo valor.
        }
        mapa[*pacmanX][*pacmanY] = VAZIO;
        *pacmanX = novoX;
        *pacmanY = novoY;
        mapa[*pacmanX][*pacmanY] = PACMAN;        
    }
}