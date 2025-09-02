#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
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
#define FANTASMA 'G'


void iniciarMapa(char mapa[LINHAS][COLUNAS]);

void moverFantasma (char mapa[LINHAS][COLUNAS], int *fantasmaX, int *fantasmaY);

void printarMapa (char mapa[LINHAS][COLUNAS], int pontuacao);

char telaGameOver();

void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY,  char direcao, int *pPontuacao);
//*pPontuacao é o que a função moverPacman usa para ter acesso ao endereço de memoria da pontuacao, assim a função pode modificar o valor da variavel pontuacao

void resetarJogo (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *pontuacao, bool *gameOver, char *direcao);

int main (){
    char mapa[LINHAS][COLUNAS];
    int pacmanX, pacmanY, pontuacao = 0, fantasmaX, fantasmaY;
    char direcao;
    bool gameOver = false;

    srand(time(NULL));

    while(true) {

        resetarJogo(mapa, &pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &pontuacao, &gameOver, &direcao);

        while (!gameOver)
        {
            printarMapa(mapa, pontuacao);

            if (kbhit()) //A função kbhit retorna um valor que o C interpreta interpreta  como verdadeiro se uma tecla foi pressionada. Caso contrário ela retorna um valor falso.
            {
                direcao = getch();
                //a cada interação do loop ele verifica se uma tecla foi pressionada
                //se sim, ele atualiza a direção do movimento com a nova tecla que vc pressionou
                //se não, ele continua usando a ultima direção.
            }
            if(direcao == 'q'){
                gameOver = true;
                continue;
            }

            moverPacman(mapa, &pacmanX, &pacmanY, direcao, &pontuacao);
            moverFantasma(mapa, &fantasmaX, &fantasmaY);
            Sleep(100); //atraso no loop de 10 millisegundos, pausa o código por 10 milisegundos a cada movimento do pacman.
            if(pacmanX == fantasmaX && pacmanY == fantasmaY){
                gameOver = true;
            }
        }

        char escolha = telaGameOver();

        if (escolha == '2'){
            break;
        } else{
            continue;
        }

        return 0;
    }
}
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
        "     #-##-----------#-#     ",
        "######-##-######-##-#-######",
        "      -##-######-##---      ",
        "######-##-######-##---######",
        "     #-##----G------#-#     ",
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

void printarMapa (char mapa[LINHAS][COLUNAS], int pontuacao){
    CONSOLE_CURSOR_INFO cursorInfo; 
    //CONSOLE_CURSOR_INFO: estura de dados (struct), armazena informações do console, 'cursorInfo' é o nome da variável do tipo
    cursorInfo.dwSize = 100;
    //Um dos campos da variável cursorInfo. controla o tamanho ou espessura do cursor. 100 faz com que o cursor ocupe 100% da altura da linha do caractere, parecendo um bloco sólido
    cursorInfo.bVisible = FALSE;
    //bVisible valor booleano que controla a visibilidade do cursor
    // atribui FALSE (mesmo que zero para o pc) a este campo é que faz o cursor se tornar invisível
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    //aplica o que defini na variável 'cursorInfo'
    //SetConsoleCursorInfo: função que define a aparencia do cursor
    //GetStdHandle(STD_OUTPUT_HANDLE): apenas a forma do codigo dizer ao windows 'quero aplicar essa configuração no terminal que está mostrando a saída programa
    //&cursorInfo: & obtem o endereço da variavel cursorInfo. A função precisa do endereço para saber onde encontrar informações da configuração que defini

    COORD pos; //A estrutura COORD é a forma de passar as coordenadas X e Y para a função
    pos.X = 0; //ponto de partida do console no canto superior esqeuerdo da tela, o valor de X aumenta conforme você move o cursor para a direita
    pos.Y = 0; //ponto de partida do console no canto superior esqeuerdo da tela, o valor de Y aumenta conforme você move o cursor para a baixo
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //Move o cursor para a posição que você especifica

    printf("Pontos: %d\n", pontuacao); //espaços para uma melhor visualização
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

        //Teleporte horizontal
    if (novoY < 0) { // Se estiver tentando sair pela esquerda
        if (novoX == 10) {
                novoY = COLUNAS - 1;
            }
        } else if (novoY >= COLUNAS) { // Se estiver tentando sair pela direita
            if (novoX ==10) {
            novoY = 0;
            }
        }
    //Teleporte vertical
    if (novoX < 0) { // Se estiver tentando sair por cima
        if (novoY == 3 || novoY == 24) {
            novoX = LINHAS - 1;
        }
    } else if (novoX >= LINHAS) { // Se estiver tentando sair por baixo
        if (novoY == 3 || novoY == 24) {
            novoX = 0;
        }
    }

    if (novoX >= 0 && novoX < LINHAS && novoY >= 0 && novoY < COLUNAS && mapa[novoX][novoY] != PAREDE)
    {
        if (mapa[novoX][novoY] == PONTO) //está verificando a próxima posição para onde o Pac-Man vai se mover. Ela faz uma pergunta: "A próxima casa é um ponto (-)?"
        {
            *pPontuacao = *pPontuacao + 1;
            //pega o valor atual da pontuação (*pPontuacao) e soma 1 a esse valor
            //e entao atualiza a variável original da pontuacao na função main com o novo valor.

            PlaySound("eating.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);

            //funçao para tocar o som do pacman comendo
            //eating.wav nome do arquivo com som
        }
        mapa[*pacmanX][*pacmanY] = VAZIO;
        *pacmanX = novoX;
        *pacmanY = novoY;
        mapa[*pacmanX][*pacmanY] = PACMAN;        
    }
}

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
        mapa[*fantasmaX][*fantasmaY] = VAZIO;
        *fantasmaX = novoX;
        *fantasmaY = novoY;
        //Atualiza a posição para printar o fantasma
        mapa[*fantasmaX][*fantasmaY] = FANTASMA;   

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
    printf("      1 - Tentar Novamente\n");
    printf("      2 - Sair do Jogo\n");
    printf("\n\n      Escolha uma opcao: ");

    return getch();
}

void resetarJogo (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *pontuacao, bool *gameOver, char *direcao){

    iniciarMapa(mapa);
    *pontuacao = 0;
    *gameOver = false;
    *direcao = 'd';

    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            if (mapa[i][j] == PACMAN){
                *pacmanX = i;
                *pacmanY = j;
            }
            if(mapa[i][j] == FANTASMA){
                *fantasmaX = i;
                *fantasmaY = j;
            }
        }
    }

}