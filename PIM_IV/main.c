/* Declaracao de bibliotecas nativas da linguagem C */
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

/* Constante definida para ser usada na rotina de carregamento das structs*/
#define MAX 40

/* Criacao de struct para armazenar dados de Entradas e Saidas de Veiculos e de Pagamentos */
struct EntradaVeiculo {
	int    iCodEntrada;
	char   cPlaca[9];
	char   cDataEntrada[12];
	char   cHorarioEntrada[8];
	time_t tDataHorarioEntrada;
	double dDataHorarioEntrada;
};

struct SaidaVeiculo {
	int    iCodSaida;
	char   cPlaca[9];
	char   cDataSaida[12];
	char   cHorarioSaida[8];
	time_t tDataHorarioSaida;
	double dDataHorarioSaida;
};

struct Pagamento {
	int   iCodPagamento;
	char  cPlaca[9];
	char  cDataPagamento[12];
	char  cHorarioPagamento[8];
	float fValorPagamento;
};

void limparTela() {
#ifdef linux || LINUX || Linux || UNIX
	printf("\e[H\e[2J"); /* Limpa a tela do sistema - Linux*/
#elif defined WIN32
	system("cls"); /* Limpa a tela do sistema - Windows*/
#endif;
}

void pausarMenuPrincipal() {
#ifdef linux || LINUX || Linux || UNIX
	fflush(stdin);
	getchar();
#elif defined WIN32
	system("pause>>null");
#endif;
}

int verTamanhoArquivoTexto(const char* file_name) {
	FILE *file = fopen(file_name, "r");

	if (file == NULL)
		return 0;

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fclose(file);

	return size;
}

/* Metodo......: main
 * Parametros..: Sem parametros
 * Descricao...: Metodo principal do aplicativo.
 * Data Criacao: 18/05/2015
 * Retorno.....: int
 */
