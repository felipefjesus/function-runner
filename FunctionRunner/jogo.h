#ifndef JOGO_H
#define JOGO_H

#include "jogador.h"
#include "fase.h"
#include "modal.h"

typedef enum { MENU, JOGANDO, VITORIA, GAME_OVER } EstadoJogo;

typedef struct {
    EstadoJogo estado;
    Player jogador;
    Fase fase;
    Modal modal;
    bool rodando;
    bool tecla_A, tecla_D;
} Jogo;

void reiniciar_fase(Jogo* j);
void inicializar_jogo(Jogo* j);
void processar_tecla_down(Jogo* j, int tecla);
void processar_tecla_up(Jogo* j, int tecla);
void processar_mouse_down(Jogo* j, int x, int y);
void atualizar_fisica(Jogo* j, float dt_fator);
void desenhar_jogo(const Jogo* j, ALLEGRO_FONT* fonte);

#endif
