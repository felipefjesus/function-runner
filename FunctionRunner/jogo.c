#include "jogo.h"
#include "lapis.h"
#include "porta.h"
#include "modal.h"
#include "telas.h"
#include <allegro5/allegro_ttf.h>
#include <string.h>

static ALLEGRO_FONT* carregar_fonte_segura(const char* caminho, int tamanho) {
    ALLEGRO_FONT* f = al_load_ttf_font(caminho, tamanho, 0);
    if (!f) f = al_create_builtin_font();
    return f;
}

void carregar_recursos(Recursos* r) {
    memset(r, 0, sizeof(*r));
    r->fonte_pequena = carregar_fonte_segura("assets/JetBrainsMono-Regular.ttf", 15);
    r->fonte         = carregar_fonte_segura("assets/JetBrainsMono-Bold.ttf", 20);
    r->fonte_grande  = carregar_fonte_segura("assets/JetBrainsMono-Bold.ttf", 28);
    r->fonte_titulo  = carregar_fonte_segura("assets/JetBrainsMono-Bold.ttf", 40);

    r->robo       = al_load_bitmap("assets/robo.png");
    r->fundo_lab  = al_load_bitmap("assets/fundo_lab.png");
    r->fundo_menu = al_load_bitmap("assets/fundo_menu.png");
    r->lapis      = al_load_bitmap("assets/lapis.png");
    r->porta      = al_load_bitmap("assets/porta.png");
}

void liberar_recursos(Recursos* r) {
    if (r->fonte_pequena) al_destroy_font(r->fonte_pequena);
    if (r->fonte)         al_destroy_font(r->fonte);
    if (r->fonte_grande)  al_destroy_font(r->fonte_grande);
    if (r->fonte_titulo)  al_destroy_font(r->fonte_titulo);

    if (r->robo)       al_destroy_bitmap(r->robo);
    if (r->fundo_lab)  al_destroy_bitmap(r->fundo_lab);
    if (r->fundo_menu) al_destroy_bitmap(r->fundo_menu);
    if (r->lapis)      al_destroy_bitmap(r->lapis);
    if (r->porta)      al_destroy_bitmap(r->porta);
}

void reiniciar_fase(Jogo* j) {
    const Fase* f = &FASES[j->fase_atual];
    float chao_esq = unidade_para_px_y(f->origem_y, f->altura_esquerda);

    inicializar_jogador(&j->jogador, chao_esq);
    j->tecla_A = false;
    j->tecla_D = false;
    j->modal_aberta = false;
    j->rampa_solida = false;
    j->coef_selecionado = 0;

    for (int i = 0; i < MAX_COEF; i++) {
        j->coef[i] = (i < f->num_coef) ? f->coef_inicial[i] : 0.0f;
        j->coef_solido[i] = j->coef[i];
    }

    j->tempo_restante = f->tempo_limite;
    j->tentativas++;
    j->estado = JOGANDO;
}

void iniciar_fase(Jogo* j, int fase) {
    if (fase < 0 || fase >= NUM_FASES) fase = 0;
    j->fase_atual = fase;
    j->tentativas = 0;
    reiniciar_fase(j);
}

void inicializar_jogo(Jogo* j) {
    memset(j, 0, sizeof(*j));
    j->estado = MENU;
    j->rodando = true;
}

