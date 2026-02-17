#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define max_terra 5
#define tam_string 150

typedef struct {
    char nome_terra[tam_string];
    char cor[tam_string];
    char objetivo[tam_string];
    int id_objetivo;
    int tropa;
} territorio;

// Protótipos corrigidos
void limpar();
void menu();
void cadas(territorio *war);
void lista(const territorio *war);
void menu_2();
void menu_movimentos();
int condicao(int op); 
void ataque(territorio *war);
void gerar_objetivo(territorio *war);
void listar_objetivo(territorio *war);
void check(territorio *war);


int main() {
    srand(time(NULL));
    int op;
    territorio *war = (territorio*) calloc(max_terra, sizeof(territorio));

    menu();
    cadas(war);
    gerar_objetivo(war);
    menu_2();

    do {
        menu_movimentos();
        scanf("%d", &op);
        limpar();

        int status = condicao(op);
        
        if (status == 0) break; 
        
        if (status == 1) {
            
            ataque(war);
            check(war);
            lista(war);

        } else if (status == 2) {

            listar_objetivo(war);

        } else if (status == 3){

            lista(war);

        } else {

            printf("\nOpcao invalida! Tente novamente.\n");

        }

    } while (op != 0);

    printf("\n\nMovimentos encerrados com sucesso!!!");
    free(war);
    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

void limpar() {

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu() {

    printf("===================================\n");
    printf("      CADASTRO DE TERRITORIOS\n");
    printf("===================================\n");
}

void cadas(territorio *war) {

    for (int i = 0; i < max_terra; i++) {
        printf("\n--- CADASTRO DO TERRITORIO %d ---\n", i + 1);
        printf("Nome: ");
        fgets(war[i].nome_terra, tam_string, stdin);
        war[i].nome_terra[strcspn(war[i].nome_terra, "\n")] = '\0';
        printf("Cor: ");
        fgets(war[i].cor, tam_string, stdin);
        war[i].cor[strcspn(war[i].cor, "\n")] = '\0';
        printf("Tropas: ");
        scanf("%d", &war[i].tropa);
        limpar();
    }
}

void lista(const territorio *war) {

    printf("\n--- SITUACAO ATUAL DO MAPA ---\n");
    for (int j = 0; j < max_terra; j++) {
        printf("[%d] %s (%s) - Tropas: %d\n", j + 1, war[j].nome_terra, war[j].cor, war[j].tropa);
    }
}

void menu_2() {

    printf("\nCadastro concluido!\n");

}

void menu_movimentos() {

    printf("\n--- Menu ---\n");
    printf("\n0 - Sair\n");
    printf("\n1 - Atacar\n");
    printf("\n2 - Visualizar Objetivo\n");
    printf("\n3 - Listagem de Territorios\n");
    printf("\nQaul sua escolha: ");
}

int condicao(int op) {

    if (op == 0) return 0;
    if (op == 1) return 1;
    if (op == 2) return 2;
    if (op == 3) return 3;
    return -1;
}

void ataque(territorio *war) {

    int atk, def, sorte_atk, sorte_def;

    printf("\nID do Atacante (1-5): ");
    scanf("%d", &atk);
    printf("ID do Defensor (1-5): ");
    scanf("%d", &def);
    limpar();

    atk--; def--;

    if (atk < 0 || atk >= max_terra || def < 0 || def >= max_terra) {
        printf("\nERRO: Territorio invalido!\n");
        return;
    }
    if (strcmp(war[atk].cor, war[def].cor) == 0) {
        printf("\nERRO: Nao pode atacar aliados!\n");
        return;
    }
    if (war[atk].tropa <= 1) {
        printf("\nERRO: O atacante precisa de pelo menos 2 tropas!\n");
        return;
    }

    sorte_atk = (rand() % 6) + 1;
    sorte_def = (rand() % 6) + 1;

    printf("\nDado Atacante [%d] vs Dado Defensor [%d]\n", sorte_atk, sorte_def);

    if (sorte_atk > sorte_def) {
        printf("Vitoria do Atacante!\n");
        war[def].tropa--;
    } else {
        printf("Vitoria do Defensor!\n");
        war[atk].tropa--;
    }

    if (war[def].tropa <= 0) {
        printf("\n--- %s FOI CONQUISTADO! ---\n", war[def].nome_terra);
        strcpy(war[def].cor, war[atk].cor);
        war[def].tropa = 1;
        war[atk].tropa--;
    }
}

void gerar_objetivo(territorio *war){

    char obj[max_terra][tam_string];
    for (int j = 0; j < max_terra; j++){
        strcpy(obj[j], war[j].nome_terra);
    }

    static char missao[max_terra][tam_string];
    strcpy(missao[0],"\nDominação Territorial - Ocupar 3/5 dos Territorios!\n");
    strcpy(missao[1],"\nPacificador - Eliminar uma Cor!\n");
    strcpy(missao[2],"\nFortificação Máxima - Possuir 2 Territorios e cada um possuir 3 tropas!\n");
    strcpy(missao[3],"\nEstrategista do Centro 'ID 3' - Conquistar o Territorio Central\n");
    strcpy(missao[4],"\nExpansão Contínua - Possuir 4 Territorios Aleatorios\n");

    int indice[5] = {0, 1, 2, 3, 4};
    
    for (int i = 4; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indice[i];
        indice[i] = indice[j];
        indice[j] = temp;
    }
    
    for (int k = 0; k < max_terra; k ++){
       strcpy(war[k].objetivo, missao[indice[k]]);
       war[k].id_objetivo = indice[k];
    }
    
}
void listar_objetivo(territorio *war) {

    for (int i = 0; i < max_terra; i++) {

        printf("\nJogador %s tem o objetivo:%s", war[i].cor, war[i].objetivo);

    }
}
void check(territorio *war) {

    for (int j = 0; j < max_terra; j++) {

        char *cor_atual = war[j].cor;
        int objetivo = war[j].id_objetivo;

        int territorios_contagem = 0;
        int territorios_com_3_tropas = 0;
        int cores_restantes = 0;
        char cores_encontradas[max_terra][tam_string];

        for (int i = 0; i < max_terra; i++) {

            if (strcmp(war[i].cor, cor_atual) == 0) {
                territorios_contagem++;
                if (war[i].tropa >= 3)
                    territorios_com_3_tropas++;
            }

            int ja_existe = 0;
            for (int k = 0; k < cores_restantes; k++) {
                if (strcmp(cores_encontradas[k], war[i].cor) == 0) {
                    ja_existe = 1;
                    break;
                }
            }

            if (!ja_existe) {
                strcpy(cores_encontradas[cores_restantes], war[i].cor);
                cores_restantes++;
            }
        } 

    
        switch (objetivo) {

            case 0:
                if (territorios_contagem >= 3)
                    printf("\n[VITORIA] %s ocupou 3/5 do mapa!", cor_atual);
                break;

            case 1:
                if (cores_restantes == 1 && territorios_contagem == max_terra)
                    printf("\n[VITORIA] %s e o Pacificador!", cor_atual);
                break;

            case 2:
                if (territorios_com_3_tropas >= 2)
                    printf("\n[VITORIA] %s fortificou seus dominios!", cor_atual);
                break;

            case 3:
                if (strcmp(war[2].cor, cor_atual) == 0)
                    printf("\n[VITORIA] %s dominou o centro estrategico!", cor_atual);
                break;

            case 4:
                if (territorios_contagem >= 4)
                    printf("\n[VITORIA] %s expandiu seu imperio!", cor_atual);
                break;
        }
    }
}
