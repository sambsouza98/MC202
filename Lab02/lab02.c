#include <stdio.h>
#include <stdlib.h>

//Código feito por Samuel Batista de Souza RA: 205681.
/*Objetivo: O objetivo do código é gerar uma lista de atendimento de um hospital, respeitando as prioridades de emergência e ordem de atendimento de forma ordenada de idades decrescente. A lista funcionará de acordo com números que serão responsáveis por realizar operações na própria lista. 	As operações são:
	(1) Adicionar: Dado um paciente na lista com código, idade e categoria correspondente, é adicionado na lista.
	(2) Remover: Dado o código de um paciente, ele é removido da lista.
	(3) Chamar: Dada uma categoria, um paciente é chamado por ordem prioritária.
	(4)	Mudar: Dado o código de um paciente e uma categoria, esse é mudado da categoria atual para a categoria passada.
	(5)	Imprimir categoria: Dada uma categoria são exibidos todos os pacientes dessa categoria.
	(6) Imprimir pacientes: Dado um numero t são exibidos os t primeiros pacientes da lista.
*/

typedef struct no{
	int codigo;
	int idade;
	struct no *prox;
}No;

typedef struct{
	No *inicio;
}Lista;

Lista* alocaListas();
void inicializaListas(Lista* lista);
void listaDeOperacoes(int operacao, Lista* geral, Lista* especial);
void adicionarPaciente(Lista* geral, Lista* especial, int codigo, int prioridade, int idade);
void inserirInicio(Lista* lista, int prioridade, int codigo, int idade);
void inserirMeio(Lista* lista, int prioridade, int codigo, int idade, No *pred);
No* buscarListaIdade(Lista* lista, int categoria, int idade, No** pre);
void removerPaciente(Lista* geral, Lista* especial, int codigo, int removido);
No* buscarListaCodigo(Lista* lista, int categoria, int codigo, No** pre);
void removerInicio(Lista* lista, int prioridade);
void removerMeio(Lista* lista, int prioridade, No* pred);
void chamarPaciente(Lista* geral, Lista* especial, int prioridade);
void mudarPaciente(Lista* geral, Lista* especial, int codigo, int prioridade);
void imprimirLista(Lista* geral, Lista* especial, int prioridade);
void imprimirPacientes(Lista* geral, Lista* especial, int pacientes);

int main(){
// Sao declarados dois ponteiros pra lista pois a ideia é fazer um vetor de listas em cada variavel, um vetor pra lista geral e um pra lista especial.
	Lista *listaGeral, *listaEspecial; 
	int operacao;

//Alocaçao das listas
	listaGeral = alocaListas(); 
	listaEspecial = alocaListas();

//Inicialização das listas
	inicializaListas(listaGeral);
	inicializaListas(listaEspecial);

//Leitura das operaçoes e passagem das listas e da operação para a função correspondente
	while(scanf("%d", &operacao) != EOF){
		listaDeOperacoes(operacao, listaGeral, listaEspecial);
	}

	return 0;
}

Lista* alocaListas(){
	Lista *pont;

	pont = malloc(5*sizeof(Lista)); //Como a alocação será feita para um vetor de listas, será alocado um espaço correspondente a 5 listas, pois é a quantidade máxima de prioridades

	return pont;
}

void inicializaListas(Lista* lista){
	int i;

	for(i = 0; i < 5; i++){
		lista[i].inicio = NULL; //Como as listas estao com seu início em um vetor, para inicializá-las é necessário um laço para percorre-lo
	}
}


//Chamadas das funçoes dado o código cada operação
void listaDeOperacoes(int operacao, Lista* geral, Lista* especial){
	int codigo, prioridade, idade, pacientes;

	switch(operacao)
	{
		case 1:
			scanf("%d %d %d", &codigo, &prioridade, &idade);
			adicionarPaciente(geral, especial, codigo, prioridade, idade);
			printf("Paciente %d (idade %d) adicionado com sucesso na lista de prioridade %d!\n", codigo, idade, prioridade);
		break;

		case 2:
			scanf("%d", &codigo);
			removerPaciente(geral, especial, codigo, 1);
		break;

		case 3:
			scanf("%d", &prioridade);
			chamarPaciente(geral, especial, prioridade);
		break;

		case 4:
			scanf("%d %d", &codigo, &prioridade);
			mudarPaciente(geral, especial, codigo, prioridade);
		break;

		case 5:
			scanf("%d", &prioridade);
			imprimirLista(geral, especial, prioridade);
		break;

		case 6:
			scanf("%d", &pacientes);
			imprimirPacientes(geral, especial, pacientes);
		break;
	}
}


