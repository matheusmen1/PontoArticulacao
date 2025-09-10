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
int contaNos(ListaAdjacencia * lista){
	int i = 0;
	
	while(lista != NULL){
		lista = lista->head;
		i++;
	}
	
	return i;
}

ListaAdjacencia * formarPontilhado(ListaAdjacencia * lista, ListaAdjacencia * arvore)
{
	ListaAdjacencia * pontilhado = NULL, *auxLista;
	
	while(lista != NULL)
	{
		auxLista = lista->tail;
		while(auxLista != NULL)
		{
			if(!existeOrigemDestinoLista(arvore,lista->vertice,auxLista->vertice))
			{
			 //se n?o existe na minha ?rvore eu adiciono na minha lista de pontilhados
				inserirListaPeso(&pontilhado, lista->vertice, auxLista->vertice, 1);
				inserirListaPeso(&pontilhado,auxLista->vertice, lista->vertice,1);
			}
			
			auxLista = auxLista->tail;
		}
		
		lista = lista->head;
	}
	
	return pontilhado;
}

int charParaIndice(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    } else {
        return -1;
    }
}

void DFS_Articulacao(int indice, int ordem[], int menor[], int pai[], int marcado[], char articulacao[], ListaAdjacencia * arvore, ListaAdjacencia * pontilhado) {
    int filhos = 0;
    int indiceVizinho;
    ListaAdjacencia *auxArvore, *auxPont;

    marcado[indice] = 1;
    menor[indice] = ordem[indice];

    auxArvore = arvore;
    for(int i=0; i<indice; i++){
        auxArvore = auxArvore->head;
    }
    auxArvore = auxArvore->tail;
    //percorre vizinhos na árvore
    while (auxArvore != NULL) {
        indiceVizinho = charParaIndice(auxArvore->vertice);

        if (!marcado[indiceVizinho]) {
            pai[indiceVizinho] = indice;
            filhos++;

            DFS_Articulacao(indiceVizinho, ordem, menor, pai, marcado, articulacao, arvore, pontilhado);

            if(menor[indice] < menor[indiceVizinho])
                menor[indice] = menor[indice];
            else
                menor[indice] = menor[indiceVizinho];

            if (pai[indice] == -1 && filhos > 1)
                articulacao[indice] = 1;

            if (pai[indice] != -1 && menor[indiceVizinho] >= ordem[indice])
                articulacao[indice] = 1;

        } else if (indiceVizinho != pai[indice]) {
            if(menor[indice] < ordem[indiceVizinho])
                menor[indice] = menor[indice];
            else
                menor[indice] = ordem[indiceVizinho];
        }

        auxArvore = auxArvore->tail;
    }

    auxPont = pontilhado;
    for(int i=0; i<indice; i++){
        auxPont = auxPont->head;
    }
    auxPont = auxPont->tail;
    // percorre vizinhos no pontilhado
    while (auxPont != NULL) {
        indiceVizinho = charParaIndice(auxPont->vertice);
        if(menor[indice] < ordem[indiceVizinho])
            menor[indice] = menor[indice];
        else
            menor[indice] = ordem[indiceVizinho];
        auxPont = auxPont->tail;
    }
}

void buscaProfundidadeArticulacao(ListaAdjacencia * arvore, ListaAdjacencia * pontilhado, InfoNo * visitados[], int quantNos)
{
    int ordem[quantNos], menor[quantNos], pai[quantNos], marcado[quantNos], indice;
    char articulacao[quantNos]; //vetor de 0 ou 1, para verdadeiro ou falso

    //preencher o vetor de ordem de visita
    for(int i=0; i<quantNos; i++)
	{
        ordem[i] = visitados[i]->ordemVisita;
    }

    // para cada nó u em grafo:
    //     marcado[u] = falso
    //     pai[u] = NULL
    //     articulacao[u] = falso
    ListaAdjacencia * auxArvore = arvore;
    while(auxArvore != NULL){
        indice = charParaIndice(auxArvore->vertice);
        marcado[indice] = 0;
        pai[indice] = -1;
        articulacao[indice] = 0;
        menor[indice] = ordem[indice];

        auxArvore = auxArvore->head;
    }

    // para cada nó u em grafo:
    //     se não visitado[u]:
    //         DFS_Articulacao(u)
    auxArvore = arvore;
    while(auxArvore != NULL)
	{
        indice = charParaIndice(auxArvore->vertice);
        if(!marcado[indice])
		{ //se ele não está marcado
            DFS_Articulacao(indice, ordem, menor, pai, marcado, articulacao, arvore, pontilhado);
        }
        
        auxArvore = auxArvore->head;
    }

    // para cada nó u em grafo:
    //     se articulacao[u]:
    //         imprimir(u, "é ponto de articulação")
    auxArvore = arvore;
    while(auxArvore != NULL){ //exibição dos pontos que são articulação
        indice = charParaIndice(auxArvore->vertice);
        if(articulacao[indice] == 1){
            printf("O vertice %c eh ponto de articulacao!!\n", auxArvore->vertice);
        }

        auxArvore = auxArvore->head;
    }
}

//void acharPontosArticulacao(ListaAdjacencia * lista){
//	ListaAdjacencia *arvore = NULL, *pontilhado = NULL;
//	int quantNos = contaNos(lista); //conta a quantidade de nos do meu grafo
//	InfoNo * visitados[quantNos]; //o vetor que ir? armazenar a ordem das visitas dos Nos
//	
//	formarVetorVisitadosZerado(visitados,lista); //deixar o vetor de visitados zerado, ou seja, ningu?m ainda foi visitado pois n?o criei a arvore ainda
//	arvore = formarArvore(lista, visitados); //formar a arvore para realizar a busca em profundidade e marcar os visitados cada qual com sua ordem
//	pontilhado = formarPontilhado(lista, arvore); //esse algoritmo subtrai a lista pela arvore, gerando assim as liga??es pontilhadas
//	
//	exibir as listas geradas
//	exibirListaAdjacencia(lista);
//	exibirListaAdjacencia(arvore);
//	exibirListaAdjacencia(pontilhado);

//	agora com o vetor com a ordem dos visitados...
//	com a arvore formada...
//	e com o back (pontilhado) formado... posso agora fazer a busca em profundidade
//	buscaProfundidadeArticulacao(arvore, pontilhado, visitados, quantNos);
//}

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
    ListaAdjacencia *listaPontilhado = NULL;
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
            //imprimirVisitados(visitados, qtdeVertices);	
            printf ("\nEXIBINDO LISTA ADJACENCIA\n");
            exibirListaAdjacencia(lista);
            printf ("\nEXIBINDO ARVORE\n");
            exibirArvore(arvore);
            listaPontilhado = formarPontilhado(lista, arvore);
        	printf ("\nEXIBINDO LISTA PONTILHADO\n");
            exibirListaAdjacencia(listaPontilhado);
            buscaProfundidadeArticulacao(arvore, listaPontilhado, visitados, qtdeVertices);
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