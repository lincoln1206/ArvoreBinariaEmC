#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

void gotoxy(int coluna, int linha)
{ 
	COORD point;
	point.X = coluna;
	point.Y = linha;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

typedef struct elemento{
	int numero;
}t_elemento;

typedef struct arvore {
	struct arvore * esq;
	t_elemento data;
	struct arvore * dir;
}node;

void salvaArq(node * tree);
void salvaItem(node *tree,FILE* file);

//INSERIR
void inserir(node ** tree, int val)
{	
    node *temp = NULL;
    if(!(*tree))
    {
        temp = (node *)malloc(sizeof(node));
        temp->esq = temp->dir = NULL;
        temp->data.numero = val;
        *tree = temp;
        return;
    }

    if(val < (*tree)->data.numero)
    {
        inserir(&(*tree)->esq, val);
    }
    else if(val > (*tree)->data.numero)
    {
        inserir(&(*tree)->dir, val);
    }
}

//IMPRIMIR EM PREORDEM
void imprimePre(node *tree)
{
    if (tree)
    {
        printf("%d\n",tree->data.numero);
        imprimePre(tree->esq);
        imprimePre(tree->dir);
    }

}

//IMPRIMIR INORDEM
void imprimeIn(node *tree)
{
    if (tree)
    {
        imprimeIn(tree->esq);
        printf("%d\n",tree->data.numero);
        imprimeIn(tree->dir);
    }
}

//IMPRIMIR EM POSORDEM
void imprimePos(node *tree)
{
    if (tree)
    {
        imprimePos(tree->esq);
        imprimePos(tree->dir);
        printf("%d\n",tree->data.numero);
    }
}

//DELETAR ARVORE
void deletarArvore(node * tree)
{
    if (tree)
    {
        deletarArvore(tree->esq);
        deletarArvore(tree->dir);
        free(tree);
    }
}

//BUSCAR
node* busca(node ** tree, int val)
{
    if(!(*tree))
    {
        return NULL;
    }

    if(val < (*tree)->data.numero)
    {
        busca(&((*tree)->esq), val);
    }
    else if(val > (*tree)->data.numero)
    {
        busca(&((*tree)->dir), val);
    }
    else if(val == (*tree)->data.numero)
    {
        return *tree;
    }
}

//EXIBIR GRAFICAMENTE
void exibirGraficamente(node *tree, int col, int lin, int desloc)
{
    if (tree == NULL)
        return; // condicao de parada do procedimento recursivo
    gotoxy(col,lin);
    printf("|%d|",tree->data.numero);
    if (tree->esq != NULL)
        exibirGraficamente(tree->esq,col-desloc,lin+2,desloc/2+1);
    if (tree->dir != NULL)
        exibirGraficamente(tree->dir,col+desloc,lin+2,desloc/2+1);
}

//PEGA O VALOR MINIMO
node * minValor(node **node)
{
	while ((*node)->esq != NULL)
		(*node)= (*node)->esq;

	return (*node);
}

//DELETA UM NO
node* deletaNo(node **tree, int val)
{
	if ((*tree) == NULL) {
		printf("Elemento %d nao esta na arvore!\n\n",val);
		return (*tree);
	}
	
	if((*tree)!=NULL){
		if (val < (*tree)->data.numero)
			(*tree)->esq = deletaNo(&((*tree)->esq), val);
		
		else if (val > (*tree)->data.numero)
			(*tree)->dir = deletaNo(&((*tree)->dir), val);
		
		else
		{
			if((*tree)->esq == NULL && (*tree)->dir == NULL){
				free(*tree);
				(*tree)=NULL;
			}
			
			else if((*tree)->esq == NULL)
			{
				node *temp = (*tree)->dir;
				free((*tree));
				return temp;
			}
			
			else if ((*tree)->dir == NULL)
			{
				node *temp = (*tree)->esq;
				free((*tree));
				return temp;
			}
			
			else{
				node* temp = minValor(&((*tree)->dir));
			
				(*tree)->data.numero = temp->data.numero;
			
				(*tree)->dir = deletaNo(&((*tree)->dir), temp->data.numero);
			}
			
		}
	}
	return (*tree);
}

//Carrega dados do arquivo
node *carregaArquivo (node *no){
    FILE *arq;
    int val;
    arq=fopen("arquivo.bin", "rb");
    if(arq!=NULL){
        while(!feof(arq)){
            fread(&val,sizeof(int),1,arq);
        if(!feof(arq))
            inserir(&no,val);
        }
    }
            return no;
}

int menu();
int opcao;


int main(){
	FILE *arq;
	node *raiz = NULL;
    node *tmp;
    int n;
	 
	do{
		menu();
		
		switch(opcao){
			case 0://SAIR
				exit(0);
			case 1://INSERIR
				system("cls||clear");
				printf("Digite um numero:");
				scanf("%d",&n);
				inserir(&raiz,n);
				salvaArq(raiz);
				break;
				
			case 2://REMOVER APENAS UM NÓ
				system("cls||clear");
				printf("Digite um numero:");
				scanf("%d",&n);
				deletaNo(&raiz,n);
				break;
			case 3://PESQUISAR
				system("cls||clear");
				printf("Digite um numero:");
				scanf("%d",&n);
				tmp = busca(&raiz,n);
				if (tmp)
			    {
			        printf("Elemento encontrado na arvore!\n");
			    }
			    else
			    {
			        printf("Elemento nao encontrado na arvore!\n");
			    }
				break;
			case 4://ESVAZIAR
				system("cls||clear");
				deletarArvore(raiz);
				break;
			case 5://EXIBIR ARVORE
				system("cls||clear");
				if(raiz!=NULL){
					printf("PRE-ORDEM:\n");
					imprimePre(raiz);
					printf("IN-ORDEM:\n");
					imprimeIn(raiz);
					printf("POS-ORDEM:\n");
					imprimePre(raiz);
					printf("\nPressione qualquer tecla para exibir graficamente...");
					getch();
					system("cls||clear");
					printf("\tARVORE(Pressione qualquer tecla para retornar ao menu...)");
					exibirGraficamente(raiz,10,1,4);
					getch();
					system("cls||clear");
				
				}
				else{
					printf("Nao existe dados na arvore ainda...\n\n");
				}
				break;
			default:
				system("cls||clear");
				printf("OPCAO INVALIDA!\n\n");
		}
	}while(opcao!=0);
}

int menu(){
	printf("\nALUNO: LINCOLN MAX PAIVA D'OLIVEIRA\nDISCIPLINA: ESTRUTURA DE DADOS I\nPROFESSOR: WALACE BONFIM\n\nEDITOR DE ARVORE\n\n1 - INSERIR\n2 - REMOVER APENAS UM NO\n3 - PESQUISAR\n4 - ESVAZIAR A ARVORE\n5 - EXIBIR A ARVORE\n0 - SAIR\n\nDIGITE SUA OPCAO:");
	scanf("%d",&opcao);
	
	return opcao;
}

void salvaArq(node ** tree)
{
  FILE* file;
   if(fopen_s(&"arquivo.txt","a+")!=0)
   {
     puts("failure\n");
   }
   else
   {
    salvaItem(&(*tree),file);
    fclose(file);
   }
  }

void salvaItem(node **tree,FILE* file)
{
  if(tree != NULL)
   {
    fprintf(file,"%d\n",*tree->data.numero);
    salvaItem(&(*tree->esq),file);
	salvaItem(&(*tree->dir),file);
   }
}










