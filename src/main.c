/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int prime(unsigned long long number);

int main() {
	/* entrada do tipo unsigned long long para aceitar entradas
	no intervalo de 0 até 2^64 - 1*/
  unsigned long long entrada[100];
  int n_entradas =0, i =0;
  /* Caracter de escape da entrada. Neste caso foi considerado
  a quebra de linha ou EOF como fim da entrada*/
  char escape;
  /* Declara um vetor de processos*/
  pid_t processos[4];

  /* Variáveis para compartilhamento de memória*/
	int protection = PROT_READ | PROT_WRITE ;
	int visibility = MAP_SHARED | MAP_ANON ;

	/* variável compartilhada para armazenamento do numero total de 
	numeros primos*/
	int *n_prime;
	n_prime = (int *) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
	/* inicialização da variável*/
	*n_prime =0;

	/* Entrada de dados*/
  do {
  	scanf("%llu%c", &entrada[n_entradas], &escape);
  	n_entradas++;
  } while (escape != '\n' && escape != EOF);

  /* Vetor de criação e fechamento de processos*/ 
  for (int j=0; j<4, i<n_entradas; j++, i++){
  	/* cria o processo j*/
  	processos[j] = fork();
  	/* se estiver dentro do processo j, executa o calculo do primo
  	a função prime(entrada), retorna 1 se o valor da entrada for um numero
  	primo e 0 caso contrário*/ 
  	if (processos[j] == 0){
  		*n_prime += prime(entrada[i]);
  		/* sai do processo no termino de sua funcao*/
  		exit(0);
  	}
  	/* espera todos os processos terminarem em 2 casos:
  		1 - alcançou o numero máximo de processos paralelos
  		2 - alcançou o ultimo numero do vetor de numeros da entrada*/
  	if (j == 3 || i == n_entradas-1){
  		for (int k=0; k<4; k++) waitpid(processos[k], NULL, 0);
  		j = -1;
  	}
  }

  // printa entradas
  printf("%d\n", *n_prime);

  return 0;
}


int prime(unsigned long long number){
	/* algoritmo simples pra calculo de primos 
		ENTRADA: um numero int de 0 até 2^64 - 1
		SAÍDA: 1 se o número é primo
					 0 se o número não é primo
	*/

	int divisores =0, retorno =0;
	unsigned long long i=0;
	if (number<=1) retorno = 0;
  else{
	  for(i=1; i<=number/2; i++){
	  	if (number%i == 0) divisores++;
	  	if (divisores >2){
	  		retorno = 0;
	  		break;
	  	} 
	  } 
  	if (divisores <= 1) retorno = 1;
  }
  return retorno;
}