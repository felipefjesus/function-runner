#include "lapis.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>

bool perto_do_lapis(float jogador_x, bool no_chao) {
    float centro_x = jogador_x + TAMANHO_JOGADOR / 2.0f;
    return (fabsf(centro_x - LAPIS_X) <= RAIO_INTERACAO_LAPIS) && no_chao;
}

bool clique_no_lapis(float topo_esquerda, int mouse_x, int mouse_y) {
    float topo_lapis = topo_esquerda - 70.0f;
    return (mouse_x >= LAPIS_X - 25 && mouse_x <= LAPIS_X + 25 &&
            mouse_y >= topo_lapis - 10 && mouse_y <= topo_lapis + 75);
}

void desenhar_lapis(ALLEGRO_BITMAP* img_lapis, ALLEGRO_FONT* fonte_pequena, float topo_esquerda, bool perto, bool modal_aberta) {
    if (img_lapis) {
        al_draw_bitmap(img_lapis, LAPIS_X - 20.0f, topo_esquerda - 70.0f, 0);
    } else {
        al_draw_filled_rectangle(LAPIS_X - 6.0f, topo_esquerda - 50.0f, LAPIS_X + 6.0f, topo_esquerda, al_map_rgb(245, 190, 40));
    }

    if (perto && !modal_aberta) {
        al_draw_text(fonte_pequena, al_map_rgb(255, 230, 90), LAPIS_X, topo_esquerda - 90.0f, ALLEGRO_ALIGN_CENTRE, "[E] Projetar");
    }
}
