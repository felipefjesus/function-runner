#ifndef PORTA_H
#define PORTA_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "configuracao.h"

// Verifica se o robo alcancou a porta de saida na plataforma direita
bool jogador_na_porta(float jogador_x, float jogador_y, bool no_chao, float topo_direita);

// Desenha a porta/elevador de saida (fechada ou aberta quando a funcao foi projetada)
void desenhar_porta(ALLEGRO_BITMAP* img_porta, float topo_direita, bool aberta);

#endif
