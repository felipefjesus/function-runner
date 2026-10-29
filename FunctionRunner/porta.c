#include "porta.h"
#include <allegro5/allegro_primitives.h>

void inicializar_porta(Porta* porta)
{
    porta->x = 720.0f;
    porta->y = 360.0f;
    porta->largura = 45.0f;
    porta->altura = 70.0f;
}

bool jogador_na_porta(const Porta* porta, const Player* p)
{
    return p->no_chao &&
        p->x + TAMANHO_JOGADOR >= porta->x &&
        p->x <= porta->x + porta->largura &&
        p->y + TAMANHO_JOGADOR <= porta->y + 5.0f;
}

void desenhar_porta(const Porta* porta)
{
    al_draw_filled_rectangle(porta->x, porta->y - porta->altura, porta->x + porta->largura, porta->y, al_map_rgb(40, 160, 220));
    al_draw_rectangle(porta->x, porta->y - porta->altura, porta->x + porta->largura, porta->y, al_map_rgb(200, 240, 255), 3.0f);
}
