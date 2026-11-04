#ifndef MODAL_H
#define MODAL_H

#include "jogo.h"

// Abre a janela modal e para o movimento do robo
void abrir_modal(Jogo* j);

// Copia os coeficientes ajustados para a rampa solida e fecha a modal
void confirmar_funcao(Jogo* j);

// Altera o valor de um coeficiente respeitando o passo da fase
void ajustar_coeficiente(Jogo* j, int indice, int sinal);

// Trata as teclas pressionadas enquanto a janela modal esta aberta
void processar_tecla_modal(Jogo* j, int tecla);

// Trata os cliques do mouse nos botoes [-], [+], Confirmar e Cancelar
void processar_clique_modal(Jogo* j, int mx, int my);

// Desenha a janela modal da funcao no topo da tela
void desenhar_modal(const Jogo* j, const Recursos* r);

#endif
