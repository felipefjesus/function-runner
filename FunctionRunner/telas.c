#include "telas.h"
#include "lapis.h"
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <math.h>

static void desenhar_painel(float x0, float y0, float x1, float y1, ALLEGRO_COLOR borda) {
    al_draw_filled_rounded_rectangle(x0, y0, x1, y1, 14, 14, al_map_rgba(14, 18, 32, 230));
    al_draw_rounded_rectangle(x0, y0, x1, y1, 14, 14, borda, 3.0f);
}

static void desenhar_botao_menu(ALLEGRO_FONT* fonte, float x0, float y0, float x1, float y1, const char* texto, int tipo_cor, int mx, int my) {
    bool hover = (mx >= x0 && mx <= x1 && my >= y0 && my <= y1);
    ALLEGRO_COLOR fundo;
    ALLEGRO_COLOR borda;

    if (hover) {
        fundo = al_map_rgb(75, 100, 155);
        borda = al_map_rgb(255, 220, 90);
    } else if (tipo_cor == 1) { // Verde
        fundo = al_map_rgb(42, 135, 72);
        borda = al_map_rgb(110, 230, 145);
    } else if (tipo_cor == 2) { // Vermelho
        fundo = al_map_rgb(155, 52, 52);
        borda = al_map_rgb(235, 120, 120);
    } else { // Azul padrao
        fundo = al_map_rgb(44, 56, 92);
        borda = al_map_rgb(110, 150, 220);
    }

    al_draw_filled_rounded_rectangle(x0, y0, x1, y1, 10, 10, fundo);
    al_draw_rounded_rectangle(x0, y0, x1, y1, 10, 10, borda, 2.0f);

    float ty = (y0 + y1) / 2.0f - al_get_font_line_height(fonte) / 2.0f;
    al_draw_text(fonte, hover ? al_map_rgb(255, 235, 130) : al_map_rgb(240, 244, 255),
                 (x0 + x1) / 2.0f, ty, ALLEGRO_ALIGN_CENTRE, texto);
}

static void desenhar_fundo_menu(const Recursos* r) {
    if (r->fundo_menu) {
        al_draw_bitmap(r->fundo_menu, 0, 0, 0);
    } else {
        al_clear_to_color(al_map_rgb(12, 18, 36));
    }
}

void desenhar_hud(const Jogo* j, const Recursos* r) {
    const Fase* f = &FASES[j->fase_atual];
    char texto[96];

    // Barra superior escura
    al_draw_filled_rectangle(0, 0, LARGURA, 36, al_map_rgba(8, 10, 20, 220));
    al_draw_line(0, 36, LARGURA, 36, al_map_rgb(60, 110, 200), 2.0f);

    // Esquerda: Nome da fase
    al_draw_text(r->fonte_pequena, al_map_rgb(120, 220, 255), 12, 10, 0, f->titulo);

    // Centro: Cronometro regressivo MM:SS
    int segundos = (int)ceilf(j->tempo_restante);
    snprintf(texto, sizeof(texto), "%02d:%02d", segundos / 60, segundos % 60);
    ALLEGRO_COLOR cor_tempo = (j->tempo_restante <= 10.0f) ? al_map_rgb(255, 80, 80) : al_map_rgb(255, 230, 90);
    al_draw_text(r->fonte, cor_tempo, LARGURA / 2.0f, 5, ALLEGRO_ALIGN_CENTRE, texto);

    // Direita: Tentativas
    snprintf(texto, sizeof(texto), "Tentativa %d", j->tentativas);
    al_draw_text(r->fonte_pequena, al_map_rgb(200, 205, 220), LARGURA - 12, 10, ALLEGRO_ALIGN_RIGHT, texto);

    // Rodape com dica de controles
    if (!j->modal_aberta) {
        bool perto = perto_do_lapis(j->jogador.x, j->jogador.no_chao);
        const char* ajuda = perto
            ? "[E] ou clique no lápis: projetar função"
            : "A/D: mover | ESPAÇO: pular | R: reiniciar | ESC: pausa";
        al_draw_text(r->fonte_pequena, al_map_rgb(220, 225, 240), 12, ALTURA - 22, 0, ajuda);
    }
}

