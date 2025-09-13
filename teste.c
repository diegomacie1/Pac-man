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
#define MAX_VIDAS 10

void iniciarMapa(char mapa[LINHAS][COLUNAS]);

void moverFantasma (char mapa[LINHAS][COLUNAS], int *fantasmaX, int *fantasmaY, int *pacmanX, int *pacmanY, bool *fantasmaParado, clock_t *tempoParado);

void printarMapa (char mapa[LINHAS][COLUNAS], int pontuacao, int vidas);

char telaGameOver();

void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY,  char direcao, int *pPontuacao, int *pPontosdenivel);

void resetarJogo (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *pontos, int *pontosdenivel, char *direcao, int *fantasmaX2, int *fantasmaY2, int *vidas);

void posicionarPersonagens(int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *fantasmaX2, int *fantasmaY2);

//variaveis globais:
//para a segunda camada do mapa
char mapaDinamico[LINHAS][COLUNAS];
//Para controlar o estado de medo
bool medoFantasma = false;
clock_t tempoMedo;
const int DURACAO_MEDO = 15; // Segundos
// Variaveis para o tempo de espera do fantasma na base
bool fantasmaParado1 = false;
clock_t tempoParado1;
bool fantasmaParado2 = false;
clock_t tempoParado2;
const int DURACAO_ESPERA = 5; // Segundos

