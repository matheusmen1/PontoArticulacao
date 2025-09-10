#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <conio2.h>
#define TF 50
#include "TADPilha.h"

//STRUCT LISTA --------------------------------------------------------------------------------------



//FUNCOES PARA LISTA DE ADJACENCIA ----------------------------------------------------------------------------------
void lerTxtListaAdjacencia(ListaAdjacencia **lista)
{
	FILE * ptr = fopen("entradaLA.txt", "r");
	ListaAdjacencia * caixa;
	char atual, peso[10], vertice, verLinha;
	
	//inicializar o peso
	memset(peso,'\0',sizeof(peso));

	while(!feof(ptr)){
		atual = getc(ptr); //o vertice da linha
		verLinha = atual; //pegar o vertice do inicio da linha
		inserirListaPeso(&(*lista),verLinha,' ',0);
		atual = getc(ptr); //pegar o espaco em branco
		atual = getc(ptr); //pegar o proximo vertice para comecar a tratar

		//ate o final da linha
		while(!feof(ptr) && atual!='\n'){
			if(atual > 47 && atual < 58){
				//coletar o peso
				peso[strlen(peso)] = atual;
			}
			else{
				if(atual == ' '){//j? tenho o peso
					inserirListaPeso(&(*lista),verLinha,vertice,atoi(peso));
					memset(peso,'\0',sizeof(peso));
				}
				else{
					if(atual != ','){ //quer dizer que eu agora possou o meu vertice
						vertice = atual;
					}
				}
			}
			atual = getc(ptr);
		}
		inserirListaPeso(&(*lista),verLinha,vertice,atoi(peso));
		memset(peso,'\0',sizeof(peso));
	}
}

void exibirListaAdjacencia(ListaAdjacencia * lista)
{
	ListaAdjacencia *auxVert, *auxHori;
	//clrscr();
	auxVert = lista;
	//descer verticalmente
	while(auxVert != NULL)
	{
		printf("Vertice: %c -> ",auxVert->vertice);
		auxHori = auxVert->tail;
		while(auxHori != NULL)
		{
			if(auxHori->tail == NULL)
			{
				printf("%c,%d",auxHori->vertice,auxHori->peso);	
			}
			else
			{
				printf("%c,%d || ",auxHori->vertice,auxHori->peso);
			}
			auxHori = auxHori->tail;
		}
		printf("\n");
		auxVert = auxVert->head;
	}
}

char existeOrigemDestinoLista(ListaAdjacencia * lista, char origem, char destino){
	ListaAdjacencia *auxHori, *auxVert = lista;
	
	//achar o vertice de origem
	while(auxVert != NULL && auxVert->vertice != origem)
		auxVert = auxVert->head;
	if(auxVert == NULL) //nao existe essa vertice, nao existe essa aresta
		return 0;
	
	auxHori = auxVert->tail;
	while(auxHori != NULL && auxHori->vertice != destino)
		auxHori = auxHori->tail;
	if(auxHori == NULL) //nao existe essa origem
		return 0;
		
	//printf("Retornei! Origem: %c -> Destino: %c\n",origem, destino);
	return 1; //nao entrou em nenhum return falso
}

