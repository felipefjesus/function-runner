#include "jogo.h"
#include "configuracao.h"
#include "telas.h"

void reiniciar_fase(Jogo* j)
{
    reiniciar_jogador(&j->jogador);
    inicializar_fase(&j->fase);
    reiniciar_modal(&j->modal);
    j->tecla_A = false;
    j->tecla_D = false;
}

void inicializar_jogo(Jogo* j)
{
    j->estado = MENU;
    j->rodando = true;
    reiniciar_fase(j);
}

void processar_tecla_down(Jogo* j, int tecla)
{
    switch (j->estado) {
    case MENU:
        if (tecla == ALLEGRO_KEY_ENTER || tecla == ALLEGRO_KEY_SPACE) {
            reiniciar_fase(j);
            j->estado = JOGANDO;
        }
        else if (tecla == ALLEGRO_KEY_ESCAPE) {
            j->rodando = false;
        }
        break;

    case JOGANDO:
        if (j->modal.aberta) {
            processar_modal_tecla(&j->modal, &j->fase.rampa, tecla);
        }
        else {
            if (tecla == ALLEGRO_KEY_E && perto_do_lapis(&j->fase.lapis, j->jogador.x + TAMANHO_JOGADOR / 2.0f)) {
                j->modal.aberta = true;
                j->tecla_A = false;
                j->tecla_D = false;
                j->jogador.vx = 0.0f;
            }
            else if (tecla == ALLEGRO_KEY_A) {
                j->tecla_A = true;
            }
            else if (tecla == ALLEGRO_KEY_D) {
                j->tecla_D = true;
            }
            else if (tecla == ALLEGRO_KEY_SPACE) {
                pular_jogador(&j->jogador);
            }
            else if (tecla == ALLEGRO_KEY_R) {
                reiniciar_fase(j);
            }
            else if (tecla == ALLEGRO_KEY_ESCAPE) {
                j->estado = MENU;
            }
        }
        break;

    case GAME_OVER:
    case VITORIA:
        if (tecla == ALLEGRO_KEY_R || tecla == ALLEGRO_KEY_ENTER) {
            reiniciar_fase(j);
            j->estado = JOGANDO;
        }
        else if (tecla == ALLEGRO_KEY_ESCAPE) {
            j->estado = MENU;
        }
        break;
    }
}

void processar_tecla_up(Jogo* j, int tecla)
{
    if (j->estado == JOGANDO) {
        if (tecla == ALLEGRO_KEY_A)      j->tecla_A = false;
        else if (tecla == ALLEGRO_KEY_D) j->tecla_D = false;
    }
}

void processar_mouse_down(Jogo* j, int x, int y)
{
    switch (j->estado) {
    case MENU:
        // Clicar na caixa do menu central para iniciar
        if (x >= 180 && x <= 620 && y >= 160 && y <= 440) {
            reiniciar_fase(j);
            j->estado = JOGANDO;
        }
        break;

    case JOGANDO:
        if (j->modal.aberta) {
            processar_modal_mouse(&j->modal, &j->fase.rampa, x, y);
        }
        break;

    case GAME_OVER:
    case VITORIA:
        reiniciar_fase(j);
        j->estado = JOGANDO;
        break;
    }
}

void atualizar_fisica(Jogo* j, float dt_fator)
{
    if (j->estado != JOGANDO || j->modal.aberta) return;
    atualizar_jogador(&j->jogador, &j->fase, j->tecla_A, j->tecla_D, dt_fator);
    if (j->jogador.y > ALTURA) j->estado = GAME_OVER;
    if (jogador_na_porta(&j->fase.porta, &j->jogador)) j->estado = VITORIA;
}

static void desenhar_cena(const Jogo* j, ALLEGRO_FONT* fonte)
{
    desenhar_fase(&j->fase);
    if (j->modal.aberta) {
        desenhar_previa_rampa(&j->fase.rampa, j->modal.a, j->modal.b);
    }
    desenhar_jogador(&j->jogador);

    if (!j->modal.aberta) {
        if (perto_do_lapis(&j->fase.lapis, j->jogador.x + TAMANHO_JOGADOR / 2.0f)) {
            al_draw_text(fonte, al_map_rgb(255, 230, 80), 12, 12, 0,
                "A/D: mover | ESPACO: pular | R: reiniciar | [E] ou Clique: Projetar");
            al_draw_text(fonte, al_map_rgb(255, 230, 80), j->fase.lapis.x, j->fase.lapis.y - 55, ALLEGRO_ALIGN_CENTRE,
                "[E] Projetar");
        }
        else {
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 12, 12, 0,
                "A/D: mover | ESPACO: pular | R: reiniciar | ESC: menu");
        }
    }
    else {
        desenhar_modal(fonte, &j->modal);
    }
}

void desenhar_jogo(const Jogo* j, ALLEGRO_FONT* fonte)
{
    switch (j->estado) {
    case MENU:
        desenhar_tela_menu(fonte);
        break;
    case JOGANDO:
        desenhar_cena(j, fonte);
        break;
    case GAME_OVER:
        desenhar_tela_game_over(fonte);
        break;
    case VITORIA:
        desenhar_tela_vitoria(fonte);
        break;
    }

    al_flip_display();
}
