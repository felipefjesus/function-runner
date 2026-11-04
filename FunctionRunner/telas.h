#ifndef TELAS_H
#define TELAS_H

#include "jogo.h"

// Desenha a barra superior (HUD) com titulo da fase, cronometro MM:SS e tentativas
void desenhar_hud(const Jogo* j, const Recursos* r);

// Telas da maquina de estados (Cartao 07)
void desenhar_tela_menu(const Jogo* j, const Recursos* r);
void desenhar_tela_selecao(const Jogo* j, const Recursos* r);
void desenhar_tela_como_jogar(const Jogo* j, const Recursos* r);
void desenhar_tela_pausa(const Jogo* j, const Recursos* r);
void desenhar_tela_vitoria(const Jogo* j, const Recursos* r);
void desenhar_tela_game_over(const Jogo* j, const Recursos* r);
void desenhar_tela_fim(const Jogo* j, const Recursos* r);

// Trata os cliques do mouse nos botoes dos menus
void processar_clique_telas(Jogo* j, int mx, int my);

#endif
