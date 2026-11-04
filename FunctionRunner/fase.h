#ifndef FASE_H
#define FASE_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "configuracao.h"
#include "rampa.h"

typedef enum {
    OBST_TETO,
    OBST_ESPINHOS
} TipoObstaculo;

typedef struct {
    TipoObstaculo tipo;
    float x0, x1; // Posicao horizontal em unidades do plano (0 a 8)
    float y0, y1; // Posicao vertical em unidades do plano (y0 < y1)
} Obstaculo;

typedef struct {
    const char* titulo;
    const char* formula;
    const char* objetivo;
    TipoFuncao tipo;
    float origem_y;        // Pixel Y onde fica y = 0 no plano
    float altura_esquerda; // Altura da plataforma esquerda em unidades
    float altura_direita;  // Altura da plataforma direita em unidades
    int num_coef;
    const char* nome_coef[MAX_COEF];
    float coef_inicial[MAX_COEF];
    float passo_coef[MAX_COEF];
    int num_obstaculos;
    Obstaculo obstaculos[MAX_OBSTACULOS];
    float tempo_limite;
} Fase;

// Tabela global com a definicao estrategica das 4 fases (definida em fases_dados.c)
extern const Fase FASES[NUM_FASES];

// Calcula a altura Y do chao (plataforma esquerda, plataforma direita ou curva solida)
bool obter_chao(const Fase* f, bool rampa_solida, const float* coef_solido, float x, float* altura);

// Verifica se o retangulo do robo colidiu com algum obstaculo da fase
bool colide_obstaculo(const Fase* f, float jogador_x, float jogador_y);

// Desenha o fundo, a grade cartesiana, o fosso, os obstaculos e as duas plataformas
void desenhar_cenario_fase(const Fase* f, ALLEGRO_BITMAP* fundo_lab, ALLEGRO_FONT* fonte_pequena);

#endif
