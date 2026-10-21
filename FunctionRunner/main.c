#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// CONSTANTES GLOBAIS
const int LARGURA = 800;
const int ALTURA = 600;
const float FPS = 60.0f;

const float GRAVIDADE = 0.5f;

const float FORCA_PULO = -10.0f;
const float VELOCIDADE = 4.0f;
const int TAMANHO_JOGADOR = 40;

const int PISO = 500;
const float INICIO_RAMPA = 280.0f;
const float FIM_RAMPA = 560.0f;
const float PISO_DIREITO = 360.0f;

// Desenho Lápis e Porta de Saída
const float LAPIS_X = 250.0f;
const float RAIO_INTERACAO_LAPIS = 60.0f;
const float PORTA_X = 720.0f;
const float PORTA_LARGURA = 45.0f;
const float PORTA_ALTURA = 70.0f;

// MÁQUINA DE ESTADOS 
typedef enum {
    MENU,
    JOGANDO,
    VITORIA,
    GAME_OVER
} EstadoJogo;

// ESTRUTURAS DE DADOS
typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    bool no_chao;
} Player;

typedef struct {
    EstadoJogo estado;
    Player jogador;
    bool rodando;
    bool tecla_A;
    bool tecla_D;
    bool modal_aberta;
    bool rampa_solida;
    float a;
    float b;
    float a_solido;
    float b_solido;
} Jogo;

// FUNÇÕES MATEMÁTICAS E DE CHÃO
float altura_rampa(float x, float a, float b)
{
    return PISO - (a * (x - INICIO_RAMPA) + b * 100.0f);
}

bool obter_chao(float x, bool solida, float a, float b, float* altura)
{
    if (x < INICIO_RAMPA) *altura = (float)PISO;
    else if (x > FIM_RAMPA) *altura = PISO_DIREITO;
    else if (solida) *altura = altura_rampa(x, a, b);
    else return false;
    return true;
}

bool jogador_perto_do_lapis(const Jogo* j)
{
    float centro_x = j->jogador.x + TAMANHO_JOGADOR / 2.0f;
    return (fabsf(centro_x - LAPIS_X) <= RAIO_INTERACAO_LAPIS);
}

// GERENCIAMENTO DE ESTADO E REINICIALIZAÇÃO
void reiniciar_fase(Jogo* j)
{
    j->jogador.x = 100.0f;
    j->jogador.y = 100.0f;
    j->jogador.vx = 0.0f;
    j->jogador.vy = 0.0f;
    j->jogador.no_chao = false;

    j->tecla_A = false;
    j->tecla_D = false;
    j->modal_aberta = false;
    j->rampa_solida = false;
    j->a = 0.0f;
    j->b = 0.0f;
    j->a_solido = 0.0f;
    j->b_solido = 0.0f;
}

void inicializar_jogo(Jogo* j)
{
    j->estado = MENU;
    j->rodando = true;
    reiniciar_fase(j);
}

// PROCESSAMENTO DE ENTRADAS (TECLADO)
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
        if (j->modal_aberta) {
            if (tecla == ALLEGRO_KEY_LEFT)        j->a -= 0.1f;
            else if (tecla == ALLEGRO_KEY_RIGHT) j->a += 0.1f;
            else if (tecla == ALLEGRO_KEY_UP)    j->b += 0.1f;
            else if (tecla == ALLEGRO_KEY_DOWN)  j->b -= 0.1f;
            else if (tecla == ALLEGRO_KEY_ENTER) {
                j->a_solido = j->a;
                j->b_solido = j->b;
                j->rampa_solida = true;
                j->modal_aberta = false;
            }
            else if (tecla == ALLEGRO_KEY_ESCAPE) {
                j->modal_aberta = false;
            }
        }
        else {
            if (tecla == ALLEGRO_KEY_E && jogador_perto_do_lapis(j)) {
                j->modal_aberta = true;
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
                if (j->jogador.no_chao) {
                    j->jogador.vy = FORCA_PULO;
                    j->jogador.no_chao = false;
                }
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

// PROCESSAMENTO DE CLIQUES DO MOUSE (NOVO)
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
        if (j->modal_aberta) {
            // Botão [-a] (200, 115, 250, 145)
            if (x >= 200 && x <= 250 && y >= 115 && y <= 145) {
                j->a -= 0.1f;
            }
            // Botão [+a] (260, 115, 310, 145)
            else if (x >= 260 && x <= 310 && y >= 115 && y <= 145) {
                j->a += 0.1f;
            }
            // Botão [+b] (490, 115, 540, 145)
            else if (x >= 490 && x <= 540 && y >= 115 && y <= 145) {
                j->b += 0.1f;
            }
            // Botão [-b] (550, 115, 600, 145)
            else if (x >= 550 && x <= 600 && y >= 115 && y <= 145) {
                j->b -= 0.1f;
            }
            // Botão Confirmar (240, 165, 380, 195)
            else if (x >= 240 && x <= 380 && y >= 165 && y <= 195) {
                j->a_solido = j->a;
                j->b_solido = j->b;
                j->rampa_solida = true;
                j->modal_aberta = false;
            }
            // Botão Cancelar (400, 165, 540, 195)
            else if (x >= 400 && x <= 540 && y >= 165 && y <= 195) {
                j->modal_aberta = false;
            }
        }
        break;

    case GAME_OVER:
    case VITORIA:
        reiniciar_fase(j);
        j->estado = JOGANDO;
        break;
    }
}

