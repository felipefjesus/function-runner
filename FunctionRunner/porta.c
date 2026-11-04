#include "porta.h"
#include <allegro5/allegro_primitives.h>

bool jogador_na_porta(float jogador_x, float jogador_y, bool no_chao, float topo_direita) {
    return (no_chao &&
            jogador_x + TAMANHO_JOGADOR >= PORTA_X + 10.0f &&
            jogador_x <= PORTA_X + PORTA_LARGURA - 10.0f &&
            jogador_y + TAMANHO_JOGADOR <= topo_direita + 2.0f);
}

void desenhar_porta(ALLEGRO_BITMAP* img_porta, float topo_direita, bool aberta) {
    if (img_porta) {
        float sx = aberta ? 48.0f : 0.0f;
        al_draw_bitmap_region(img_porta, sx, 0.0f, PORTA_LARGURA, PORTA_ALTURA, PORTA_X, topo_direita - PORTA_ALTURA, 0);
    } else {
        al_draw_filled_rectangle(PORTA_X, topo_direita - PORTA_ALTURA, PORTA_X + PORTA_LARGURA, topo_direita, al_map_rgb(40, 160, 220));
    }
}
