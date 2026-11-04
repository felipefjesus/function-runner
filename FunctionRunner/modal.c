#include "modal.h"
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <math.h>

// Calcula a posicao X de cada grupo "[-] a = 0.00 [+]" centralizado na tela
static float x_controle_coef(int num_coef, int indice) {
    const float largura_grupo = 176.0f;
    const float espaco = 20.0f;
    float total = num_coef * largura_grupo + (num_coef - 1) * espaco;
    return LARGURA / 2.0f - total / 2.0f + indice * (largura_grupo + espaco);
}

void abrir_modal(Jogo* j) {
    j->modal_aberta = true;
    j->tecla_A = false;
    j->tecla_D = false;
    j->jogador.vx = 0.0f;
}

void confirmar_funcao(Jogo* j) {
    for (int i = 0; i < MAX_COEF; i++) {
        j->coef_solido[i] = j->coef[i];
    }
    j->rampa_solida = true;
    j->modal_aberta = false;
}

void ajustar_coeficiente(Jogo* j, int indice, int sinal) {
    const Fase* f = &FASES[j->fase_atual];
    if (indice < 0 || indice >= f->num_coef) return;

    j->coef_selecionado = indice;
    j->coef[indice] += sinal * f->passo_coef[indice];
    if (j->coef[indice] > 20.0f) j->coef[indice] = 20.0f;
    if (j->coef[indice] < -20.0f) j->coef[indice] = -20.0f;
}

void processar_tecla_modal(Jogo* j, int tecla) {
    const Fase* f = &FASES[j->fase_atual];

    if (tecla == ALLEGRO_KEY_UP || tecla == ALLEGRO_KEY_W) {
        j->coef_selecionado = (j->coef_selecionado + f->num_coef - 1) % f->num_coef;
    } else if (tecla == ALLEGRO_KEY_DOWN || tecla == ALLEGRO_KEY_S || tecla == ALLEGRO_KEY_TAB) {
        j->coef_selecionado = (j->coef_selecionado + 1) % f->num_coef;
    } else if (tecla == ALLEGRO_KEY_LEFT || tecla == ALLEGRO_KEY_A) {
        ajustar_coeficiente(j, j->coef_selecionado, -1);
    } else if (tecla == ALLEGRO_KEY_RIGHT || tecla == ALLEGRO_KEY_D) {
        ajustar_coeficiente(j, j->coef_selecionado, +1);
    } else if (tecla == ALLEGRO_KEY_ENTER) {
        confirmar_funcao(j);
    } else if (tecla == ALLEGRO_KEY_ESCAPE) {
        j->modal_aberta = false;
    }
}

void processar_clique_modal(Jogo* j, int mx, int my) {
    const Fase* f = &FASES[j->fase_atual];

    // Cliques nos botoes [-] e [+] de cada coeficiente
    for (int i = 0; i < f->num_coef; i++) {
        float x = x_controle_coef(f->num_coef, i);
        if (mx >= x && mx <= x + 44 && my >= 130 && my <= 166) {
            ajustar_coeficiente(j, i, -1);
            return;
        }
        if (mx >= x + 132 && mx <= x + 176 && my >= 130 && my <= 166) {
            ajustar_coeficiente(j, i, +1);
            return;
        }
    }

    // Botao Confirmar (230, 182 ate 390, 222)
    if (mx >= 230 && mx <= 390 && my >= 182 && my <= 222) {
        confirmar_funcao(j);
        return;
    }

    // Botao Cancelar (410, 182 ate 570, 222)
    if (mx >= 410 && mx <= 570 && my >= 182 && my <= 222) {
        j->modal_aberta = false;
        return;
    }
}

static void desenhar_botao_modal(ALLEGRO_FONT* fonte, float x0, float y0, float x1, float y1, const char* texto, ALLEGRO_COLOR fundo, ALLEGRO_COLOR borda) {
    al_draw_filled_rounded_rectangle(x0, y0, x1, y1, 6, 6, fundo);
    al_draw_rounded_rectangle(x0, y0, x1, y1, 6, 6, borda, 2.0f);
    float ty = (y0 + y1) / 2.0f - al_get_font_line_height(fonte) / 2.0f;
    al_draw_text(fonte, al_map_rgb(240, 244, 255), (x0 + x1) / 2.0f, ty, ALLEGRO_ALIGN_CENTRE, texto);
}

void desenhar_modal(const Jogo* j, const Recursos* r) {
    const Fase* f = &FASES[j->fase_atual];
    char formula[96];

    // Painel da modal no topo da tela
    al_draw_filled_rounded_rectangle(30, 44, 770, 254, 14, 14, al_map_rgba(14, 18, 32, 230));
    al_draw_rounded_rectangle(30, 44, 770, 254, 14, 14, al_map_rgb(80, 140, 240), 3.0f);

    // Objetivo da fase e equacao em destaque
    al_draw_text(r->fonte_pequena, al_map_rgb(200, 210, 230), 400, 52, ALLEGRO_ALIGN_CENTRE, f->objetivo);
    montar_formula(f->tipo, j->coef, formula, sizeof(formula));
    al_draw_text(r->fonte_grande, al_map_rgb(255, 225, 80), 400, 74, ALLEGRO_ALIGN_CENTRE, formula);

    // Controles "[-] a = 0.00 [+]" lado a lado
    for (int i = 0; i < f->num_coef; i++) {
        float x = x_controle_coef(f->num_coef, i);
        char valor[64];
        float v = fabsf(j->coef[i]) < 0.0005f ? 0.0f : j->coef[i];
        snprintf(valor, sizeof(valor), "%s = %.2f", f->nome_coef[i], v);

        if (i == j->coef_selecionado) {
            al_draw_rounded_rectangle(x - 5, 125, x + 181, 171, 8, 8, al_map_rgb(255, 210, 80), 2.0f);
        }

        desenhar_botao_modal(r->fonte_pequena, x, 130, x + 44, 166, "-", al_map_rgb(45, 58, 92), al_map_rgb(120, 150, 210));
        al_draw_text(r->fonte_pequena, (i == j->coef_selecionado) ? al_map_rgb(255, 230, 120) : al_map_rgb(230, 235, 250),
                     x + 88, 139, ALLEGRO_ALIGN_CENTRE, valor);
        desenhar_botao_modal(r->fonte_pequena, x + 132, 130, x + 176, 166, "+", al_map_rgb(45, 58, 92), al_map_rgb(120, 150, 210));
    }

    // Botoes Confirmar (verde) e Cancelar (vermelho)
    desenhar_botao_modal(r->fonte, 230, 182, 390, 222, "Confirmar", al_map_rgb(42, 135, 72), al_map_rgb(110, 230, 145));
    desenhar_botao_modal(r->fonte, 410, 182, 570, 222, "Cancelar", al_map_rgb(155, 52, 52), al_map_rgb(235, 120, 120));

    // Texto de ajuda no rodape da modal
    al_draw_text(r->fonte_pequena, al_map_rgb(170, 180, 200), 400, 228, ALLEGRO_ALIGN_CENTRE,
                 "↑↓ escolhe o coeficiente  •  ←→ ajusta  •  ENTER confirma  •  ESC cancela");
}
