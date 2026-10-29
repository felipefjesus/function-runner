#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdbool.h>
#define TAMANHO_JOGADOR 40

typedef struct {
    float x, y;
    float vx, vy;
    bool no_chao;
} Player;

struct Fase;
void reiniciar_jogador(Player* p);
void pular_jogador(Player* p);
void atualizar_jogador(Player* p, const struct Fase* fase,
    bool tecla_A, bool tecla_D, float dt_fator);
void desenhar_jogador(const Player* p);

#endif
