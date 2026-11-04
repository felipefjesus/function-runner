#include "rampa.h"
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <math.h>

// Calcula o resultado matematico de f(x) dependendo do tipo da fase atual
float avaliar_funcao(TipoFuncao tipo, const float* c, float x) {
    switch (tipo) {
        case FUNCAO_CONSTANTE:  return c[0];                             // f(x) = c
        case FUNCAO_AFIM:       return c[0] * x + c[1];                  // f(x) = a*x + b
        case FUNCAO_QUADRATICA: return c[0] * x * x + c[1] * x + c[2];   // f(x) = a*x^2 + b*x + c
        case FUNCAO_SENOIDAL:   return c[0] * sinf(c[1] * x);            // f(x) = A*sin(B*x)
    }
    return 0.0f;
}

// No nosso jogo, cada 1 unidade do grafico equivale a 40 pixels na tela (UNIDADE = 40)
float unidade_para_px_x(float x) {
    return VAO_INICIO + x * UNIDADE;
}

float px_para_unidade_x(float px) {
    return (px - VAO_INICIO) / UNIDADE;
}

// No Allegro o Y cresce para baixo, entao subtraimos da origem_y para que o Y matematico cresca para cima
float unidade_para_px_y(float origem_y, float y) {
    return origem_y - y * UNIDADE;
}

// Dado um X em pixels da tela, descobre em qual altura Y (em pixels) passa a curva
float altura_curva(TipoFuncao tipo, float origem_y, const float* c, float px) {
    float x_unidade = px_para_unidade_x(px);
    float y_unidade = avaliar_funcao(tipo, c, x_unidade);
    return unidade_para_px_y(origem_y, y_unidade);
}

static void formatar_numero(char* destino, int tam, float v) {
    if (fabsf(v) < 0.0005f) v = 0.0f;
    snprintf(destino, tam, "%.2f", v);
}

// Monta o texto da equacao que aparece em amarelo no topo da modal
void montar_formula(TipoFuncao tipo, const float* c, char* s, int tam) {
    char n0[16], n1[16], n2[16];
    formatar_numero(n0, sizeof(n0), c[0]);
    formatar_numero(n1, sizeof(n1), fabsf(c[1]));
    formatar_numero(n2, sizeof(n2), fabsf(c[2]));

    switch (tipo) {
        case FUNCAO_CONSTANTE:
            snprintf(s, tam, "f(x) = %s", n0);
            break;
        case FUNCAO_AFIM:
            snprintf(s, tam, "f(x) = %s·x %c %s", n0, c[1] < 0 ? '-' : '+', n1);
            break;
        case FUNCAO_QUADRATICA:
            snprintf(s, tam, "f(x) = %s·x² %c %s·x %c %s", n0, c[1] < 0 ? '-' : '+', n1, c[2] < 0 ? '-' : '+', n2);
            break;
        case FUNCAO_SENOIDAL:
            formatar_numero(n1, sizeof(n1), c[1]);
            snprintf(s, tam, "f(x) = %s·sin(%s·x)", n0, n1);
            break;
    }
}

void desenhar_grade(float origem_y, ALLEGRO_FONT* fonte_pequena) {
    ALLEGRO_COLOR cor_linha = al_map_rgba(70, 130, 220, 45);
    ALLEGRO_COLOR cor_eixo = al_map_rgba(150, 200, 255, 150);
    ALLEGRO_COLOR cor_texto = al_map_rgba(160, 200, 255, 170);
    char rotulo[8];

    // Linhas verticais e numeracao do eixo X (de 0 ate 8 unidades)
    for (int i = 0; i <= 8; i++) {
        float x = unidade_para_px_x((float)i);
        al_draw_line(x, 40, x, NIVEL_ACIDO, cor_linha, 1.0f);
        snprintf(rotulo, sizeof(rotulo), "%d", i);
        al_draw_text(fonte_pequena, cor_texto, x, origem_y + 4, ALLEGRO_ALIGN_CENTRE, rotulo);
    }

    // Linhas horizontais da grade
    for (float y = origem_y; y > 40; y -= UNIDADE) {
        al_draw_line(VAO_INICIO, y, VAO_FIM, y, cor_linha, 1.0f);
    }
    for (float y = origem_y + UNIDADE; y < NIVEL_ACIDO; y += UNIDADE) {
        al_draw_line(VAO_INICIO, y, VAO_FIM, y, cor_linha, 1.0f);
    }

    // Numeracao do eixo Y
    for (int i = -3; i <= 11; i++) {
        float y = unidade_para_px_y(origem_y, (float)i);
        if (i == 0 || y < 50 || y > NIVEL_ACIDO - 8) continue;
        snprintf(rotulo, sizeof(rotulo), "%d", i);
        al_draw_text(fonte_pequena, cor_texto, VAO_INICIO + 4, y - 8, 0, rotulo);
    }

    // Eixos principais X e Y mais fortes
    al_draw_line(VAO_INICIO, origem_y, VAO_FIM, origem_y, cor_eixo, 2.0f);
    al_draw_line(VAO_INICIO, 40, VAO_INICIO, NIVEL_ACIDO, cor_eixo, 2.0f);
    al_draw_text(fonte_pequena, cor_eixo, VAO_FIM - 12, origem_y - 18, 0, "x");
    al_draw_text(fonte_pequena, cor_eixo, VAO_INICIO + 6, 44, 0, "y");
}

// Percorre o buraco de 4 em 4 pixels ligando pequenos segmentos de reta para formar a curva
void desenhar_curva(TipoFuncao tipo, float origem_y, const float* c, ALLEGRO_COLOR cor, float espessura, bool tracejada) {
    const float passo = 4.0f;
    for (float x = VAO_INICIO; x < VAO_FIM; x += passo) {
        // Quando e a previa amarela (tracejada == true), falha 1 pedacinho a cada 3 para dar o efeito tracejado
        if (tracejada && ((int)((x - VAO_INICIO) / passo) % 3 == 2)) {
            continue;
        }
        float y0 = altura_curva(tipo, origem_y, c, x);
        float y1 = altura_curva(tipo, origem_y, c, x + passo);

        if ((y0 < -50 && y1 < -50) || (y0 > ALTURA + 50 && y1 > ALTURA + 50)) {
            continue;
        }
        al_draw_line(x, y0, x + passo, y1, cor, espessura);
    }
}
