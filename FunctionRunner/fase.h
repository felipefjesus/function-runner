#ifndef FASE_H
#define FASE_H

#include "rampa.h"
#include "lapis.h"
#include "porta.h"

typedef struct Fase {
    float piso_esquerdo, piso_direito;
    Rampa rampa;
    Lapis lapis;
    Porta porta;
} Fase;

void inicializar_fase(Fase* fase);
bool obter_chao(const Fase* fase, float x, float* altura);
void desenhar_fase(const Fase* fase);

#endif