int main() {
	/* ----------- Inicio do metodo principal ----------- */

	/* Criacao de nomes para as structs de Entradas e Saidas de Veiculos e Pagamentos */
	struct EntradaVeiculo listaEntradas[3000];
	struct SaidaVeiculo   listaSaidas[3000];
	struct Pagamento      listaPagamentos[3000];

	/* Variaveis para armazenar os valores de placas e a linha do arquivo do carregamento das structs */
	char cPlaca[9]           = " ",
		 cPlacaAux[9]        = " ",
		 cLinhaArquivo[MAX]  = " ",
		 cDataAbertura[12]   = " ",
		 cNomeArquivo[512]   = " ",
		 cExtensaoArquivo[5] = ".txt";

	char *cResultLinhaArquivo;
	char *cCharAux;

	/* Declaraco de Variaveis do sistema */
	int i,
	    iAux,
		iOpcao,
		iOpcaoEscolhida,
		iVeiculoEncontrado,
		iTotalVagasEstacionamento,
		iTotalVagasDisponiveis,
		iUltimoCodEntrada,
		iUltimoCodSaida,
		iUltimoCodPagamento,
		iPagamentoConfirmado;

	/* Variaveis para armazenar o valor da primeira hora e das demais horas */
	float fValorPrimeiraHora,
	      fValorDemaisHoras,
		  fValorConfirmado;

	/* Variavel para trabalhar com a data e hora atual */
	time_t tNow;
	time_t tDataAbertura;

	/* Ponteiros para arquivos de entradas, saidas, pagamentos e tabela de tarifas */
	FILE* fEntradas;
	FILE* fSaidas;
	FILE* fPagamentos;
	FILE* fConfiguracoes;

	/* Declaraco de Variaveis das Entradas de Veiculos */
	int    iCodEntrada,
	       iCodEntradaAux;
	char   cDataEntrada[12]   = " ";
	char   cHorarioEntrada[8] = " ";
	double dDataHorarioEntrada;

	/* Declaraco de Variaveis das Saidas de Veiculos */
	int    iCodSaida,
	       iCodSaidaAux;
	char   cDataSaida[12]   = " ";
	char   cHorarioSaida[8] = " ";
	double dDataHorarioSaida;

	/* Declaracao de Variaveis dos Pagamentos */
	int    iCodPagamento,
	       iCodPagamentoAux;
	char   cDataPagamento[12]   = " ";
	char   cHorarioPagamento[8] = " ";
	float  fValorPagamento;
	double dDiferencaSegundos;

	/* Inicializacao e limpeza de variaveis */
	iCodEntrada               = 0;
	iCodEntradaAux            = 0;
	iCodSaida                 = 0;
	iCodSaidaAux              = 0;
	iCodPagamento             = 0;
	iCodPagamentoAux          = 0;
	i                         = 0;
	iAux                      = 0;
	iOpcaoEscolhida           = 0;
	iVeiculoEncontrado        = 0;
	iTotalVagasEstacionamento = 0;
	iTotalVagasDisponiveis    = 0;
	iUltimoCodEntrada         = 0;
	iUltimoCodSaida           = 0;
	iUltimoCodPagamento       = 0;
	iPagamentoConfirmado      = 0;
	fValorPagamento           = 0;
	fValorConfirmado          = 0;
	dDataHorarioEntrada       = 0;
	dDataHorarioEntrada       = 0;

	limparTela();

	/********* ----------- Inicio da rotina de carregamento das structs ----------- *********/
	tDataAbertura = time(NULL); /* Variavel de data da abertura do sistema */
	strftime(cDataAbertura, 20, "%d%m%Y", localtime(&tDataAbertura));

	/* Cria o nome do arquivo de entradas */
	sprintf(cNomeArquivo, "%s%s%s", "entradas_", cDataAbertura, cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fEntradas = fopen(cNomeArquivo, "rt");

	if (fEntradas == NULL) /* Se houve erro na abertura */
	{
		/* Cria um arquivo de texto caso nao encontrar nenhum arquivo no diretorio do sistema */
		if ((fEntradas = fopen(cNomeArquivo, "a+")) == NULL) {
			perror("Erro ao abrir arquivo de entradas de veiculos.");
			exit(1);
		}
	}

	/* Inicializacao das variaveis */
	i      = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));
	memset(&cResultLinhaArquivo, 0, sizeof(cResultLinhaArquivo));

	while (!feof(fEntradas)) {
		cResultLinhaArquivo = fgets(cLinhaArquivo, MAX, fEntradas); /* Le uma linha (inclusive com o '\n') */

		if (cResultLinhaArquivo) /* Se foi possivel ler */
		{
			if (strcmp(cLinhaArquivo, "Entrada de Veiculo\n") == 0)
				iOpcao = 0;

			if (iOpcao > -1)
				cResultLinhaArquivo = fgets(cLinhaArquivo, MAX, fEntradas);

			if (iOpcao == 0) {
				iCodEntrada = atoi(cLinhaArquivo);
				iUltimoCodEntrada = iCodEntrada;
				listaEntradas[iCodEntrada].iCodEntrada = iCodEntrada;
				fgets(listaEntradas[iCodEntrada].cPlaca, MAX, fEntradas);
				fgets(listaEntradas[iCodEntrada].cDataEntrada, MAX, fEntradas);
				fgets(listaEntradas[iCodEntrada].cHorarioEntrada, MAX, fEntradas);
				fgets(cLinhaArquivo, MAX, fEntradas);
				dDataHorarioEntrada = strtod(cLinhaArquivo, NULL);
				listaEntradas[iCodEntrada].dDataHorarioEntrada = dDataHorarioEntrada;
				listaEntradas[iCodEntrada].tDataHorarioEntrada = dDataHorarioEntrada;
				break;
			}
		}
		iOpcao = -1;
		i++;
	}

	/* Cria o nome do arquivo de saidas */
	sprintf(cNomeArquivo, "%s%s%s", "saidas_", cDataAbertura, cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fSaidas = fopen(cNomeArquivo, "rt");

	if (fSaidas == NULL) /* Se houve erro na abertura */
	{
		/* Cria um arquivo de texto caso nao encontrar nenhum arquivo no diretorio do sistema */
		if ((fSaidas = fopen(cNomeArquivo, "a+")) == NULL) {
			perror("Erro ao abrir arquivo de saidas de veiculos.");
			exit(1);
		}
	}

	/* Inicializacao das variaveis */
	i      = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));
	memset(&cResultLinhaArquivo, 0, sizeof(cResultLinhaArquivo));

	while (!feof(fSaidas)) {
		cResultLinhaArquivo = fgets(cLinhaArquivo, MAX, fSaidas); /* Le uma linha (inclusive com o '\n') */

		if (cResultLinhaArquivo) /* Se foi possivel ler */
		{
			if (strcmp(cLinhaArquivo, "Saida de Veiculo\n") == 0)
				iOpcao = 0;

			if (iOpcao > -1)
				cResultLinhaArquivo = fgets(cLinhaArquivo, MAX, fSaidas);

			if (iOpcao == 0) {
				iCodSaida = atoi(cLinhaArquivo);
				iUltimoCodSaida = iCodSaida;
				listaSaidas[iCodSaida].iCodSaida = iCodSaida;
				fgets(listaSaidas[iCodSaida].cPlaca, MAX, fSaidas);
				fgets(listaSaidas[iCodSaida].cDataSaida, MAX, fSaidas);
				fgets(listaSaidas[iCodSaida].cHorarioSaida, MAX, fSaidas);
				fgets(cLinhaArquivo, MAX, fSaidas);
				dDataHorarioSaida = strtod(cLinhaArquivo, NULL);
				listaSaidas[iCodSaida].dDataHorarioSaida = dDataHorarioSaida;
				listaSaidas[iCodSaida].tDataHorarioSaida = dDataHorarioEntrada;
				break;
			}
		}
		iOpcao = -1;
		i++;
	}

	/* Cria o nome do arquivo de pagamentos */
	sprintf(cNomeArquivo, "%s%s%s", "pagamentos_", cDataAbertura, cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fPagamentos = fopen(cNomeArquivo, "rt");

	if (fPagamentos == NULL) /* Se houve erro na abertura */
	{
		/* Cria um arquivo de texto caso nao encontrar nenhum arquivo no diretorio do sistema */
		if ((fPagamentos = fopen(cNomeArquivo, "a+")) == NULL) {
			perror("Erro ao abrir arquivo de pagamentos.");
			exit(1);
		}
	}

	/* Inicializacao das variaveis */
	i      = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));
	memset(&cResultLinhaArquivo, 0, sizeof(cResultLinhaArquivo));

	while (!feof(fPagamentos)) {
		cResultLinhaArquivo = fgets(cLinhaArquivo, MAX, fPagamentos); /* Le uma linha (inclusive com o '\n') */

		if (cResultLinhaArquivo) /* Se foi possivel ler */
		{
			if (strcmp(cLinhaArquivo, "Pagamento\n") == 0)
				iOpcao = 0;

			if (iOpcao > -1)
				cResultLinhaArquivo = fgets(cLinhaArquivo, MAX, fPagamentos);

			if (iOpcao == 0) {
				iCodPagamento = atoi(cLinhaArquivo);
				iUltimoCodPagamento = iCodPagamento;
				listaPagamentos[iCodPagamento].iCodPagamento = iCodPagamento;
				fgets(listaPagamentos[iCodPagamento].cPlaca, MAX, fPagamentos);
				fgets(listaPagamentos[iCodPagamento].cDataPagamento, MAX, fPagamentos);
				fgets(listaPagamentos[iCodPagamento].cHorarioPagamento, MAX, fPagamentos);
				fgets(cLinhaArquivo, MAX, fPagamentos);
				fValorPagamento = strtod(cLinhaArquivo, NULL);
				listaPagamentos[iCodPagamento].fValorPagamento = fValorPagamento;
				break;
			}
		}
		iOpcao = -1;
		i++;
	}

	fclose(fEntradas);
	fclose(fSaidas);
	fclose(fPagamentos);
	/********* ----------- Fim da da rotina de carregamento das structs ----------- *********/

	limparTela();

	/* Carregamento de variaveis de valores da primeira hora, das demais horas, total de vagas e total de vagas livres. */
	/* Cria o nome do arquivo de configuracoes */
	sprintf(cNomeArquivo, "%s%s", "configuracoes", cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fConfiguracoes = fopen(cNomeArquivo, "rt");

	if (fConfiguracoes == NULL) /* Se houve erro na abertura */
	{
		/* Cria um arquivo de texto caso nao encontrar nenhum arquivo no diretorio do sistema */
		if ((fConfiguracoes = fopen(cNomeArquivo, "a+")) == NULL) {
			perror("Erro ao abrir arquivo de configuracoes.");
			exit(1);
		}
	}

	/* Inicializacao das variaveis */
	i      = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));

	while (i <= 4) {
		fgets(cLinhaArquivo, MAX, fConfiguracoes);

		switch (i) {
		case 1:
			fValorPrimeiraHora = strtod(cLinhaArquivo, NULL);
			break;

		case 2:
			fValorDemaisHoras = strtod(cLinhaArquivo, NULL);
			break;

		case 3:
			iTotalVagasEstacionamento = strtod(cLinhaArquivo, NULL);
			break;

		case 4:
			iTotalVagasDisponiveis = strtod(cLinhaArquivo, NULL);
			break;
		}
		i++;
	}

	fclose(fConfiguracoes);
	/* Fim leitura arquivos */

	if (iTotalVagasEstacionamento == 0) {
		printf(" __________________________________________________________________________\n");
		printf(" ::::::::::::::::  SISTEMA DE ESTACIONAMENTO DE VEICULOS  :::::::::::::::::\n\n");
		printf(" ::            PARA COMECAR A USAR ESTE SISTEMA, E NECESSARIO            ::\n");
		printf(" ::              DEFINIR ALGUMAS CONFIGURACOES IMPORTANTES.              ::\n");
		printf(" ::           VAMOS DEFINIR AGORA OS VALORES DA PRIMEIRA HORA,           ::\n");
		printf(" ::          DAS DEMAIS HORAS, TOTAL DE VAGAS DO ESTACIONAMENTO          ::\n");
		printf(" ::                  E O TOTAL DE VAGAS DISPONIVEIS.                     ::\n");
		printf("\n\n ::        Todos os dados informados somente serao armazenados           ::");
		printf("\n ::                no arquivo de texto ao sair do sistema.               ::");
		printf("\n\n :: Observacao: Os valores em decimais devem ser informados com pontos.  ::");
		printf("\n ::             Caso for informado com virgula entao ocorrera erro.      ::");
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

		printf("\n :: Informe o no. total de vagas do estacionamento: ");
		scanf("%d", &iTotalVagasEstacionamento);

		if (iTotalVagasEstacionamento == 0) {
			while (iTotalVagasEstacionamento == 0) {
				printf("\n :: O no. total de vagas do estacionamento deve ser maior que 0.");
				printf("\n :: Informe o no. total de vagas do estacionamento: ");
				scanf("%d", &iTotalVagasEstacionamento);
			}
		}

		printf("\n :: E, por ultimo, informe o no. total de vagas disponiveis: ");
		scanf("%d", &iTotalVagasDisponiveis);

		if (iTotalVagasDisponiveis == 0) {
			while (iTotalVagasDisponiveis == 0) {
				printf("\n :: O no. total de vagas disponiveis deve ser maior que 0.");
				printf("\n :: Informe o no. total de vagas disponiveis: ");
				scanf("%d", &iTotalVagasDisponiveis);
			}
		}
		printf(" __________________________________________________________________________\n");
		fflush(stdin); /* Limpa a entrada padrao do sistema (teclado) */
		printf("\n\n Tecle 'Enter' para entrar no menu do sistema.\n");
		getchar();
	}

	limparTela();

	/* Laco do Menu Principal */
	while (1) {
		printf(" __________________________________________________________________________\n");
		printf(" ::::::::::::::::  SISTEMA DE ESTACIONAMENTO DE VEICULOS  :::::::::::::::::\n\n");
		printf("   == INFORMACOES UTEIS ==\n");
		printf("   Valor da Primeira Hora:         R$ %.2lf\n", fValorPrimeiraHora);
		printf("   Valor das Demais Horas:         R$ %.2lf\n", fValorDemaisHoras);
		printf("   No. Total de Vagas:             %d\n", iTotalVagasEstacionamento);
		printf("   No. Total de Vagas Disponiveis: %d\n\n\n", iTotalVagasDisponiveis);
		printf(" << Menu do Sistema >> \n");
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
		printf(" __________________________________________________________________________\n");
		printf("\n Escolha uma das opcoes acima: ");
		scanf("%d", &iOpcaoEscolhida);
		getchar();
		limparTela();

		switch (iOpcaoEscolhida) /* Laco das opcoes do menu principal */
		{

		case 1: /* Entrada de Veiculo */
			if (iTotalVagasDisponiveis == 0)
				printf(" Nao ha vagas disponiveis no estacionamento.");
			else {
				printf(" ::: ENTRADA DE VEICULO :::\n");
				printf(" ::: AVISO: A placa deve ter 8 caracteres. Exemplo: AAA-1234. :::\n");
				printf("     Informe a Placa do Veiculo: ");
				scanf("%s", &cPlaca);

				/* Altera os caracteres da placa para ficarem maiusculos */
				i = 0;
				while (i < 10) {
					cPlaca[i] = toupper(cPlaca[i]);
					i++;
				}

				/* Validacao para procurar por veiculos ja cadastrados */
				for (i = 1; i <= iCodEntrada; i++) {
					if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaEntradas[i].cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if (strcmp(cPlaca, listaEntradas[i].cPlaca) == 0) {
						while (strcmp(cPlaca, listaEntradas[i].cPlaca) == 0) {
							printf("\n\n    O veiculo ja se encontra no estacionamento.");
							printf("\n    Por favor, informe outra placa: ");
							scanf("%s", &cPlaca);

							/* Altera os caracteres da placa para ficarem maiusculos */
							i = 0;
							while (i < 10) {
								cPlaca[i] = toupper(cPlaca[i]);
								i++;
							}

						}
					}
				}

				if (strlen(cPlaca) != 8) {
					while (strlen(cPlaca) != 8) {
						printf("    A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n", cPlaca);
						printf("    Informe a Placa do Veiculo: ");
						scanf("%s", &cPlaca);

						/* Altera os caracteres da placa para ficarem maiusculos */
						i = 0;
						while (i < 10) {
							cPlaca[i] = toupper(cPlaca[i]);
							i++;
						}
					}
				}

				iCodEntrada++; /* Incrementa o codigo de entrada a cada nova entrada de veiculos */
				tNow = time(NULL);
				strftime(cDataEntrada, 20, "%d/%m/%Y", localtime(&tNow));
				strftime(cHorarioEntrada, 10, "%H:%M:%S", localtime(&tNow));
				printf("\n ::: Entrada de Veiculo realizada com Sucesso :::\n\n");

				/* Imprimir recibo de entrada de veiculo */
				printf(" ________________________________________\n");
				printf(" ***** RECIBO DE ENTRADA DE VEICULO *****\n");
				printf("   ID Entrada: %d\n", iCodEntrada);
				printf("   Placa:      %s\n", cPlaca);
				printf("   Data:       %s\n", cDataEntrada);
				printf("   Horario:    %s\n", cHorarioEntrada);
				printf(" ****************************************\n");
				printf(" ________________________________________\n");

				iUltimoCodEntrada = iCodEntrada;
				listaEntradas[iCodEntrada].iCodEntrada = iCodEntrada;
				strcpy(listaEntradas[iCodEntrada].cPlaca, cPlaca);
				strcpy(listaEntradas[iCodEntrada].cDataEntrada, cDataEntrada);
				strcpy(listaEntradas[iCodEntrada].cHorarioEntrada, cHorarioEntrada);
				time(&listaEntradas[iCodEntrada].tDataHorarioEntrada);
				listaEntradas[iCodEntrada].dDataHorarioEntrada = listaEntradas[iCodEntrada].tDataHorarioEntrada;
				iTotalVagasDisponiveis--;

				printf("\n\n ::::: No. Total de Vagas Disponiveis: %d :::::\n", iTotalVagasDisponiveis);
			}

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 2: /* Saida de Veiculo */
			printf(" ::: SAIDA DE VEICULO :::\n");
			iCodSaida++; /* Incrementa o codigo de saida a cada nova saida de veiculo */
			iCodPagamento++; /* Incrementa o codigo de pagamento a cada nova saida de veiculo */
			printf(" ::: AVISO: A placa deve ter 8 caracteres. Exemplo: AAA-1234. :::\n");
			printf("     Informe a Placa do Veiculo: ");
			scanf("%s", &cPlaca);

			/* Altera os caracteres da placa para ficarem maiusculos */
			i = 0;
			while (i < 10) {
				cPlaca[i] = toupper(cPlaca[i]);
				i++;
			}

			/* Validacao da Placa que foi recebida na variavel cPlaca */
			if (strlen(cPlaca) != 8) {
				while (strlen(cPlaca) != 8) {
					printf(" A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n", cPlaca);
					printf("    Informe a Placa do Veiculo: ");
					scanf("%s", &cPlaca);

					/* Altera os caracteres da placa para ficarem maiusculos */
					i = 0;
					while (i < 10) {
						cPlaca[i] = toupper(cPlaca[i]);
						i++;
					}
				}
			}

			for (i = 1; i <= iCodEntrada; i++) {
				if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(listaEntradas[i].cPlaca, '\n')) != NULL)
					*cCharAux = '\0';

				/* Pesquisa por veiculo estacionado */
				if (strcasecmp(listaEntradas[i].cPlaca, cPlaca) == 0) {
					iVeiculoEncontrado = 1;
					iCodEntrada = i;
				} else
					iVeiculoEncontrado = 0;
			}

			/* Se encontrou entao ira realizar a saida do veiculo */
			if (iVeiculoEncontrado == 1) {
				tNow = time(NULL);
				strftime(cDataSaida, 20, "%d/%m/%Y", localtime(&tNow));
				strftime(cHorarioSaida, 10, "%H:%M:%S", localtime(&tNow));

				/* Registra a saida */
				iUltimoCodSaida = iCodSaida;
				listaSaidas[iCodSaida].iCodSaida = iCodSaida;
				strcpy(listaSaidas[iCodSaida].cPlaca, cPlaca);
				strcpy(listaSaidas[iCodSaida].cDataSaida, cDataSaida);
				strcpy(listaSaidas[iCodSaida].cHorarioSaida, cHorarioSaida);
				time(&listaSaidas[iCodSaida].tDataHorarioSaida);
				listaSaidas[iCodSaida].dDataHorarioSaida = listaSaidas[iCodSaida].tDataHorarioSaida;

				/* Calculo para gerar o valor a ser pago */
				/* Para obter o periodo de tempo que o veiculo ficou estacionado
				 e necessario usar a funcao difftime. O resultado dela e sempre em segundos. */
				dDiferencaSegundos = difftime(listaSaidas[iCodSaida].dDataHorarioSaida,
						                      listaEntradas[iCodEntrada].dDataHorarioEntrada);

				/* Nessa linha e realizada a conversao de segundos para horas. */
				dDiferencaSegundos = (dDiferencaSegundos / 3600);

				/* Limpo o valor confirmado com um valor 0 */
				fValorConfirmado = 0;

				if (dDiferencaSegundos < 1)
					fValorPagamento = (fValorPrimeiraHora);
				else
					fValorPagamento = (fValorPrimeiraHora +
							           ((dDiferencaSegundos - 1) * fValorDemaisHoras));
				/* Fim do calculo */

				printf("\n >>>> O Valor a Ser Confirmado para Pagamento: R$ %.2lf<<<<\n", fValorPagamento);
				printf("\n Deseja confirmar o pagamento?\n   0: Recusar\n   1: Confirmar. ");
				scanf("%d", &iPagamentoConfirmado);

				if (iPagamentoConfirmado == 0) {
					while (iPagamentoConfirmado == 0)
						printf("\n Para concluir a saida do veiculo, precisa confirmar o pagamento.");
					printf("\n Deseja confirmar o pagamento? 0: Recusar ou 1: Confirmar.");
					scanf("%s", &iPagamentoConfirmado);
				}

				printf("\n >>>>>> Pagamento Confirmado com Sucesso. <<<<<<\n");
				fflush(stdin); /* Limpa a entrada padrao do sistema (teclado) */
				printf("\n Tecle 'Enter' para gerar o recibo de saida de veiculo e de pagamento.\n");
				getchar();

				/* Registra o pagamento */
				strftime(cDataPagamento, 20, "%d/%m/%Y", localtime(&tNow));
				strftime(cHorarioPagamento, 10, "%H:%M:%S", localtime(&tNow));

				iUltimoCodPagamento = iCodPagamento;
				listaPagamentos[iCodPagamento].iCodPagamento = iCodPagamento;
				strcpy(listaPagamentos[iCodPagamento].cPlaca, cPlaca);
				strcpy(listaPagamentos[iCodPagamento].cDataPagamento, cDataPagamento);
				strcpy(listaPagamentos[iCodPagamento].cHorarioPagamento, cHorarioPagamento);
				listaPagamentos[iCodPagamento].fValorPagamento = fValorPagamento;
				printf("\n ::: Saida de Veiculo realizada com Sucesso :::\n\n");

				/* Imprimir recibo de saida de veiculo */
				printf(" ____________________________________________________\n");
				printf(" ****** RECIBO DE SAIDA DE VEICULO E PAGAMENTO ******\n");
				printf("   ID Saida:     %d\n", iCodSaida);
				printf("   ID Pagamento: %d\n", iCodPagamento);
				printf("   Placa:        %s\n", cPlaca);
				printf("   Data:         %s\n", cDataSaida);
				printf("   Horario:      %s\n", cHorarioSaida);
				printf("   Valor Pago:   %.2lf\n", fValorPagamento);
				printf(" ****************************************************\n");
				printf(" ____________________________________________________\n");
				iTotalVagasDisponiveis++;

				printf("\n\n ::::: No. de Vagas Disponiveis: %d :::::\n", iTotalVagasDisponiveis);
			} else {
				printf(" ::: Veiculo com placa %s nao teve entrada no estacionamento. :::", cPlaca);
			}

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 3: /* Pesquisa de Registro de Entrada */
			printf(" ::: Pesquisa de Registro de Entrada :::\n");
			printf(" ::: Informe a Placa do Veiculo que deseja pesquisar: ");
			scanf("%s", &cPlacaAux);

			if (strlen(cPlacaAux) != 8) {
				while (strlen(cPlacaAux) != 8) {
					printf("    A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n", cPlacaAux);
					printf("    Informe a Placa do Veiculo que deseja pesquisar: ");
					scanf("%s", &cPlacaAux);

					/* Altera os caracteres da placa para ficarem maiusculos */
					i = 0;
					while (i < 10) {
						cPlaca[i] = toupper(cPlaca[i]);
						i++;
					}
				}
			}

			/* Altera os caracteres da placa para ficarem maiusculos */
			i = 0;
			while (i < 10) {
				cPlacaAux[i] = toupper(cPlacaAux[i]);
				i++;
			}
			fflush(stdin);

			for (i = 1; i <= iCodEntrada; i++) {
				if ((cCharAux = strchr(cPlacaAux, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(listaEntradas[i].cPlaca, '\n')) != NULL)
					*cCharAux = '\0';

				if (strcasecmp(listaEntradas[i].cPlaca, cPlacaAux) == 0) {
					iCodEntrada = i;
					strcpy(cPlaca, listaEntradas[iCodEntrada].cPlaca);
					strcpy(cDataEntrada, listaEntradas[iCodEntrada].cDataEntrada);
					strcpy(cHorarioEntrada, listaEntradas[iCodEntrada].cHorarioEntrada);

					if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cDataEntrada, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cHorarioEntrada, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n\n ::: Veiculo Encontrado com Sucesso :::\n");
					printf("\n :: ID Entrada: %d\n", iCodEntrada);
					printf("    Placa:      %s\n", cPlaca);
					printf("    Data:       %s\n", cDataEntrada);
					printf("    Horario:    %s\n", cHorarioEntrada);
					iVeiculoEncontrado = 1;
				} else {
					iVeiculoEncontrado = 0;
				}
			}

			if (iVeiculoEncontrado == 0) {
				printf("\n\n ::: Veiculo nao Encontrado. :::\n");
			}

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 4: /* Pesquisa de Registro de Saida */
			printf(" ::: Pesquisa de Registro de Saida :::\n");
			printf(" ::: Informe a Placa do Veiculo que deseja pesquisar: ");
			scanf("%s", &cPlacaAux);

			if (strlen(cPlacaAux) != 8) {
				while (strlen(cPlacaAux) != 8) {
					printf("    A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n", cPlacaAux);
					printf("    Informe a Placa do Veiculo que deseja pesquisar: ");
					scanf("%s", &cPlacaAux);

					/* Altera os caracteres da placa para ficarem maiusculos */
					i = 0;
					while (i < 10) {
						cPlacaAux[i] = toupper(cPlacaAux[i]);
						i++;
					}
				}
			}

			/* Altera os caracteres da placa para ficarem maiusculos */
			i = 0;
			while (i < 10) {
				cPlacaAux[i] = toupper(cPlacaAux[i]);
				i++;
			}
			fflush(stdin);

			for (i = 1; i <= iCodSaida; i++) {
				if ((cCharAux = strchr(cPlacaAux, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(listaSaidas[i].cPlaca, '\n')) != NULL)
					*cCharAux = '\0';

				if (strcasecmp(listaSaidas[i].cPlaca, cPlacaAux) == 0) {
					iCodSaida = i;
					strcpy(cPlaca, listaSaidas[iCodSaida].cPlaca);
					strcpy(cDataEntrada, listaSaidas[iCodSaida].cDataSaida);
					strcpy(cHorarioEntrada, listaSaidas[iCodSaida].cHorarioSaida);

					if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cDataSaida, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cHorarioSaida, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n\n ::: Veiculo Encontrado com Sucesso :::\n");
					printf("\n :: ID Saida: %d\n", iCodSaida);
					printf("    Placa:    %s\n", cPlaca);
					printf("    Data:     %s\n", cDataSaida);
					printf("    Horario:  %s\n", cHorarioSaida);
					iVeiculoEncontrado = 1;
				} else {
					iVeiculoEncontrado = 0;
				}
			}

			if (iVeiculoEncontrado == 0) {
				printf("\n\n ::: Veiculo nao Encontrado. :::\n");
			}

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 5: /* Pesquisa de Registro de Pagamento */
			printf(" ::: Pesquisa de Registro de Pagamento :::\n");
			printf(" ::: Informe a Placa do Veiculo que deseja pesquisar: ");
			scanf("%s", &cPlacaAux);

			if (strlen(cPlacaAux) != 8) {
				while (strlen(cPlacaAux) != 8) {
					printf("    A placa %s deve ter 8 caracteres. Exemplo: AAA-1234.\n", cPlacaAux);
					printf("    Informe a Placa do Veiculo que deseja pesquisar: ");
					scanf("%s", &cPlacaAux);

					/* Altera os caracteres da placa para ficarem maiusculos */
					i = 0;
					while (i < 10) {
						cPlacaAux[i] = toupper(cPlacaAux[i]);
						i++;
					}
				}
			}

			/* Altera os caracteres da placa para ficarem maiusculos */
			i = 0;
			while (i < 10) {
				cPlacaAux[i] = toupper(cPlacaAux[i]);
				i++;
			}
			fflush(stdin);

			for (i = 1; i <= iCodPagamento; i++) {
				if ((cCharAux = strchr(cPlacaAux, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(listaPagamentos[i].cPlaca, '\n')) != NULL)
					*cCharAux = '\0';

				if (strcasecmp(listaPagamentos[i].cPlaca, cPlacaAux) == 0) {
					iCodPagamento = i;
					strcpy(cPlaca, listaPagamentos[iCodPagamento].cPlaca);
					strcpy(cDataEntrada, listaPagamentos[iCodPagamento].cDataPagamento);
					strcpy(cHorarioEntrada, listaPagamentos[iCodPagamento].cHorarioPagamento);
					fValorPagamento = listaPagamentos[iCodPagamento].fValorPagamento;

					if ((cCharAux = strchr(cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cDataSaida, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cHorarioSaida, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n\n ::: Pagamento Encontrado com Sucesso :::\n");
					printf("\n :: ID Pagamento:    %d\n", iCodPagamento);
					printf("    Placa:           %s\n", cPlaca);
					printf("    Data:            %s\n", cDataPagamento);
					printf("    Horario:         %s\n", cHorarioPagamento);
					printf("    Valor Pagamento: %f\n", fValorPagamento);
					iVeiculoEncontrado = 1;
				} else {
					iVeiculoEncontrado = 0;
				}
			}

			if (iVeiculoEncontrado == 0) {
				printf("\n\n ::: Veiculo nao Encontrado. :::\n");
			}

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 6: /* Relatorio de todas as entradas */
			printf(" :::: Relatorio de todas as entradas ::::\n");

			for (i = 1; i <= iCodEntrada; i++) {
				if (listaEntradas[i].iCodEntrada > 0) {
					if ((cCharAux = strchr(listaEntradas[i].cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaEntradas[i].cDataEntrada, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaEntradas[i].cHorarioEntrada, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n :: ID Entrada: %d\n", listaEntradas[i].iCodEntrada);
					printf("    Placa:      %s\n", listaEntradas[i].cPlaca);
					printf("    Data:       %s\n", listaEntradas[i].cDataEntrada);
					printf("    Horario:    %s\n\n", listaEntradas[i].cHorarioEntrada);
				}
			}
			printf(" :::: FIM DO RELATORIO ::::");

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 7: /* Relatorio de todas as saidas */
			printf(" :::: Relatorio de todas as saidas ::::\n");

			for (i = 1; i <= iCodSaida; i++) {
				if (listaSaidas[i].iCodSaida > 0) {
					if ((cCharAux = strchr(listaSaidas[i].cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaSaidas[i].cDataSaida, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaSaidas[i].cHorarioSaida, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n :: ID Saida: %d\n", listaSaidas[i].iCodSaida);
					printf("    Placa:    %s\n", listaSaidas[i].cPlaca);
					printf("    Data :    %s\n", listaSaidas[i].cDataSaida);
					printf("    Horario:  %s\n\n", listaSaidas[i].cHorarioSaida);
				}
			}
			printf(" :::: FIM DO RELATORIO ::::");

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 8: /* Relatorio de todos os pagamentos */
			printf(" :::: Relatorio de todos os pagamentos ::::\n");

			for (i = 1; i <= iCodPagamento; i++) {
				if (listaPagamentos[i].iCodPagamento > 0) {
					if ((cCharAux = strchr(listaPagamentos[i].cPlaca, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaPagamentos[i].cDataPagamento, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaPagamentos[i].cHorarioPagamento, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n :: ID Pagamento: %d\n", listaPagamentos[i].iCodPagamento);
					printf("    Placa:        %s\n", listaPagamentos[i].cPlaca);
					printf("    Data:         %s\n", listaPagamentos[i].cDataPagamento);
					printf("    Horario:      %s\n", listaPagamentos[i].cHorarioPagamento);
					printf("    Pagamento:    %.2lf\n\n", listaPagamentos[i].fValorPagamento);
				}
			}
			printf(" :::: FIM DO RELATORIO ::::");

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 9: /* Fechamento */
			printf(" :::: Fechamento do Caixa ::::\n");
			printf("\n  Processo ainda em fase de construcao.");
			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 0: /* Salva todos os dados que foram gravados nas structs e sai do sistema
		 /********** ----------- Inicio da rotina de salvamento dos dados das structs -----------**********/
			/* Criacao ou abertura do arquivo de texto */
			/* Cria o nome do arquivo de saidas */
			sprintf(cNomeArquivo, "%s%s%s", "entradas_", cDataAbertura, cExtensaoArquivo);

			if ((fEntradas = fopen(cNomeArquivo, "a+")) == NULL) {
				perror("Erro ao abrir arquivo de entradas de veiculos.");
				exit(1);
			}

			/* Salvar Entradas de Veiculos */
			if (iCodEntrada > iUltimoCodEntrada) {
				if (iUltimoCodEntrada == 0)
					iAux = 1;
				else
					iAux = iUltimoCodEntrada + 1;

				iCodEntradaAux = iCodEntrada;

				if (iCodEntradaAux > 0) {
					for (i = iAux; i <= iCodEntradaAux; i++) {
						iCodEntrada = i;

						if ((cCharAux = strchr(listaEntradas[iCodEntrada].cPlaca, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaEntradas[iCodEntrada].cDataEntrada, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaEntradas[iCodEntrada].cHorarioEntrada, '\n')) != NULL)
							*cCharAux = '\0';

						if (verTamanhoArquivoTexto(cNomeArquivo) == 0)
							fprintf(fEntradas, "Entrada de Veiculo\n");
						else
							fprintf(fEntradas, "\n\nEntrada de Veiculo\n");

						fprintf(fEntradas, "%d\n", listaEntradas[iCodEntrada].iCodEntrada);
						fprintf(fEntradas, "%s\n", listaEntradas[iCodEntrada].cPlaca);
						fprintf(fEntradas, "%s\n", listaEntradas[iCodEntrada].cDataEntrada);
						fprintf(fEntradas, "%s\n", listaEntradas[iCodEntrada].cHorarioEntrada);
						fprintf(fEntradas, "%d", listaEntradas[iCodEntrada].dDataHorarioEntrada);
					}
				}
			}
			sprintf(cNomeArquivo, "%s%s%s", "saidas_", cDataAbertura, cExtensaoArquivo);

			if ((fSaidas = fopen(cNomeArquivo, "a+")) == NULL) {
				perror("Erro ao abrir arquivo de saidas de veiculos.");
				exit(1);
			}

			/* Salvar Saidas de Veiculos */
			if (iCodSaida > iUltimoCodSaida) {
				if (iUltimoCodSaida == 0)
					iAux = 1;
				else
					iAux = iUltimoCodSaida + 1;

				iCodSaidaAux = iCodSaida;

				if (iCodSaidaAux > 0) {
					for (i = iAux; i <= iCodSaidaAux; i++) {
						iCodSaida = i;

						if ((cCharAux = strchr(listaSaidas[iCodSaida].cPlaca, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaSaidas[iCodSaida].cDataSaida, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaSaidas[iCodSaida].cHorarioSaida, '\n')) != NULL)
							*cCharAux = '\0';

						if (verTamanhoArquivoTexto(cNomeArquivo) == 0)
							fprintf(fSaidas, "Saida de Veiculo\n");
						else
							fprintf(fSaidas, "\n\nSaida de Veiculo\n");

						fprintf(fSaidas, "%d\n", listaSaidas[iCodSaida].iCodSaida);
						fprintf(fSaidas, "%s\n", listaSaidas[iCodSaida].cPlaca);
						fprintf(fSaidas, "%s\n", listaSaidas[iCodSaida].cDataSaida);
						fprintf(fSaidas, "%s\n", listaSaidas[iCodSaida].cHorarioSaida);
						fprintf(fSaidas, "%d", listaSaidas[iCodSaida].dDataHorarioSaida);
					}
				}
			}
			sprintf(cNomeArquivo, "%s%s%s", "pagamentos_", cDataAbertura, cExtensaoArquivo);

			if ((fPagamentos = fopen(cNomeArquivo, "a+")) == NULL) {
				perror("Erro ao abrir arquivo de pagamentos.");
				exit(1);
			}

			/* Salvar Pagamentos */
			if (iCodPagamento > iUltimoCodPagamento) {
				if (iUltimoCodPagamento == 0)
					iAux = 1;
				else
					iAux = iUltimoCodPagamento + 1;

				iCodPagamentoAux = iCodPagamento;

				if (iCodPagamentoAux > 0) {
					for (i = iAux; i <= iCodPagamentoAux; i++) {
						iCodPagamento = i;

						if ((cCharAux = strchr(listaPagamentos[iCodPagamento].cPlaca, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaPagamentos[iCodPagamento].cDataPagamento, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaPagamentos[iCodPagamento].cHorarioPagamento, '\n')) != NULL)
							*cCharAux = '\0';

						if (verTamanhoArquivoTexto(cNomeArquivo) == 0)
							fprintf(fPagamentos, "Pagamento\n");
						else
							fprintf(fPagamentos, "\n\nPagamento\n");

						fprintf(fPagamentos, "%d\n", listaPagamentos[iCodPagamento].iCodPagamento);
						fprintf(fPagamentos, "%s\n", listaPagamentos[iCodPagamento].cPlaca);
						fprintf(fPagamentos, "%s\n", listaPagamentos[iCodPagamento].cDataPagamento);
						fprintf(fPagamentos, "%s\n", listaPagamentos[iCodPagamento].cHorarioPagamento);
						fprintf(fPagamentos, "%f", listaPagamentos[iCodPagamento].fValorPagamento);
					}
				}
			}

			sprintf(cNomeArquivo, "%s%s", "configuracoes", cExtensaoArquivo);

			if ((fConfiguracoes = fopen(cNomeArquivo, "w")) == NULL) {
				perror("Erro ao abrir arquivo de configuracoes.");
				exit(1);
			}

			/* Salvar Configuracoes */
			fprintf(fConfiguracoes, "%.2lf\n", fValorPrimeiraHora);
			fprintf(fConfiguracoes, "%.2lf\n", fValorDemaisHoras);
			fprintf(fConfiguracoes, "%d\n", iTotalVagasEstacionamento);
			fprintf(fConfiguracoes, "%d\n", iTotalVagasDisponiveis);

			fclose(fEntradas);
			fclose(fSaidas);
			fclose(fPagamentos);
			fclose(fConfiguracoes);
			/********** ----------- Fim da rotina de salvamento dos dados das structs -----------**********/
			return 0;

		default: /* Opcao Invalida */
			printf(" Opcao Invalida");
			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
		} /* Fecha o laco switch */
		pausarMenuPrincipal();
		limparTela();
	} /* Fecha o laco while */
	/* ----------- Fim do metodo principal ----------- */
} /* Fecha o metodo main */
