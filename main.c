#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "abb_rb.h"

#define MAX 10000

TipoElemento randomInteger (int low, int high){
    double d;
    d = (double) rand () / ((double) RAND_MAX + 1);
    int k = d * (high - low + 1);
    return ((TipoElemento)low + k);
}

void teste1()
{
	No *arvore;
	rb_criar(&arvore);
	rb_inserir(&arvore, 20);
	rb_inserir(&arvore, 10);
	rb_inserir(&arvore, 30);
	rb_inserir(&arvore, 5);
	rb_inserir(&arvore, 15);
	rb_inserir(&arvore, 11);
	rb_inserir(&arvore, 16);

	printf("\nAltura: %d\n", rb_altura(arvore));
	printf("Total de Vertices: %d\n", rb_total_vertices(arvore));
	printf("Total de Folhas: %d\n", rb_total_folhas(arvore));

	printf("\n-PreOrdem---\n");
	rb_pre_ordem(arvore);
	printf("\n--EmOrdem--\n");
	rb_em_ordem(arvore);
	printf("\n---PosOrdem-\n");
	rb_pos_ordem(arvore);
	printf("\n-------------\n");

	rb_destruir(&arvore);
}

int teste2()
{
	No *arvore;
	rb_criar(&arvore);
	rb_inserir(&arvore, 20);
	rb_inserir(&arvore, 10);
	rb_inserir(&arvore, 30);
	rb_inserir(&arvore, 40);
	rb_inserir(&arvore, 25);
	rb_inserir(&arvore, 15);
	rb_inserir(&arvore, 5);

	printf("\nAltura: %d\n", rb_altura(arvore));
	printf("Total de Vertices: %d\n", rb_total_vertices(arvore));
	printf("Total de Folhas: %d\n", rb_total_folhas(arvore));

	printf("\n-PreOrdem---\n");
	rb_pre_ordem(arvore);
	printf("\n--EmOrdem--\n");
	rb_em_ordem(arvore);
	printf("\n---PosOrdem-\n");
	rb_pos_ordem(arvore);
	printf("\n-------------\n");

	rb_remover(&arvore, 5);
	rb_remover(&arvore, 20);
	rb_remover(&arvore, 15);
	rb_remover(&arvore, 10);
	rb_remover(&arvore, 30);
	rb_remover(&arvore, 25);
	rb_remover(&arvore, 40);
	printf("\n%p\n", arvore);

	rb_inserir(&arvore, 20);
	rb_inserir(&arvore, 10);
	rb_inserir(&arvore, 30);
	rb_inserir(&arvore, 40);
	rb_inserir(&arvore, 25);
	rb_inserir(&arvore, 15);
	rb_inserir(&arvore, 5);

	printf("\nAltura: %d\n", rb_altura(arvore));
	printf("Total de Vertices: %d\n", rb_total_vertices(arvore));
	printf("Total de Folhas: %d\n", rb_total_folhas(arvore));

	printf("\n-PreOrdem---\n");
	rb_pre_ordem(arvore);
	printf("\n--EmOrdem--\n");
	rb_em_ordem(arvore);
	printf("\n---PosOrdem-\n");
	rb_pos_ordem(arvore);

	rb_destruir(&arvore);
}

void testeRB(int* vet, int size) {
	No *arvore;
	rb_criar(&arvore);
    
	for (int i = 0;i < size;i++) {
        TipoElemento n = vet[i];
		printf(" %d=(%d)", i,n);
		rb_inserir(&arvore, n);
    }
}

int main()
{

	teste1();
	printf("\n");
	teste2();
	printf("\n");

	TipoElemento vet[MAX];
    for (int i = 0;i < MAX;i++) {
        vet[i] = randomInteger(0, MAX);
    }
	testeRB(vet, MAX);
	printf("\n");
	return 0;
}