// ATUALIZAÇÃO DA FÍSICA E REGRAS DE JOGO 
void atualizar_fisica(Jogo* j, float dt_fator)
{
    if (j->estado != JOGANDO || j->modal_aberta) {
        return;
    }

    if (dt_fator > 3.0f) dt_fator = 3.0f;

    Player* p = &j->jogador;

    p->vx = 0.0f;
    if (j->tecla_A) p->vx -= VELOCIDADE;
    if (j->tecla_D) p->vx += VELOCIDADE;

    float x_anterior = p->x + TAMANHO_JOGADOR / 2.0f;
    float pes_anteriores = p->y + TAMANHO_JOGADOR;
    bool estava_no_chao = p->no_chao;
    float chao_anterior = 0.0f;
    bool tinha_chao = obter_chao(x_anterior, j->rampa_solida, j->a_solido, j->b_solido, &chao_anterior);

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
    if (obter_chao(x_pes, j->rampa_solida, j->a_solido, j->b_solido, &chao)) {
        float referencia = tinha_chao ? chao_anterior : chao;
        float tolerancia = fabsf(j->a_solido * (x_pes - x_anterior)) + 1.0f;
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

    if (p->y > ALTURA) {
        j->estado = GAME_OVER;
    }

    if (p->no_chao &&
        p->x + TAMANHO_JOGADOR >= PORTA_X &&
        p->x <= PORTA_X + PORTA_LARGURA &&
        p->y + TAMANHO_JOGADOR <= PISO_DIREITO + 5.0f) {
        j->estado = VITORIA;
    }
}

// RENDERIZAÇÃO E TELAS
void desenhar_lapis(void)
{
    al_draw_filled_rectangle(LAPIS_X - 5, PISO - 30, LAPIS_X + 5, PISO - 6, al_map_rgb(245, 190, 40));
    al_draw_filled_triangle(LAPIS_X - 5, PISO - 30, LAPIS_X + 5, PISO - 30, LAPIS_X, PISO - 42, al_map_rgb(230, 185, 145));
    al_draw_filled_triangle(LAPIS_X - 2, PISO - 37, LAPIS_X + 2, PISO - 37, LAPIS_X, PISO - 42, al_map_rgb(40, 40, 40));
    al_draw_filled_rectangle(LAPIS_X - 5, PISO - 6, LAPIS_X + 5, PISO, al_map_rgb(220, 80, 80));
}

void desenhar_porta(void)
{
    al_draw_filled_rectangle(PORTA_X, PISO_DIREITO - PORTA_ALTURA, PORTA_X + PORTA_LARGURA, PISO_DIREITO, al_map_rgb(40, 160, 220));
    al_draw_rectangle(PORTA_X, PISO_DIREITO - PORTA_ALTURA, PORTA_X + PORTA_LARGURA, PISO_DIREITO, al_map_rgb(200, 240, 255), 3.0f);
}

void desenhar_modal(ALLEGRO_FONT* fonte, float a, float b)
{
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

void desenhar_tela_menu(ALLEGRO_FONT* fonte)
{
    al_clear_to_color(al_map_rgb(15, 18, 28));

    al_draw_filled_rectangle(180, 160, 620, 440, al_map_rgb(28, 34, 52));
    al_draw_rectangle(180, 160, 620, 440, al_map_rgb(80, 140, 250), 3.0f);

    al_draw_text(fonte, al_map_rgb(80, 200, 255), 400, 210, ALLEGRO_ALIGN_CENTRE, "FUNCTION RUNNER");
    al_draw_text(fonte, al_map_rgb(200, 200, 210), 400, 240, ALLEGRO_ALIGN_CENTRE, "Graph Parkour - Laboratorio de Funcoes");

    al_draw_text(fonte, al_map_rgb(255, 230, 80), 400, 310, ALLEGRO_ALIGN_CENTRE, "Clique aqui ou pressione [ENTER] para Iniciar");
    al_draw_text(fonte, al_map_rgb(160, 160, 175), 400, 360, ALLEGRO_ALIGN_CENTRE, "Pressione [ESC] para Sair");
}

void desenhar_fase(const Jogo* j, ALLEGRO_FONT* fonte)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_filled_rectangle(0, PISO, INICIO_RAMPA, ALTURA, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(FIM_RAMPA, PISO_DIREITO, LARGURA, ALTURA, al_map_rgb(255, 255, 255));

    desenhar_lapis();
    desenhar_porta();

    if (j->rampa_solida) {
        al_draw_line(INICIO_RAMPA, altura_rampa(INICIO_RAMPA, j->a_solido, j->b_solido),
            FIM_RAMPA, altura_rampa(FIM_RAMPA, j->a_solido, j->b_solido),
            al_map_rgb(60, 220, 120), 4.0f);
    }

    if (j->modal_aberta) {
        al_draw_line(INICIO_RAMPA, altura_rampa(INICIO_RAMPA, j->a, j->b),
            FIM_RAMPA, altura_rampa(FIM_RAMPA, j->a, j->b),
            al_map_rgb(255, 220, 60), 2.0f);
    }

    al_draw_filled_rectangle(
        j->jogador.x,
        j->jogador.y,
        j->jogador.x + TAMANHO_JOGADOR,
        j->jogador.y + TAMANHO_JOGADOR,
        al_map_rgb(255, 0, 0)
    );

    if (!j->modal_aberta) {
        if (jogador_perto_do_lapis(j)) {
            al_draw_text(fonte, al_map_rgb(255, 230, 80), 12, 12, 0,
                "A/D: mover | ESPACO: pular | R: reiniciar | [E] ou Clique: Projetar");
            al_draw_text(fonte, al_map_rgb(255, 230, 80), LAPIS_X, PISO - 55, ALLEGRO_ALIGN_CENTRE,
                "[E] Projetar");
        }
        else {
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 12, 12, 0,
                "A/D: mover | ESPACO: pular | R: reiniciar | ESC: menu");
        }
    }
    else {
        desenhar_modal(fonte, j->a, j->b);
    }
}

void desenhar_tela_game_over(ALLEGRO_FONT* fonte)
{
    al_clear_to_color(al_map_rgb(30, 10, 15));

    al_draw_filled_rectangle(200, 190, 600, 410, al_map_rgb(50, 20, 25));
    al_draw_rectangle(200, 190, 600, 410, al_map_rgb(220, 60, 60), 3.0f);

    al_draw_text(fonte, al_map_rgb(255, 80, 80), 400, 235, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    al_draw_text(fonte, al_map_rgb(230, 210, 210), 400, 275, ALLEGRO_ALIGN_CENTRE, "O robo caiu no abismo do laboratorio!");
    al_draw_text(fonte, al_map_rgb(255, 230, 80), 400, 330, ALLEGRO_ALIGN_CENTRE, "Clique ou [R] para Reiniciar");
}

void desenhar_tela_vitoria(ALLEGRO_FONT* fonte)
{
    al_clear_to_color(al_map_rgb(10, 30, 20));

    al_draw_filled_rectangle(200, 190, 600, 410, al_map_rgb(20, 50, 35));
    al_draw_rectangle(200, 190, 600, 410, al_map_rgb(60, 220, 120), 3.0f);

    al_draw_text(fonte, al_map_rgb(80, 255, 140), 400, 235, ALLEGRO_ALIGN_CENTRE, "SALA CONCLUIDA!");
    al_draw_text(fonte, al_map_rgb(210, 240, 220), 400, 275, ALLEGRO_ALIGN_CENTRE, "Voce projetou a funcao corretamente!");
    al_draw_text(fonte, al_map_rgb(255, 230, 80), 400, 330, ALLEGRO_ALIGN_CENTRE, "Clique ou [R] para Jogar Novamente");
}

void desenhar_jogo(const Jogo* j, ALLEGRO_FONT* fonte)
{
    switch (j->estado) {
    case MENU:
        desenhar_tela_menu(fonte);
        break;
    case JOGANDO:
        desenhar_fase(j, fonte);
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

// --- FUNÇÃO PRINCIPAL (MAIN) ---
int main(void)
{
    if (!al_init()) return 1;
    if (!al_install_keyboard()) return 1;
    if (!al_install_mouse()) return 1; // ADICIONADO: Instalar mouse
    if (!al_init_primitives_addon()) return 1;
    al_init_font_addon();

    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA, ALTURA);
    if (!janela) return 1;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        al_destroy_display(janela);
        return 1;
    }

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 1;
    }

    ALLEGRO_FONT* fonte = al_create_builtin_font();
    if (!fonte) {
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 1;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source()); // ADICIONADO: Registrar fonte de eventos do mouse

    Jogo jogo;
    inicializar_jogo(&jogo);

    bool redesenhar = true;
    double tempo_anterior = al_get_time();

    al_start_timer(timer);

    while (jogo.rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            processar_tecla_down(&jogo, evento.keyboard.keycode);
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            processar_tecla_up(&jogo, evento.keyboard.keycode);
        }
        else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { // ADICIONADO: Tratamento de clique do mouse
            processar_mouse_down(&jogo, evento.mouse.x, evento.mouse.y);
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            double tempo_atual = al_get_time();
            double delta_time = tempo_atual - tempo_anterior;
            tempo_anterior = tempo_atual;

            float dt_fator = (float)(delta_time * FPS);
            atualizar_fisica(&jogo, dt_fator);

            redesenhar = true;
        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogo.rodando = false;
        }

        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            redesenhar = false;
            desenhar_jogo(&jogo, fonte);
        }
    }

    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_display(janela);

    return 0;
}