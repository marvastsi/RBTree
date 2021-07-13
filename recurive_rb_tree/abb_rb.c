#include "abb_rb.h"
/**************************************
* DADOS
**************************************/
#define BLACK 0
#define RED 1

struct no
{
    int cor;
    TipoElemento dado;
    struct no *esq, *dir, *pai;
};

/**************************************
* FUNÇÕES AUXILIARES
**************************************/

No *criar_no(TipoElemento elemento)
{
    No *no = (No *)malloc(sizeof(No));
    no->dado = elemento;
    no->esq = NULL;
    no->dir = NULL;
    no->pai = NULL;
    no->cor = RED;
    return no;
}

bool eh_vazia(No *raiz)
{
    return (raiz == NULL ? true : false);
}

void destruir_recursivo(No *raiz)
{
    if (!eh_vazia(raiz))
    {
        destruir_recursivo(raiz->esq);
        destruir_recursivo(raiz->dir);
        free(raiz);
    }
}

int no_cor(No *no)
{
    return (no == NULL) ? BLACK : no->cor;
}

/**
 * @deprecated
 * @brief 
 * 
 * @param no 
 */
void troca_cor(No *no)
{
    no->cor = !no->cor;
    if (no->esq != NULL)
        no->esq->cor = !no->esq->cor;
    if (no->dir != NULL)
        no->dir->cor = !no->dir->cor;
}

No *no_avo(No *raiz)
{
    if ((raiz != NULL) && (raiz->pai != NULL))
        return (raiz->pai)->pai;
    else
        return NULL;
}

No *no_tio(No *raiz)
{
    No *avo = no_avo(raiz);
    if (avo == NULL)
        return NULL;

    if (raiz->pai == avo->esq)
        return avo->dir;
    else
        return avo->esq;
}

/** menor No  sempre a esquerda*/
No *menor(No *no)
{
    return (no->esq != NULL) ? menor(no->esq) : no;
}

/** menor No  sempre a direita*/
No *maior(No *no)
{
    return (no->dir != NULL) ? maior(no->dir) : no;
}

/** Rotaciona à direita LL*/
void rotacao_direita(No **enderecoRaiz)
{
    if (*enderecoRaiz == NULL)
        return;
    //rotação normal
    No *aux = (*enderecoRaiz)->esq;
    (*enderecoRaiz)->esq = aux->dir;
    aux->dir = *enderecoRaiz;
    //rotação RB
    aux->pai = (*enderecoRaiz)->pai;
    (*enderecoRaiz)->pai = aux;
    aux->cor = BLACK;
    (*enderecoRaiz)->cor = RED;

    *enderecoRaiz = aux;
}

/** Rotaciona à esquerda RR*/
void rotacao_esquerda(No **enderecoRaiz)
{
    if (*enderecoRaiz == NULL)
        return;
    //rotação normal
    No *aux = (*enderecoRaiz)->dir;
    (*enderecoRaiz)->dir = aux->esq;
    aux->esq = *enderecoRaiz;
    //rotação RB
    aux->pai = (*enderecoRaiz)->pai;
    (*enderecoRaiz)->pai = aux;
    aux->cor = BLACK;
    (*enderecoRaiz)->cor = RED;

    *enderecoRaiz = aux;
}

/** rotacao dupla a direita LR */
void dupla_direita(No **enderecoRaiz)
{
    rotacao_esquerda(&((*enderecoRaiz)->esq));
    rotacao_direita(&(*enderecoRaiz));
}

/** rotacao dupla a esquerda RL */
void dupla_esquerda(No **enderecoRaiz)
{
    rotacao_direita(&((*enderecoRaiz)->dir));
    rotacao_esquerda(&(*enderecoRaiz));
}

/**
 * @brief Busca um No na arvore de forma recursiva, com base no dado/chave
 * 
 * @param raiz raiz da arvore ou subarvore
 * @param dado dado a ser buscado
 * @return No* retorna um ponteiro para o No caso encontrado ou NULL caso contrario.
 */
No *rb_consultar_No(No *raiz, TipoElemento dado)
{
    if (eh_vazia(raiz))
        return NULL;
    else if (raiz->dado > dado)
        return rb_consultar_No(raiz->esq, dado);
    else if (raiz->dado < dado)
        return rb_consultar_No(raiz->dir, dado);
    else
        return raiz;
}

