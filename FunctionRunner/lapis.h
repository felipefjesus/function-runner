#ifndef LAPIS_H
#define LAPIS_H

#include <stdbool.h>

typedef struct {
    float x, y;
    float raio_interacao;
} Lapis;

void inicializar_lapis(Lapis* l);
bool perto_do_lapis(const Lapis* l, float centro_x);
void desenhar_lapis(const Lapis* l);

#endif
