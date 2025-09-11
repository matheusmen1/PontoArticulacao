//LISTA -------------------------------------------------------------------------------------------
struct listaAdjacencia
{
    char vertice;
    int peso;
    struct listaAdjacencia *head, *tail;
};
typedef struct listaAdjacencia ListaAdjacencia;

//INFONO -------------------------------------------------------------------------------------------
typedef struct infoNo
{
	char vertice;
	int ordemVisita;
} InfoNo;

//TAD de InfoNo -------------------------------------------------------------------------------------
InfoNo *NCInfoNO()
{
	InfoNo *novo = (InfoNo*)malloc(sizeof(InfoNo));
	novo->vertice = '0';
	novo->ordemVisita = 0;
	return novo;
}

//TAD da LISTA ---------------------------------------------------------------------------------------
ListaAdjacencia *novaCaixaVazia(void)
{
    ListaAdjacencia *nova = (ListaAdjacencia *)malloc(sizeof(ListaAdjacencia));
    nova->vertice = '0';
    nova->peso = 0;
    nova->head = nova->tail = NULL;
    return nova;
}

ListaAdjacencia *novaCaixaLista(char vertice, int peso)
{
    ListaAdjacencia *nova = (ListaAdjacencia *)malloc(sizeof(ListaAdjacencia));
    nova->vertice = vertice;
    nova->peso = peso;
    nova->head = nova->tail = NULL;
    return nova;
}

void inserirLista(ListaAdjacencia**lista, char vertice, int peso){
	*lista = novaCaixaLista(vertice, peso);
}

void inserirListaPeso(ListaAdjacencia**lista, char origem, char vertice, int peso){
	ListaAdjacencia * nova, *aux, *ant;

	//se lista vazia
	if(*lista == NULL){
		inserirLista(&(*lista),origem, peso);
	}
	else{
		//procurar o vertice origem
		aux = ant = *lista;
		while(aux != NULL && origem != aux->vertice){
			ant = aux;
			aux = aux->head;
		}
		//se nao existe eu crio o vertice origem
		if(aux == NULL){ //entao nao existe esse vertice origem
			ant->head = novaCaixaLista(origem, peso);
		}
		else{
			//fazer a busca na horizontal, buscar o vertice de destino
			ant = aux;
			aux = aux->tail;
			while(aux != NULL && vertice != aux->vertice){
				ant = aux;
				aux = aux->tail;
			}
			//nao existe o vertice para o destino
			if(aux == NULL){
				nova = novaCaixaLista(vertice, peso);
				nova->peso = peso;
				ant->tail = nova;
			}
		}
	}
}

//PILHA =--------------------------------------------------------------------------------
struct pilha{
	
	ListaAdjacencia *no;
	char pai;
	struct pilha *prox;

};
typedef struct pilha Pilha;

void init(Pilha **p)
{
	*p = NULL;
}

void push(Pilha **p, ListaAdjacencia *no, char pai)
{
	Pilha *NC;
	NC = (Pilha*)malloc(sizeof(Pilha));
	NC->pai = pai;
	NC->prox = NULL;
	NC->no = no;
	if(*p == NULL)
		*p = NC;
	else
	{
		NC->prox = *p;
		*p = NC;
	} 
	
}

void pop(Pilha **P, ListaAdjacencia **x, char *pai)
{
	
	Pilha *aux;
	aux = *P;
	*x = (*P)->no ;
	*pai = (*P)->pai;
	*P = (*P)->prox;
	free(aux);
	
}

char isEmpty(Pilha *p){
	
	return p == NULL;
}
