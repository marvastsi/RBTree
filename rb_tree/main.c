#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "rb_tree.h"

#define MAX 10000

int randomInteger (int low, int high){
    double d;
    d = (double) rand () / ((double) RAND_MAX + 1);
    int k = d * (high - low + 1);
    return ((int)low + k);
}

void testeRB(int* vet, int size) {
	rb_tree_t *tree = NULL;
    
	for (int i = 0;i < size;i++) {
        int n = vet[i];
		printf(" %d=(%d)", i, n);
		rb_tree_insert(&tree, n);
    }
    printf("\n");
    for (int i = 0;i < size;i++) {
        int n = vet[i];
		bool res = rb_tree_search(tree, n);
		printf(" %d=(%s)", n, res ? "true":"false");
    }
}

int main()
{
	printf("\n");

	int vet[MAX];
    for (int i = 0;i < MAX;i++) {
        vet[i] = randomInteger(0, MAX);
    }
	
    testeRB(vet, MAX);

	printf("\n");
	return 0;
}