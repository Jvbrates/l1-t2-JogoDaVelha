

#ifndef __btn 
#define __btn



typedef struct {
    int pos_x;
    int pos_y;
    int x;
    int y;
    char *texto;
    int lt;
    int id;
    int cor_borda;
    int cor_fundo;
    int cor_texto;
    bool hold;
} button;
//Funções que executam a função-parametro sobre os itens do array até atingir o limite lencht ou um retorno diferente de 0
int iterable_f(button *array, int length, int f());// Pode obter informações do objeto mas nao altera-lo
int iterable_fp(button ar[], int length, int (*f)(button *)); //Pode alterá-lo através de ponteiros
// Cria e retorna uma estrutura button
button init_button(int pos_x, int pos_y, int x, int y, char *texto, int lt, int id, int cor_borda, int cor_fundo, int cor_texto);

//Desenha o botao
int draw_button(button btn); //Draw the object

//Enquanto o mouse estiver pressionado e em cima do botão. este é encolhido
int click_hold(button *btn); 

// Retorna o id do botão caso o mouse esteja em cima
int mouse_on(button btn); 

//Atoa, desenha uma seta ">" quando o mouse está acima do botao
int atoa(button ar);

#endif