void processar_tecla_down(Jogo* j, int tecla) {
    switch (j->estado) {
        case MENU:
            if (tecla == ALLEGRO_KEY_ENTER || tecla == ALLEGRO_KEY_SPACE) iniciar_fase(j, 0);
            else if (tecla == ALLEGRO_KEY_F) j->estado = SELECAO_FASE;
            else if (tecla == ALLEGRO_KEY_H) j->estado = COMO_JOGAR;
            else if (tecla == ALLEGRO_KEY_ESCAPE) j->rodando = false;
            break;

        case SELECAO_FASE:
            if (tecla >= ALLEGRO_KEY_1 && tecla < ALLEGRO_KEY_1 + NUM_FASES) {
                iniciar_fase(j, tecla - ALLEGRO_KEY_1);
            } else if (tecla == ALLEGRO_KEY_ESCAPE) {
                j->estado = MENU;
            }
            break;

        case COMO_JOGAR:
            if (tecla == ALLEGRO_KEY_ESCAPE || tecla == ALLEGRO_KEY_ENTER) {
                j->estado = MENU;
            }
            break;

        case JOGANDO:
            if (j->modal_aberta) {
                processar_tecla_modal(j, tecla);
            } else {
                if (tecla == ALLEGRO_KEY_E && perto_do_lapis(j->jogador.x, j->jogador.no_chao)) {
                    abrir_modal(j);
                } else if (tecla == ALLEGRO_KEY_A || tecla == ALLEGRO_KEY_LEFT) {
                    j->tecla_A = true;
                } else if (tecla == ALLEGRO_KEY_D || tecla == ALLEGRO_KEY_RIGHT) {
                    j->tecla_D = true;
                } else if (tecla == ALLEGRO_KEY_SPACE || tecla == ALLEGRO_KEY_W || tecla == ALLEGRO_KEY_UP) {
                    if (j->jogador.no_chao) {
                        j->jogador.vy = FORCA_PULO;
                        j->jogador.no_chao = false;
                    }
                } else if (tecla == ALLEGRO_KEY_R) {
                    reiniciar_fase(j);
                } else if (tecla == ALLEGRO_KEY_ESCAPE || tecla == ALLEGRO_KEY_P) {
                    j->tecla_A = false;
                    j->tecla_D = false;
                    j->estado = PAUSA;
                }
            }
            break;

        case PAUSA:
            if (tecla == ALLEGRO_KEY_ESCAPE || tecla == ALLEGRO_KEY_P || tecla == ALLEGRO_KEY_ENTER) j->estado = JOGANDO;
            else if (tecla == ALLEGRO_KEY_R) reiniciar_fase(j);
            else if (tecla == ALLEGRO_KEY_Q) j->estado = MENU;
            break;

        case VITORIA:
            if (tecla == ALLEGRO_KEY_ENTER || tecla == ALLEGRO_KEY_N) {
                if (j->fase_atual + 1 < NUM_FASES) iniciar_fase(j, j->fase_atual + 1);
                else j->estado = FIM_DE_JOGO;
            } else if (tecla == ALLEGRO_KEY_R) {
                reiniciar_fase(j);
            } else if (tecla == ALLEGRO_KEY_ESCAPE) {
                j->estado = MENU;
            }
            break;

        case GAME_OVER:
            if (tecla == ALLEGRO_KEY_R || tecla == ALLEGRO_KEY_ENTER) reiniciar_fase(j);
            else if (tecla == ALLEGRO_KEY_ESCAPE) j->estado = MENU;
            break;

        case FIM_DE_JOGO:
            if (tecla == ALLEGRO_KEY_ENTER || tecla == ALLEGRO_KEY_ESCAPE) j->estado = MENU;
            break;
    }
}

void processar_tecla_up(Jogo* j, int tecla) {
    if (j->estado == JOGANDO) {
        if (tecla == ALLEGRO_KEY_A || tecla == ALLEGRO_KEY_LEFT) j->tecla_A = false;
        else if (tecla == ALLEGRO_KEY_D || tecla == ALLEGRO_KEY_RIGHT) j->tecla_D = false;
    }
}

void processar_mouse_down(Jogo* j, int x, int y) {
    if (j->estado == JOGANDO) {
        if (j->modal_aberta) {
            processar_clique_modal(j, x, y);
        } else {
            const Fase* f = &FASES[j->fase_atual];
            float topo_esq = unidade_para_px_y(f->origem_y, f->altura_esquerda);
            if (perto_do_lapis(j->jogador.x, j->jogador.no_chao) && clique_no_lapis(topo_esq, x, y)) {
                abrir_modal(j);
            }
        }
    } else {
        processar_clique_telas(j, x, y);
    }
}

