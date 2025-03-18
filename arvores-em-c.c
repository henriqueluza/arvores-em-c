#include <stdio.h>
#include <stdlib.h>

// Estrutura para nó de Árvore de Busca Binária (BST)
typedef struct NoBST {
    int valor;
    struct NoBST* esquerda;
    struct NoBST* direita;
} NoBST;

// Estrutura para nó de Árvore AVL
typedef struct NoAVL {
    int valor;
    struct NoAVL* esquerda;
    struct NoAVL* direita;
    int altura;
} NoAVL;

// Estrutura para nó de Árvore 2-3-4
typedef struct No234 {
    int chaves[3];  // Máximo de 3 chaves por nó
    struct No234* filhos[4];  // Máximo de 4 filhos por nó
    int numChaves;  // Número atual de chaves
} No234;

// ======== FUNÇÕES PARA ÁRVORE BST ========

// Criar um novo nó para BST
NoBST* criarNoBST(int valor) {
    NoBST* novoNo = (NoBST*)malloc(sizeof(NoBST));
    if (novoNo == NULL) {
        fprintf(stderr, "Erro na alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    novoNo->valor = valor;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

// Inserir valor na BST
NoBST* inserirBST(NoBST* raiz, int valor) {
    if (raiz == NULL) {
        return criarNoBST(valor);
    }
    
    if (valor < raiz->valor) {
        raiz->esquerda = inserirBST(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = inserirBST(raiz->direita, valor);
    }
    
    return raiz;
}

// Buscar valor na BST
NoBST* buscarBST(NoBST* raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor) {
        return raiz;
    }
    
    if (valor < raiz->valor) {
        return buscarBST(raiz->esquerda, valor);
    }
    
    return buscarBST(raiz->direita, valor);
}

// Encontrar valor mínimo na BST
NoBST* encontrarMinimoBST(NoBST* raiz) {
    NoBST* atual = raiz;
    while (atual && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

// Remover valor da BST
NoBST* removerBST(NoBST* raiz, int valor) {
    if (raiz == NULL) {
        return raiz;
    }
    
    if (valor < raiz->valor) {
        raiz->esquerda = removerBST(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = removerBST(raiz->direita, valor);
    } else {
        // Nó com um ou nenhum filho
        if (raiz->esquerda == NULL) {
            NoBST* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            NoBST* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        
        // Nó com dois filhos
        NoBST* temp = encontrarMinimoBST(raiz->direita);
        raiz->valor = temp->valor;
        raiz->direita = removerBST(raiz->direita, temp->valor);
    }
    
    return raiz;
}

// Percurso em ordem (inorder) para BST
void percorrerInordemBST(NoBST* raiz) {
    if (raiz != NULL) {
        percorrerInordemBST(raiz->esquerda);
        printf("%d ", raiz->valor);
        percorrerInordemBST(raiz->direita);
    }
}

// Exibir BST de forma visual
void exibirBST(NoBST* raiz, int nivel) {
    if (raiz == NULL) {
        return;
    }
    
    exibirBST(raiz->direita, nivel + 1);
    
    for (int i = 0; i < nivel; i++) {
        printf("    ");
    }
    
    printf("%d\n", raiz->valor);
    
    exibirBST(raiz->esquerda, nivel + 1);
}

// Liberar memória da BST
void liberarBST(NoBST* raiz) {
    if (raiz != NULL) {
        liberarBST(raiz->esquerda);
        liberarBST(raiz->direita);
        free(raiz);
    }
}

// ======== FUNÇÕES PARA ÁRVORE AVL ========

// Obter altura do nó AVL
int altura(NoAVL* no) {
    if (no == NULL) {
        return 0;
    }
    return no->altura;
}

// Calcular máximo entre dois números
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Criar um novo nó AVL
NoAVL* criarNoAVL(int valor) {
    NoAVL* no = (NoAVL*)malloc(sizeof(NoAVL));
    if (no == NULL) {
        fprintf(stderr, "Erro na alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    no->valor = valor;
    no->esquerda = NULL;
    no->direita = NULL;
    no->altura = 1;  // Novo nó é inicialmente inserido como folha
    return no;
}

// Rotação à direita
NoAVL* rotacaoDireita(NoAVL* y) {
    NoAVL* x = y->esquerda;
    NoAVL* T2 = x->direita;
    
    // Realizar rotação
    x->direita = y;
    y->esquerda = T2;
    
    // Atualizar alturas
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    
    return x;
}

// Rotação à esquerda
NoAVL* rotacaoEsquerda(NoAVL* x) {
    NoAVL* y = x->direita;
    NoAVL* T2 = y->esquerda;
    
    // Realizar rotação
    y->esquerda = x;
    x->direita = T2;
    
    // Atualizar alturas
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    
    return y;
}

// Obter fator de balanceamento
int fatorBalanceamento(NoAVL* no) {
    if (no == NULL) {
        return 0;
    }
    return altura(no->esquerda) - altura(no->direita);
}

// Inserir nó na árvore AVL
NoAVL* inserirAVL(NoAVL* no, int valor) {
    // Passo 1: Inserção BST normal
    if (no == NULL) {
        return criarNoAVL(valor);
    }
    
    if (valor < no->valor) {
        no->esquerda = inserirAVL(no->esquerda, valor);
    } else if (valor > no->valor) {
        no->direita = inserirAVL(no->direita, valor);
    } else {
        // Valores duplicados não são permitidos
        return no;
    }
    
    // Passo 2: Atualizar altura do nó atual
    no->altura = 1 + max(altura(no->esquerda), altura(no->direita));
    
    // Passo 3: Obter fator de balanceamento
    int balance = fatorBalanceamento(no);
    
    // Passo 4: Se o nó estiver desbalanceado, considerar 4 casos
    
    // Caso Esquerda-Esquerda
    if (balance > 1 && valor < no->esquerda->valor) {
        return rotacaoDireita(no);
    }
    
    // Caso Direita-Direita
    if (balance < -1 && valor > no->direita->valor) {
        return rotacaoEsquerda(no);
    }
    
    // Caso Esquerda-Direita
    if (balance > 1 && valor > no->esquerda->valor) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }
    
    // Caso Direita-Esquerda
    if (balance < -1 && valor < no->direita->valor) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }
    
    // Nó está balanceado, retorná-lo sem alterações
    return no;
}

// Encontrar o nó com valor mínimo na árvore AVL
NoAVL* encontrarMinimoAVL(NoAVL* no) {
    NoAVL* atual = no;
    while (atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

// Remover nó da árvore AVL
NoAVL* removerAVL(NoAVL* raiz, int valor) {
    // Passo 1: Remoção BST padrão
    if (raiz == NULL) {
        return raiz;
    }
    
    if (valor < raiz->valor) {
        raiz->esquerda = removerAVL(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = removerAVL(raiz->direita, valor);
    } else {
        // Nó com um ou nenhum filho
        if (raiz->esquerda == NULL || raiz->direita == NULL) {
            NoAVL* temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            
            // Nenhum filho
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                // Um filho
                *raiz = *temp;  // Copiar conteúdo do filho
            }
            
            free(temp);
        } else {
            // Nó com dois filhos
            NoAVL* temp = encontrarMinimoAVL(raiz->direita);
            raiz->valor = temp->valor;
            raiz->direita = removerAVL(raiz->direita, temp->valor);
        }
    }
    
    // Se a árvore tinha apenas um nó
    if (raiz == NULL) {
        return raiz;
    }
    
    // Passo 2: Atualizar altura do nó atual
    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));
    
    // Passo 3: Obter fator de balanceamento
    int balance = fatorBalanceamento(raiz);
    
    // Passo 4: Rebalancear se necessário
    
    // Caso Esquerda-Esquerda
    if (balance > 1 && fatorBalanceamento(raiz->esquerda) >= 0) {
        return rotacaoDireita(raiz);
    }
    
    // Caso Esquerda-Direita
    if (balance > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }
    
    // Caso Direita-Direita
    if (balance < -1 && fatorBalanceamento(raiz->direita) <= 0) {
        return rotacaoEsquerda(raiz);
    }
    
    // Caso Direita-Esquerda
    if (balance < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }
    
    return raiz;
}

// Exibir árvore AVL
void exibirAVL(NoAVL* raiz, int nivel) {
    if (raiz == NULL) {
        return;
    }
    
    exibirAVL(raiz->direita, nivel + 1);
    
    for (int i = 0; i < nivel; i++) {
        printf("    ");
    }
    
    printf("%d (h=%d)\n", raiz->valor, raiz->altura);
    
    exibirAVL(raiz->esquerda, nivel + 1);
}

// Liberar memória da árvore AVL
void liberarAVL(NoAVL* raiz) {
    if (raiz != NULL) {
        liberarAVL(raiz->esquerda);
        liberarAVL(raiz->direita);
        free(raiz);
    }
}

// ======== FUNÇÕES PARA ÁRVORE 2-3-4 ========

// Criar novo nó para árvore 2-3-4
No234* criarNo234() {
    No234* no = (No234*)malloc(sizeof(No234));
    if (no == NULL) {
        fprintf(stderr, "Erro na alocacao de memória\n");
        exit(EXIT_FAILURE);
    }
    no->numChaves = 0;
    
    for (int i = 0; i < 4; i++) {
        no->filhos[i] = NULL;
    }
    
    return no;
}

// Verificar se um nó é folha
int ehFolha(No234* no) {
    return (no->filhos[0] == NULL);
}

// Buscar chave na árvore 2-3-4
int buscar234(No234* raiz, int chave, No234** noEncontrado, int* posicao) {
    if (raiz == NULL) {
        return 0;
    }
    
    int i = 0;
    while (i < raiz->numChaves && chave > raiz->chaves[i]) {
        i++;
    }
    
    if (i < raiz->numChaves && chave == raiz->chaves[i]) {
        *noEncontrado = raiz;
        *posicao = i;
        return 1;
    }
    
    if (ehFolha(raiz)) {
        return 0;
    }
    
    return buscar234(raiz->filhos[i], chave, noEncontrado, posicao);
}

// Inserir chave em nó não-cheio da árvore 2-3-4
void inserirEmNoNaoCheio(No234* no, int chave) {
    int i = no->numChaves - 1;
    
    if (ehFolha(no)) {
        // Encontrar posição correta para inserir
        while (i >= 0 && chave < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        
        no->chaves[i + 1] = chave;
        no->numChaves++;
    } else {
        // Encontrar o filho onde a chave deve ser inserida
        while (i >= 0 && chave < no->chaves[i]) {
            i--;
        }
        i++;
        
        // Se o filho está cheio, dividir primeiro
        if (no->filhos[i]->numChaves == 3) {
            // Implementar divisão de nó
            No234* y = no->filhos[i];
            No234* z = criarNo234();
            
            // Mover a chave do meio do filho para o nó atual
            no->chaves[no->numChaves] = y->chaves[1];
            no->numChaves++;
            
            // Configurar o novo nó z
            z->numChaves = 1;
            z->chaves[0] = y->chaves[2];
            
            // Se y não for folha, ajustar os filhos
            if (!ehFolha(y)) {
                z->filhos[0] = y->filhos[2];
                z->filhos[1] = y->filhos[3];
                y->filhos[2] = NULL;
                y->filhos[3] = NULL;
            }
            
            // Ajustar y
            y->numChaves = 1;
            
            // Inserir z como filho de no
            for (int j = no->numChaves; j > i + 1; j--) {
                no->filhos[j] = no->filhos[j-1];
            }
            no->filhos[i+1] = z;
            
            // Determinar em qual filho a chave deve ser inserida
            if (chave > no->chaves[i]) {
                i++;
            }
        }
        
        inserirEmNoNaoCheio(no->filhos[i], chave);
    }
}

// Inserir chave na árvore 2-3-4
No234* inserir234(No234* raiz, int chave) {
    // Se a raiz for NULL, criar novo nó
    if (raiz == NULL) {
        raiz = criarNo234();
        raiz->chaves[0] = chave;
        raiz->numChaves = 1;
        return raiz;
    }
    
    // Se a raiz estiver cheia, dividir
    if (raiz->numChaves == 3) {
        No234* novaRaiz = criarNo234();
        novaRaiz->filhos[0] = raiz;
        
        // Dividir raiz e mover chave do meio para cima
        No234* novoNo = criarNo234();
        novoNo->numChaves = 1;
        novoNo->chaves[0] = raiz->chaves[2];
        
        // Ajustar filhos
        if (!ehFolha(raiz)) {
            novoNo->filhos[0] = raiz->filhos[2];
            novoNo->filhos[1] = raiz->filhos[3];
            raiz->filhos[2] = NULL;
            raiz->filhos[3] = NULL;
        }
        
        // Ajustar raiz
        novaRaiz->chaves[0] = raiz->chaves[1];
        novaRaiz->numChaves = 1;
        raiz->numChaves = 1;
        
        // Configurar filhos da nova raiz
        novaRaiz->filhos[1] = novoNo;
        
        // Inserir chave no nó apropriado
        inserirEmNoNaoCheio(novaRaiz, chave);
        
        return novaRaiz;
    } else {
        inserirEmNoNaoCheio(raiz, chave);
    }
    
    return raiz;
}

// Exibir árvore 2-3-4
void exibir234(No234* raiz, int nivel) {
    if (raiz == NULL) {
        return;
    }
    
    int i;
    for (i = 0; i < nivel; i++) {
        printf("    ");
    }
    
    printf("[");
    for (i = 0; i < raiz->numChaves; i++) {
        printf("%d", raiz->chaves[i]);
        if (i < raiz->numChaves - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    
    for (i = 0; i <= raiz->numChaves; i++) {
        if (raiz->filhos[i] != NULL) {
            exibir234(raiz->filhos[i], nivel + 1);
        }
    }
}

// Liberar memória da árvore 2-3-4
void liberar234(No234* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    for (int i = 0; i <= raiz->numChaves; i++) {
        if (raiz->filhos[i] != NULL) {
            liberar234(raiz->filhos[i]);
        }
    }
    
    free(raiz);
}

// ======== FUNÇÃO MAIN ========

int main() {
    int escolha, valor;
    NoBST* raizBST = NULL;
    NoAVL* raizAVL = NULL;
    No234* raiz234 = NULL;
    
    do {
        printf("\n==== MENU DE OPERACOES COM ARVORES ====\n");
        printf("1. Arvore de Busca Binaria (BST)\n");
        printf("2. Arvore AVL\n");
        printf("3. Arvore 2-3-4\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        
        switch (escolha) {
            case 1: // BST
                do {
                    printf("\n--- Operacoes com BST ---\n");
                    printf("1. Inserir valor\n");
                    printf("2. Buscar valor\n");
                    printf("3. Remover valor\n");
                    printf("4. Exibir arvore\n");
                    printf("0. Voltar\n");
                    printf("Escolha uma opcao: ");
                    scanf("%d", &escolha);
                    
                    switch (escolha) {
                        case 1:
                            printf("Digite o valor a inserir: ");
                            scanf("%d", &valor);
                            raizBST = inserirBST(raizBST, valor);
                            printf("Valor %d inserido com sucesso!\n", valor);
                            break;
                        case 2:
                            printf("Digite o valor a buscar: ");
                            scanf("%d", &valor);
                            if (buscarBST(raizBST, valor) != NULL) {
                                printf("Valor %d encontrado na arvore!\n", valor);
                            } else {
                                printf("Valor %d não encontrado na arvore.\n", valor);
                            }
                            break;
                        case 3:
                            printf("Digite o valor a remover: ");
                            scanf("%d", &valor);
                            raizBST = removerBST(raizBST, valor);
                            printf("Operacao de remocao concluida.\n");
                            break;
                        case 4:
                            printf("\nExibicao da BST:\n");
                            if (raizBST == NULL) {
                                printf("Arvore vazia\n");
                            } else {
                                exibirBST(raizBST, 0);
                            }
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opcao invalida!\n");
                    }
                } while (escolha != 0);
                escolha = -1; // Retorna ao menu principal
                break;
                
            case 2: // AVL
                do {
                    printf("\n--- Operacoes com AVL ---\n");
                    printf("1. Inserir valor\n");
                    printf("2. Remover valor\n");
                    printf("3. Exibir arvore\n");
                    printf("0. Voltar\n");
                    printf("Escolha uma opcao: ");
                    scanf("%d", &escolha);
                    
                    switch (escolha) {
                        case 1:
                            printf("Digite o valor a inserir: ");
                            scanf("%d", &valor);
                            raizAVL = inserirAVL(raizAVL, valor);
                            printf("Valor %d inserido com sucesso!\n", valor);
                            break;
                        case 2:
                            printf("Digite o valor a remover: ");
                            scanf("%d", &valor);
                            raizAVL = removerAVL(raizAVL, valor);
                            printf("Operacao de remocao concluida.\n");
                            break;
                        case 3:
                            printf("\nExibicao da arvore AVL:\n");
                            if (raizAVL == NULL) {
                                printf("Arvore vazia\n");
                            } else {
                                exibirAVL(raizAVL, 0);
                            }
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opcao invalida!\n");
                    }
                } while (escolha != 0);
                escolha = -1; // Retorna ao menu principal
                break;
                
            case 3: // Árvore 2-3-4
                do {
                    printf("\n--- Operacoes com Arvore 2-3-4 ---\n");
                    printf("1. Inserir valor\n");
                    printf("2. Buscar valor\n");
                    printf("3. Exibir arvore\n");
                    printf("0. Voltar\n");
                    printf("Escolha uma opcao: ");
                    scanf("%d", &escolha);
                    
                    switch (escolha) {
                        case 1:
                            printf("Digite o valor a inserir: ");
                            scanf("%d", &valor);
                            raiz234 = inserir234(raiz234, valor);
                            printf("Valor %d inserido com sucesso!\n", valor);
                            break;
                        case 2: {
                            printf("Digite o valor a buscar: ");
                            scanf("%d", &valor);
                            No234* noEncontrado = NULL;
                            int posicao = -1;
                            if (buscar234(raiz234, valor, &noEncontrado, &posicao)) {
                                printf("Valor %d encontrado na arvore!\n", valor);
                            } else {
                                printf("Valor %d não encontrado na arvore.\n", valor);
                            }
                            break;
                        }
                        case 3:
                            printf("\nExibicao da arvore 2-3-4:\n");
                            if (raiz234 == NULL) {
                                printf("Arvore vazia\n");
                            } else {
                                exibir234(raiz234, 0);
                            }
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opcao invalida!\n");
                    }
                } while (escolha != 0);
                escolha = -1; // Retorna ao menu principal
                break;
                
            case 0:
                printf("Encerrando programa...\n");
                // Liberar memória
                liberarBST(raizBST);
                liberarAVL(raizAVL);
                liberar234(raiz234);
                break;
                
            default:
                printf("Opção inválida!\n");
        }
    } while (escolha != 0);
    
    return 0;
}