/**
 * @brief RB-Insert-Fixup: Corrige o balanceamento da arvore ou sub-arvore
 * apos a inserção de um No
 * 
 * @param enderecoRaiz ponteiro de ponteiro para a raiz da subarvore a ser rebalanceada
 */
void corrige_inserir(No **enderecoRaiz)
{
    No *tio, *avo, *pai;
    if (eh_vazia(*enderecoRaiz) || no_cor((*enderecoRaiz)->pai) == BLACK)
        return;

    if (no_cor((*enderecoRaiz)) == RED)
    {
        tio = no_tio(*enderecoRaiz);
        pai = (*enderecoRaiz)->pai;
        if (no_cor(tio) == RED)
        {                                                           /* a avo.dir eh vermelho */
            avo = no_avo(*enderecoRaiz);                            /* Caso 1: violacao da 4 No inserido e No.pai sao RED */
                                                                    /* Caso 1: inverte cor trocando a cor do pai e tio pela cor do avo */
            pai->cor = BLACK;
            tio->cor = BLACK;
            if (avo->pai != NULL)
                avo->cor = RED;
        }
        else
        {                                                           /* No a avo.dir eh preto */
            avo = no_avo(*enderecoRaiz);
            if (!eh_vazia(avo))
            {
                if (pai == avo->esq)                                /* insere na s-a-e */
                {                                                   /* Caso 2: desbalanceada na s-a-d do filho esq, filho e pai RED, mas tio BLACK*/
                    if (*enderecoRaiz == pai->dir)                  /* Caso 2: faz rotacao dupla direita LR esq­-dir */
                        dupla_direita(&avo);                        /* ==> As rotacoes trocam as cores tambem */
                    else
                        rotacao_direita(&avo);                      /* Caso 3: faz rotacao simples a direita LL esq-­esq */
                }
                else                                                /* a avo.dir eh preto */
                {                                                   /* Caso 2: desbalanceada na s-a-d do filho esq, filho e pai RED, mas tio BLACK */
                    if (*enderecoRaiz == pai->esq)                  /* Caso 2: faz rotacao dupla  esquerda RL ­dir-esq */
                        dupla_esquerda(&avo);                       /* ==> As rotacoes trocam as cores tambem  */
                    else                                            /* Caso 3: filho e pai RED, mas é filho a direita */
                        rotacao_esquerda(&avo);                     /* Caso 3: Faz rotacao simples a esquerda RR dir-dir */
                }
            }
        }
    }
}

/**
 * @brief Insere um No na arvore, de forma recursiva, com base no pai
 * 
 * @param enderecoRaiz raiz da arvore ou subarvore atual (No atual)
 * @param pai pai da raiz
 * @param elemento elemento a ser inserido
 * @return TRUE ou FALSE se foi bem sucedido ou não a operação.
 */
bool inserir_No(No **enderecoRaiz, No *pai, TipoElemento elemento)
{
    if (eh_vazia(*enderecoRaiz))
    {
        *enderecoRaiz = criar_no(elemento);
        (*enderecoRaiz)->pai = pai;
    }
    else
    {
        if (elemento < ((*enderecoRaiz)->dado))
        {
            inserir_No(&((*enderecoRaiz)->esq), *enderecoRaiz, elemento); //Insere a esquerda
            corrige_inserir(&((*enderecoRaiz)->esq));
        }
        else if (elemento > ((*enderecoRaiz)->dado))
        {
            inserir_No(&((*enderecoRaiz)->dir), *enderecoRaiz, elemento); //Insere a direita
            corrige_inserir(&((*enderecoRaiz)->dir));
        }
        else
        {
            return false;
        }
    }
    // Se for a raiz pinta da cor preta
    if (pai == NULL)
        (*enderecoRaiz)->cor = BLACK;

    return true;
}

/**
 * @brief Coloca o sucessor ao lado no lugar do No removido (removido) 
 * 
 * @param removido No a ser removido
 * @param sucessor sucessor que ocupará o lugar do No removido
 */
void transplanta_No(No **enderecoRaiz, No *sucessor)
{
    No *aux = *enderecoRaiz;
    if ((*enderecoRaiz)->pai == NULL)
        *enderecoRaiz = sucessor;
    else if (*enderecoRaiz == (*enderecoRaiz)->pai->esq)
        (*enderecoRaiz)->pai->esq = sucessor;
    else
        (*enderecoRaiz)->pai->dir = sucessor;

    if (sucessor != NULL)
        sucessor->pai = aux->pai;
}

/**
 * @brief Corrige o balanceamento da árvore após a remoção de um No.
 * 
 * @param no No filho do que foi removido
 */