void desenhar_tela_menu(const Jogo* j, const Recursos* r) {
    desenhar_fundo_menu(r);

    al_draw_text(r->fonte_titulo, al_map_rgb(110, 220, 255), 400, 75, ALLEGRO_ALIGN_CENTRE, "FUNCTION RUNNER");
    al_draw_text(r->fonte, al_map_rgb(255, 215, 80), 400, 140, ALLEGRO_ALIGN_CENTRE, "G R A P H   P A R K O U R");
    al_draw_text(r->fonte_pequena, al_map_rgb(200, 210, 230), 400, 195, ALLEGRO_ALIGN_CENTRE,
                 "Projete funções matemáticas para guiar o robô pelo laboratório");

    // Robo estatico de enfeite na lateral esquerda
    if (r->robo) {
        al_draw_scaled_bitmap(r->robo, 0, 0, 40, 40, 110, 330, 120, 120, 0);
    }

    desenhar_botao_menu(r->fonte, 280, 250, 520, 302, "Jogar", 0, j->mouse_x, j->mouse_y);
    desenhar_botao_menu(r->fonte, 280, 315, 520, 367, "Selecionar Fase", 0, j->mouse_x, j->mouse_y);
    desenhar_botao_menu(r->fonte, 280, 380, 520, 432, "Como Jogar", 0, j->mouse_x, j->mouse_y);
    desenhar_botao_menu(r->fonte, 280, 445, 520, 497, "Sair", 2, j->mouse_x, j->mouse_y);

    al_draw_text(r->fonte_pequena, al_map_rgb(160, 170, 190), 400, 560, ALLEGRO_ALIGN_CENTRE,
                 "Projeto Integrador II  •  ENTER jogar  •  ESC sair");
}

void desenhar_tela_selecao(const Jogo* j, const Recursos* r) {
    desenhar_fundo_menu(r);
    al_draw_text(r->fonte_titulo, al_map_rgb(120, 225, 255), 400, 40, ALLEGRO_ALIGN_CENTRE, "Selecionar Fase");

    for (int i = 0; i < NUM_FASES; i++) {
        const Fase* f = &FASES[i];
        float x0 = 40.0f + (i % 2) * 370.0f;
        float y0 = 120.0f + (i / 2) * 190.0f;
        float x1 = x0 + 350.0f;
        float y1 = y0 + 170.0f;

        bool hover = (j->mouse_x >= x0 && j->mouse_x <= x1 && j->mouse_y >= y0 && j->mouse_y <= y1);
        ALLEGRO_COLOR borda = hover ? al_map_rgb(255, 210, 80) : al_map_rgb(80, 140, 240);
        desenhar_painel(x0, y0, x1, y1, borda);

        al_draw_text(r->fonte, al_map_rgb(230, 236, 250), x0 + 18, y0 + 16, 0, f->titulo);
        al_draw_text(r->fonte, al_map_rgb(255, 220, 90), x0 + 18, y0 + 50, 0, f->formula);

        char info[64];
        snprintf(info, sizeof(info), "Tempo limite: %.0f s", f->tempo_limite);
        al_draw_text(r->fonte_pequena, al_map_rgb(200, 210, 230), x0 + 18, y0 + 96, 0, info);

        snprintf(info, sizeof(info), "Tecla [%d] ou clique para jogar", i + 1);
        al_draw_text(r->fonte_pequena, al_map_rgb(150, 180, 220), x0 + 18, y0 + 132, 0, info);
    }

    desenhar_botao_menu(r->fonte, 280, 520, 520, 572, "Voltar", 0, j->mouse_x, j->mouse_y);
}

void desenhar_tela_como_jogar(const Jogo* j, const Recursos* r) {
    static const char* linhas[] = {
        "O robô precisa atravessar as 4 salas do laboratório até a porta EXIT.",
        "Entre as plataformas há um vão de ácido: só a sua função vira piso!",
        "",
        "1. Ande com [A] e [D] e pule com [ESPAÇO].",
        "2. Vá até a estação do lápis e aperte [E] (ou clique nele).",
        "3. Ajuste os coeficientes com as setas ou clicando nos botões [-] e [+].",
        "4. A linha tracejada amarela mostra a prévia do gráfico no plano.",
        "5. Aperte [ENTER] (ou Confirmar) para solidificar a rampa verde.",
        "",
        "Atenção às regras das fases:",
        "• Rampas muito íngremes bloqueiam a subida do robô.",
        "• Blocos elétricos no teto e espinhos no chão causam derrota.",
        "• atravesse até a porta EXIT antes que o cronômetro chegue a zero!"
    };

    desenhar_fundo_menu(r);
    desenhar_painel(40, 30, 760, 500, al_map_rgb(80, 140, 240));
    al_draw_text(r->fonte_titulo, al_map_rgb(120, 225, 255), 400, 44, ALLEGRO_ALIGN_CENTRE, "Como Jogar");

    float y = 110;
    for (int i = 0; i < (int)(sizeof(linhas) / sizeof(linhas[0])); i++) {
        al_draw_text(r->fonte_pequena, al_map_rgb(225, 230, 245), 70, y, 0, linhas[i]);
        y += 27;
    }

    desenhar_botao_menu(r->fonte, 280, 520, 520, 572, "Voltar", 0, j->mouse_x, j->mouse_y);
}

