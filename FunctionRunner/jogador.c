#include "jogador.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>

// Coloca o robo na plataforma esquerda quando a fase comeca ou reinicia
void inicializar_jogador(Player* p, float chao_esquerda) {
    p->x = 60.0f;
    p->y = chao_esquerda - TAMANHO_JOGADOR;
    p->vx = 0.0f;
    p->vy = 0.0f;
    p->no_chao = true;
    p->virado_esquerda = false;
    p->animacao = ANIM_REPOUSO;
    p->tempo_animacao = 0.0f;
}

// Decide qual linha da spritesheet usar (parado, correndo ou pulando)
void atualizar_animacao_jogador(Player* p, float dt) {
    EstadoAnimacao nova;
    if (!p->no_chao) {
        nova = ANIM_SALTO;
    } else if (fabsf(p->vx) > 0.01f) {
        nova = ANIM_CORRIDA;
    } else {
        nova = ANIM_REPOUSO;
    }

    if (nova != p->animacao) {
        p->animacao = nova;
        p->tempo_animacao = 0.0f;
    }
    p->tempo_animacao += dt;

    if (p->vx < 0.0f) p->virado_esquerda = true;
    else if (p->vx > 0.0f) p->virado_esquerda = false;
}

void atualizar_fisica_jogador(Player* p, const Fase* f, bool rampa_solida, const float* coef_solido, bool tecla_A, bool tecla_D, float dt) {
    float dt_fator = dt * FPS;

    p->vx = 0.0f;
    if (tecla_A) p->vx -= VELOCIDADE;
    if (tecla_D) p->vx += VELOCIDADE;

    float pes = p->y + TAMANHO_JOGADOR;
    float x_antigo = p->x;

    // 1. Movimento horizontal (A e D) e limites laterais da janela
    p->x += p->vx * dt_fator;
    if (p->x < 0.0f) p->x = 0.0f;
    if (p->x > LARGURA - TAMANHO_JOGADOR) p->x = (float)(LARGURA - TAMANHO_JOGADOR);

    // 2. Verifica se a rampa ou parede na frente e ingreme demais para subir andando
    float dx = fabsf(p->x - x_antigo);
    float centro_antigo = x_antigo + TAMANHO_JOGADOR / 2.0f;
    float centro_novo = p->x + TAMANHO_JOGADOR / 2.0f;
    bool cruzou_borda = (centro_antigo < VAO_INICIO) != (centro_novo < VAO_INICIO) ||
                        (centro_antigo > VAO_FIM) != (centro_novo > VAO_FIM);

    float subida_permitida = (p->no_chao ? dx * INCLINACAO_MAXIMA + 0.5f : 0.0f) +
                             ((cruzou_borda || !p->no_chao) ? DEGRAU_MAXIMO : 0.0f);
    float chao_novo;
    if (dx > 0.0f && obter_chao(f, rampa_solida, coef_solido, centro_novo, &chao_novo)) {
        if (chao_novo < pes - subida_permitida) {
            p->x = x_antigo; // bloqueia o avanco se for parede ou subida muito inclinada
            dx = 0.0f;
        }
    }

    // 3. Aplica a gravidade na velocidade vertical (vy)
    bool estava_no_chao = p->no_chao;
    p->vy += GRAVIDADE * dt_fator;
    p->y += p->vy * dt_fator;
    p->no_chao = false;

    // 4. Verifica se os pes do robo tocaram na plataforma ou na curva solida da funcao
    float chao;
    if (p->vy >= 0.0f && obter_chao(f, rampa_solida, coef_solido, p->x + TAMANHO_JOGADOR / 2.0f, &chao)) {
        float novos_pes = p->y + TAMANHO_JOGADOR;
        float descida_colada = dx * 2.5f + 4.0f;
        bool acompanhar = estava_no_chao && chao >= pes - subida_permitida && chao <= pes + descida_colada;
        bool aterrissar = pes <= chao + DEGRAU_MAXIMO && novos_pes >= chao;
        if (acompanhar || aterrissar) {
            p->y = chao - TAMANHO_JOGADOR;
            p->vy = 0.0f;
            p->no_chao = true;
        }
    }

    atualizar_animacao_jogador(p, dt);
}

// Recorta um quadrado 40x40 da imagem robo.png dependendo da linha (estado) e coluna (quadro)
void desenhar_jogador(const Player* p, ALLEGRO_BITMAP* sprite_robo) {
    if (!sprite_robo) {
        al_draw_filled_rectangle(p->x, p->y, p->x + TAMANHO_JOGADOR, p->y + TAMANHO_JOGADOR, al_map_rgb(255, 60, 60));
        return;
    }

    int linha = 0;
    int quadro = 0;
    switch (p->animacao) {
        case ANIM_REPOUSO: linha = 0; quadro = (int)(p->tempo_animacao * 4.0f) % 4; break;  // Linha 0: 4 quadros parado
        case ANIM_CORRIDA: linha = 1; quadro = (int)(p->tempo_animacao * 12.0f) % 6; break; // Linha 1: 6 quadros correndo
        case ANIM_SALTO:   linha = 2; quadro = (p->vy < 0.0f) ? 0 : 1; break;               // Linha 2: subindo (0) ou caindo (1)
    }

    // Se estiver andando para a esquerda, espelha o desenho horizontalmente
    int flags = p->virado_esquerda ? ALLEGRO_FLIP_HORIZONTAL : 0;
    al_draw_bitmap_region(sprite_robo, quadro * 40.0f, linha * 40.0f, 40.0f, 40.0f, p->x, p->y, flags);
}
