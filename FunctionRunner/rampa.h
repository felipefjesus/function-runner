#ifndef RAMPA_H
#define RAMPA_H

#include <stdbool.h>

typedef struct {
    float inicio_x, fim_x, base_y;
    float a, b;
    bool solida;
} Rampa;

void inicializar_rampa(Rampa* r);
float altura_rampa(const Rampa* r, float x, float a, float b);
void desenhar_rampa(const Rampa* r);
void desenhar_previa_rampa(const Rampa* r, float a, float b);

#endif