void desenhar_tela_pausa(const Jogo* j, const Recursos* r) {
    al_draw_filled_rectangle(0, 0, LARGURA, ALTURA, al_map_rgba(0, 0, 0, 160));
    desenhar_painel(240, 160, 560, 450, al_map_rgb(80, 140, 240));
    al_draw_text(r->fonte_titulo, al_map_rgb(120, 225, 255), 400, 178, ALLEGRO_ALIGN_CENTRE, "PAUSA");

    desenhar_botao_menu(r->fonte, 280, 240, 520, 292, "Continuar", 1, j->mouse_x, j->mouse_y);
    desenhar_botao_menu(r->fonte, 280, 305, 520, 357, "Reiniciar Fase", 0, j->mouse_x, j->mouse_y);
    desenhar_botao_menu(r->fonte, 280, 370, 520, 422, "Menu Principal", 2, j->mouse_x, j->mouse_y);
}

void desenhar_tela_vitoria(const Jogo* j, const Recursos* r) {
    const Fase* f = &FASES[j->fase_atual];
    char formula[96];
    char info[96];

    al_draw_filled_rectangle(0, 0, LARGURA, ALTURA, al_map_rgba(0, 30, 15, 160));
    desenhar_painel(130, 130, 670, 480, al_map_rgb(80, 230, 140));
    al_draw_text(r->fonte_titulo, al_map_rgb(110, 255, 160), 400, 155, ALLEGRO_ALIGN_CENTRE, "SALA CONCLUÍDA!");

    montar_formula(f->tipo, j->coef_solido, formula, sizeof(formula));
    snprintf(info, sizeof(info), "Função utilizada: %s", formula);
    al_draw_text(r->fonte, al_map_rgb(255, 225, 90), 400, 235, ALLEGRO_ALIGN_CENTRE, info);

    snprintf(info, sizeof(info), "Tempo restante: %d s   |   Tentativas: %d", (int)ceilf(j->tempo_restante), j->tentativas);
    al_draw_text(r->fonte_pequena, al_map_rgb(210, 235, 220), 400, 290, ALLEGRO_ALIGN_CENTRE, info);

    desenhar_botao_menu(r->fonte, 150, 380, 310, 430, "Próxima", 1, j->mouse_x, j->mouse_y);
    desenhar_botao_menu(r->fonte, 320, 380, 480, 430, "Repetir", 0, j->mouse_x, j->mouse_y);
    desenhar_botao_menu(r->fonte, 490, 380, 650, 430, "Menu", 2, j->mouse_x, j->mouse_y);
}