void adicionarPaciente(Lista* geral, Lista* especial, int codigo, int prioridade, int idade){
	No *novo, *pre, *busca;
	pre = NULL;

	if((idade <= 3) || (idade >= 65)){ //Verifica a idade para saber em qual grupo de prioridade (geral ou especial) ele se encaixa
		if(especial[prioridade].inicio == NULL){ //Se a lista estiver vazia ele adiciona no começo
			novo = malloc(sizeof(No));
			novo->idade = idade;
			novo->codigo = codigo;
			novo->prox = NULL;
			especial[prioridade].inicio = novo;
		} else { //senao ele acha o lugar correto para ser colocado.
			busca = buscarListaIdade(especial, prioridade, idade, &pre);
			if(busca != NULL){
				pre = busca;
			}
			if(pre == NULL){
				inserirInicio(especial, prioridade, codigo, idade);
			} else {	
				inserirMeio(especial, prioridade, codigo, idade, pre);
			}
		}
	} else { //Grupo de prioridade geral
		if(geral[prioridade].inicio == NULL){
			novo = malloc(sizeof(No));
			novo->idade = idade;
			novo->codigo = codigo;
			novo->prox = NULL;
			geral[prioridade].inicio = novo;
		} else {
			busca = buscarListaIdade(geral, prioridade, idade, &pre);
			if(busca != NULL){
				pre = busca;
			}
			if(pre == NULL){
				inserirInicio(geral, prioridade, codigo, idade);
			} else {
				inserirMeio(geral, prioridade, codigo, idade, pre);
			}
		}
	}
}

//Funcao para adicionar no inicio da lista nao vazia
void inserirInicio(Lista* lista, int prioridade, int codigo, int idade){
	No *novo = malloc(sizeof(No));

	if(novo == NULL){
		printf("ERRO");
	}

	novo->codigo = codigo;
	novo->idade = idade;
	novo->prox = lista[prioridade].inicio;
	lista[prioridade].inicio = novo;
}

//Funcao para adicionar no meio da lista
void inserirMeio(Lista* lista, int prioridade, int codigo, int idade, No *pred){
	No *novo = malloc(sizeof(No));

	if(novo == NULL){
		printf("ERRO");
	}

	novo->codigo = codigo;
	novo->idade = idade;
	novo->prox = pred->prox;
	pred->prox = novo;
}

//Busca da posição para adicionar o paciente
No* buscarListaIdade(Lista* lista, int categoria, int idade, No** pre){
	*pre = NULL;
	No *corrente = lista[categoria].inicio;

	while((corrente != NULL) && (corrente->idade > idade)){
		*pre = corrente;
		corrente = corrente->prox;
	}

	if((corrente != NULL) && (corrente->idade == idade)){
		return corrente;
	}

	return NULL;
}

void removerPaciente(Lista* geral, Lista* especial, int codigo, int removido){
	int i, prioridade, ehGeral, idade;
	No *busca, *pred;
	pred = NULL;
	busca = NULL;

	for(i = 0; (i < 5) && busca == NULL; i++){ //Busca pelo paciente em todas as categorias da prioridade geral
		busca = buscarListaCodigo(geral, i, codigo, &pred);
		ehGeral = 1;
	}
	prioridade = i - 1;

	for(i = 0; (i < 5) && busca == NULL; i++){ //Busca pelo paciente em todas as categorias da prioridade especial
		busca = buscarListaCodigo(especial, i, codigo, &pred);
		ehGeral = 0;
		prioridade = i;
	}

	if(ehGeral){ //Se for geral, ele procura o lugar do paciente a ser removido e remove na lista geral
		if(pred == NULL){
			idade = geral[prioridade].inicio->idade;
			removerInicio(geral, prioridade);
		} else {
			idade = pred->prox->idade;
			removerMeio(geral, prioridade, pred);
		}
	} else { //Se nao for, ele procura o lugar do paciente a ser removido e remove na lista especial
		if(pred == NULL){
			idade = especial[prioridade].inicio->idade;
			removerInicio(especial, prioridade);
		} else {
			idade = pred->prox->idade;
			removerMeio(especial, prioridade, pred);
		}
	}

	if(removido){ //Foi passado como parametro pra funcao a variavel 'removido' para a mensagem nao ser printada em outra funcao que utiliza a remover
		printf("Paciente %d (idade %d) removido com sucesso na lista de prioridade %d!\n", codigo, idade, prioridade);
	}
}

//Funcao para remover o paciente no inicio da lista
void removerInicio(Lista* lista, int prioridade){
	No *temp = lista[prioridade].inicio;

	lista[prioridade].inicio = temp->prox;
	free(temp);
}

//Funcao para remover o paciente no meio da lista
void removerMeio(Lista* lista, int prioridade, No* pred){
	No *temp = pred->prox;

	pred->prox = temp->prox;
	free(temp);
}

