#include "jogador.h"
#include "fase.h"
#include "configuracao.h"
#include <math.h>
#include <allegro5/allegro_primitives.h>

// Valores originais de movimento da POC.
static const float GRAVIDADE = 0.5f;
static const float FORCA_PULO = -10.0f;
static const float VELOCIDADE = 4.0f;

void reiniciar_jogador(Player* p)
{
    p->x = 100.0f;
    p->y = 100.0f;
    p->vx = 0.0f;
    p->vy = 0.0f;
    p->no_chao = false;
}

void pular_jogador(Player* p)
{
    if (p->no_chao) {
        p->vy = FORCA_PULO;
        p->no_chao = false;
    }
}

void atualizar_jogador(Player* p, const Fase* fase,
    bool tecla_A, bool tecla_D, float dt_fator)
{
    if (dt_fator > 3.0f) dt_fator = 3.0f;


    p->vx = 0.0f;
    if (tecla_A) p->vx -= VELOCIDADE;
    if (tecla_D) p->vx += VELOCIDADE;

    float x_anterior = p->x + TAMANHO_JOGADOR / 2.0f;
    float pes_anteriores = p->y + TAMANHO_JOGADOR;
    bool estava_no_chao = p->no_chao;
    float chao_anterior = 0.0f;
    bool tinha_chao = obter_chao(fase, x_anterior, &chao_anterior);

    p->vy += GRAVIDADE * dt_fator;
    p->x += p->vx * dt_fator;

    if (p->x < 0.0f) p->x = 0.0f;
    if (p->x > LARGURA - TAMANHO_JOGADOR) {
        p->x = (float)(LARGURA - TAMANHO_JOGADOR);
    }

    p->y += p->vy * dt_fator;
    p->no_chao = false;

    float x_pes = p->x + TAMANHO_JOGADOR / 2.0f;
    float chao = 0.0f;
    if (obter_chao(fase, x_pes, &chao)) {
        float referencia = tinha_chao ? chao_anterior : chao;
        float tolerancia = fabsf(fase->rampa.a * (x_pes - x_anterior)) + 1.0f;
        bool acompanhar = estava_no_chao && tinha_chao && fabsf(chao - chao_anterior) <= tolerancia;
        bool aterrissar = pes_anteriores <= referencia + 0.5f &&
            pes_anteriores <= chao + tolerancia &&
            p->y + TAMANHO_JOGADOR >= chao;

        if (p->vy >= 0.0f && (acompanhar || aterrissar)) {
            p->y = chao - TAMANHO_JOGADOR;
            p->vy = 0.0f;
            p->no_chao = true;
        }
    }

}

void desenhar_jogador(const Player* p)
{
    al_draw_filled_rectangle(p->x, p->y,
        p->x + TAMANHO_JOGADOR, p->y + TAMANHO_JOGADOR,
        al_map_rgb(255, 0, 0));
}
