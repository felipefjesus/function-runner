#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "configuracao.h"
#include "fase.h"

typedef enum {
    ANIM_REPOUSO,
    ANIM_CORRIDA,
    ANIM_SALTO
} EstadoAnimacao;

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    bool no_chao;
    bool virado_esquerda;
    EstadoAnimacao animacao;
    float tempo_animacao;
} Player;

// Posiciona o robo no inicio da plataforma esquerda
void inicializar_jogador(Player* p, float chao_esquerda);

// Atualiza o quadro da animacao na spritesheet
void atualizar_animacao_jogador(Player* p, float dt);

// Atualiza movimentacao horizontal, limite de inclinacao, gravidade e colisao com a rampa/piso
void atualizar_fisica_jogador(Player* p, const Fase* f, bool rampa_solida, const float* coef_solido, bool tecla_A, bool tecla_D, float dt);

// Desenha o robo recortando o quadro correto da spritesheet (al_draw_bitmap_region)
void desenhar_jogador(const Player* p, ALLEGRO_BITMAP* sprite_robo);

#endif
