//João Vitor Belmonte Rates
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "buttons.h"
#include "tela.h"
#define DEBUGVAR 0

//Receberão as posições do mouse
int mouse_x, mouse_y;

//Structs and Enums
typedef enum{
    menu = 1,
    play,
    quit
} jogo;

typedef struct{
    bool run;
    jogo estado;
    int vez; 
    int placar[5];  
} Jogo;

typedef enum{
    X_win = 0,
    X_gup,
    Tie,
    O_gup,
    O_win,
    Running,
} resultado;

typedef struct{
    bool run;
    bool menu;
    char tab[3][3];
    resultado estado;
} partida;


// Funções Draw
void draw_score(int *);
void draw_o(int size, int cor, int x, int y, float l, int cor_int);
void draw_x(int size, int cor, int x, int y, float l);
void draw_menu();
void draw_table(char tab[3][3]);
void play_text(resultado r, int vez);
//Funções placar_arquivo
bool vettorfile(char filename[],int lenvet, int vet[]);//Salva o vetor atual no arquivo, atividades de vetores
bool filetovet(char filename[],int lenvet, int vet[]); // Autoexplicativa
void resetScore(int *score); // Autoexplicativa
void updateScore(int *score, resultado r); // Atualiza || Incrementa o placar

//Seleciona vez
void set_turn(int *v);

//retorna um numero aleatorio
int aleat(int, int);

//Inicializa as estruturas do jogo
void init_jogo(Jogo *j);

//Inicializa as estruturas da partida
void init_play(partida *);

//Função principal da partida
void playing(partida *p, Jogo *j);

//Condição de vitória
bool detectWin(char tab[3][3]);
//Condição para empate
bool detectTie(char tab[3][3]);

//Insere valores na tabela|matriz
bool update_table(int, int, char[3][3]);

//Função de despedida
bool despedida(double r, int delay);

int main(){
    int r;
    Jogo estado_jogo;//Variavel estrutura do jogo;

    //TODO Adiciona isso aqui no fim srand(time(0));

    init_jogo(&estado_jogo);//Set the init values of the game

    tela_inicio(600, 600, "Meu sistema não tem titlebar"); //Cria a tela;
    
    
    partida partida_var;
    while (estado_jogo.run) {
        
        // Gravar as posições do mouse em uma variavel.
        mouse_x = tela_rato_x();
        mouse_y = tela_rato_y();

        //Desenha placar, a todo momento -- e o fundo tambem
        draw_score(estado_jogo.placar);

        //Estados do jogo -- Reset não é mais um estado |  |
        switch (estado_jogo.estado) {
            case quit:
                if(despedida(r, 5)){
                    estado_jogo.run = false;
                    tela_fim();
                    return 0;

                }    
            break;
            case play:
                
                if(partida_var.estado != Running && partida_var.menu == false){
                    
                    switch (partida_var.estado) {
                        case X_win:
                            updateScore(estado_jogo.placar, X_win);    
                        break;
                        case X_gup:
                            updateScore(estado_jogo.placar, X_gup);
                        break;
                        case O_win:
                            updateScore(estado_jogo.placar, O_win);
                        break;
                        case O_gup:
                            updateScore(estado_jogo.placar, O_gup);
                        break;
                        case Tie:
                            updateScore(estado_jogo.placar, Tie);
                        break;
                        case Running:
                            printf("Isto não deve ocorrer");
                    }
                    vettorfile("score", 5, estado_jogo.placar);
                    
                }
                playing(&partida_var, &estado_jogo);
            break;
            case menu:
                {
                    //Botões do Menu
                    button array_btn[3];
                    array_btn[0] = init_button(220, 220, 160, 50, "Nova Partida", 10, 1, transparente, verde, branco);
                    array_btn[1] = init_button(220, 320, 160, 50, "Resetar Placar", 10, 2, transparente, amarelo, branco);
                    array_btn[2] = init_button(220, 420, 160, 50, "Sair", 10, 3, transparente, vermelho, branco);
                    
                    
                    iterable_fp(array_btn, 3, click_hold);
                    iterable_f(array_btn, 3, draw_button);
                    

                    int t = iterable_f(array_btn, 3, mouse_on);
                    
                    if(t){
                        tela_texto_esq(array_btn[t-1].pos_x-10, array_btn[t-1].pos_y, array_btn[t-1].y, array_btn[t-1].cor_fundo, ">");
                    }
                    if(tela_rato_clicado()){
                        switch (t) {
                        case 1:
                            estado_jogo.estado = play;
                            
                            init_play(&partida_var);
                            set_turn(&estado_jogo.vez);
                            break;
                        case 2: //Reset Não é mais um placar -- Adicionar VetToFile Depois;
                            resetScore(estado_jogo.placar);
                            vettorfile("score", 5, estado_jogo.placar);
                            
                            break;
                        case 3:
                            
                            r = relogio();
                            estado_jogo.estado = quit;
                        break;
                        }
                    }
                    
                }
            break;
        }
        
        // Desenha mouse a todo momento
        draw_x(5, branco, tela_rato_x(), tela_rato_y(), 3);
        

        tela_atualiza();

    }

    //TODO defina despedidas
    return 0;
}

