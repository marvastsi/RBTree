#ifndef _ABB_RB_H_
#define _ABB_RB_H_

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

typedef int TipoElemento;
typedef struct no No;

void rb_criar(No** enderecoRaiz);
void rb_destruir(No** enderecoRaiz);
bool rb_consultar(No* raiz, TipoElemento dado);
void rb_pre_ordem(No* raiz);
void rb_em_ordem(No* raiz);
void rb_pos_ordem(No* raiz);
int rb_altura(No *raiz);
int rb_total_vertices(No *raiz);
int rb_total_folhas(No *raiz);
int rb_total_folhas_null(No *raiz);

bool rb_inserir(No **enderecoRaiz, TipoElemento elemento);
bool rb_remover(No **enderecoRaiz, TipoElemento dado);

#endif