//Funcao de busca do paciente em uma dada lista
No* buscarListaCodigo(Lista* lista, int categoria, int codigo, No** pre){
	*pre = NULL;
	No *corrente = lista[categoria].inicio;

	while((corrente != NULL) && (corrente->codigo != codigo)){
		*pre = corrente;
		corrente = corrente->prox;
	}

	if((corrente != NULL) && (corrente->codigo == codigo)){
		return corrente;
	}

	return NULL;
}

//Funcao para chamar o paciente
void chamarPaciente(Lista* geral, Lista* especial, int prioridade){
	int codigo, idade;

	if(especial[prioridade].inicio != NULL){ //Encontra o primeiro paciente da lista de prioridade especial (se existir) e o remove da lista
		codigo = especial[prioridade].inicio->codigo;
		idade = especial[prioridade].inicio->idade;
		removerPaciente(geral, especial, codigo, 0);
	} else { //Encontra o primeiro paciente da lista de prioridade geral (se existir) e o remove da lista
		codigo = geral[prioridade].inicio->codigo;
		idade = geral[prioridade].inicio->idade;
		removerPaciente(geral, especial, codigo, 0);
	}

	printf("Paciente %d (idade %d) da categoria de prioridade %d chamado para atendimento!\n", codigo, idade, prioridade);
}

void mudarPaciente(Lista* geral, Lista* especial, int codigo, int prioridade){
	int idade, i, ehGeral, prioridadeInicial;
	No *busca1, *pred1;
	pred1 = NULL;
	busca1 = NULL;

//Verifica em qual lista o paciente se encontra
	for(i = 0; (i < 5) && busca1 == NULL; i++){ 
		busca1 = buscarListaCodigo(geral, i, codigo, &pred1);
		ehGeral = 1;
	}
	prioridadeInicial = i-1;

	for(i = 0; (i < 5) && busca1 == NULL; i++){
		busca1 = buscarListaCodigo(especial, i, codigo, &pred1);
		ehGeral = 0;
		prioridadeInicial = i;
		
	}

	if(ehGeral){ //Se estiver na lista geral:
		if(pred1 == NULL){ //Se estiver no inicio da lista, recebe o valor da idade do primeiro paciente
			idade = geral[prioridadeInicial].inicio->idade;
		} else { //Se nao estiver, recebe o valor da idade do paciente encontrado
			idade = pred1->prox->idade;
		}
	} else { //Se estiver na lista especial
		if(pred1 == NULL){ //Se estiver no inicio da lista, recebe o valor da idade do primeiro paciente
			idade = especial[prioridadeInicial].inicio->idade;
		} else { //Se nao estiver, recebe o valor da idade do paciente encontrado
			idade = pred1->prox->idade;
		}
	}

//Dado o codigo do paciente encontrado ele é removido de onde esta e é adicionado na nova lista
	removerPaciente(geral, especial, codigo, 0); 
	adicionarPaciente(geral, especial, codigo, prioridade, idade);

	printf("Paciente %d (idade %d) mudou para a categoria de prioridade %d!\n", codigo, idade, prioridade);
}

void imprimirLista(Lista* geral, Lista* especial, int prioridade){
	int codigo, idade;
	No *atual;

	if((geral[prioridade].inicio == NULL) && (especial[prioridade].inicio == NULL)){ //Verificacao se as duas prioridades estao vazias
		printf("Nenhum paciente na categoria %d\n", prioridade);
	} else {
		atual = especial[prioridade].inicio;
		while(atual != NULL){ //Percorre todos os pacientes da lista recebendo os dados de codigo e idade e imprimindo-os (prioridade especial)
			codigo = atual->codigo;
			idade = atual->idade;

			printf("Paciente %d (idade %d) da lista %d\n", codigo, idade, prioridade);
			atual = atual->prox;
		}

		atual = geral[prioridade].inicio;
		while(atual != NULL){ //Percorre todos os pacientes da lista recebendo os dados de codigo e idade e imprimindo-os (prioridade geral)
			codigo = atual->codigo;
			idade = atual->idade;

			printf("Paciente %d (idade %d) da lista %d\n", codigo, idade, prioridade);
			atual = atual->prox;
		}
	}
}

void imprimirPacientes(Lista* geral, Lista* especial, int pacientes){
	int i, idade, codigo;
	No *atual;

	for(i = 0; (i < 5) && (pacientes > 0); i++){ //Percorre todas as listas nas prioridades especial e geral e conta até chegar o numero desejado de pacientes
		atual = especial[i].inicio;
		while((atual != NULL) && (pacientes > 0)){
			codigo = atual->codigo;
			idade = atual->idade;

			printf("Paciente %d (idade %d) da lista %d\n", codigo, idade, i);
			atual = atual->prox;
			pacientes--;
		}

		atual = geral[i].inicio;
		while((atual != NULL) && (pacientes > 0)){
			codigo = atual->codigo;
			idade = atual->idade;

			printf("Paciente %d (idade %d) da lista %d\n", codigo, idade, i);
			atual = atual->prox;
			pacientes--;
		}
	}
}