//Numero aleatorio -- Usado para escolha de quem começa
int aleat(int min, int max){
    return rand()%(max-min)+min;
}

//Inicializa os valores da estrutura do jogo
void init_jogo(Jogo *j){
    j->run = true;
    j->estado= menu;
    j->vez = -1;
    if(!filetovet("score", 5, j->placar)){
        resetScore(j->placar);
    };
    
}

//Inicializa os valores da estrutura da partida
void init_play(partida *p){ // TODO pergunte: O que é melhor, alterar como ponteirou ou criar e retornar, se tu ja sabe a resposta corrijá
    p->run = true;
    p->estado = Running;
    p->menu = false;
 
    //Zera a tabela
    for (int i =0; i<3; i++) {
        for (int j = 0; j<3; j++) {
            p->tab[i][j] = ' ';
        }
    }

     
}

//Funções draw 
void draw_x(int size, int cor, int x, int y, float l){
    tela_linha(x, y, x+size, y+size, l, cor);
    tela_linha(x+size, y, x, y+size, l, cor);
}

void draw_o(int size, int cor, int x, int y, float l, int cor_int){
    tela_circulo(x+(float)size/2, y+(float)size/2, (float)size/2, l, cor, cor_int);
}

void draw_score(int *score){
    tela_retangulo(0, 0, 600, 600, 0, fundo, fundo);
    tela_retangulo(0, 0, 600, 90, 0 ,fundo_t, fundo_t);
    draw_x(70, roxo, 10, 10, 10);
    draw_o(70, amarelo, 520, 10, 10, fundo);
    char test[30];
    sprintf(test, "%d %d [%d] %d %d", score[0], score[1], score[2], score[3], score[4]);
    
    tela_texto(300, 50, 40, branco, test);
}

void draw_mouse(jogo estado, int vez){
}

void draw_table(char tab[3][3]){
    //tela_retangulo(105, 130, 495, 520, 5, branco, verde);
    //Desenha Linhas
    for (int i = 0;i<2; i++) {
     tela_linha(105, (i+1)*130+130, 495, (i+1)*130+130, 5, preto);
    }
    //Desenha Colunas
    for (int i = 0;i<2; i++) {
     tela_linha(105+(i+1)*130, 130, 105+(i+1)*130, 520, 5, preto);
    }

    for(int i = 0; i <3; i++){
        //printf("\n");
        for (int j = 0; j < 3; j++) {
        if(tab[i][j] != ' '){
            //printf("|(%c)%i-%i|",tab[i][j], i,j);
            
            switch (tab[i][j]) {
            case 'x':
                draw_x(80, roxo, (j)*130+130, (i+1)*130+20, 5);
            break;
            case 'o':
                draw_o(80, amarelo, (j)*130+130, (i+1)*130+20, 5, transparente);
            break;
            }
        }
        }
    }
}

