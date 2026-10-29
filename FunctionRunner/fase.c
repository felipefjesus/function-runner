#include "fase.h"
#include "configuracao.h"
#include <allegro5/allegro_primitives.h>

void inicializar_fase(Fase* fase)
{
    fase->piso_esquerdo = 500.0f;
    fase->piso_direito = 360.0f;
    inicializar_rampa(&fase->rampa);
    fase->rampa.base_y = fase->piso_esquerdo;
    inicializar_lapis(&fase->lapis);
    fase->lapis.y = fase->piso_esquerdo;
    inicializar_porta(&fase->porta);
    fase->porta.y = fase->piso_direito;
}

bool obter_chao(const Fase* fase, float x, float* altura)
{
    const Rampa* r = &fase->rampa;
    if (x < r->inicio_x) *altura = fase->piso_esquerdo;
    else if (x > r->fim_x) *altura = fase->piso_direito;
    else if (r->solida) *altura = altura_rampa(r, x, r->a, r->b);
    else return false;
    return true;
}

void desenhar_fase(const Fase* fase)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(0, fase->piso_esquerdo, fase->rampa.inicio_x, ALTURA,
        al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(fase->rampa.fim_x, fase->piso_direito, LARGURA, ALTURA,
        al_map_rgb(255, 255, 255));
    desenhar_lapis(&fase->lapis);
    desenhar_porta(&fase->porta);
    desenhar_rampa(&fase->rampa);
}