void atualizar_jogo(Jogo* j, float dt) {
    if (j->estado != JOGANDO) return;
    if (dt > 0.05f) dt = 0.05f;

    // Cronometro regressivo da sala (Cartao 11)
    j->tempo_restante -= dt;
    if (j->tempo_restante <= 0.0f) {
        j->tempo_restante = 0.0f;
        j->motivo = MORTE_TEMPO;
        j->estado = GAME_OVER;
        return;
    }

    if (j->modal_aberta) {
        j->jogador.vx = 0.0f;
        atualizar_animacao_jogador(&j->jogador, dt);
        return;
    }

    const Fase* f = &FASES[j->fase_atual];
    atualizar_fisica_jogador(&j->jogador, f, j->rampa_solida, j->coef_solido, j->tecla_A, j->tecla_D, dt);

    // Colisao com obstaculos da sala
    if (colide_obstaculo(f, j->jogador.x, j->jogador.y)) {
        j->motivo = MORTE_OBSTACULO;
        j->estado = GAME_OVER;
        return;
    }

    // Queda no acido ou fora da tela
    if ((j->jogador.y + TAMANHO_JOGADOR > NIVEL_ACIDO + 6.0f &&
         j->jogador.x + TAMANHO_JOGADOR > VAO_INICIO &&
         j->jogador.x < VAO_FIM) || j->jogador.y > ALTURA) {
        j->motivo = MORTE_ABISMO;
        j->estado = GAME_OVER;
        return;
    }

    // Chegada na porta de saida da plataforma direita
    float topo_dir = unidade_para_px_y(f->origem_y, f->altura_direita);
    if (jogador_na_porta(j->jogador.x, j->jogador.y, j->jogador.no_chao, topo_dir)) {
        j->estado = (j->fase_atual == NUM_FASES - 1) ? FIM_DE_JOGO : VITORIA;
    }
}

static void desenhar_partida(const Jogo* j, const Recursos* r) {
    const Fase* f = &FASES[j->fase_atual];
    float topo_esq = unidade_para_px_y(f->origem_y, f->altura_esquerda);
    float topo_dir = unidade_para_px_y(f->origem_y, f->altura_direita);

    // 1. Cenario, grade, obstaculos e plataformas
    desenhar_cenario_fase(f, r->fundo_lab, r->fonte_pequena);

    // 2. Estacao do lapis e porta de saida
    bool perto = perto_do_lapis(j->jogador.x, j->jogador.no_chao);
    desenhar_lapis(r->lapis, r->fonte_pequena, topo_esq, perto, j->modal_aberta);
    desenhar_porta(r->porta, topo_dir, j->rampa_solida);

    // 3. Curva solida (verde) e previa tracejada (amarela)
    if (j->rampa_solida) {
        desenhar_curva(f->tipo, f->origem_y, j->coef_solido, al_map_rgb(90, 255, 150), 4.0f, false);
    }
    if (j->modal_aberta) {
        desenhar_curva(f->tipo, f->origem_y, j->coef, al_map_rgb(255, 220, 60), 3.0f, true);
    }

    // 4. Robo e HUD
    desenhar_jogador(&j->jogador, r->robo);
    desenhar_hud(j, r);

    // 5. Modal (se estiver aberta)
    if (j->modal_aberta) {
        desenhar_modal(j, r);
    }
}

void desenhar_jogo(const Jogo* j, const Recursos* r) {
    switch (j->estado) {
        case MENU:
            desenhar_tela_menu(j, r);
            break;
        case SELECAO_FASE:
            desenhar_tela_selecao(j, r);
            break;
        case COMO_JOGAR:
            desenhar_tela_como_jogar(j, r);
            break;
        case JOGANDO:
            desenhar_partida(j, r);
            break;
        case PAUSA:
            desenhar_partida(j, r);
            desenhar_tela_pausa(j, r);
            break;
        case VITORIA:
            desenhar_partida(j, r);
            desenhar_tela_vitoria(j, r);
            break;
        case GAME_OVER:
            desenhar_partida(j, r);
            desenhar_tela_game_over(j, r);
            break;
        case FIM_DE_JOGO:
            desenhar_tela_fim(j, r);
            break;
    }

    al_flip_display();
}
