#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ABASTECIMENTOS 100
#define NOME_ARQUIVO "abastecimentos.dat"

struct abastecimento {
    float quilometragem;
    float valor_total;
    float preco_combustivel;
    char tipo_combustivel;
};

void limparBuffer();
int carregarDados(struct abastecimento lista[]);
void salvarDados(const struct abastecimento lista[], int qtd);
const char* obterNomeTipo(char tipo);

void adicionarAbastecimento(struct abastecimento lista[], int *qtd);
void listarAbastecimentos(const struct abastecimento lista[], int qtd);
void exibirTotalLitros(const struct abastecimento lista[], int qtd);
void exibirMaiorValor(const struct abastecimento lista[], int qtd);
void calcularTotalGasto(const struct abastecimento lista[], int qtd);
void exibirResumo(const struct abastecimento lista[], int qtd);

int main() {
    struct abastecimento lista[MAX_ABASTECIMENTOS];
    int quantidade = carregarDados(lista);
    int opcao = 0;

    do {
        printf("\n===== CONTROLE DE ABASTECIMENTOS =====\n");
        printf("1 - Adicionar abastecimento\n");
        printf("2 - Listar abastecimentos\n");
        printf("3 - Exibir a quantidade total de litros abastecidos\n");
        printf("4 - Exibir o abastecimento de maior valor\n");
        printf("5 - Calcular total gasto com combustivel\n");
        printf("6 - Exibir resumo dos abastecimentos\n");
        printf("7 - Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = 0;
            continue;
        }

        switch (opcao) {
            case 1:
                adicionarAbastecimento(lista, &quantidade);
                break;
            case 2:
                listarAbastecimentos(lista, quantidade);
                break;
            case 3:
                exibirTotalLitros(lista, quantidade);
                break;
            case 4:
                exibirMaiorValor(lista, quantidade);
                break;
            case 5:
                calcularTotalGasto(lista, quantidade);
                break;
            case 6:
                exibirResumo(lista, quantidade);
                break;
            case 7:
                salvarDados(lista, quantidade);
                printf("\nDados salvos com sucesso! Saindo do programa...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 7);

    return 0;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

const char* obterNomeTipo(char tipo) {
    switch (toupper(tipo)) {
        case 'G': return "Gasolina";
        case 'E': return "Etanol";
        case 'D': return "Diesel";
        default: return "Desconhecido";
    }
}

int carregarDados(struct abastecimento lista[]) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    
    if (arquivo == NULL) {
        arquivo = fopen(NOME_ARQUIVO, "wb");
        if (arquivo != NULL) {
            fclose(arquivo);
        }
        return 0;
    }

    int qtd = fread(lista, sizeof(struct abastecimento), MAX_ABASTECIMENTOS, arquivo);
    fclose(arquivo);
    return qtd;
}

void salvarDados(const struct abastecimento lista[], int qtd) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "wb");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo para salvar os dados.\n");
        return;
    }

    fwrite(lista, sizeof(struct abastecimento), qtd, arquivo);
    fclose(arquivo);
}

void adicionarAbastecimento(struct abastecimento lista[], int *qtd) {
    if (*qtd >= MAX_ABASTECIMENTOS) {
        printf("\nLimite maximo de %d abastecimentos atingido!\n", MAX_ABASTECIMENTOS);
        return;
    }

    struct abastecimento novo;

    printf("\nQuilometragem atual: ");
    scanf("%f", &novo.quilometragem);

    printf("Valor total: ");
    scanf("%f", &novo.valor_total);

    printf("Preco do combustivel: ");
    scanf("%f", &novo.preco_combustivel);

    do {
        printf("Tipo de combustivel (G - Gasolina, E - Etanol, D - Diesel): ");
        limparBuffer();
        scanf("%c", &novo.tipo_combustivel);
        novo.tipo_combustivel = toupper(novo.tipo_combustivel);
        
        if (novo.tipo_combustivel != 'G' && novo.tipo_combustivel != 'E' && novo.tipo_combustivel != 'D') {
            printf("Tipo invalido! Por favor, informe G, E ou D.\n");
        }
    } while (novo.tipo_combustivel != 'G' && novo.tipo_combustivel != 'E' && novo.tipo_combustivel != 'D');

    lista[*qtd] = novo;
    (*qtd)++;

    printf("\nAbastecimento adicionado com sucesso!\n");
}