void desenhar_tela_game_over(const Jogo* j, const Recursos* r) {
    static const char* motivos[] = {
        "O robô caiu no ácido do laboratório!",
        "O robô colidiu com um obstáculo!",
        "O tempo acabou e a sala foi trancada!"
    };
    char dica[96];

    al_draw_filled_rectangle(0, 0, LARGURA, ALTURA, al_map_rgba(60, 0, 10, 160));
    desenhar_painel(170, 170, 630, 480, al_map_rgb(230, 70, 70));
    al_draw_text(r->fonte_titulo, al_map_rgb(255, 90, 90), 400, 195, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    al_draw_text(r->fonte, al_map_rgb(240, 220, 220), 400, 270, ALLEGRO_ALIGN_CENTRE, motivos[j->motivo]);

    snprintf(dica, sizeof(dica), "Tentativa %d  •  Fórmula da sala: %s", j->tentativas, FASES[j->fase_atual].formula);
    al_draw_text(r->fonte_pequena, al_map_rgb(200, 190, 200), 400, 315, ALLEGRO_ALIGN_CENTRE, dica);

    desenhar_botao_menu(r->fonte, 200, 380, 390, 432, "Tentar de Novo", 1, j->mouse_x, j->mouse_y);
    desenhar_botao_menu(r->fonte, 410, 380, 600, 432, "Menu", 2, j->mouse_x, j->mouse_y);
}

void desenhar_tela_fim(const Jogo* j, const Recursos* r) {
    desenhar_fundo_menu(r);
    desenhar_painel(100, 110, 700, 490, al_map_rgb(255, 210, 80));
    al_draw_text(r->fonte_titulo, al_map_rgb(255, 220, 90), 400, 145, ALLEGRO_ALIGN_CENTRE, "LABORATÓRIO CONCLUÍDO!");
    al_draw_text(r->fonte, al_map_rgb(220, 230, 245), 400, 225, ALLEGRO_ALIGN_CENTRE,
                 "Parabéns! Você venceu todas as 4 fases do jogo:");
    al_draw_text(r->fonte_pequena, al_map_rgb(120, 225, 255), 400, 275, ALLEGRO_ALIGN_CENTRE,
                 "1. Função Constante   •   2. Função Afim");
    al_draw_text(r->fonte_pequena, al_map_rgb(120, 225, 255), 400, 305, ALLEGRO_ALIGN_CENTRE,
                 "3. Função Quadrática  •   4. Função Senoidal");

    desenhar_botao_menu(r->fonte, 200, 390, 390, 442, "Selecionar Fase", 0, j->mouse_x, j->mouse_y);
    desenhar_botao_menu(r->fonte, 410, 390, 600, 442, "Menu Principal", 1, j->mouse_x, j->mouse_y);
}

void processar_clique_telas(Jogo* j, int mx, int my) {
    switch (j->estado) {
        case MENU:
            if (mx >= 280 && mx <= 520) {
                if (my >= 250 && my <= 302) iniciar_fase(j, 0);
                else if (my >= 315 && my <= 367) j->estado = SELECAO_FASE;
                else if (my >= 380 && my <= 432) j->estado = COMO_JOGAR;
                else if (my >= 445 && my <= 497) j->rodando = false;
            }
            break;

        case SELECAO_FASE:
            for (int i = 0; i < NUM_FASES; i++) {
                float x0 = 40.0f + (i % 2) * 370.0f;
                float y0 = 120.0f + (i / 2) * 190.0f;
                if (mx >= x0 && mx <= x0 + 350.0f && my >= y0 && my <= y0 + 170.0f) {
                    iniciar_fase(j, i);
                    return;
                }
            }
            if (mx >= 280 && mx <= 520 && my >= 520 && my <= 572) {
                j->estado = MENU;
            }
            break;

        case COMO_JOGAR:
            if (mx >= 280 && mx <= 520 && my >= 520 && my <= 572) {
                j->estado = MENU;
            }
            break;

        case PAUSA:
            if (mx >= 280 && mx <= 520) {
                if (my >= 240 && my <= 292) j->estado = JOGANDO;
                else if (my >= 305 && my <= 357) reiniciar_fase(j);
                else if (my >= 370 && my <= 422) j->estado = MENU;
            }
            break;

        case VITORIA:
            if (my >= 380 && my <= 430) {
                if (mx >= 150 && mx <= 310) {
                    if (j->fase_atual + 1 < NUM_FASES) iniciar_fase(j, j->fase_atual + 1);
                    else j->estado = FIM_DE_JOGO;
                } else if (mx >= 320 && mx <= 480) {
                    reiniciar_fase(j);
                } else if (mx >= 490 && mx <= 650) {
                    j->estado = MENU;
                }
            }
            break;

        case GAME_OVER:
            if (my >= 380 && my <= 432) {
                if (mx >= 200 && mx <= 390) reiniciar_fase(j);
                else if (mx >= 410 && mx <= 600) j->estado = MENU;
            }
            break;

        case FIM_DE_JOGO:
            if (my >= 390 && my <= 442) {
                if (mx >= 200 && mx <= 390) j->estado = SELECAO_FASE;
                else if (mx >= 410 && mx <= 600) j->estado = MENU;
            }
            break;

        default:
            break;
    }
}