void formarVetorVisitadosZerado(InfoNo * visitados[], ListaAdjacencia * lista, int *qtdeVertices){
	int i=0;
	InfoNo *novo;
	
	while(lista){
		novo = NCInfoNO();
		novo->vertice = lista->vertice;
		visitados[i++] = novo;
		*qtdeVertices = *qtdeVertices + 1;
		lista = lista->head;
	}

}
int verificarVisitado(InfoNo * visitados[], char vertice, int qtdeVertices)
{
    for (int i = 0; i < qtdeVertices; i++)
    {
        if(visitados[i]->vertice == vertice && visitados[i]->ordemVisita != 0) // ja visitado
            return -1;
    }
    return 1;

}
void marcarVisitado(InfoNo * visitados[], char vertice, int qtdeVertices, int cont)
{
    for (int i = 0; i < qtdeVertices; i++)
    {
        if(visitados[i]->vertice == vertice) 
            visitados[i]->ordemVisita = cont;
    }

}
void buscaPosicao (ListaAdjacencia **aux, ListaAdjacencia *listaOriginal, char vertice)
{
	while(listaOriginal != NULL && listaOriginal->vertice != vertice)	
		listaOriginal = listaOriginal->head;
	*aux = listaOriginal;
	
}
void formarArvore(ListaAdjacencia *lista, ListaAdjacencia **arvore, InfoNo *visitados[], int qtdeVertices)
{
    ListaAdjacencia *no, *noPai;
    char pai, auxPai;
    int cont = 1;
    Pilha *P;
    init(&P);
    push(&P, lista, '\0'); // começa pelo vértice inicial
	
    while (!isEmpty(P))
    {
        pop(&P, &no, &pai); 
        buscaPosicao(&no, lista, no->vertice);
        noPai = no;
        if(pai == '\0') //eu sou o primeiro no a ser construido
		{ 
        	inserirListaPeso(&(*arvore), no->vertice, ' ',1);	
        }
        else
		{
        	inserirListaPeso(&(*arvore), no->vertice, ' ', 1);
        	inserirListaPeso(&(*arvore), no->vertice, pai, 1);
        	inserirListaPeso(&(*arvore), pai, no->vertice, 1);
        }
        
        marcarVisitado(visitados, no->vertice, qtdeVertices, cont);
        printf("Visitando %c (ordem %d)\n", no->vertice, cont);
		cont++;
       	while(no != NULL && verificarVisitado(visitados, no->vertice, qtdeVertices) == -1)
       		no = no->tail;
       	if (no != NULL)
       	{
		   push(&P, no, noPai->vertice);
       	}
       	
       		
    }
			    
}

void exibirArvore(ListaAdjacencia *arvore)
{
	ListaAdjacencia *cauda;
	while(arvore != NULL)
	{
		printf ("(%c) ->", arvore->vertice);
		cauda = arvore->tail;
		while(cauda != NULL)
		{
			printf (" (%c) -> ", cauda->vertice);
			cauda = cauda->tail;	
		}
		printf("\n");
		arvore = arvore->head;
	}
	
}

void imprimirVisitados(InfoNo *visitados[], int qtdeVertices)
{
    printf("Vertices visitados:\n");
    for (int i = 0; i < qtdeVertices; i++)
	 {
        if (visitados[i])
		{
            printf("%c ", visitados[i]->vertice);
        }
    }
    printf("\n");
}

//OUTRAS FUNCOES ---------------------------------------------------------------------------------------------------
void continuar(){
	printf("\n\nPressione qualquer tecla para continuar...");
	getchar();
}

char menu (void)
{
	
	printf("[A] - Ponto de Articulacao\n");
    printf("[B] - Exibir Lista de Adjacencia\n");
	printf ("[ESC] - Sair\n");
	
	return toupper(getch());
}
void executar(void)
{
    ListaAdjacencia *lista = NULL;
    lerTxtListaAdjacencia(&lista);
    ListaAdjacencia *arvore = NULL;
    InfoNo *visitados[50];
    int qtdeVertices = 0;
    char op;
    do
    {
    	op = menu();
        
        switch(op)
        {
            case 'A':
            clrscr();
            formarVetorVisitadosZerado(visitados, lista, &qtdeVertices);
            formarArvore(lista, &arvore, visitados, qtdeVertices);
            imprimirVisitados(visitados, qtdeVertices);	
            exibirArvore(arvore);
            break;
            case 'B':
            clrscr();
            exibirListaAdjacencia(lista);
            break;
            
        }

    }while(op != 27);

}
int main(void)
{
    executar();
    return 0;
}