//Desenha o texto rótulo durante a partida
void play_text(resultado r, int vez){
    char texto[80];
    switch (r) {
    case Running:
    {
        sprintf(texto, "Vez de");//TODO muda a cor
        if(!vez){
            draw_o(10, amarelo, 90, 550, 2, fundo);
        } else {
            draw_x(10, roxo, 90, 550, 2);
        }
        break;
    }
    case X_win:
    case O_win:
        sprintf(texto, "Vitoria de  ");//Todo muda a cor
        if(vez){
            draw_o(10, amarelo, 130, 550, 2, fundo);
        } else {
            draw_x(10, roxo, 130, 550, 2);
        }
    break;

    case X_gup:
    case O_gup:
    {
        sprintf(texto, "  Desistiu");//Todo muda a cor
        if(vez){
            draw_o(10, amarelo, 20, 550, 2, fundo);
        } else {
            draw_x(10, roxo,20, 550, 2);
        }
        break;
    }
    case Tie:
    {   
        sprintf(texto, "Empate!");
        break;
    }
    
    }

    tela_texto_dir(20, 550, 10, branco, texto);
    
}

//Função principal da partida
void playing(partida *partida_s, Jogo *j){
        
    draw_table(partida_s->tab);
        
        
    if (partida_s->menu) {
        
        
        button ok = init_button(395, 540, 100, 50, "OK", 10, 1, verde, verde, branco);
        click_hold(&ok);
        draw_button(ok);
        int ok_id = mouse_on(ok);
        if(ok_id){
            tela_texto_esq(ok.pos_x-10, ok.pos_y, ok.y, ok.cor_fundo, ">");
        }
        if(tela_rato_clicado() && ok_id){
            switch (partida_s->estado) {
                case X_win:
                case O_gup:
                    j->vez = 1;
                break;
                case O_win:
                case X_gup:
                    j->vez = 0;
                break;
                case Tie:
                    j->vez = j->vez;
                break;
                case Running:
                    printf("Isto não deve occorrer");
                break;
                }
            j->estado = menu;
        }
    } else {
        if(partida_s->estado != Running){
            partida_s->menu = true;
        }   
        button btn_tab_array[10] = {
                    init_button(105, 130, 130, 130, "1-1", 10, 11, verde, branco, branco),
                    init_button(235, 130, 130, 130, "1-2", 10, 12, verde, branco, branco),
                    init_button(365, 130, 130, 130, "1-3", 10, 13, verde, branco, branco),
                    init_button(105, 260, 130, 130, "1-1", 10, 21, verde, branco, branco),
                    init_button(235, 260, 130, 130, "1-2", 10, 22, verde, branco, branco),
                    init_button(365, 260, 130, 130, "1-3", 10, 23, verde, branco, branco),
                    init_button(105, 390, 130, 130, "1-1", 10, 31, verde, branco, branco),
                    init_button(235, 390, 130, 130, "1-2", 10, 32, verde, branco, branco),
                    init_button(365, 390, 130, 130, "1-3", 10, 33, verde, branco, branco),
                    init_button(395, 540, 100, 50, "Desistir", 10, -1, branco, vermelho, branco)};
                //terable_f(btn_tab_array, 10, draw_button);
                click_hold(&btn_tab_array[9]);
                draw_button(btn_tab_array[9]);//


                int input = iterable_f(btn_tab_array, 9, mouse_on);
                int give_up = mouse_on(btn_tab_array[9]);
                if(give_up){
                        tela_texto_esq(btn_tab_array[9].pos_x-10, btn_tab_array[9].pos_y, btn_tab_array[9].y, btn_tab_array[9].cor_fundo, ">");
                }
                //Empate
                if(detectTie(partida_s->tab)){
                    partida_s->estado = Tie; //It's like php 
                }
                                //Vitoria
                if(detectWin(partida_s->tab)){
                    switch (j->vez) {
                    case 0:
                        partida_s->estado = X_win;
                        break;
                    case 1:
                        partida_s->estado = O_win;
                        break;
                    }
                }


                if(tela_rato_clicado()){
                    if(input > 0){
                        
                        if(update_table(!(j->vez), input, partida_s->tab)){
                        j->vez = !(j->vez);
                    }
                    //Case give up
                    } 
                    if (give_up < 0) {
                        j->vez = !(j->vez);
                        switch (j->vez) {
                        case 0:
                            partida_s->estado = X_gup;
                        break;
                        case 1:
                            partida_s->estado = O_gup;
                        break;
                        }
                    }


                    
                }
    }
    play_text(partida_s->estado, j->vez);
}

