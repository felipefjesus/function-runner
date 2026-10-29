#include "lapis.h"
#include <math.h>
#include <allegro5/allegro_primitives.h>

void inicializar_lapis(Lapis* l)
{
    l->x = 250.0f;
    l->y = 500.0f;
    l->raio_interacao = 60.0f;
}

bool perto_do_lapis(const Lapis* l, float centro_x)
{
    return fabsf(centro_x - l->x) <= l->raio_interacao;
}

void desenhar_lapis(const Lapis* l)
{
    al_draw_filled_rectangle(l->x - 5, l->y - 30, l->x + 5, l->y - 6, al_map_rgb(245, 190, 40));
    al_draw_filled_triangle(l->x - 5, l->y - 30, l->x + 5, l->y - 30, l->x, l->y - 42, al_map_rgb(230, 185, 145));
    al_draw_filled_triangle(l->x - 2, l->y - 37, l->x + 2, l->y - 37, l->x, l->y - 42, al_map_rgb(40, 40, 40));
    al_draw_filled_rectangle(l->x - 5, l->y - 6, l->x + 5, l->y, al_map_rgb(220, 80, 80));
}
