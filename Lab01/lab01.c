#include <stdio.h>
#include <stdlib.h>

//Código produzido por Samuel Batista de Souza RA: 205681;
//Objetivo: o objetivo do programa é fazer o armazenamento de produtos em um galpão de tamanho especificado pelo usuário do programa, fazendo a leitura de cada linha de um arquivo com o setor, o número de identificação do produto e sua quantidade correspondente.

int** alocGalpao(int n, int m);
void armazenar(int** galpao, int* produtos, int* quantidades);
void imprimirGalpao(int setores, int* quantidades, int* nProdutos);
void buscaIguais(int** galpao, int* produtos, int setores);

int main(){
	int **galpao_IDs, *setores_numero_de_produtos, *setores_quantidades;
	int setores, maxProdutos;

//Leitura da primeira linha do arquivo que contém a quantidade de setores e o limite de produtos distintos por setor
	scanf("%d %d", &setores, &maxProdutos);

//Alocação dinamica de uma matriz para os IDs dos produtos e dois vetores que guardarão a quantidade de produtos distintos e a quantidade total de produtos por setor	
	galpao_IDs = alocGalpao(setores, maxProdutos);
	setores_numero_de_produtos = malloc(setores*sizeof(int));
	setores_quantidades = malloc(setores*sizeof(int));

//Verificação inicial da quantidade de setores: se for igual a zero automaticamente a mensagem NAO é impressa na tela dizendo que nao há produtos iguais, se não for, são chamadas as funçoes responsáveis por armazenar os produtos no galpão, imprimir o que há em cada galpão e buscar se há produtos iguais.
	if(setores != 0){
		armazenar(galpao_IDs, setores_numero_de_produtos, setores_quantidades);
		imprimirGalpao(setores, setores_quantidades, setores_numero_de_produtos);
		buscaIguais(galpao_IDs, setores_numero_de_produtos, setores);
	} else {
		printf("NAO");
	}
}

//Função de alocação de uma matriz com n linhas e m colunas
int** alocGalpao(int n, int m){
	int **pont;
	int i;
	
	pont = malloc(n*sizeof(int*));

	for(i = 0; i < n; i++){
		pont[i] = malloc(m*sizeof(int));
	}

	return pont;
}

//Função para o armazenamento dos produtos no galpão
void armazenar(int** galpao, int* produtos, int* quantidades){
	int setor = 0, ID = 0, quantidade = 0;

	//Leitura da primeira linha de produtos do arquivo e armazenamento dos dados em variáveis correspondentes
	scanf("%d %d %d", &setor, &ID, &quantidade);
	
	//Laço para verificar cada linha do arquivo até o final do arquivo (EOF =  End Of File) e salvar os em cada setor da matriz correspondente	
	do{
		galpao[setor][produtos[setor]] = ID; //Salva o ID do produto no lugar correspondente da matriz
		quantidades[setor] = quantidades[setor] + quantidade; //Aumenta a quantidade de produtos no setor e salva no vetor de setores
		produtos[setor]++; //Aumenta a quantidade de produtos diferentes no vetor produtos
	} while(scanf("%d %d %d", &setor, &ID, &quantidade) != EOF);
}

//Função para imprimir os dados necessários
void imprimirGalpao(int setores, int* quantidades, int* nProdutos){
	int i;
	double media;
	
	//Laço para percorrer os vetores quantidades e nProdutos. Calcula a média e retorna em um double e imprime o numero do setor, a quantidade de produtos e a média de produtos do setor
	for(i = 0; i < setores; i++){
		media = (double)quantidades[i]/nProdutos[i];
		printf("%d %d %.2lf\n", i, quantidades[i], media);
	}
}

//Função para buscar produtos iguais em setores diferentes
void buscaIguais(int** galpao, int* produtos, int setores){
	int i, j, k, l;

	//Laços encaixados para percorres a matriz verificando cada produto de um setor i com todos os outros da matriz de outros setores. Se houver um produto igual é impressa a mensagem SIM. se ele percorrer a matriz inteira e nao houver nenhum produto igual em diferentes setores, é impressa a mensagem NAO
	for(i = 0; i < setores; i++){
		for(j = 0; j < produtos[i]; j++){
			for(k = i + 1; k < setores; k++){
				for(l = 0; l < produtos[k]; l++){
					if(galpao[i][j] == galpao[k][l]){
						printf("SIM\n");
						return;
					}
				}
			}
		}
	}
	printf("NAO\n");
}
