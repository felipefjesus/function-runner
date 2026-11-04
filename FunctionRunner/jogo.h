#ifndef JOGO_H
#define JOGO_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "configuracao.h"
#include "fase.h"
#include "jogador.h"

// Recursos graficos e fontes carregados na inicializacao
typedef struct {
    ALLEGRO_FONT* fonte_pequena;
    ALLEGRO_FONT* fonte;
    ALLEGRO_FONT* fonte_grande;
    ALLEGRO_FONT* fonte_titulo;
    ALLEGRO_BITMAP* robo;
    ALLEGRO_BITMAP* fundo_lab;
    ALLEGRO_BITMAP* fundo_menu;
    ALLEGRO_BITMAP* lapis;
    ALLEGRO_BITMAP* porta;
} Recursos;

// Estrutura central com o estado atual da partida
typedef struct {
    EstadoJogo estado;
    Player jogador;
    bool rodando;
    bool tecla_A;
    bool tecla_D;
    bool modal_aberta;
    bool rampa_solida;
    int fase_atual;
    int coef_selecionado;
    float coef[MAX_COEF];
    float coef_solido[MAX_COEF];
    float tempo_restante;
    int tentativas;
    MotivoDerrota motivo;
    int mouse_x;
    int mouse_y;
} Jogo;

// Carrega e libera as fontes e imagens da pasta assets/
void carregar_recursos(Recursos* r);
void liberar_recursos(Recursos* r);

// Inicializacao e controle de fases
void inicializar_jogo(Jogo* j);
void iniciar_fase(Jogo* j, int fase);
void reiniciar_fase(Jogo* j);

// Eventos e loop logico
void processar_tecla_down(Jogo* j, int tecla);
void processar_tecla_up(Jogo* j, int tecla);
void processar_mouse_down(Jogo* j, int x, int y);
void atualizar_jogo(Jogo* j, float dt);
void desenhar_jogo(const Jogo* j, const Recursos* r);

#endif
