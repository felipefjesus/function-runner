#ifndef PORTA_H
#define PORTA_H

#include <stdbool.h>
#include "jogador.h"

typedef struct {
    float x, y;
    float largura, altura;
} Porta;

void inicializar_porta(Porta* porta);
bool jogador_na_porta(const Porta* porta, const Player* p);
void desenhar_porta(const Porta* porta);

#endif
