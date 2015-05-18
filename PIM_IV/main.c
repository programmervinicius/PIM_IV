/* Declaracao de bibliotecas nativas da linguagem C */
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

/* Constante definida para ser usada na
 rotina de carregamento das structs*/
#define MAX 40

/* Criacao de struct para armazenar dados de
 Entradas e Saidas de Veiculos e de Pagamentos */
struct EntradaVeiculo {
	int iCodigoEntrada;
	char cPlaca[9];
	char cDataEntrada[12];
	char cHorarioEntrada[8];
	time_t tDataHorarioEntrada;
	double dDataHorarioEntrada;
};

struct SaidaVeiculo {
	int iCodigoSaida;
	char cPlaca[9];
	char cDataSaida[12];
	char cHorarioSaida[8];
	time_t tDataHorarioSaida;
	double dDataHorarioSaida;
};

struct Pagamento {
	int iCodigoPagamento;
	char cPlaca[9];
	char cDataPagamento[12];
	char cHorarioPagamento[8];
	float fValorPagamento;
};

void limparTela() {
#ifdef linux || LINUX || Linux || UNIX
	printf("\e[H\e[2J"); /* Limpa a tela do sistema - Linux*/
#elif defined WIN32
	system ("cls"); /* Limpa a tela do sistema - Windows*/
#endif
}

void pausarMenuPrincipal() {
#ifdef defined WIN32
	system("pause>>null");
#endif;
}

/* Metodo......: main
 * Parametros..: Sem parametros
 * Descricao...: Executa as rotinas do aplicativo.
 * Data Criacao: 10/11/2014
 * Retorno.....: Vazio
 */
