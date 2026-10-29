#ifndef MODAL_H
#define MODAL_H

#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include "rampa.h"

typedef struct {
    bool aberta;
    float a, b;
} Modal;

void reiniciar_modal(Modal* m);
void processar_modal_tecla(Modal* m, Rampa* r, int tecla);
void processar_modal_mouse(Modal* m, Rampa* r, int x, int y);
void desenhar_modal(ALLEGRO_FONT* fonte, const Modal* m);

#endif
