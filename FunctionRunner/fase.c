#include "fase.h"
#include <allegro5/allegro_primitives.h>

// Descobre onde fica o chao embaixo do robo para um determinado X da tela
bool obter_chao(const Fase* f, bool rampa_solida, const float* coef_solido, float x, float* altura) {
    // Se esta antes do buraco, o chao e a plataforma da esquerda
    if (x < VAO_INICIO) {
        *altura = unidade_para_px_y(f->origem_y, f->altura_esquerda);
        return true;
    }
    // Se ja passou do buraco, o chao e a plataforma da direita
    if (x > VAO_FIM) {
        *altura = unidade_para_px_y(f->origem_y, f->altura_direita);
        return true;
    }
    // Se esta em cima do buraco, so existe chao se o jogador ja confirmou a funcao na modal
    if (rampa_solida) {
        *altura = altura_curva(f->tipo, f->origem_y, coef_solido, x);
        return true;
    }
    return false;
}

// Colisao entre dois retangulos (hitboxes): o retangulo do robo e o retangulo do obstaculo
bool colide_obstaculo(const Fase* f, float jogador_x, float jogador_y) {
    const float margem = 6.0f; // margem pequena para a colisao ficar mais justa visualmente
    float px0 = jogador_x + margem;
    float px1 = jogador_x + TAMANHO_JOGADOR - margem;
    float py0 = jogador_y + margem;
    float py1 = jogador_y + TAMANHO_JOGADOR - 2.0f;

    for (int i = 0; i < f->num_obstaculos; i++) {
        const Obstaculo* o = &f->obstaculos[i];
        // Converte a posicao do obstaculo (que esta em unidades do grafico) para pixels da tela
        float ox0 = unidade_para_px_x(o->x0);
        float ox1 = unidade_para_px_x(o->x1);
        float oy0 = unidade_para_px_y(f->origem_y, o->y1);
        float oy1 = unidade_para_px_y(f->origem_y, o->y0);

        if (px1 > ox0 && px0 < ox1 && py1 > oy0 && py0 < oy1) {
            return true;
        }
    }
    return false;
}

// Desenha os blocos das plataformas da esquerda e da direita usando retangulos do Allegro
static void desenhar_plataforma_estatica(float x0, float x1, float topo) {
    al_draw_filled_rectangle(x0, topo, x1, ALTURA, al_map_rgb(62, 74, 98));        // corpo principal do bloco
    al_draw_filled_rectangle(x0, topo, x1, topo + 8.0f, al_map_rgb(130, 150, 185)); // borda clara onde o robo pisa
    al_draw_rectangle(x0, topo, x1, ALTURA, al_map_rgb(35, 42, 60), 2.0f);          // contorno escuro
}

void desenhar_cenario_fase(const Fase* f, ALLEGRO_BITMAP* fundo_lab, ALLEGRO_FONT* fonte_pequena) {
    // 1. Desenha a imagem de fundo do laboratorio
    if (fundo_lab) {
        al_draw_bitmap(fundo_lab, 0, 0, 0);
    } else {
        al_clear_to_color(al_map_rgb(16, 20, 32));
    }

    // 2. Desenha as linhas e numeros do plano cartesiano no buraco central
    desenhar_grade(f->origem_y, fonte_pequena);

    // 3. Desenha o acido verde la embaixo no fundo do buraco (entre X=240 e X=560)
    al_draw_filled_rectangle(VAO_INICIO, NIVEL_ACIDO, VAO_FIM, ALTURA, al_map_rgb(45, 175, 65));
    al_draw_filled_rectangle(VAO_INICIO, NIVEL_ACIDO, VAO_FIM, NIVEL_ACIDO + 6.0f, al_map_rgb(110, 240, 120));

    // 4. Desenha os blocos de obstaculo da fase (se a fase tiver algum)
    for (int i = 0; i < f->num_obstaculos; i++) {
        const Obstaculo* o = &f->obstaculos[i];
        float x0 = unidade_para_px_x(o->x0);
        float x1 = unidade_para_px_x(o->x1);
        float y0 = unidade_para_px_y(f->origem_y, o->y1);
        float y1 = unidade_para_px_y(f->origem_y, o->y0);

        if (y0 < 36.0f) y0 = 36.0f;
        if (y1 > NIVEL_ACIDO) y1 = NIVEL_ACIDO;

        if (o->tipo == OBST_TETO) {
            // Bloco eletrico roxo que vem do teto (Fases 3 e 4)
            al_draw_filled_rectangle(x0, y0, x1, y1, al_map_rgb(95, 50, 120));
            al_draw_rectangle(x0, y0, x1, y1, al_map_rgb(245, 210, 80), 2.0f);
        } else {
            // Bloco vermelho de espinhos que vem de baixo (Fase 4)
            al_draw_filled_rectangle(x0, y0, x1, y1, al_map_rgb(175, 55, 65));
            al_draw_rectangle(x0, y0, x1, y1, al_map_rgb(245, 130, 130), 2.0f);
        }
    }

    // 5. Converte a altura das plataformas (em unidades) para pixels e desenha os dois lados
    float topo_esq = unidade_para_px_y(f->origem_y, f->altura_esquerda);
    float topo_dir = unidade_para_px_y(f->origem_y, f->altura_direita);
    desenhar_plataforma_estatica(0.0f, VAO_INICIO, topo_esq);
    desenhar_plataforma_estatica(VAO_FIM, (float)LARGURA, topo_dir);
}