int main (){
    char mapa[LINHAS][COLUNAS];
    int pacmanX, pacmanY, pontos = 0, fantasmaX, fantasmaY, pacmanOldX, pacmanOldY, fantasmaOldX, fantasmaOldY, vidas, pontosdenivel, fantasmaX2, fantasmaY2, fantasmaOldX2, fantasmaOldY2;
    char direcao;

    srand(time(NULL));

    while(true) {
        vidas = 3;

        resetarJogo(mapa, &pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &pontos, &pontosdenivel, &direcao, &fantasmaX2, &fantasmaY2, &vidas);
        
        while (vidas > 0)
        {
            printarMapa(mapa, pontos, vidas);

            if (kbhit()) //A função kbhit retorna um valor que o C interpreta interpreta  como verdadeiro se uma tecla foi pressionada. Caso contrário ela retorna um valor falso.
            {
                direcao = getch();
                //a cada interação do loop ele verifica se uma tecla foi pressionada
                //se sim, ele atualiza a direção do movimento com a nova tecla que vc pressionou
                //se não, ele continua usando a ultima direção.
            }
            if(direcao == 'q'){
                vidas = 0; // Se o jogador apertar 'q', zera as vidas para sair do loop
            }
            
            if (pontosdenivel <= 0)
            {
                Sleep(2000);
                resetarJogo(mapa, &pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &pontos, &pontosdenivel, &direcao, &fantasmaX2, &fantasmaY2, &vidas);
                vidas++;
            }

            //Guarda as posições antigas do pacman e dos fantasmas
            pacmanOldX = pacmanX;
            pacmanOldY = pacmanY;
            fantasmaOldX = fantasmaX;
            fantasmaOldY = fantasmaY;
            fantasmaOldX2 = fantasmaX2;
            fantasmaOldY2 = fantasmaY2;

            moverPacman(mapa, &pacmanX, &pacmanY, direcao, &pontos, &pontosdenivel);
            moverFantasma(mapa, &fantasmaX, &fantasmaY, &pacmanX, &pacmanY, &fantasmaParado1, &tempoParado1);
            moverFantasma(mapa, &fantasmaX2, &fantasmaY2, &pacmanX, &pacmanY, &fantasmaParado2, &tempoParado2);

            //atraso de movimento
            Sleep(150); //atraso no loop de 250 millisegundos, pausa o código por 250 milisegundos a cada movimento do pacman.
            
            // Condição para detectar as possiveis colisões entre o pacman e o fantasma 1 caso ela seja frontal e caso um passe pelo outro
            if ((pacmanX == fantasmaX && pacmanY == fantasmaY) || (pacmanX == fantasmaOldX && pacmanY == fantasmaOldY && pacmanOldX == fantasmaX && pacmanOldY == fantasmaY)){
                // Colisão com o primeiro fantasma
                if (medoFantasma) {
                    pontos += 200; // Pontos por comer o fantasma
                    mapaDinamico[fantasmaX][fantasmaY] = VAZIO; // Limpa a posição antiga do fantasma
        
                    // Reposiciona o fantasma e ativa o cronometro de espera
                    fantasmaX = 10;
                    fantasmaY = 13;
                    fantasmaParado1 = true;
                    tempoParado1 = clock();
        
                    mapaDinamico[fantasmaX][fantasmaY] = FANTASMA; // Coloca o fantasma de volta no mapa dinâmico
                } else {
                    vidas--;

                    mapaDinamico[pacmanX][pacmanY] = VAZIO;
                    mapaDinamico[fantasmaX][fantasmaY] = VAZIO;
                    mapaDinamico[fantasmaX2][fantasmaY2] = VAZIO;
                    PlaySound("die.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
                    Sleep(2500);
                    posicionarPersonagens(&pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &fantasmaX2, &fantasmaY2);
                    direcao = 'd';
                    continue; // recomeça o loop.
                }
            }
            if ((pacmanX == fantasmaX2 && pacmanY == fantasmaY2) || (pacmanX == fantasmaOldX2 && pacmanY == fantasmaOldY2 && pacmanOldX == fantasmaX2 && pacmanOldY == fantasmaY2)) {
                // Colisão com o segundo fantasma
                if (medoFantasma) {
                    pontos += 200;
                    mapaDinamico[fantasmaX2][fantasmaY2] = VAZIO;
                    
                    // Reposiciona o fantasma e ativa o cronometro de espera
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
                    continue; // recomeça o loop.
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
            mapa[i][j] = mapaInicial[i][j]; //mapa principal
            mapaDinamico[i][j] = VAZIO; //Mapa vazio, só tem o pacman e o fantasma e as paredes. (uma camada a mais)
        }
    }
}

void printarMapa (char mapa[LINHAS][COLUNAS], int pontos, int vidas){
    
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

    printf("Pontos: %d\n", pontos);
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            // Prioriza o mapa dinâmico para imprimir os personagens
            if (mapaDinamico[i][j] == PACMAN){
                printf("%c", mapaDinamico[i][j]);
            } else if (mapaDinamico[i][j] == FANTASMA) {
                if (medoFantasma) {
                    printf("w"); // Imprime 'm' se o fantasma estiver com medo
                } else {
                    printf("%c", mapaDinamico[i][j]); // Imprime o fantasma normal
                }
            } else {
                // Caso contrário, imprime o mapa de fundo (paredes, pontos, etc.)
                printf("%c", mapa[i][j]);
            }
        }   
        printf("\n");
    }

    printf("\n");

    for (int i = 0; i < vidas; i++) //Imprime as vidas totais atuais
    {
        printf("C ");
    }

    for (int i = 0; i < MAX_VIDAS - vidas; i++) // imprime espaços para apagar os C's anteriores que não sumiam
    {
        printf("  ");
    }
    printf("\n\n\n");
}

//Função para mover o pacman
void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, char direcao, int *pPontuacao, int *pPontosdenivel){ //*pPontuacao é o que a função moverPacman usa para ter acesso ao endereço de memoria da pontuacao, assim a função pode modificar o valor da variavel pontuacao
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

    //Verifica se o movimento do pacman é valido
    if (novoX >= 0 && novoX < LINHAS && novoY >= 0 && novoY < COLUNAS && mapa[novoX][novoY] != PAREDE)
    {
        //novoX >= 0 && novoX < LINHAS -> verifica se a nova linha do personagem (novoX) está dentro dos limites verticais do mapa
        //novoY >= 0 && novoY < COLUNAS -> verifica se a nova coluna do personagem (novoY) está dentro dos limites horizontais do mapa
        //novoY >= 0 && novoY < COLUNAS -> verifica se o local não é uma parede
        if (mapa[novoX][novoY] == PONTO) //está verificando a próxima posição para onde o Pac-Man vai se mover. Ela faz uma pergunta: "A próxima casa é um ponto (-)?"
        {
            *pPontuacao += 10;
            //pega o valor atual da pontuação (*pPontuacao) e soma 10 a esse valor
            //e entao atualiza a variável original da pontuacao na função main com o novo valor.

            *pPontosdenivel-=1; //cada vez que um ponto é comido o contador de pontos para o proximo nivel diminui em um, usamos isso para reiniciar o mapa na proxima fase

            mapa[novoX][novoY] = VAZIO; //se o proximo movimento do pacman tem um ponto ele troca por um espaço vazio no Mapa PRINCIPAL

            PlaySound("pellet.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
            //funçao para tocar o som do pacman comendo
            //eating.wav nome do arquivo com som

        }else if (mapa[novoX][novoY] == ESPECIAL) { // Condição para o ponto especial
            *pPontuacao += 100; // Pontuação extra
            PlaySound("specialT.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
            medoFantasma = true; // Ativa o estado de medo
            tempoMedo = clock(); // Inicia o cronômetro
            mapa[novoX][novoY] = VAZIO;
        }
        // Move o Pac-Man apenas no mapa dinâmico
        mapaDinamico[*pacmanX][*pacmanY] = VAZIO; //Usa a posição atual do pacman para limpar o mapa dinamico
        //atualizam as variávieis do pacman com as novas coordenadas, dando as posições futuras
        *pacmanX = novoX;
        *pacmanY = novoY;
        //atualiza a nova posição no mapa dinamico
        mapaDinamico[*pacmanX][*pacmanY] = PACMAN;       
    }
}

void moverFantasma (char mapa[LINHAS][COLUNAS], int *fantasmaX, int *fantasmaY, int *pacmanX, int *pacmanY, bool *fantasmaParado, clock_t *tempoParado){

     // Se o fantasma estiver "parado" na base e o tempo nao expirou, a funcao termina aqui
    if (*fantasmaParado && (clock() - *tempoParado) / CLOCKS_PER_SEC < DURACAO_ESPERA) {
        return; // Sai da funcao sem mover o fantasma
    } else if (*fantasmaParado && (clock() - *tempoParado) / CLOCKS_PER_SEC >= DURACAO_ESPERA) {
        // Se o tempo acabou, desativa o flag para permitir o movimento novamente
        *fantasmaParado = false;
    }

    // A logica do resto da funcao continua como esta, sem mudancas...
    if (medoFantasma && (clock() - tempoMedo) / CLOCKS_PER_SEC >= DURACAO_MEDO) {
        medoFantasma = false;
    }
    
    int novoX = *fantasmaX; //pega a posição do fantasma nas linhas
    int novoY = *fantasmaY; //pega a posição dos fantasmas nas colulnas

    // Logica para perseguir o pacman
    int distX = *pacmanX - *fantasmaX;
    int distY = *pacmanY - *fantasmaY;

    //controle de movimento
    if (medoFantasma) { // Lógica de FUGA (inverte os sinais)
        if (abs(distX) > abs(distY)) {
            if (distX > 0) { // Se o Pac-Man está abaixo, fuja para cima
                novoX = *fantasmaX - 1;
            } else { // Se o Pac-Man está acima, fuja para baixo
                novoX = *fantasmaX + 1;
            }
        } else {
            if (distY > 0) { // Se o Pac-Man está à direita, fuja para a esquerda
                novoY = *fantasmaY - 1;
            } else { // Se o Pac-Man está à esquerda, fuja para a direita
                novoY = *fantasmaY + 1;
            }
        }
    } else { // Lógica de PERSEGUIÇÃO
        if (abs(distX) > abs(distY)) { //Pega o modulo do valor de distX e distY, se a distancia vertical (linhas) for maior que a horizontal (colunas)
            if (distX > 0) { //Se distX for positivo então decide se dece ir para baixo
                novoX = *fantasmaX + 1;
            } else {
                novoX = *fantasmaX - 1; 
            }
        } else { //se a distancia horizontal (colunas) for maior ou igual a vertical (linhas)
            if (distY > 0) { //Se a distancia horizontal for positiva
                novoY = *fantasmaY + 1; //fantasma se move para a direita
            } else {
                novoY = *fantasmaY - 1; //fantasma se move para a esquerda
            }
        }
    }
    
    //novoX < 0 || novoX >= LINHAS || novoY < 0 -> Verifica se a nova linha está acima ou abaixo dos limites do mapa
    //novoY < 0 || novoY >= COLUNAS -> Verifica se a nova coluna está a esquerda ou diteita dos limites do mapa
    //mapa[novoX][novoY] == PAREDE -> Verifica se o proximo passo seria direto dentro de uma parede
    if (novoX < 0 || novoX >= LINHAS || novoY < 0 || novoY >= COLUNAS || mapa[novoX][novoY] == PAREDE) {
        novoX = *fantasmaX; //pega a posição do fantasma nas linhas
        novoY = *fantasmaY; //pega a posição dos fantasmas nas colulnas
        
        if (abs(distX) > abs(distY)) { //Pega o modulo do valor de distX e distY, se a distancia vertical (linhas) for maior que a horizontal (colunas)
            //diferente do if principal este prioriza o movimento horizontal
            if (distY > 0) { //fantasma se move para a direita
                novoY = *fantasmaY + 1;
            } else {
                novoY = *fantasmaY - 1; //fantasma se move para a esquerda
            }
        } else {
            if (distX > 0) {
                novoX = *fantasmaX + 1; //fantasma se move para a baixo 
            } else {
                novoX = *fantasmaX - 1; //fatasma se move para cima
            }
        }
    }

    //novoX < 0 || novoX >= LINHAS || novoY < 0 -> Verifica se a nova linha está acima ou abaixo dos limites do mapa
    //novoY < 0 || novoY >= COLUNAS -> Verifica se a nova coluna está a esquerda ou diteita dos limites do mapa
    //mapa[novoX][novoY] == PAREDE -> Verifica se o proximo passo seria direto dentro de uma parede
    if (novoX < 0 || novoX >= LINHAS || novoY < 0 || novoY >= COLUNAS || mapa[novoX][novoY] == PAREDE) {
        int direcao = rand() % 4; //código que gera um número aleatorio entre (0, 3)
        
        switch(direcao){ //o código usa o número gerado para forçar o fantasma a se mover em uma das quatro direções possiveis, este não se preocupa com a posição do pacman
            case 0: novoX = *fantasmaX - 1; break;
            case 1: novoX = *fantasmaX + 1; break;
            case 2: novoY = *fantasmaY - 1; break;
            case 3: novoY = *fantasmaY + 1; break;
        }
    }
    
    //Teleporte horizontal
    if (novoY < 0) { // Se estiver tentando sair pela esquerda
        if (novoX == 10) {
                novoY = COLUNAS - 1;
            }
        } else if (novoY >= COLUNAS) { // Se estiver tentando sair pela direita
            if (novoX == 10) {
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

    // Verifica se o movimento do fantasma é valido
    if (novoX >= 0 && novoX < LINHAS && novoY >= 0 && novoY < COLUNAS && mapa[novoX][novoY] != PAREDE) {
        
        // Apenas move o fantasma no mapa dinâmico
        mapaDinamico[*fantasmaX][*fantasmaY] = VAZIO; //Troca a posição do fantasma por um espaço vazio
        //atualiza as novas posições do fantasma
        *fantasmaX = novoX;
        *fantasmaY = novoY;
        //coloca a nova posição do fantasma no mapa dinamico
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

    Sleep(1000); // Pausa por 1 
    
    // Limpa o buffer do teclado antes de ler a nova escolha
    while (kbhit()) {
        getch();
    }

    return getch();
}

void resetarJogo (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *pontos, int *pontosdenivel, char *direcao, int *fantasmaX2, int *fantasmaY2, int *vidas){

    iniciarMapa(mapa);

    // Zera a pontuação apenas se for um novo jogo completo
    if (*pontos == 0) {
        *pontos = 0;
        posicionarPersonagens(pacmanX, pacmanY, fantasmaX, fantasmaY, fantasmaX2, fantasmaY2);
        printarMapa(mapa, *pontos, *vidas);
        PlaySound("begin.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
        Sleep(1300);

    }
    
    // Calcula a quantidade de pontos no novo mapa
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
//Função para resetar apenas os personagens
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
