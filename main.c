#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define COLUNAS 28
#define LINHAS 22
#define PACMAN 'C'
#define PAREDE '#'
#define VAZIO ' '
#define PONTO '-'
#define ESPECIAL '*'
#define FANTASMA 'G'

void iniciarMapa(char mapa[LINHAS][COLUNAS]);

void moverFantasma (char mapa[LINHAS][COLUNAS], int *fantasmaX, int *fantasmaY, int *pacmanX, int *pacmanY);

void printarMapa (char mapa[LINHAS][COLUNAS], int pontuacao, int vidas);

char telaGameOver();

void moverPacman (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY,  char direcao, int *pPontuacao, int *pPontosdenivel);

void resetarJogo (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *pontos, int *pontosdenivel, char *direcao, int *fantasmaX2, int *fantasmaY2);

void posicionarPersonagens(int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *fantasmaX2, int *fantasmaY2);

char mapaDinamico[LINHAS][COLUNAS];

int main (){
    char mapa[LINHAS][COLUNAS];
    int pacmanX, pacmanY, pontos = 0, fantasmaX, fantasmaY, pacmanOldX, pacmanOldY, fantasmaOldX, fantasmaOldY, vidas, pontosdenivel, fantasmaX2, fantasmaY2, fantasmaOldX2, fantasmaOldY2;
    char direcao;

    srand(time(NULL));

    while(true) {
        vidas = 3;

        resetarJogo(mapa, &pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &pontos, &pontosdenivel, &direcao, &fantasmaX2, &fantasmaY2);

        while (kbhit()) {
            getch(); // Limpa o buffer do teclado
        }
        
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
                resetarJogo(mapa, &pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &pontos, &pontosdenivel, &direcao, &fantasmaX2, &fantasmaY2);
            }

            //Guarda as posições antigas do pacman e dos fantasmas
            pacmanOldX = pacmanX;
            pacmanOldY = pacmanY;
            fantasmaOldX = fantasmaX;
            fantasmaOldY = fantasmaY;
            fantasmaOldX2 = fantasmaX2;
            fantasmaOldY2 = fantasmaY2;

            moverPacman(mapa, &pacmanX, &pacmanY, direcao, &pontos, &pontosdenivel);
            moverFantasma(mapa, &fantasmaX, &fantasmaY, &pacmanX, &pacmanY);
            moverFantasma(mapa, &fantasmaX2, &fantasmaY2, &pacmanX, &pacmanY);

            //atraso de movimento
            Sleep(200); //atraso no loop de 250 millisegundos, pausa o código por 250 milisegundos a cada movimento do pacman.
            
            // Condição para detectar as possiveis colisões entre o pacman e os fantasmas caso ela seja frontal e caso um passe pelo outro
            if ((pacmanX == fantasmaX && pacmanY == fantasmaY) || (pacmanX == fantasmaOldX && pacmanY == fantasmaOldY && pacmanOldX == fantasmaX && pacmanOldY == fantasmaY) || (pacmanX == fantasmaX2 && pacmanY == fantasmaY2) || (pacmanX == fantasmaOldX2 && pacmanY == fantasmaOldY2 && pacmanOldX == fantasmaX2 && pacmanOldY == fantasmaY2)){
                vidas--; // Diminui 1 vida

                //No mapa dinamico pacman e o fantasma são substituidos por espaços vazios
                mapaDinamico[pacmanX][pacmanY] = VAZIO;
                mapaDinamico[fantasmaX][fantasmaY] = VAZIO;
                mapaDinamico[fantasmaX2][fantasmaY2] = VAZIO;

                Sleep(2500); // Pausa por 2,5 segundo para mostrar a colisão

                //atualiza o número de vidas
                posicionarPersonagens(&pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &fantasmaX2, &fantasmaY2);
            }
        }

        char escolha = telaGameOver();
        if (escolha == '2'){
            break;
        } else{
            resetarJogo(mapa, &pacmanX, &pacmanY, &fantasmaX, &fantasmaY, &pontos, &pontosdenivel, &direcao, &fantasmaX2, &fantasmaY2);
            pontos = 0;
        }
    }
    return 0;
}
void iniciarMapa(char mapa[LINHAS][COLUNAS]){

    char mapaInicial[LINHAS][COLUNAS] = {

        "### #################### ###",
        "#----#-------##-------##---#",
        "#----#-##-##----##-##-##-#-#",
        "#-##---##-##-##-##-##-##-#-#",
        "#-##---##----------##----#-#",
        "#-####----########----####-#",
        "#------##----##----##------#",
        "######-#####-##-#####-###-##",
        "#--------           -------#",
        "##-###-##  ###  ### ##-##-##",
        "---------           --------",
        "##-#-#-##  ###  ### ##-##-##",
        "#----#-##           #-----##",
        "#-#-##-##-########-##-##-###",
        "#------------##------------#",
        "#-#-##-#####-##-#####-##-#-#",
        "#---##------ ---------##---#",
        "###-##-##-##-##-##-##-##-###",
        "#------##----##----##------#",
        "#-####----##-##-##----####-#",
        "#------##----------##------#",
        "### #################### ###",
    };
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            mapa[i][j] = mapaInicial[i][j]; //mapa principal
            mapaDinamico[i][j] = VAZIO; //Mapa vazio, só tem o pacman e o fantasma e as paredes, uma camada a mais
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
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++){
            // Prioriza o mapa dinâmico para imprimir os personagens
            if (mapaDinamico[i][j] == PACMAN || mapaDinamico[i][j] == FANTASMA){
                printf("%c", mapaDinamico[i][j]);
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

    for (int i = 0; i < 3 - vidas; i++) // imprime espaços para apagar os C's anteriores que não sumiam
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
        // novoX >= 0 && novoX < LINHAS -> verifica se a nova linha do personagem (novoX) está dentro dos limites verticais do mapa
        //novoY >= 0 && novoY < COLUNAS -> verifica se a nova coluna do personagem (novoY) está dentro dos limites horizontais do mapa
        //novoY >= 0 && novoY < COLUNAS -> verifica se o local não é uma parede
        if (mapa[novoX][novoY] == PONTO) //está verificando a próxima posição para onde o Pac-Man vai se mover. Ela faz uma pergunta: "A próxima casa é um ponto (-)?"
        {
            *pPontuacao += 10;
            //pega o valor atual da pontuação (*pPontuacao) e soma 10 a esse valor
            //e entao atualiza a variável original da pontuacao na função main com o novo valor.

            *pPontosdenivel-=1; //cada vez que um ponto é comido o contador de pontos para o proximo nivel diminui em um, usamos isso para reiniciar o mapa na proxima fase

            mapa[novoX][novoY] = VAZIO; //se o proximo movimento do pacman tem um ponto ele troca por um espaço vazio no Mapa PRINCIPAL
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

void moverFantasma (char mapa[LINHAS][COLUNAS], int *fantasmaX, int *fantasmaY, int *pacmanX, int *pacmanY){

    int novoX = *fantasmaX;
    int novoY = *fantasmaY;

    // A logica para perseguir o Pac-Man
    int distX = *pacmanX - *fantasmaX; //calcula a distancia vertical entre o pacman e os fantasmas, se for positivo, o pacman está abaixo do fantasma, negativo o pacman esta acima
    int distY = *pacmanY - *fantasmaY; //calcula a distancia horizontal entre o pacman e os fantasmas, se for positivo o pacman está a direita do fantamas, se for negativo o pacman esta a esquerda dos fantasmaa

    // A logica de perseguir principal de perseguição
    if (abs(distX) > abs(distY)) { //compara distancia vertical (distX) com a distancia horizontal (distY), o abs() garante que a comparação seja feita com os valores positvios, igonora se o pacmana está a esquerda ou a direita, acima ou abaixo
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

void resetarJogo (char mapa[LINHAS][COLUNAS], int *pacmanX, int *pacmanY, int *fantasmaX, int *fantasmaY, int *pontos, int *pontosdenivel, char *direcao, int *fantasmaX2, int *fantasmaY2){

    iniciarMapa(mapa);

    // Zera a pontuação apenas se for um novo jogo completo
    if (*pontos == 0) {
        *pontos = 0;
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



