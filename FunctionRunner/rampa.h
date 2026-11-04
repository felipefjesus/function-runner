#ifndef RAMPA_H
#define RAMPA_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "configuracao.h"

// Tipos de funcao matematica disponiveis nas 4 fases
typedef enum {
    FUNCAO_CONSTANTE,
    FUNCAO_AFIM,
    FUNCAO_QUADRATICA,
    FUNCAO_SENOIDAL
} TipoFuncao;

// Calcula o valor matematico de f(x) em unidades do plano cartesiano
float avaliar_funcao(TipoFuncao tipo, const float* c, float x);

// Conversoes entre unidades do plano cartesiano e pixels da tela
float unidade_para_px_x(float x);
float px_para_unidade_x(float px);
float unidade_para_px_y(float origem_y, float y);

// Retorna a coordenada Y em pixels da curva para um X em pixels
float altura_curva(TipoFuncao tipo, float origem_y, const float* c, float px);

// Monta a string da equacao (ex: "f(x) = 0.50*x + 1.00")
void montar_formula(TipoFuncao tipo, const float* c, char* destino, int tamanho);

// Desenha o plano cartesiano numerado no vao central
void desenhar_grade(float origem_y, ALLEGRO_FONT* fonte_pequena);

// Desenha a curva da funcao (tracejada para previa, continua para rampa solida)
void desenhar_curva(TipoFuncao tipo, float origem_y, const float* c, ALLEGRO_COLOR cor, float espessura, bool tracejada);

#endif