void listarAbastecimentos(const struct abastecimento lista[], int qtd) {
    if (qtd == 0) {
        printf("\nNenhum abastecimento cadastrado.\n");
        return;
    }

    printf("\n===== ABASTECIMENTOS =====\n");
    for (int i = 0; i < qtd; i++) {
        float litros = lista[i].valor_total / lista[i].preco_combustivel;
        printf("Abastecimento %d\n", i + 1);
        printf("Quilometragem: %.0f km\n", lista[i].quilometragem);
        printf("Valor total: R$ %.2f\n", lista[i].valor_total);
        printf("Preco combustivel: R$ %.2f\n", lista[i].preco_combustivel);
        printf("Tipo: %s\n", obterNomeTipo(lista[i].tipo_combustivel));
        printf("Litros: %.2f\n\n", litros);
    }
}

void exibirTotalLitros(const struct abastecimento lista[], int qtd) {
    if (qtd == 0) {
        printf("\nNenhum abastecimento cadastrado.\n");
        return;
    }

    float total_litros = 0;
    for (int i = 0; i < qtd; i++) {
        total_litros += (lista[i].valor_total / lista[i].preco_combustivel);
    }

    printf("\nTotal de litros abastecidos: %.2f L\n", total_litros);
}

void exibirMaiorValor(const struct abastecimento lista[], int qtd) {
    if (qtd == 0) {
        printf("\nNenhum abastecimento cadastrado.\n");
        return;
    }

    int indice_maior = 0;
    for (int i = 1; i < qtd; i++) {
        if (lista[i].valor_total > lista[indice_maior].valor_total) {
            indice_maior = i;
        }
    }

    float litros = lista[indice_maior].valor_total / lista[indice_maior].preco_combustivel;

    printf("\n===== ABASTECIMENTO DE MAIOR VALOR =====\n");
    printf("Quilometragem: %.0f km\n", lista[indice_maior].quilometragem);
    printf("Valor total: R$ %.2f\n", lista[indice_maior].valor_total);
    printf("Preco combustivel: R$ %.2f\n", lista[indice_maior].preco_combustivel);
    printf("Tipo: %s\n", obterNomeTipo(lista[indice_maior].tipo_combustivel));
    printf("Litros: %.2f\n", litros);
}

void calcularTotalGasto(const struct abastecimento lista[], int qtd) {
    if (qtd == 0) {
        printf("\nNenhum abastecimento cadastrado.\n");
        return;
    }

    float total_gasto = 0;
    for (int i = 0; i < qtd; i++) {
        total_gasto += lista[i].valor_total;
    }

    printf("\nTotal gasto com combustivel: R$ %.2f\n", total_gasto);
}

void exibirResumo(const struct abastecimento lista[], int qtd) {
    if (qtd == 0) {
        printf("\nNenhum abastecimento cadastrado.\n");
        return;
    }

    float total_gasto = 0;
    float total_litros = 0;
    float soma_precos = 0;
    float maior = lista[0].valor_total;
    float menor = lista[0].valor_total;

    for (int i = 0; i < qtd; i++) {
        total_gasto += lista[i].valor_total;
        total_litros += (lista[i].valor_total / lista[i].preco_combustivel);
        soma_precos += lista[i].preco_combustivel;

        if (lista[i].valor_total > maior) maior = lista[i].valor_total;
        if (lista[i].valor_total < menor) menor = lista[i].valor_total;
    }

    float preco_medio = soma_precos / qtd;

    printf("\n===== ABASTECIMENTOS =====\n");
    printf("Quantidade de abastecimentos: %d\n", qtd);
    printf("Total gasto: R$ %.2f\n", total_gasto);
    printf("Total de litros: %.2f L\n", total_litros);
    printf("Maior abastecimento: R$ %.2f\n", maior);
    printf("Menor abastecimento: R$ %.2f\n", menor);
    printf("Preco medio do combustivel: R$ %.2f\n", preco_medio);
}