int main() {
	/* ----------- Inicio do metodo principal ----------- */

	/* Criacao de nomes para as structs de
	 Entradas e Saidas de Veiculos e Pagamentos */
	struct EntradaVeiculo listaEntradas[3000];
	struct SaidaVeiculo listaSaidas[3000];
	struct Pagamento listaPagamentos[3000];

	/* Variaveis para armazenar os valores de placas e a
	 linha do arquivo do carregamento das structs */
	char cPlaca[9] = " ", cPlacaAux[9] = " ", cLinhaArquivo[MAX] = " ",
			cDataAbertura[12] = " ", cNomeArquivo[60] = " ",
			cExtensaoArquivo[5] = ".txt";

	char *cResultadoLinhaArquivo;
	char *cCharAux;

	/* Declaraco de Variaveis do sistema */
	int iContador, iContadorAux, iOpcao, iOpcaoDigitada, iResultado,
			iVeiculoEncontrado, iNumeroVagasDisponiveis, iUltimoCodigoEntrada,
			iUltimoCodigoSaida, iUltimoCodigoPagamento, iPagamentoConfirmado;

	/* Variaveis para armazenar o valor da
	 primeira hora e das demais horas */
	float fValorPrimeiraHora, fValorDemaisHoras, fValorConfirmado;

	/* Variavel para trabalhar com a data e hora atual */
	time_t tNow;
	time_t tDataAbertura;

	/* Ponteiros para arquivos de entradas, saidas, pagamentos e tabela de tarifas */
	FILE* fEntradas;
	FILE* fSaidas;
	FILE* fPagamentos;
	FILE* fTabelaTarifas;

	/* Declaraco de Variaveis das Entradas de Veiculos */
	int iCodigoEntrada, iCodigoEntradaAux;
	char cDataEntrada[12] = " ";
	char cHorarioEntrada[8] = " ";
	double dDataHorarioEntrada;

	/* Declaraco de Variaveis das Saidas de Veiculos */
	int iCodigoSaida, iCodigoSaidaAux;
	char cDataSaida[12] = " ";
	char cHorarioSaida[8] = " ";
	double dDataHorarioSaida;

	/* Declaracao de Variaveis dos Pagamentos */
	int iCodigoPagamento, iCodigoPagamentoAux;
	char cDataPagamento[12] = " ";
	char cHorarioPagamento[8] = " ";
	float fValorPagamento;
	double dDiferencaSegundos;

	/* Inicializacao e limpeza de variaveis */
	iCodigoEntrada = 0;
	iCodigoEntradaAux = 0;
	iCodigoSaida = 0;
	iCodigoSaidaAux = 0;
	iCodigoPagamento = 0;
	iCodigoPagamentoAux = 0;
	iContador = 0;
	iContadorAux = 0;
	iOpcaoDigitada = 0;
	iVeiculoEncontrado = 0;
	iResultado = 0;
	iUltimoCodigoEntrada = 0;
	iUltimoCodigoSaida = 0;
	iUltimoCodigoPagamento = 0;
	iPagamentoConfirmado = 0;
	fValorPagamento = 0;
	fValorConfirmado = 0;
	dDataHorarioEntrada = 0;
	dDataHorarioEntrada = 0;

	limparTela;

	/********* ----------- Inicio da rotina de carregamento das structs ----------- *********/
	tDataAbertura = time(NULL); /* Variavel de data da abertura do sistema */
	strftime(cDataAbertura, 20, "%d%m%Y", localtime(&tDataAbertura));

	/* Cria o nome do arquivo de entradas */
	sprintf(cNomeArquivo, "%s%s%s", "entradas_", cDataAbertura,
			cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fEntradas = fopen(cNomeArquivo, "rt");

	if (fEntradas == NULL) /* Se houve erro na abertura */
	{
		/* Cria um arquivo de texto caso n�o encontrar nenhum arquivo no diretorio do sistema */
		if ((fEntradas = fopen(cNomeArquivo, "a+")) == NULL) {
			perror("Descricao do erro.");
			exit(1);
		}
	}

	/* Inicializacao das variaveis */
	iContador = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));
	memset(&cResultadoLinhaArquivo, 0, sizeof(cResultadoLinhaArquivo));

	while (!feof(fEntradas)) {
		cResultadoLinhaArquivo = fgets(cLinhaArquivo, MAX, fEntradas); /* Le uma linha (inclusive com o '\n') */

		if (cResultadoLinhaArquivo) /* Se foi possivel ler */
		{
			if (strcmp(cLinhaArquivo, "Entrada de Veiculo\n") == 0)
				iOpcao = 0;

			if (iOpcao > -1)
				cResultadoLinhaArquivo = fgets(cLinhaArquivo, MAX, fEntradas);

			if (iOpcao == 0) {
				iCodigoEntrada = atoi(cLinhaArquivo);
				iUltimoCodigoEntrada = iCodigoEntrada;
				listaEntradas[iCodigoEntrada].iCodigoEntrada = iCodigoEntrada;
				fgets(listaEntradas[iCodigoEntrada].cPlaca, MAX, fEntradas);
				fgets(listaEntradas[iCodigoEntrada].cDataEntrada, MAX,
						fEntradas);
				fgets(listaEntradas[iCodigoEntrada].cHorarioEntrada, MAX,
						fEntradas);
				fgets(cLinhaArquivo, MAX, fEntradas);
				dDataHorarioEntrada = strtod(cLinhaArquivo, NULL);
				listaEntradas[iCodigoEntrada].dDataHorarioEntrada =
						dDataHorarioEntrada;
				listaEntradas[iCodigoEntrada].tDataHorarioEntrada =
						dDataHorarioEntrada;
				break;
			}
		}
		iOpcao = -1;
		iContador++;
	}

	/* Cria o nome do arquivo de saidas */
	sprintf(cNomeArquivo, "%s%s%s", "saidas_", cDataAbertura, cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fSaidas = fopen(cNomeArquivo, "rt");

	if (fSaidas == NULL) /* Se houve erro na abertura */
	{
		/* Cria um arquivo de texto caso n�o encontrar nenhum arquivo no diretorio do sistema */
		if ((fSaidas = fopen(cNomeArquivo, "a+")) == NULL) {
			perror("Descricao do erro.");
			exit(1);
		}
	}

	/* Inicializacao das variaveis */
	iContador = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));
	memset(&cResultadoLinhaArquivo, 0, sizeof(cResultadoLinhaArquivo));

	while (!feof(fSaidas)) {
		cResultadoLinhaArquivo = fgets(cLinhaArquivo, MAX, fSaidas); /* Le uma linha (inclusive com o '\n') */

		if (cResultadoLinhaArquivo) /* Se foi possivel ler */
		{
			if (strcmp(cLinhaArquivo, "Saida de Veiculo\n") == 0)
				iOpcao = 0;

			if (iOpcao > -1)
				cResultadoLinhaArquivo = fgets(cLinhaArquivo, MAX, fSaidas);

			if (iOpcao == 0) {
				iCodigoSaida = atoi(cLinhaArquivo);
				iUltimoCodigoSaida = iCodigoSaida;
				listaSaidas[iCodigoSaida].iCodigoSaida = iCodigoSaida;
				fgets(listaSaidas[iCodigoSaida].cPlaca, MAX, fSaidas);
				fgets(listaSaidas[iCodigoSaida].cDataSaida, MAX, fSaidas);
				fgets(listaSaidas[iCodigoSaida].cHorarioSaida, MAX, fSaidas);
				fgets(cLinhaArquivo, MAX, fSaidas);
				dDataHorarioSaida = strtod(cLinhaArquivo, NULL);
				listaSaidas[iCodigoSaida].dDataHorarioSaida = dDataHorarioSaida;
				listaSaidas[iCodigoSaida].tDataHorarioSaida =
						dDataHorarioEntrada;
				break;
			}
		}
		iOpcao = -1;
		iContador++;
	}

	/* Cria o nome do arquivo de pagamentos */
	sprintf(cNomeArquivo, "%s%s%s", "pagamentos_", cDataAbertura,
			cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fPagamentos = fopen(cNomeArquivo, "rt");

	if (fPagamentos == NULL) /* Se houve erro na abertura */
	{
		/* Cria um arquivo de texto caso n�o encontrar nenhum arquivo no diretorio do sistema */
		if ((fPagamentos = fopen(cNomeArquivo, "a+")) == NULL) {
			perror("Descricao do erro.");
			exit(1);
		}
	}

	/* Inicializacao das variaveis */
	iContador = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));
	memset(&cResultadoLinhaArquivo, 0, sizeof(cResultadoLinhaArquivo));

	while (!feof(fPagamentos)) {
		cResultadoLinhaArquivo = fgets(cLinhaArquivo, MAX, fPagamentos); /* Le uma linha (inclusive com o '\n') */

		if (cResultadoLinhaArquivo) /* Se foi possivel ler */
		{
			if (strcmp(cLinhaArquivo, "Pagamento\n") == 0)
				iOpcao = 0;

			if (iOpcao > -1)
				cResultadoLinhaArquivo = fgets(cLinhaArquivo, MAX, fPagamentos);

			if (iOpcao == 0) {
				iCodigoPagamento = atoi(cLinhaArquivo);
				iUltimoCodigoPagamento = iCodigoPagamento;
				listaPagamentos[iCodigoPagamento].iCodigoPagamento =
						iCodigoPagamento;
				fgets(listaPagamentos[iCodigoPagamento].cPlaca, MAX,
						fPagamentos);
				fgets(listaPagamentos[iCodigoPagamento].cDataPagamento, MAX,
						fPagamentos);
				fgets(listaPagamentos[iCodigoPagamento].cHorarioPagamento, MAX,
						fPagamentos);
				fgets(cLinhaArquivo, MAX, fPagamentos);
				fValorPagamento = strtod(cLinhaArquivo, NULL);
				listaPagamentos[iCodigoPagamento].fValorPagamento =
						fValorPagamento;
				break;
			}
		}
		iOpcao = -1;
		iContador++;
	}

	fclose(fEntradas);
	fclose(fSaidas);
	fclose(fPagamentos);
	/********* ----------- Fim da da rotina de carregamento das structs ----------- *********/

	limparTela;

	/* Carregamento vari�veis de valores da primeira hora e das demais horas */
	/* Cria o nome do arquivo de tabela de tarifas */
	sprintf(cNomeArquivo, "%s%s%s", "tabela_tarifas_", cDataAbertura,
			cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fTabelaTarifas = fopen(cNomeArquivo, "rt");

	if (fTabelaTarifas == NULL) /* Se houve erro na abertura */
	{
		/* Cria um arquivo de texto caso n�o encontrar nenhum arquivo no diretorio do sistema */
		if ((fTabelaTarifas = fopen(cNomeArquivo, "a+")) == NULL) {
			perror("Descricao do erro.");
			exit(1);
		}
	}

	/* Inicializacao das variaveis */
	iContador = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));
	memset(&cResultadoLinhaArquivo, 0, sizeof(cResultadoLinhaArquivo));

	while (!feof(fTabelaTarifas)) {
		cResultadoLinhaArquivo = fgets(cLinhaArquivo, MAX, fTabelaTarifas); /* Le uma linha (inclusive com o '\n') */

		if (cResultadoLinhaArquivo) /* Se foi possivel ler */
		{
			fgets(cLinhaArquivo, MAX, fTabelaTarifas);
			fValorPrimeiraHora = strtod(cLinhaArquivo, NULL);
			fgets(cLinhaArquivo, MAX, fTabelaTarifas);
			fValorDemaisHoras = strtod(cLinhaArquivo, NULL);
		}
	}

	fclose(fTabelaTarifas);
	/* Fim leitura arquivos */

	printf(
			"___________________________________________________________________________\n");
	printf("\t\t:: SISTEMA DE ESTACIONAMENTO DE VEICULOS ::\n\n");
	printf(" ::   PARA COMECAR A USAR ESTE SISTEMA, E NECESSARIO    ::\n");
	printf(" ::      DEFINIR ALGUMAS INFORMACOES IMPORTANTES.       ::\n");
	printf(" :: VAMOS DEFINIR AGORA OS VALORES DA PRIMEIRA HORA     ::\n");
	printf(" :: E DAS DEMAIS HORAS E O NUMERO DE VAGAS DISPONIVEIS. ::\n");
	printf("\n\n :: Todos os dados informados somente serao armazenados ::");
	printf("\n :: no arquivo de texto ao sair do sistema.             ::");
	printf(
			"\n\n :: Observacao: Os valores em decimais devem ser informados com pontos.::");
	printf(
			"\n ::             Caso for informado com virgula entao ocorrera erro.    ::");
	printf("\n\n :: Informe o valor para a primeira hora: ");
	scanf("%f", &fValorPrimeiraHora);

	if (fValorPrimeiraHora == 0) {
		while (fValorPrimeiraHora == 0) {
			printf("\n :: O valor deve ser maior que 0.");
			printf("\n :: Informe o valor para a primeira hora: ");
			scanf("%f", &fValorPrimeiraHora);
		}
	}

	printf("\n :: Informe o valor para as demais horas: ");
	scanf("%f", &fValorDemaisHoras);

	if (fValorDemaisHoras == 0) {
		while (fValorDemaisHoras == 0) {
			printf("\n :: O valor deve ser maior que 0.");
			printf("\n :: Informe o valor para as demais horas: ");
			scanf("%f", &fValorDemaisHoras);
		}
	}

	printf("\n :: E, por ultimo, informe o numero de vagas disponiveis: ");
	scanf("%d", &iNumeroVagasDisponiveis);

	if (iNumeroVagasDisponiveis == 0) {
		while (iNumeroVagasDisponiveis == 0) {
			printf("\n :: O numero de vagas disponiveis deve ser maior que 0.");
			printf("\n :: Informe o numero de vagas disponiveis: ");
			scanf("%d", &iNumeroVagasDisponiveis);
		}
	}

	printf("\n\n\t\t\t::::: Obrigado. :::::");
	fflush(stdin); /* Limpa a entrada padrao do sistema (teclado) */
	printf("\n\nTecle 'Enter' para entrar no menu principal.\n");
	getchar();

	limparTela();

	/* Laco do Menu Principal */
	while (1) {
		printf(
				"___________________________________________________________________________\n");
		printf("\t\t:: SISTEMA DE ESTACIONAMENTO DE VEICULOS ::\n");
		printf(" << Menu Principal >> \n");
		printf("  << Entradas e Saidas de Veiculos >> \n");
		printf("  1 - Entrada de Veiculo.\n");
		printf("  2 - Saida de Veiculo.\n\n");
		printf("  << Pesquisas >> \n");
		printf("  3 - Pesquisa de Registro de Entrada.\n");
		printf("  4 - Pesquisa de Registro de Saida.\n");
		printf("  5 - Pesquisa de Registro de Pagamento.\n\n");
		printf("  << Relatorios >> \n");
		printf("  6 - Impressao de Todas as Entradas.\n");
		printf("  7 - Impressao de Todas as Saidas.\n");
		printf("  8 - Impressao de Todos os Pagamentos.\n\n");
		printf("  << Gestao do Caixa >> \n");
		printf("  9 - Fechamento.\n\n");
		printf("  0 - Sair\n");
		printf(
				"___________________________________________________________________________\n");
		printf("\nEscolha uma das opcoes acima para continuar: ");
		scanf("%d", &iOpcaoDigitada);
		getchar();
		limparTela();

		switch (iOpcaoDigitada) /* Laco das opcoes do menu principal */
		{

		case 1: /* Entrada de Veiculo */
			if (iNumeroVagasDisponiveis == 0)
				printf("Nao ha vagas disponiveis no estacionamento.");
			else {
				printf("::: Entrada de Veiculo :::\n");
				iCodigoEntrada++; /* Incrementa o codigo de entrada a cada
				 nova entrada de veiculos */
				printf(
						"::: AVISO: A placa deve ter 8 caracteres. Exemplo: AAA-1234. :::\n");
				printf("    Informe a Placa do Veiculo: ");
				scanf("%s", &cPlaca);
				toupper(cPlaca); /* Altera o caracteres da placa para ficarem maiusculos */

				/* Validacao para procurar por veiculos ja cadastrados */
				for (iContador = 1; iContador <= iCodigoEntrada; iContador++) {
					if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaEntradas[iContador].cPlaca,
							'\n')) != NULL)
						*cCharAux = '\0';

					if (strcmp(cPlaca, listaEntradas[iContador].cPlaca) == 0) {
						while (strcmp(cPlaca, listaEntradas[iContador].cPlaca)
								== 0) {
							printf(
									"\n\n    O veiculo ja se encontra no estacionamento.");
							printf("\n    Por favor, informe outra placa: ");
							scanf("%s", &cPlaca);
							toupper(cPlaca); /* Altera o caracteres da placa para ficarem maiusculos */
						}
					}
				}

				if (strlen(cPlaca) != 8) {
					while (strlen(cPlaca) != 8) {
						printf(
								"    A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n",
								cPlaca);
						printf("    Informe a Placa do Veiculo: ");
						scanf("%s", &cPlaca);
						toupper(cPlaca); /* Altera os caracteres da placa para ficarem maiusculos */
					}
				}

				tNow = time(NULL);
				strftime(cDataEntrada, 20, "%d/%m/%Y", localtime(&tNow));
				strftime(cHorarioEntrada, 10, "%H:%M:%S", localtime(&tNow));
				printf(
						"\n::: Entrada de Veiculo realizada com Sucesso :::\n\n");

				/* Imprimir recibo de entrada de veiculo */
				printf("________________________________________\n");
				printf("***** RECIBO DE ENTRADA DE VEICULO *****\n");
				printf("   Entrada Numero: %d\n", iCodigoEntrada);
				printf("   Placa:          %s\n", cPlaca);
				printf("   Data:           %s\n", cDataEntrada);
				printf("   Horario:        %s\n", cHorarioEntrada);
				printf("****************************************\n");
				printf("________________________________________\n");

				listaEntradas[iCodigoEntrada].iCodigoEntrada = iCodigoEntrada;
				strcpy(listaEntradas[iCodigoEntrada].cPlaca, cPlaca);
				strcpy(listaEntradas[iCodigoEntrada].cDataEntrada,
						cDataEntrada);
				strcpy(listaEntradas[iCodigoEntrada].cHorarioEntrada,
						cHorarioEntrada);
				time(&listaEntradas[iCodigoEntrada].tDataHorarioEntrada);
				listaEntradas[iCodigoEntrada].dDataHorarioEntrada =
						listaEntradas[iCodigoEntrada].tDataHorarioEntrada;
				iNumeroVagasDisponiveis--;

				printf("\n\n::::: Numero de Vagas Disponiveis: %d :::::\n",
						iNumeroVagasDisponiveis);
			}

			printf("\n\nTecle 'Enter' para sair.");
			break;

		case 2: /* Saida de Veiculo */
			printf("::: Saida de Veiculo :::\n");
			iCodigoSaida++; /* Incrementa o codigo de saida a cada
			 nova saida de veiculo */
			iCodigoPagamento++; /* Incrementa o codigo de pagamento a cada
			 nova saida de veiculo */
			printf(
					"::: AVISO: A placa deve ter 8 caracteres. Exemplo: AAA-1234. :::\n");
			printf("    Informe a Placa do Veiculo: ");
			scanf("%s", &cPlaca);
			toupper(cPlaca); /* Altera o caracteres da placa para ficarem maiusculos */

			/* Valida��o da Placa que foi recebida na variavel cPlaca */
			if (strlen(cPlaca) != 8) {
				while (strlen(cPlaca) != 8) {
					printf(
							"A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n",
							cPlaca);
					printf("    Informe a Placa do Veiculo: ");
					scanf("%s", &cPlaca);
					toupper(cPlaca); /* Altera os caracteres da placa para ficarem maiusculos */
				}
			}

			for (iContador = 1; iContador <= iCodigoEntrada; iContador++) {
				if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(listaEntradas[iContador].cPlaca, '\n'))
						!= NULL)
					*cCharAux = '\0';

				/* Pesquisa por veiculo estacionado */
				if (strcasecmp(listaEntradas[iContador].cPlaca, cPlaca) == 0) {
					iVeiculoEncontrado = 1;
					iCodigoEntrada = iContador;
				} else
					iVeiculoEncontrado = 0;
			}

			/* Se encontrou ent�o ira realizar a saida do veiculo */
			if (iVeiculoEncontrado == 1) {
				tNow = time(NULL);
				strftime(cDataSaida, 20, "%d/%m/%Y", localtime(&tNow));
				strftime(cHorarioSaida, 10, "%H:%M:%S", localtime(&tNow));

				/* Registra a saida */
				listaSaidas[iCodigoSaida].iCodigoSaida = iCodigoSaida;
				strcpy(listaSaidas[iCodigoSaida].cPlaca, cPlaca);
				strcpy(listaSaidas[iCodigoSaida].cDataSaida, cDataSaida);
				strcpy(listaSaidas[iCodigoSaida].cHorarioSaida, cHorarioSaida);
				time(&listaSaidas[iCodigoSaida].tDataHorarioSaida);
				listaSaidas[iCodigoSaida].dDataHorarioSaida =
						listaSaidas[iCodigoSaida].tDataHorarioSaida;

				/* Calculo para gerar o valor a ser pago */
				/* Para obter o periodo de tempo que o veiculo ficou estacionado
				 e necessario usar a funcao difftime. O resultado dela e sempre em segundos. */
				dDiferencaSegundos = difftime(
						listaSaidas[iCodigoSaida].dDataHorarioSaida,
						listaEntradas[iCodigoEntrada].dDataHorarioEntrada);

				/* Nessa linha e realizada a conversao de segundos para horas. */
				dDiferencaSegundos = (dDiferencaSegundos / 3600);

				/* Limpo o valor confirmado com um valor 0 */
				fValorConfirmado = 0;

				if (dDiferencaSegundos < 1)
					fValorPagamento = (fValorPrimeiraHora);
				else
					fValorPagamento = (fValorPrimeiraHora
							+ ((dDiferencaSegundos - 1) * fValorDemaisHoras));
				/* Fim do calculo */

				printf(
						"\n  >>>> O Valor a Ser Confirmado para Pagamento: R$ %.2lf<<<<\n",
						fValorPagamento);
				printf(
						"\n  Deseja confirmar o pagamento?\n   0: Recusar\n   1: Confirmar. ");
				scanf("%d", &iPagamentoConfirmado);

				if (iPagamentoConfirmado == 0) {
					while (iPagamentoConfirmado == 0)
						printf(
								"\nPara concluir a saida do veiculo, precisa confirmar o pagamento.");
					printf(
							"\nDeseja confirmar o pagamento? 0: Recusar ou 1: Confirmar.");
					scanf("%s", &iPagamentoConfirmado);
				}

				printf("\n>>>>>> Pagamento Confirmado com Sucesso. <<<<<<\n");
				fflush(stdin); /* Limpa a entrada padrao do sistema (teclado) */
				printf(
						"\n  Tecle 'Enter' para gerar o recibo de saida de veiculo e de pagamento.\n");
				getchar();

				/* Registra o pagamento */
				strftime(cDataPagamento, 20, "%d/%m/%Y", localtime(&tNow));
				strftime(cHorarioPagamento, 10, "%H:%M:%S", localtime(&tNow));

				listaPagamentos[iCodigoPagamento].iCodigoPagamento =
						iCodigoPagamento;
				strcpy(listaPagamentos[iCodigoPagamento].cPlaca, cPlaca);
				strcpy(listaPagamentos[iCodigoPagamento].cDataPagamento,
						cDataPagamento);
				strcpy(listaPagamentos[iCodigoPagamento].cHorarioPagamento,
						cHorarioPagamento);
				listaPagamentos[iCodigoPagamento].fValorPagamento =
						fValorPagamento;
				printf("\n::: Saida de Veiculo realizada com Sucesso :::\n\n");

				/* Imprimir recibo de saida de veiculo */
				printf(
						"____________________________________________________\n");
				printf(
						"****** RECIBO DE SAIDA DE VEICULO E PAGAMENTO ******\n");
				printf("   Saida Numero:     %d\n", iCodigoSaida);
				printf("   Pagamento Numero: %d\n", iCodigoPagamento);
				printf("   Placa:            %s\n", cPlaca);
				printf("   Data:             %s\n", cDataSaida);
				printf("   Horario:          %s\n", cHorarioSaida);
				printf("   Valor Pagamento:  %.2lf\n", fValorPagamento);
				printf(
						"****************************************************\n");
				printf(
						"____________________________________________________\n");
				iNumeroVagasDisponiveis++;

				printf("\n\n::::: Numero de Vagas Disponiveis: %d :::::\n",
						iNumeroVagasDisponiveis);
			} else {
				printf(
						"::: Veiculo com placa %s nao teve entrada no estacionamento. :::",
						cPlaca);
			}

			printf("\n\nTecle 'Enter' para sair.");
			break;

		case 3: /* Pesquisa de Registro de Entrada */
			printf("::: Pesquisa de Registro de Entrada :::\n");
			printf("::: Informe a Placa do Veiculo que deseja pesquisar: ");
			scanf("%s", &cPlacaAux);

			if (strlen(cPlacaAux) != 8) {
				while (strlen(cPlacaAux) != 8) {
					printf(
							"    A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n",
							cPlacaAux);
					printf(
							"    Informe a Placa do Veiculo que deseja pesquisar: ");
					scanf("%s", &cPlacaAux);
					toupper(cPlacaAux); /* Altera os caracteres da placa para ficarem maiusculos */
				}
			}

			toupper(cPlacaAux);
			fflush(stdin);

			for (iContador = 1; iContador <= iCodigoEntrada; iContador++) {
				if ((cCharAux = strchr(cPlacaAux, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(listaEntradas[iContador].cPlaca, '\n'))
						!= NULL)
					*cCharAux = '\0';

				if (strcasecmp(listaEntradas[iContador].cPlaca, cPlacaAux)
						== 0) {
					iCodigoEntrada = iContador;
					strcpy(cPlaca, listaEntradas[iCodigoEntrada].cPlaca);
					strcpy(cDataEntrada,
							listaEntradas[iCodigoEntrada].cDataEntrada);
					strcpy(cHorarioEntrada,
							listaEntradas[iCodigoEntrada].cHorarioEntrada);

					if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cDataEntrada, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cHorarioEntrada, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n\n::: Veiculo Encontrado com Sucesso :::\n");
					printf("\n:: Entrada Numero: %d\n", iCodigoEntrada);
					printf("  Placa:             %s\n", cPlaca);
					printf("  Data:              %s\n", cDataEntrada);
					printf("  Horario:           %s\n", cHorarioEntrada);
					iVeiculoEncontrado = 1;
				} else {
					iVeiculoEncontrado = 0;
				}
			}

			if (iVeiculoEncontrado == 0) {
				printf("\n\n::: Veiculo nao Encontrado. :::\n");
			}

			printf("\n\nTecle 'Enter' para sair.");
			break;

		case 4: /* Pesquisa de Registro de Saida */
			printf("::: Pesquisa de Registro de Saida :::\n");
			printf("::: Informe a Placa do Veiculo que deseja pesquisar: ");
			scanf("%s", &cPlacaAux);

			if (strlen(cPlacaAux) != 8) {
				while (strlen(cPlacaAux) != 8) {
					printf(
							"    A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n",
							cPlacaAux);
					printf(
							"    Informe a Placa do Veiculo que deseja pesquisar: ");
					scanf("%s", &cPlacaAux);
					toupper(cPlacaAux); /* Altera os caracteres da placa para ficarem maiusculos */
				}
			}

			toupper(cPlacaAux);
			fflush(stdin);

			for (iContador = 1; iContador <= iCodigoSaida; iContador++) {
				if ((cCharAux = strchr(cPlacaAux, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(listaSaidas[iContador].cPlaca, '\n'))
						!= NULL)
					*cCharAux = '\0';

				if (strcasecmp(listaSaidas[iContador].cPlaca, cPlacaAux) == 0) {
					iCodigoSaida = iContador;
					strcpy(cPlaca, listaSaidas[iCodigoSaida].cPlaca);
					strcpy(cDataEntrada, listaSaidas[iCodigoSaida].cDataSaida);
					strcpy(cHorarioEntrada,
							listaSaidas[iCodigoSaida].cHorarioSaida);

					if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cDataSaida, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cHorarioSaida, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n\n::: Veiculo Encontrado com Sucesso :::\n");
					printf("\n:: Saida Numero: %d\n", iCodigoSaida);
					printf("  Placa:           %s\n", cPlaca);
					printf("  Data:            %s\n", cDataSaida);
					printf("  Horario:         %s\n", cHorarioSaida);
					iVeiculoEncontrado = 1;
				} else {
					iVeiculoEncontrado = 0;
				}
			}

			if (iVeiculoEncontrado == 0) {
				printf("\n\n::: Veiculo nao Encontrado. :::\n");
			}

			printf("\n\nTecle 'Enter' para sair.");
			break;

		case 5: /* Pesquisa de Registro de Pagamento */
			printf("::: Pesquisa de Registro de Pagamento :::\n");
			printf("::: Informe a Placa do Veiculo que deseja pesquisar: ");
			scanf("%s", &cPlacaAux);

			if (strlen(cPlacaAux) != 8) {
				while (strlen(cPlacaAux) != 8) {
					printf(
							"    A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n",
							cPlacaAux);
					printf(
							"    Informe a Placa do Veiculo que deseja pesquisar: ");
					scanf("%s", &cPlacaAux);
					toupper(cPlacaAux); /* Altera os caracteres da placa para ficarem maiusculos */
				}
			}

			toupper(cPlacaAux);
			fflush(stdin);

			for (iContador = 1; iContador <= iCodigoPagamento; iContador++) {
				if ((cCharAux = strchr(cPlacaAux, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(listaPagamentos[iContador].cPlaca, '\n'))
						!= NULL)
					*cCharAux = '\0';

				if (strcasecmp(listaPagamentos[iContador].cPlaca, cPlacaAux)
						== 0) {
					iCodigoPagamento = iContador;
					strcpy(cPlaca, listaPagamentos[iCodigoPagamento].cPlaca);
					strcpy(cDataEntrada,
							listaPagamentos[iCodigoPagamento].cDataPagamento);
					strcpy(cHorarioEntrada,
							listaPagamentos[iCodigoPagamento].cHorarioPagamento);
					fValorPagamento =
							listaPagamentos[iCodigoPagamento].fValorPagamento;

					if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cDataSaida, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cHorarioSaida, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n\n::: Pagamento Encontrado com Sucesso :::\n");
					printf("\n:: Pagamento Numero: %d\n", iCodigoPagamento);
					printf("  Placa:               %s\n", cPlaca);
					printf("  Data:                %s\n", cDataPagamento);
					printf("  Horario:             %s\n", cHorarioPagamento);
					printf("  Valor Pagamento:     %f\n", fValorPagamento);
					iVeiculoEncontrado = 1;
				} else {
					iVeiculoEncontrado = 0;
				}
			}

			if (iVeiculoEncontrado == 0) {
				printf("\n\n::: Veiculo nao Encontrado. :::\n");
			}

			printf("\n\nTecle 'Enter' para sair.");
			break;

		case 6: /* Relatorio de todas as entradas */
			printf(":::: Relatorio de todas as entradas ::::\n");

			for (iContador = 1; iContador <= iCodigoEntrada; iContador++) {
				if (listaEntradas[iContador].iCodigoEntrada > 0) {
					if ((cCharAux = strchr(listaEntradas[iContador].cPlaca,
							'\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(
							listaEntradas[iContador].cDataEntrada, '\n'))
							!= NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(
							listaEntradas[iContador].cHorarioEntrada, '\n'))
							!= NULL)
						*cCharAux = '\0';

					printf("\n:: Entrada Numero: %d\n",
							listaEntradas[iContador].iCodigoEntrada);
					printf("   Placa:          %s\n",
							listaEntradas[iContador].cPlaca);
					printf("   Data:           %s\n",
							listaEntradas[iContador].cDataEntrada);
					printf("   Horario:        %s\n\n",
							listaEntradas[iContador].cHorarioEntrada);
				}
			}
			printf(":::: Fim do Relatorio ::::");

			printf("\n\nTecle 'Enter' para sair.");
			break;

		case 7: /* Relatorio de todas as saidas */
			printf(":::: Relatorio de todas as saidas ::::\n");

			for (iContador = 1; iContador <= iCodigoSaida; iContador++) {
				if (listaSaidas[iContador].iCodigoSaida > 0) {
					if ((cCharAux = strchr(listaSaidas[iContador].cPlaca, '\n'))
							!= NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaSaidas[iContador].cDataSaida,
							'\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaSaidas[iContador].cHorarioSaida,
							'\n')) != NULL)
						*cCharAux = '\0';

					printf("\n:: Saida Numero: %d\n",
							listaSaidas[iContador].iCodigoSaida);
					printf("   Placa:        %s\n",
							listaSaidas[iContador].cPlaca);
					printf("   Data :        %s\n",
							listaSaidas[iContador].cDataSaida);
					printf("   Horario:      %s\n\n",
							listaSaidas[iContador].cHorarioSaida);
				}
			}
			printf(":::: Fim do Relatorio ::::");

			printf("\n\nTecle 'Enter' para sair.");
			break;

		case 8: /* Relatorio de todos os pagamentos */
			printf(":::: Relatorio de todos os pagamentos ::::\n");

			for (iContador = 1; iContador <= iCodigoPagamento; iContador++) {
				if (listaPagamentos[iContador].iCodigoPagamento > 0) {
					if ((cCharAux = strchr(listaPagamentos[iContador].cPlaca,
							'\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(
							listaPagamentos[iContador].cDataPagamento, '\n'))
							!= NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(
							listaPagamentos[iContador].cHorarioPagamento, '\n'))
							!= NULL)
						*cCharAux = '\0';

					printf("\n:: Pagamento Numero: %d\n",
							listaPagamentos[iContador].iCodigoPagamento);
					printf("   Placa:            %s\n",
							listaPagamentos[iContador].cPlaca);
					printf("   Data:             %s\n",
							listaPagamentos[iContador].cDataPagamento);
					printf("   Horario:          %s\n",
							listaPagamentos[iContador].cHorarioPagamento);
					printf("   Pagamento:        %.2lf\n\n",
							listaPagamentos[iContador].fValorPagamento);
				}
			}
			printf(":::: Fim do Relatorio ::::");

			printf("\n\nTecle 'Enter' para sair.");
			break;

		case 9: /* Fechamento */
			printf(":::: Fechamento do Caixa ::::\n");
			printf("\n  Processo ainda em fase de construcao.");
			printf("\n\nTecle 'Enter' para sair.");
			break;

		case 0: /* Salva todos os dados que foram gravados nas structs e sai do sistema
		 /********** ----------- Inicio da rotina de salvamento dos dados das structs -----------**********/
			/* Cria��o ou abertura do arquivo de texto */
			/* Cria o nome do arquivo de saidas */
			sprintf(cNomeArquivo, "%s%s%s", "entradas_", cDataAbertura,
					cExtensaoArquivo);

			if ((fEntradas = fopen(cNomeArquivo, "a+")) == NULL) {
				perror("Descricao do erro.");
				exit(1);
			}

			/* Salvar Entradas de Veiculos */
			if (iCodigoEntrada > iUltimoCodigoEntrada) {
				if (iUltimoCodigoEntrada == 0)
					iContadorAux = 1;
				else
					iContadorAux = iUltimoCodigoEntrada + 1;

				iCodigoEntradaAux = iCodigoEntrada;

				if (iCodigoEntradaAux > 0) {
					for (iContador = iContadorAux;
							iContador <= iCodigoEntradaAux; iContador++) {
						iCodigoEntrada = iContador;

						if ((cCharAux = strchr(
								listaEntradas[iCodigoEntrada].cPlaca, '\n'))
								!= NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(
								listaEntradas[iCodigoEntrada].cDataEntrada,
								'\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(
								listaEntradas[iCodigoEntrada].cHorarioEntrada,
								'\n')) != NULL)
							*cCharAux = '\0';

						fprintf(fEntradas, "\n\nEntrada de Veiculo\n");
						fprintf(fEntradas, "%d\n",
								listaEntradas[iCodigoEntrada].iCodigoEntrada);
						fprintf(fEntradas, "%s\n",
								listaEntradas[iCodigoEntrada].cPlaca);
						fprintf(fEntradas, "%s\n",
								listaEntradas[iCodigoEntrada].cDataEntrada);
						fprintf(fEntradas, "%s\n",
								listaEntradas[iCodigoEntrada].cHorarioEntrada);
						fprintf(fEntradas, "%d",
								listaEntradas[iCodigoEntrada].dDataHorarioEntrada);
					}
				}
			}
			sprintf(cNomeArquivo, "%s%s%s", "saidas_", cDataAbertura,
					cExtensaoArquivo);

			if ((fSaidas = fopen(cNomeArquivo, "a+")) == NULL) {
				perror("Descricao do erro.");
				exit(1);
			}

			/* Salvar Saidas de Veiculos */
			if (iCodigoSaida > iUltimoCodigoSaida) {
				if (iUltimoCodigoSaida == 0)
					iContadorAux = 1;
				else
					iContadorAux = iUltimoCodigoSaida + 1;

				iCodigoSaidaAux = iCodigoSaida;

				if (iCodigoSaidaAux > 0) {
					for (iContador = iContadorAux; iContador <= iCodigoSaidaAux;
							iContador++) {
						iCodigoSaida = iContador;

						if ((cCharAux = strchr(listaSaidas[iCodigoSaida].cPlaca,
								'\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(
								listaSaidas[iCodigoSaida].cDataSaida, '\n'))
								!= NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(
								listaSaidas[iCodigoSaida].cHorarioSaida, '\n'))
								!= NULL)
							*cCharAux = '\0';

						fprintf(fSaidas, "\n\nSaida de Veiculo\n");
						fprintf(fSaidas, "%d\n",
								listaSaidas[iCodigoSaida].iCodigoSaida);
						fprintf(fSaidas, "%s\n",
								listaSaidas[iCodigoSaida].cPlaca);
						fprintf(fSaidas, "%s\n",
								listaSaidas[iCodigoSaida].cDataSaida);
						fprintf(fSaidas, "%s\n",
								listaSaidas[iCodigoSaida].cHorarioSaida);
						fprintf(fSaidas, "%d",
								listaSaidas[iCodigoSaida].dDataHorarioSaida);
					}
				}
			}
			sprintf(cNomeArquivo, "%s%s%s", "pagamentos_", cDataAbertura,
					cExtensaoArquivo);

			if ((fPagamentos = fopen(cNomeArquivo, "a+")) == NULL) {
				perror("Descricao do erro.");
				exit(1);
			}

			/* Salvar Pagamentos */
			if (iCodigoPagamento > iUltimoCodigoPagamento) {
				if (iUltimoCodigoPagamento == 0)
					iContadorAux = 1;
				else
					iContadorAux = iUltimoCodigoPagamento + 1;

				iCodigoPagamentoAux = iCodigoPagamento;

				if (iCodigoPagamentoAux > 0) {
					for (iContador = iContadorAux;
							iContador <= iCodigoPagamentoAux; iContador++) {
						iCodigoPagamento = iContador;

						if ((cCharAux = strchr(
								listaPagamentos[iCodigoPagamento].cPlaca, '\n'))
								!= NULL)
							*cCharAux = '\0';

						if ((cCharAux =
								strchr(
										listaPagamentos[iCodigoPagamento].cDataPagamento,
										'\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux =
								strchr(
										listaPagamentos[iCodigoPagamento].cHorarioPagamento,
										'\n')) != NULL)
							*cCharAux = '\0';

						fprintf(fPagamentos, "\n\nPagamento\n");
						fprintf(fPagamentos, "%d\n",
								listaPagamentos[iCodigoPagamento].iCodigoPagamento);
						fprintf(fPagamentos, "%s\n",
								listaPagamentos[iCodigoPagamento].cPlaca);
						fprintf(fPagamentos, "%s\n",
								listaPagamentos[iCodigoPagamento].cDataPagamento);
						fprintf(fPagamentos, "%s\n",
								listaPagamentos[iCodigoPagamento].cHorarioPagamento);
						fprintf(fPagamentos, "%f",
								listaPagamentos[iCodigoPagamento].fValorPagamento);
					}
				}
			}

			fclose(fEntradas);
			fclose(fSaidas);
			fclose(fPagamentos);
			/********** ----------- Fim da rotina de salvamento dos dados das structs -----------**********/
			return 0;

		default: /* Opcao Invalida */
			puts("Opcao Digitada Invalida");
			printf("\n\nTecle 'Enter' para sair.");
		} /* Fecha o laco switch */
		pausarMenuPrincipal();
		limparTela();
	} /* Fecha o laco while */
	/* ----------- Fim do metodo principal ----------- */
} /* Fecha o metodo main */