void corrige_remover(No **no)
{
    No *irmao;
    // conserta a arvore
    if (*no != NULL && (*no)->pai != NULL && (*no)->cor == BLACK)
    {
        // eh filho esquerdo
        if (*no == (*no)->pai->esq)
        {
            irmao = (*no)->pai->dir;  //irmao (w) de NO a direita.
            if (no_cor(irmao) == RED) //Caso 1: se irmao (w) eh vermelho
            {
                rotacao_esquerda(&((*no)->pai)); //rotacioana e troca cor do irmao (w) pela cor do pai
                irmao = (*no)->pai->dir;         // troca o ponteiro do irmao (w), para o novo filho a direita
            }
            // Caso 2: os dois filhos do irmao (w) sao pretos
            if (no_cor(irmao->esq) == BLACK && no_cor(irmao->dir) == BLACK)
            {
                irmao->cor = RED;               // apenas pinta o irmao de vermelho
                corrige_remover(&((*no)->pai)); // muda o ponteiro para o pai
            }
            else
            {
                // Caso 3: O irmao (w) de No é preto e filho do irmao (w) a esquerda eh preto
                if (no_cor(irmao->dir) == BLACK)
                {
                    rotacao_direita(&irmao);
                    irmao = (*no)->pai->dir;
                }
                //Caso 4: O irmao (w) de No é preto e filho do irmao (w) a direito eh vermelho
                rotacao_esquerda(&((*no)->pai)); //rotacioana e troca cor do irmao (w) pela cor do pai
                (*no)->pai->cor = BLACK;
                irmao->dir->cor = BLACK;
            }
        }
        else // eh filho direito
        {
            irmao = (*no)->pai->esq;  //irmao (w) de NO a esquerda.
            if (no_cor(irmao) == RED) //Caso 1: se irmao (w) eh vermelho e deve ter dois filhos pretos
            {
                rotacao_direita(&((*no)->pai)); //rotacioana e troca cor do irmao (w) pela cor do pai
                irmao = (*no)->pai->esq;        // troca o ponteiro do irmao (w), para o novo filho a esquerda
            }
            // Caso 2: o irmao (w) eh preto e os dois filhos do irmao (w) sao pretos
            if (no_cor(irmao->dir) == BLACK && no_cor(irmao->esq) == BLACK)
            {
                irmao->cor = RED;               // apenas pinta o irmao (w) de vermelho
                corrige_remover(&((*no)->pai)); // muda o ponteiro para o pai
            }
            else
            {
                // Caso 3: O irmao (w) de No eh preto e filho do irmao (w) a esquerda eh preto
                if (no_cor(irmao->esq) == BLACK)
                {
                    rotacao_esquerda(&irmao); //rotacioana e troca cor do irmao (w) pela cor do pai
                    irmao = (*no)->pai->esq;  // troca o ponteiro do irmao (w), para o novo filho a esquerda
                }
                //Caso 4: O irmao (w) de No é preto e filho do irmao (w) a direito eh vermelho
                rotacao_direita(&((*no)->pai)); //rotacioana e troca cor do irmao (w) pela cor do pai
                (*no)->pai->cor = BLACK;
                irmao->esq->cor = BLACK;
            }
        }
    }
    if ((*no) != NULL && (*no)->pai == NULL)
    {
        // apenas troca a cor
        (*no)->cor = BLACK;
    }
}

/**************************************
* IMPLEMENTAÇÃO
**************************************/

void rb_criar(No **enderecoRaiz)
{
    *enderecoRaiz = NULL;
}

void rb_destruir(No **enderecoRaiz)
{
    No *atual = *enderecoRaiz;
    if (!eh_vazia(atual))
        destruir_recursivo(atual);
    *enderecoRaiz = NULL;
}

void rb_pre_ordem(No *raiz)
{
    if (raiz != NULL)
    {
        printf(" (%d => %s)", raiz->dado, (raiz->cor ? "RED" : "BLACK"));
        rb_pre_ordem(raiz->esq);
        rb_pre_ordem(raiz->dir);
    }
}

void rb_em_ordem(No *raiz)
{
    if (raiz != NULL)
    {
        rb_em_ordem(raiz->esq);
        printf(" (%d => %s)", raiz->dado, (raiz->cor ? "RED" : "BLACK"));
        rb_em_ordem(raiz->dir);
    }
}

