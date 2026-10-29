#include "rampa.h"
#include <allegro5/allegro_primitives.h>

void inicializar_rampa(Rampa* r)
{
    r->inicio_x = 280.0f;
    r->fim_x = 560.0f;
    r->base_y = 500.0f;
    r->a = 0.0f;
    r->b = 0.0f;
    r->solida = false;
}

// Mesma conversao da funcao afim para as coordenadas da tela.
float altura_rampa(const Rampa* r, float x, float a, float b)
{
    return r->base_y - (a * (x - r->inicio_x) + b * 100.0f);
}

void desenhar_rampa(const Rampa* r)
{
    if (r->solida) {
        al_draw_line(r->inicio_x, altura_rampa(r, r->inicio_x, r->a, r->b),
            r->fim_x, altura_rampa(r, r->fim_x, r->a, r->b),
            al_map_rgb(60, 220, 120), 4.0f);
    }
}

void desenhar_previa_rampa(const Rampa* r, float a, float b)
{
    al_draw_line(r->inicio_x, altura_rampa(r, r->inicio_x, a, b),
        r->fim_x, altura_rampa(r, r->fim_x, a, b),
        al_map_rgb(255, 220, 60), 2.0f);
}
