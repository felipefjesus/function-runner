#ifndef LAPIS_H
#define LAPIS_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "configuracao.h"

// Verifica se o robo esta no chao e proximo da estacao do lapis
bool perto_do_lapis(float jogador_x, bool no_chao);

// Verifica se o clique do mouse foi em cima do lapis
bool clique_no_lapis(float topo_esquerda, int mouse_x, int mouse_y);

// Desenha o lapis estatico na plataforma esquerda e o aviso "[E] Projetar"
void desenhar_lapis(ALLEGRO_BITMAP* img_lapis, ALLEGRO_FONT* fonte_pequena, float topo_esquerda, bool perto, bool modal_aberta);

#endif
