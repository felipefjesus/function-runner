#include "modal.h"
#include <allegro5/allegro_primitives.h>

void reiniciar_modal(Modal* m)
{
    m->aberta = false;
    m->a = 0.0f;
    m->b = 0.0f;
}

// Teclado e mouse confirmam a mesma edicao.
static void confirmar_modal(Modal* m, Rampa* r)
{
    r->a = m->a;
    r->b = m->b;
    r->solida = true;
    m->aberta = false;
}

void processar_modal_tecla(Modal* m, Rampa* r, int tecla)
{
    if (tecla == ALLEGRO_KEY_LEFT) m->a -= 0.1f;
    else if (tecla == ALLEGRO_KEY_RIGHT) m->a += 0.1f;
    else if (tecla == ALLEGRO_KEY_UP) m->b += 0.1f;
    else if (tecla == ALLEGRO_KEY_DOWN) m->b -= 0.1f;
    else if (tecla == ALLEGRO_KEY_ENTER) confirmar_modal(m, r);
    else if (tecla == ALLEGRO_KEY_ESCAPE) m->aberta = false;
}

void processar_modal_mouse(Modal* m, Rampa* r, int x, int y)
{
    if (x >= 200 && x <= 250 && y >= 115 && y <= 145) m->a -= 0.1f;
    else if (x >= 260 && x <= 310 && y >= 115 && y <= 145) m->a += 0.1f;
    else if (x >= 490 && x <= 540 && y >= 115 && y <= 145) m->b += 0.1f;
    else if (x >= 550 && x <= 600 && y >= 115 && y <= 145) m->b -= 0.1f;
    else if (x >= 240 && x <= 380 && y >= 165 && y <= 195) confirmar_modal(m, r);
    else if (x >= 400 && x <= 540 && y >= 165 && y <= 195) m->aberta = false;
}

void desenhar_modal(ALLEGRO_FONT* fonte, const Modal* m)
{
    float a = m->a;
    float b = m->b;
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR amarelo = al_map_rgb(255, 230, 80);
    ALLEGRO_COLOR cinza_claro = al_map_rgb(205, 205, 215);

    // Caixa de fundo estendida para acomodar os botões de clique do mouse
    al_draw_filled_rectangle(30, 30, 770, 210, al_map_rgba(20, 20, 32, 230));
    al_draw_rectangle(30, 30, 770, 210, al_map_rgba(80, 120, 200, 220), 2.0f);

    al_draw_text(fonte, branco, 400, 40, ALLEGRO_ALIGN_CENTRE, "Ajuste da Funcao (Rampa) - Mouse ou Teclado");

    if (b >= 0.0f) {
        al_draw_textf(fonte, amarelo, 400, 68, ALLEGRO_ALIGN_CENTRE, "y = %.1f * x + %.1f", a, b);
    }
    else {
        al_draw_textf(fonte, amarelo, 400, 68, ALLEGRO_ALIGN_CENTRE, "y = %.1f * x - %.1f", a, -b);
    }

    // Botões visuais de ajuste de 'a' e 'b'
    al_draw_filled_rectangle(200, 115, 250, 145, al_map_rgb(60, 70, 90));
    al_draw_text(fonte, branco, 225, 123, ALLEGRO_ALIGN_CENTRE, "- a");

    al_draw_filled_rectangle(260, 115, 310, 145, al_map_rgb(60, 70, 90));
    al_draw_text(fonte, branco, 285, 123, ALLEGRO_ALIGN_CENTRE, "+ a");

    al_draw_filled_rectangle(490, 115, 540, 145, al_map_rgb(60, 70, 90));
    al_draw_text(fonte, branco, 515, 123, ALLEGRO_ALIGN_CENTRE, "+ b");

    al_draw_filled_rectangle(550, 115, 600, 145, al_map_rgb(60, 70, 90));
    al_draw_text(fonte, branco, 575, 123, ALLEGRO_ALIGN_CENTRE, "- b");

    // Botões de Confirmar e Cancelar
    al_draw_filled_rectangle(240, 165, 380, 195, al_map_rgb(50, 160, 80));
    al_draw_text(fonte, branco, 310, 173, ALLEGRO_ALIGN_CENTRE, "Confirmar");

    al_draw_filled_rectangle(400, 165, 540, 195, al_map_rgb(180, 60, 60));
    al_draw_text(fonte, branco, 470, 173, ALLEGRO_ALIGN_CENTRE, "Cancelar");
}
