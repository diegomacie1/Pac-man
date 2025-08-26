#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define COLUNAS 28
#define LINHAS 22
#define PACMAN 'C'
#define PAREDE '#'
#define VAZIO ' '
#define PONTO '-'
#define FANTASMA 'G'
#define ESPECIAL '*'


void iniciarMapa(char mapa[LINHAS][COLUNAS]);

void moverFantasma (char mapa[LINHAS][COLUNAS], int *fantasmaX, int *fantasmaY);

void printarMapa (char mapa[LINHAS][COLUNAS], int pontuacao);

void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY,  char direcao, int *pPontuacao);
//*pPontuacao é o que a função moverPacman usa para ter acesso ao endereço de memoria da pontuacao, assim a função pode modificar o valor da variavel pontuacao


//Função que inicaliza o mapa e define o mapa inicial quando o programa é iniciado 
void iniciarMapa(char mapa[LINHAS][COLUNAS]){

    char mapaInicial[LINHAS][COLUNAS] = {

        "############################",
        "#------------##------------#",
        "#-####-#####-##-#####-####-#",
        "#-####-#####-##-#####-####-#",
        "#--------------------------#",
        "#-####-##-########-##-####-#",
        "#------##----##----##------#",
        "######-#####-##-#####-######",
        "     #-##----------##-#     ",
        "######-##-###--###-##-######",
        "      -##-######-##-      ",
        "######-##-######-##-######",
        "     #-##----G-----##-#     ",
        "######-##-########-##-######",
        "#------------##------------#",
        "#-####-#####-##-#####-####-#",
        "#---##------C---------##---#",
        "###-##-##-########-##-##-###",
        "#------##----##----##------#",
        "#-##########-##-##########-#",
        "#--------------------------#",
        "############################",

    };
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            mapa[i][j] = mapaInicial[i][j];
        }
    }
}

//Função para printar o mapa e a pontuação na tela
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

//Função para mover o pacman
void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, char direcao, int *pPontuacao){
    int novoX = *pacmanX;
    int novoY = *pacmanY;

    //Recebe uma direcao podendo ser w s a d para mover o pacman 
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

    //Logica para impedir que o pacman atravesse paredes ou vá para fora do mapa
    if (novoX >= 0 && novoX < LINHAS && novoY >= 0 && novoY < COLUNAS && mapa[novoX][novoY] != PAREDE)
    {
        if (mapa[novoX][novoY] == PONTO) //está verificando a próxima posição para onde o Pac-Man vai se mover. Ela faz uma pergunta: "A próxima casa é um ponto (-)?"
        {
            *pPontuacao = *pPontuacao + 1;
            //pega o valor atual da pontuação (*pPontuacao) e soma 1 a esse valor
            //e entçao atualiza a variável original da pontuacao na função main com o novo valor.
        }
        //Atualiza o espaço que o Pacman passou para vazio
        mapa[*pacmanX][*pacmanY] = VAZIO;
        *pacmanX = novoX;
        *pacmanY = novoY;
        //Atualiza a posição para printar o pacman
        mapa[*pacmanX][*pacmanY] = PACMAN;        
    }
}

//Funcao para mover o fantasma
void moverFantasma (char mapa[LINHAS][COLUNAS], int *fantasmaX, int *fantasmaY){

    int novoX = *fantasmaX;
    int novoY = *fantasmaY;

    // pega valores aleatorios para usar como movimento pro fantasma
    int direcao = rand() % 4;

    switch(direcao){
        case 0:
            novoX = *fantasmaX - 1;
            break;
        case 1:
            novoX = *fantasmaX + 1;
            break;
        case 2:
            novoY = *fantasmaY - 1;
            break;
        case 3:
            novoY = *fantasmaY + 1;
            break;
    }
    // Verifica se o movimento do fantasma é valido
    if (novoX >= 0 && novoX < LINHAS && novoY >= 0 && novoY < COLUNAS && mapa[novoX][novoY] != PAREDE) {
        //Para não apagar o ponto onde o fantasma passar 
        mapa[*fantasmaX][*fantasmaY] = PONTO;
        *fantasmaX = novoX;
        *fantasmaY = novoY;
        //Atualiza a posição para printar o fantasma
        mapa[*fantasmaX][*fantasmaY] = FANTASMA;   

    }
}
int main (){
    char mapa[LINHAS][COLUNAS];
    int pacmanX, pacmanY, pontuacao = 0, fantasmaX, fantasmaY;
    char direcao;
    bool gameOver = false;

    srand(time(NULL));

    iniciarMapa(mapa);

    //loop para "encontrar" o pacman e o fantasma e saber a localizacao deles 
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            if (mapa[i][j] == PACMAN){
                pacmanX = i;
                pacmanY = j;
                break;
            }
            if (mapa[i][j] == FANTASMA){
                fantasmaX = i;
                fantasmaY = j;
                break;
            }
        }
    }

    //loop para o jogo rodar enquanto não houver colisao entra o pacman e o fantasma
    while (!gameOver)
    {
        printarMapa(mapa, pontuacao);

        direcao = getch();

        moverPacman(mapa, &pacmanX, &pacmanY, direcao, &pontuacao);
        moverFantasma(mapa, &fantasmaX, &fantasmaY);

        if(pacmanX == fantasmaX && pacmanY == fantasmaY){
            gameOver = true;
        }
    }

    system("cls");
    printf("\n    GAME OVER!\n\n");
    printf("    O fantasma te pegou!\n\n");

    return 0;
}