void rb_pos_ordem(No *raiz)
{
    if (raiz != NULL)
    {
        rb_pos_ordem(raiz->esq);
        rb_pos_ordem(raiz->dir);
        printf(" (%d => %s)", raiz->dado, (raiz->cor ? "RED" : "BLACK"));
    }
}

int rb_altura(No *raiz)
{
    if (eh_vazia(raiz) || (raiz->esq == NULL && raiz->dir == NULL))
        return 0;
    else
    {
        int alt_esq = rb_altura(raiz->esq);
        int alt_dir = rb_altura(raiz->dir);
        return (alt_esq > alt_dir) ? (alt_esq + 1) : (alt_dir + 1);
    }
}

/**
 * @brief Total de vertices internos, incluindo a raiz da arvore.
 *  Não considera os folhas NULL
 * 
 * @param raiz 
 * @return int 
 */
int rb_total_vertices(No *raiz)
{
    if (eh_vazia(raiz))
        return 0;

    return (rb_total_vertices(raiz->esq) + rb_total_vertices(raiz->dir)) + 1;
}

/**
 * @brief Obtem o total de folhas validos (ou internos) excluindo-se a raiz da arvore
 * 
 * @param raiz 
 * @return int 
 */
int rb_total_folhas(No *raiz)
{
    if (raiz == NULL)
        return 0;
    if (raiz->esq == NULL && raiz->dir == NULL)
        return 1;
    return rb_total_folhas(raiz->esq) + rb_total_folhas(raiz->dir);
}

/**
 * @brief Obtem o total de Nos folhas NULL, incluindo o pai da raiz
 * 
 * @param raiz 
 * @return int 
 */
int rb_total_folhas_null(No *raiz)
{
    if (raiz == NULL)
        return 1;
    return rb_total_folhas(raiz->esq) + rb_total_folhas(raiz->dir);
}

/**
 * @brief Busca por um dado na arvore
 * 
 * @param raiz raiz da arvore ou subarvore atual
 * @param dado dado a ser buscado
 * @return TRUE se encontrou ou FLASE caso contrario
 */
bool rb_consultar(No *raiz, TipoElemento dado)
{
    return rb_consultar_No(raiz, dado);
}

bool rb_inserir(No **enderecoRaiz, TipoElemento elemento)
{
    return inserir_No(enderecoRaiz, NULL, elemento);
}

bool rb_remover(No **enderecoRaiz, TipoElemento dado)
{
    if (eh_vazia(*enderecoRaiz))
        return false; // se nao encontrou

    if (dado < (*enderecoRaiz)->dado)
        rb_remover(&((*enderecoRaiz)->esq), dado); // se é menor, percorre a esquerda
    else if (dado > (*enderecoRaiz)->dado)
        rb_remover(&((*enderecoRaiz)->dir), dado); // se é maior, percorre a direita
    else
    {
        // se encontrou o elemento
        No *z = *enderecoRaiz, *x;
        No *y = z;                 // cria ponteiro temporario pra evitar perder a referencia
        int yCorOriginal = y->cor; // preserva a cor para uso posterior
        if (z->esq == NULL)        // so tem filho a direita ou nenhum filho
        {
            x = z->dir;                              // guarda o filho a direita para rebalncear a arvore posteirormente
            transplanta_No(&(*enderecoRaiz), z->dir); // troca No removido pelo filho a direita
        }
        else if (z->dir == NULL) // so tem filho a esquerda
        {
            x = z->esq;                              // guarda o filho a esquerda para rebalncear a arvore posteirormente
            transplanta_No(&(*enderecoRaiz), z->esq); // troca No removido pelo filho a esquerda
        }
        else // Tem dois filhos
        {
            y = menor(z->dir);     // obtem o menor No a esquerda do filho direito para tomar o lugar do No removido (y eh novo pai)
            yCorOriginal = y->cor; // como mudou a referencia, novamente preserva a cor para uso posterior
            x = y->dir;            // salva o filho a direita do removido
            if (y->pai == z)
                x->pai = y; // troca o pai do filho a direita para não perder a referencia (y eh novo pai)
            else
            {
                transplanta_No(&y, y->dir); // troca No removido pelo filho a direita
                y->dir = z->dir;
                y->dir->pai = y;
            }
            transplanta_No(&(*enderecoRaiz), (y));
            y->esq = z->esq;
            y->esq->pai = y;
            y->cor = z->cor;
            (*enderecoRaiz) = y;
        }
        free(z);

        if (yCorOriginal == BLACK)
            corrige_remover(&x);
    }
    return true;
}