//Funções Placar Arquivo

bool vettorfile(char filename[],int lenvet, int vet[]){
    FILE *file;

    file = fopen(filename, "w");

    if(file == NULL){
        printf("erro ao abrir arquivo");
        return false;
    }
    
    for(int i = 0; i<lenvet; i++){
        fprintf(file, "%i\n", vet[i]);
    }
    fclose(file);
    return true;
}
bool filetovet(char filename[],int lenvet, int vet[]){
    FILE *file;
    int aux;
    file = fopen(filename, "r");

    if(file == NULL){
        printf("erro ao abrir arquivo| Criando um novo score file");
        return false;
    }
    int i = 0;
    int rscan = fscanf(file, "%i", &aux);
    while (!feof(file) && rscan == 1 && i<=lenvet) {
        
        vet[i]= aux;
        rscan = fscanf(file, "%i", &aux);
        i++;
    }
     
    if(feof(file)){
        vet[i]= aux;
    } 
    if(rscan == 0){
        return true;
    } else {

    }

    return true;
}
void updateScore(int *score, resultado r){
    score[r]+=1;
}
void resetScore(int *score){
    score[0] = 0;
    score[1] = 0;
    score[2] = 0;
    score[3] = 0;
    score[4] = 0;
}

//Seleciona vez
void set_turn(int *v){
    if(*v == -1){
        *v = aleat(0, 1);
    } else {
        *v = *v;
    }
}



//Insere valores na tabela|matriz
bool update_table(int vez, int coord, char tab[3][3]){
    if( tab[((int)coord/10)-1][((int)coord%10)-1] == ' '){
        switch (vez) {
        case 0:
            tab[((int)coord/10)-1][((int)coord%10)-1] = 'x';
        break;
        case 1:
            tab[((int)coord/10)-1][((int)coord%10)-1] = 'o';
        break;
        }

        return true;
    }

    return false;
};



bool detectWin(char tab[3][3]){
    //Verifica Linhas
    for(int i = 0; i<3; i++){
        
        if((tab[i][0] == tab[i][1] && tab[i][1] == tab[i][2]) && tab[i][2] != ' '){
            return true;
        }

        if((tab[0][i] == tab[1][i]) && (tab[1][i] == tab[2][i]) && (tab[2][i] != ' ')){
            return true;
        }
    }
    if(tab[1][1] != ' '){
        if((tab[0][0] == tab[1][1]) && (tab[1][1] == tab[2][2]) && (tab[2][2] == tab[1][1])){
            return true;
        }

        if((tab[0][2] == tab[1][1]) && (tab[1][1] == tab[2][0]) && (tab[0][2] == tab[1][1])){
            return true;
        }
    }

    return false;
}

bool detectTie(char tab[3][3]){
    for(int i = 0; i <3; i++){
        for (int j = 0; j < 3; j++) {
            if(tab[i][j] == ' '){
                return false;
            }
        }
    }

    return true;
}

bool despedida(double r, int delay){
    
    if(relogio() - r > delay){
        return true;
    }
    tela_texto(300, 300, 14, roxo, "Developed by joão Vitor Belmonte");
    
    

    return false;
}