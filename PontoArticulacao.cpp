#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#include <limits.h>
#include "TADPilha.h"
#include <conio2.h>

#define TF 50

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
	//system("cls");//clrscr();
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
    *qtdeVertices = 0; //evita erros se chamada mais de uma vez
	
	while(lista){
		novo = NCInfoNO();
		novo->vertice = lista->vertice;
		visitados[i++] = novo;
		*qtdeVertices = *qtdeVertices + 1;
		lista = lista->head;
	}
}

void liberarMemVetorVisit(InfoNo * visitados[]){
    int quant, pos=0;
    InfoNo * aux, * liberar;

    aux = visitados[pos];
    while(aux != NULL){
        liberar = aux;
        pos++;
        aux = visitados[pos];
        free(liberar);
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
        //printf("Visitando %c (ordem %d)\n", no->vertice, cont);
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

//REALIZAR DE FATO A BUSCA PELOS PONTOS DE ARTICULAÇÃO -------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void continuar(){
	printf("\n\nPressione qualquer tecla para continuar...\n");
	getch();
}

char isRaiz(ListaAdjacencia * arvore, ListaAdjacencia * ponteiro)
{
	if(arvore != NULL && arvore->vertice == ponteiro->vertice)
		return 1; //o ponteiro é a raiz
	return 0;
}

int filhosNoArvore(ListaAdjacencia * no, InfoNo * visitados[]) //lembrando que esse no é da minha arvore
{
	int filhos = 0, indice;
	
	no = no->tail;
	while(no != NULL)
	{
		indice = charParaIndice(no->vertice);
		if(visitados[indice]->ordemVisita == 0)
			filhos++;
		
		no = no->tail;
	}
	return filhos;
}

int ordemNo(InfoNo * visitados[], ListaAdjacencia * no)
{
	int indice = charParaIndice(no->vertice);
	return visitados[indice]->ordemVisita;
}

int menorBack(ListaAdjacencia *no, ListaAdjacencia *pontilhado, InfoNo *visitados[])
{
    ListaAdjacencia *aux = pontilhado;
    int menor = INT_MAX;

    while (aux != NULL && aux->vertice != no->vertice)
        aux = aux->head;

    if (aux != NULL) // vértice possui pelo menos 1 valor de retorno
    {
        aux = aux->tail;
        while (aux != NULL)
        {
            int indice = charParaIndice(aux->vertice);
            if (visitados[indice]->ordemVisita < menor &&
                visitados[indice]->ordemVisita > 0)
            {
                menor = visitados[indice]->ordemVisita;
            }
            aux = aux->tail;
        }
    }

    if (menor < INT_MAX)
        return menor;
    return -1;
}

void marcarVisitado(InfoNo * visitados[], char vertice, int ordem)
{
	int indice = charParaIndice(vertice);
	visitados[indice]->ordemVisita = ordem;
}

//busca de fato
int buscaArticulacao(ListaAdjacencia *no, ListaAdjacencia *arvore, ListaAdjacencia *grafo, ListaAdjacencia *pontilhado, InfoNo *visitados[], int *ordem, char pai, int qtdeVertices)
{
    int back, backFilho;
    int filhos = 0;
    ListaAdjacencia *adj;   // percorre as adjacências (uso do campo tail)
    ListaAdjacencia *viz;   // nó do grafo correspondente ao adj->vertice

    /* primeira chamada: usa o primeiro vértice do grafo se no == NULL */
    if (no == NULL)
        no = grafo; /* grafo deve apontar para o primeiro vértice */

    /* marca o vértice (discovery time) */
    marcarVisitado(visitados, no->vertice, (*ordem)++);
    
    /* inicializa 'back' com o menor valor obtido pelas arestas de retorno do próprio nó */
    back = menorBack(no, pontilhado, visitados);
    if (back == -1 || back > visitados[charParaIndice(no->vertice)]->ordemVisita)
        back = visitados[charParaIndice(no->vertice)]->ordemVisita;

    /* percorre TODAS as adjacências do vértice 'no' (usando tail como encadeamento de vizinhos) */
    adj = no->tail; 
    while (adj != NULL)
    {
        /* encontra o nó principal do grafo relativo ao caractere do vizinho */
        buscaPosicao(&viz, grafo, adj->vertice);
        if (viz != NULL)
        {
            /* se não visitado => é filho na DFS */
            if (verificarVisitado(visitados, viz->vertice, qtdeVertices) == 1)
            {
                filhos++;

                /* chamada recursiva; passa 'no->vertice' como pai do filho */
                backFilho = buscaArticulacao(viz, arvore, grafo, pontilhado, visitados, ordem, no->vertice, qtdeVertices);

                /* condição de articulação para vértice não-raiz */
                if (!isRaiz(arvore, no) &&
                    backFilho >= visitados[charParaIndice(no->vertice)]->ordemVisita)
                {
                    printf("O vertice %c eh um ponto de articulacao!!\n", no->vertice);
                    continuar();
                }

                /* atualiza low[u] */
                if (backFilho < back)
                    back = backFilho;
            }
            /* se já visitado e não é o pai -> poderia atualizar 'back',
               mas isso já é tratado em menorBack() */
        }

        adj = adj->tail; /* próximo vizinho */
    }

    /* regra da raiz: se é raiz da árvore DFS e tem mais de 1 filho */
    if (isRaiz(arvore, no) && filhos > 1)
    {
        printf("O vertice %c eh um ponto de articulacao!!\n", no->vertice);
        continuar();
    }

    return back;
}
//FIM DA BUSCA EM PROFUNDIDADE -------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

//OUTRAS FUNCOES ---------------------------------------------------------------------------------------------------
char menu (void)
{
	printf("[A] - Formar estrutura (arvore e pontilhado)\n");
    printf("[B] - Exibir Lista de Adjacencia\n");
    printf("[C] - Busca ponto de articulacao\n");
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
    int qtdeVertices = 0, ordem;
    char op;
    do
    {
        system("cls");//clrscr();
    	op = menu();
        
        switch(op)
        {
            case 'A':{
                //formar a estrutura
                formarVetorVisitadosZerado(visitados, lista, &qtdeVertices);
                formarArvore(lista, &arvore, visitados, qtdeVertices);
                listaPontilhado = formarPontilhado(lista, arvore);
                printf("Estruturas formadas!\n");
                continuar();

                break;
            }
            case 'B':{
                system("cls");//clrscr();
                printf ("\nEXIBINDO LISTA ADJACENCIA\n");
                exibirListaAdjacencia(lista);
                printf ("\nEXIBINDO ARVORE\n");
                exibirArvore(arvore);
                printf ("\nEXIBINDO LISTA PONTILHADO\n");
                exibirListaAdjacencia(listaPontilhado);
                continuar();

                break;
            }
            case 'C':{
                for(int i=0; i<50; i++)
                    visitados[i] = NULL;
                formarVetorVisitadosZerado(visitados, lista, &qtdeVertices);
                ordem = 1;
                buscaArticulacao(NULL, arvore, lista, listaPontilhado, visitados, &ordem, '\0', qtdeVertices);
                continuar();

                break;
            }
        }

    }while(op != 27);
}

int main(void)
{
    executar();
    return 0;
}