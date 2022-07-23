#include "tela.h"
#include "buttons.h"
#include <stdio.h>
extern int mouse_x, mouse_y;

//Execuuta a função sobre todos os itens
int iterable_f(button ar[], int length, int (*f)(button)){
    int r;
    for(int i = 0; i<length; i++)
    {
        r = f(ar[i]);
        if( r != 0){
           
            break;
        }
    }    
    return r;
}

int iterable_fp(button ar[], int length, int (*f)(button *)){
    int r;
    for(int i = 0; i<length; i++)
    {
        r = f(&ar[i]);
        if( r != 0){
            
            break;
        }
    }    
    return r;
}

//Criar e retorna um button;
button init_button(int pos_x, int pos_y, int x, int y, char *texto, int lt, int id, int cor_borda, int cor_fundo, int cor_texto){
    button btn;
    btn.pos_x = pos_x;
    btn.pos_y = pos_y;
    btn.x = x;
    btn.y = y;
    btn.cor_borda = cor_borda;
    btn.cor_fundo = cor_fundo;
    btn.cor_texto = cor_texto;
    btn.texto = texto;
    btn.id = id;
    btn.lt = lt;
    return btn;
}

//Desenha o button
int draw_button(button btn){
    tela_retangulo(btn.pos_x, btn.pos_y, btn.x+btn.pos_x, btn.pos_y+btn.y, 5, btn.cor_borda, btn.cor_fundo);
    tela_texto(btn.pos_x+((float)btn.x/2), btn.pos_y+((float)btn.y/2), btn.lt, btn.cor_texto, btn.texto);
    if(btn.id>3){}
    return 0;
}


//Enquanto o mouse estiver pressionado e em cima do botão. este é encolhido
int click_hold(button *ar){
    
    //printf("%i", ar->id);
    if(tela_rato_apertado() && mouse_x>ar->pos_x && mouse_y>ar->pos_y && mouse_x<ar->pos_x+ar->x && mouse_y<ar->pos_y+ar->y){
        ar->x -= 20;
        ar->pos_x += 10;
        ar->y -= 10;
        ar->pos_y += 5;
    }
    return 0;
}

// Retorna o id do botão caso o mouse esteja em cima
int mouse_on(button ar){
    if(mouse_x>ar.pos_x && mouse_y>ar.pos_y && mouse_x<ar.pos_x+ar.x && mouse_y<ar.pos_y+ar.y){
        return ar.id;
        tela_texto_dir(ar.pos_x-10, ar.pos_y+10, ar.lt, ar.cor_texto, ">>>");
    }
    return 0;
}

//Atoa, desenha uma seta ">" quando o mouse está acima do botao
int atoa(button ar){
    if(mouse_x>ar.pos_x && mouse_y>ar.pos_y && mouse_x<ar.pos_x+ar.x && mouse_y<ar.pos_y+ar.y){
        return ar.id;
        tela_texto_dir(ar.pos_x-10, ar.pos_y+10, 50, verde, ">>>sadsadsasda");
    }
    return 5;
}

