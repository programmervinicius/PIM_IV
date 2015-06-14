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
	char   cPlaca[9],
	       cDataEntrada[12],
	       cHorarioEntrada[8];
	time_t dtDataHorarioEntrada;
	double dDataHorarioEntrada;
};

struct SaidaVeiculo {
	int    iCodSaida;
	char   cPlaca[9],
	       cDataSaida[12],
	       cHorarioSaida[8];
	time_t dtDataHorarioSaida;
	double dDataHorarioSaida;
};

struct Pagamento {
	int   iCodPagamento;
	char  cPlaca[9],
	      cDataPagamento[12],
	      cHorarioPagamento[8];
	float fValorPagamento;
};

struct FechamentoCaixa {
	int    iCodFechamento;
	char   cDataFechamento[12],
	       cHorarioFechamento[8];
	time_t dtDataHorarioFechamento;
	double dDataHorarioFechamento;
	float  fValorTotalFechamento;
};

void limparTela() {
	system("cls"); /* Limpa a tela do sistema - Windows*/
}

void pausarMenuPrincipal() {
	getchar();
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
	struct EntradaVeiculo  listaEntradas[10000];
	struct SaidaVeiculo    listaSaidas[10000];
	struct Pagamento       listaPagamentos[10000];
	struct FechamentoCaixa listaFechamentos[10000];

	/* Variaveis para armazenar os valores de placas e a linha do arquivo do carregamento das structs */
	char cPlaca[9]                = " ",
		 cPlacaAux[9]             = " ",
		 cDataFechamentoAux[11]   = " ",
		 cLinhaArquivo[MAX]       = " ",
		 cDataAberturaSistema[12] = " ",
		 cNomeArquivo[512]        = " ",
		 cExtensaoArquivo[5]      = ".txt",
		 cTituloSistema[70]       = " ";

	char *cResultLinhaArquivo;
	char *cCharAux;

	/* Declaraco de Variaveis do sistema */
	int i,
	    iAux,
	    iOpcao,
	    iOpcaoEscolhida,
	    iVeiculoEncontrado,
		iPagamentoEncontrado,
		iFechamentoEncontrado,
	    iTotalEntradasHoje,
	    iTotalSaidasHoje,
	    iTotalVagasEstacionamento,
	    iTotalVagasDisponiveis,
	    iUltimoCodEntrada,
	    iUltimoCodSaida,
	    iUltimoCodPagamento,
	    iUltimoCodFechamento,
	    iPagamentoConfirmado,
	    iOpcaoFechamentoCaixa,
	    iRealizouFechamentoHoje,
	    iPodeImprimirRelatorio;

	/* Variaveis para armazenar o valor da primeira hora e das demais horas */
	float fValorPrimeiraHora,
	      fValorDemaisHoras,
	      fValorConfirmado;

	/* Variavel para trabalhar com a data e hora atual */
	time_t dtDataHorarioAgora;
	time_t dtDataAberturaSistema;

	/* Ponteiros para arquivos de entradas, saidas, pagamentos e tabela de tarifas */
	FILE* fEntradas;
	FILE* fSaidas;
	FILE* fPagamentos;
	FILE* fFechamentos;
	FILE* fConfiguracoes;
	FILE* fRelatorio;

	/* Declaraco de Variaveis das Entradas de Veiculos */
	int    iCodEntrada,
	       iCodEntradaAux;
	char   cDataEntrada[12]   = " ",
		   cHorarioEntrada[8] = " ";
	double dDataHorarioEntrada;

	/* Declaraco de Variaveis das Saidas de Veiculos */
	int    iCodSaida,
	       iCodSaidaAux;
	char   cDataSaida[12]   = " ",
		   cHorarioSaida[8] = " ";
	double dDataHorarioSaida;

	/* Declaracao de Variaveis dos Pagamentos */
	int    iCodPagamento,
	       iCodPagamentoAux;
	char   cDataPagamento[12]   = " ",
		   cHorarioPagamento[8] = " ";
	float  fValorPagamento;
	double dDiferencaSegundos;

	/* Declaracao de Variaveis dos Fechamentos */
	int    iCodFechamento,
	       iCodFechamentoAux;
	char   cDataFechamento[12]   = " ",
		   cHorarioFechamento[8] = " ";
	float  fValorTotalFechamento;

	/* Inicializacao e limpeza de variaveis */
	i                         = 0;
	iAux                      = 0;
	iCodEntrada               = 0;
	iCodEntradaAux            = 0;
	iCodSaida                 = 0;
	iCodSaidaAux              = 0;
	iCodPagamento             = 0;
	iCodPagamentoAux          = 0;
	iCodFechamento            = 0;
	iCodFechamentoAux         = 0;
	iOpcaoEscolhida           = 0;
	iVeiculoEncontrado        = 0;
	iTotalEntradasHoje        = 0;
	iTotalSaidasHoje          = 0;
	iTotalVagasEstacionamento = 0;
	iTotalVagasDisponiveis    = 0;
	iUltimoCodEntrada         = 0;
	iUltimoCodSaida           = 0;
	iUltimoCodPagamento       = 0;
	iUltimoCodFechamento      = 0;
	iPagamentoConfirmado      = 0;
	iOpcaoFechamentoCaixa     = 0;
	iRealizouFechamentoHoje   = 0;
	fValorPagamento           = 0;
	fValorTotalFechamento     = 0;
	fValorConfirmado          = 0;
	dDataHorarioEntrada       = 0;
	dDataHorarioEntrada       = 0;

	limparTela();

	/********* ----------- Inicio da rotina de carregamento das structs ----------- *********/
	dtDataAberturaSistema = time(NULL); /* Variavel de data da abertura do sistema */
	strftime(cDataAberturaSistema, 20, "%d%m%Y", localtime(&dtDataAberturaSistema));

	/* Cria o nome do arquivo de entradas */
	sprintf(cNomeArquivo, "%s%s%s", "entradas_", cDataAberturaSistema, cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fEntradas = fopen(cNomeArquivo, "a+");

	if (fEntradas == NULL) /* Se houve erro na abertura */
		printf("Erro ao abrir arquivo de entradas de veiculos.");

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
				listaEntradas[iCodEntrada].dDataHorarioEntrada  = dDataHorarioEntrada;
				listaEntradas[iCodEntrada].dtDataHorarioEntrada = dDataHorarioEntrada;
				break;
			}
		}
		iOpcao = -1;
		i++;
	}

	/* Cria o nome do arquivo de saidas */
	sprintf(cNomeArquivo, "%s%s%s", "saidas_", cDataAberturaSistema, cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fSaidas = fopen(cNomeArquivo, "a+");

	if (fSaidas == NULL) /* Se houve erro na abertura */
		printf("Erro ao abrir arquivo de saidas de veiculos.");

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
				listaSaidas[iCodSaida].dDataHorarioSaida  = dDataHorarioSaida;
				listaSaidas[iCodSaida].dtDataHorarioSaida = dDataHorarioEntrada;
				break;
			}
		}
		iOpcao = -1;
		i++;
	}

	/* Cria o nome do arquivo de pagamentos */
	sprintf(cNomeArquivo, "%s%s%s", "pagamentos_", cDataAberturaSistema, cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fPagamentos = fopen(cNomeArquivo, "a+");

	if (fPagamentos == NULL) /* Se houve erro na abertura */
		printf("Erro ao abrir arquivo de pagamentos.");

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

	/* Cria o nome do arquivo de fechamentos de caixa */
	sprintf(cNomeArquivo, "%s%s%s", "fechamentos_", cDataAberturaSistema, cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fFechamentos = fopen(cNomeArquivo, "a+");

	if (fFechamentos == NULL) /* Se houve erro na abertura */
		printf("Erro ao abrir arquivo de fechamentos de caixa.");

	/* Inicializacao das variaveis */
	i      = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));
	memset(&cResultLinhaArquivo, 0, sizeof(cResultLinhaArquivo));

	while (!feof(fFechamentos)) {
		cResultLinhaArquivo = fgets(cLinhaArquivo, MAX, fFechamentos); /* Le uma linha (inclusive com o '\n') */

		if (cResultLinhaArquivo) /* Se foi possivel ler */
		{
			if (strcmp(cLinhaArquivo, "Fechamento\n") == 0)
				iOpcao = 0;

			if (iOpcao > -1)
				cResultLinhaArquivo = fgets(cLinhaArquivo, MAX, fFechamentos);

			if (iOpcao == 0) {
				iCodFechamento = atoi(cLinhaArquivo);
				iUltimoCodFechamento = iCodFechamento;
				listaFechamentos[iCodFechamento].iCodFechamento = iCodFechamento;
				fgets(listaFechamentos[iCodFechamento].cDataFechamento, MAX, fFechamentos);
				fgets(listaFechamentos[iCodFechamento].cHorarioFechamento, MAX, fFechamentos);
				fgets(cLinhaArquivo, MAX, fFechamentos);
				fValorTotalFechamento = strtod(cLinhaArquivo, NULL);
				listaFechamentos[iCodFechamento].fValorTotalFechamento = fValorTotalFechamento;
				break;
			}
		}
		iOpcao = -1;
		i++;
	}

	//fclose(fEntradas);
	//fclose(fSaidas);
	//fclose(fPagamentos);
	//fclose(fFechamentos);
	/********* ----------- Fim da da rotina de carregamento das structs ----------- *********/

	limparTela();

	/* Carregamento de variaveis de valores da primeira hora, das demais horas, total de vagas e total de vagas livres. */
	/* Cria o nome do arquivo de configuracoes */
	sprintf(cNomeArquivo, "%s%s", "configuracoes", cExtensaoArquivo);

	/* Abre um arquivo de texto para leitura */
	fConfiguracoes = fopen(cNomeArquivo, "a+");

	if (fConfiguracoes == NULL) /* Se houve erro na abertura */
		printf("Erro ao abrir arquivo de configuracoes.");

	/* Inicializacao das variaveis */
	i      = 1;
	iOpcao = -1;
	memset(&cLinhaArquivo, 0, sizeof(cLinhaArquivo));

	while (i <= 5) {
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

		case 5:
			iRealizouFechamentoHoje = 0; //strtod(cLinhaArquivo, NULL);
			break;
		}
		i++;
	}

	//fclose(fConfiguracoes);
	/* Fim leitura arquivos */

	/* Cria um título para a janela do Sistema */
	strcpy (cTituloSistema, "TITLE SISTEMA DE ESTACIONAMENTO DE VEICULOS - DATA: ");
	strftime(cDataAberturaSistema, 20, "%d/%m/%Y", localtime(&dtDataAberturaSistema));
	strcat (cTituloSistema, cDataAberturaSistema);
	system(cTituloSistema);

	strftime(cDataAberturaSistema, 20, "%d%m%Y", localtime(&dtDataAberturaSistema));

	if (iTotalVagasEstacionamento == 0) {
		printf(" __________________________________________________________________________\n");
		printf(" ::::::::::::::::  SISTEMA DE ESTACIONAMENTO DE VEICULOS  :::::::::::::::::\n");
		printf(" ::                                                                      ::\n");
		printf(" ::            PARA COMECAR A USAR ESTE SISTEMA, E NECESSARIO            ::\n");
		printf(" ::              DEFINIR ALGUMAS CONFIGURACOES IMPORTANTES.              ::\n");
		printf(" ::           VAMOS DEFINIR AGORA OS VALORES DA PRIMEIRA HORA,           ::\n");
		printf(" ::          DAS DEMAIS HORAS, TOTAL DE VAGAS DO ESTACIONAMENTO          ::\n");
		printf(" ::                  E O TOTAL DE VAGAS DISPONIVEIS.                     ::\n");
		printf(" ::                                                                      ::\n");
		printf(" ::        Todos os dados informados somente serao armazenados           ::\n");
		printf(" ::                no arquivo de texto ao sair do sistema.               ::\n");
		printf(" :: Observacao: Os valores em decimais devem ser informados com pontos.  ::\n");
		printf(" ::             Caso for informado com virgula entao ocorrera erro.      ::\n");
		printf(" ::                                                                      ::\n");
		printf(" :: Informe o valor para a primeira hora: ");
		scanf("%f", &fValorPrimeiraHora);

		if (fValorPrimeiraHora == 0) {
			while (fValorPrimeiraHora == 0) {
				printf(" ::   Obs.: O valor deve ser maior que 0.");
				printf("\n :: Informe o valor para a primeira hora: ");
				scanf("%f", &fValorPrimeiraHora);
			}
		}

		printf(" ::");
		printf("\n :: Informe o valor para as demais horas: ");
		scanf("%f", &fValorDemaisHoras);

		if (fValorDemaisHoras == 0) {
			while (fValorDemaisHoras == 0) {
				printf(" ::   Obs.: O valor deve ser maior que 0.");
				printf("\n :: Informe o valor para as demais horas: ");
				scanf("%f", &fValorDemaisHoras);
			}
		}

		printf(" ::");
		printf("\n :: Informe o no. total de vagas do estacionamento: ");
		scanf("%d", &iTotalVagasEstacionamento);

		if (iTotalVagasEstacionamento == 0) {
			while (iTotalVagasEstacionamento == 0) {
				printf(" ::   O no. total de vagas do estacionamento deve ser maior que 0.");
				printf("\n :: Informe o no. total de vagas do estacionamento: ");
				scanf("%d", &iTotalVagasEstacionamento);
			}
		}

		printf(" ::");
		printf("\n :: E, por ultimo, informe o no. total de vagas disponiveis: ");
		scanf("%d", &iTotalVagasDisponiveis);

		printf(" __________________________________________________________________________\n");
		fflush(stdin); /* Limpa a entrada padrao do sistema (teclado) */
		printf("\n Tecle 'Enter' para entrar no menu do sistema.\n");
		pausarMenuPrincipal();
	}

	limparTela();

	/* Laco do Menu Principal */
	while (1) {
		printf(" __________________________________________________________________________\n");
		printf(" ::::::::::::::::  SISTEMA DE ESTACIONAMENTO DE VEICULOS  :::::::::::::::::\n\n");
		printf("   :: INFORMACOES UTEIS ::\n");
		printf("   Valor da Primeira Hora:         R$ %.2lf\n", fValorPrimeiraHora);
		printf("   Valor das Demais Horas:         R$ %.2lf\n", fValorDemaisHoras);
		printf("   No. Total de Vagas:             %d\n", iTotalVagasEstacionamento);
		printf("   No. Total de Vagas Disponiveis: %d\n", iTotalVagasDisponiveis);
		printf("   Os arquivos de texto gerados pelo sistema ficam armazenados na mesma\n");
		printf("   pasta do executavel e os dados gerados sao salvos na saida do sistema.\n\n\n");
		printf(" << Menu do Sistema >>\n");
		printf("  << Entradas e Saidas de Veiculos >> \n");
		printf("  1  - Entrada de Veiculo\n");
		printf("  2  - Saida de Veiculo\n\n");
		printf("  << Pesquisas >>\n");
		printf("  3  - Pesquisa de Registro de Entrada\n");
		printf("  4  - Pesquisa de Registro de Saida\n");
		printf("  5  - Pesquisa de Registro de Pagamento\n");
		printf("  6  - Pesquisa de Registro de Fechamento\n\n");
		printf("  << Relatorios >>\n");
		printf("  7  - Impressao de Todas as Entradas\n");
		printf("  8  - Impressao de Todas as Saidas\n");
		printf("  9  - Impressao de Todos os Pagamentos\n");
		printf("  10 - Impressao de Todos os Fechamentos\n\n");
		printf("  << Gestao do Caixa >>\n");
		printf("  11 - Fechamento de Caixa do Estacionamento\n");
		printf("  12 - Consulta de Status do Caixa\n\n");
		printf("  0  - Sair do Sistema\n");
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
				dtDataHorarioAgora = time(NULL);
				strftime(cDataEntrada, 20, "%d/%m/%Y", localtime(&dtDataHorarioAgora));
				strftime(cHorarioEntrada, 10, "%H:%M:%S", localtime(&dtDataHorarioAgora));
				printf("\n ::: Entrada de Veiculo realizada com Sucesso :::\n\n");

				if ((cCharAux = strchr(cDataEntrada, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(cHorarioEntrada, '\n')) != NULL)
					*cCharAux = '\0';

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
				time(&listaEntradas[iCodEntrada].dtDataHorarioEntrada);
				listaEntradas[iCodEntrada].dDataHorarioEntrada = listaEntradas[iCodEntrada].dtDataHorarioEntrada;
				iTotalVagasDisponiveis--;
				iTotalEntradasHoje++;

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
				dtDataHorarioAgora = time(NULL);
				strftime(cDataSaida, 20, "%d/%m/%Y", localtime(&dtDataHorarioAgora));
				strftime(cHorarioSaida, 10, "%H:%M:%S", localtime(&dtDataHorarioAgora));

				/* Registra a saida */
				iUltimoCodSaida = iCodSaida;
				listaSaidas[iCodSaida].iCodSaida = iCodSaida;
				strcpy(listaSaidas[iCodSaida].cPlaca, cPlaca);
				strcpy(listaSaidas[iCodSaida].cDataSaida, cDataSaida);
				strcpy(listaSaidas[iCodSaida].cHorarioSaida, cHorarioSaida);
				time(&listaSaidas[iCodSaida].dtDataHorarioSaida);
				listaSaidas[iCodSaida].dDataHorarioSaida = listaSaidas[iCodSaida].dtDataHorarioSaida;

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
					fValorPagamento = (fValorPrimeiraHora + ((dDiferencaSegundos - 1) * fValorDemaisHoras));
				/* Fim do calculo */

				printf("\n >>>> O Valor a Ser Confirmado para Pagamento: R$ %.2lf<<<<\n", fValorPagamento);
				printf("\n Deseja confirmar o pagamento?\n   0: Nao\n   1: Sim.\n");
				scanf("%d", &iPagamentoConfirmado);

				if (iPagamentoConfirmado == 1) {
					printf("\n >>>>>> Pagamento Confirmado com Sucesso. <<<<<<\n");
					fflush(stdin); /* Limpa a entrada padrao do sistema (teclado) */
					printf("\n Tecle 'Enter' para gerar o recibo de saida de veiculo e de pagamento.\n");
					getchar();

					/* Registra o pagamento */
					strftime(cDataPagamento, 20, "%d/%m/%Y", localtime(&dtDataHorarioAgora));
					strftime(cHorarioPagamento, 10, "%H:%M:%S", localtime(&dtDataHorarioAgora));

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
					iTotalSaidasHoje++;

					printf("\n\n ::::: No. de Vagas Disponiveis: %d :::::\n", iTotalVagasDisponiveis);

				} else
					printf("\n\n Saida de veiculo cancelada.");
			}
			else
				printf(" ::: Veiculo com placa %s nao teve entrada no estacionamento. :::", cPlaca);

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 3: /* Pesquisa de Registro de Entrada */
			printf(" ::: PESQUISA DE REGISTRO DE ENTRADA :::\n");
			printf(" ::: AVISO: A placa deve ter 8 caracteres. Exemplo: AAA-1234. :::\n");
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
			printf(" ::: PESQUISA DE REGISTRO DE SAIDA :::\n");
			printf(" ::: AVISO: A placa deve ter 8 caracteres. Exemplo: AAA-1234. :::\n");
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
			printf(" ::: PESQUISA DE REGISTRO DE PAGAMENTO :::\n");
			printf(" ::: AVISO: A placa deve ter 8 caracteres. Exemplo: AAA-1234. :::\n");
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
					printf("    Valor Pagamento: %.2lf\n", fValorPagamento);
					iPagamentoEncontrado = 1;
				} else {
					iPagamentoEncontrado = 0;
				}
			}

			if (iPagamentoEncontrado == 0) {
				printf("\n\n ::: Pagamento nao Encontrado. :::\n");
			}

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 6: /* Pesquisa de Registro de Fechamento */
			printf(" ::: PESQUISA DE REGISTRO DE FECHAMENTO :::\n");
			printf(" ::: AVISO: A Data do Fechamento deve ter 10 caracteres. Exemplo: 01/01/2015 :::\n");
			printf(" ::: Informe a Data do Fechamento que deseja pesquisar: ");
			scanf("%s", &cDataFechamentoAux);

			if (strlen(cDataFechamentoAux) != 10) {
				while (strlen(cDataFechamentoAux) != 10) {
					printf("    A Data do Fechamento %s deve ter 10 caracteres. Exemplo: 01/01/2015\n", cDataFechamentoAux);
					printf("    Informe a Data do Fechamento que deseja pesquisar: ");
					scanf("%s", &cDataFechamentoAux);

					/* Altera os caracteres da placa para ficarem maiusculos */
					i = 0;
					while (i < 11) {
						cDataFechamentoAux[i] = toupper(cDataFechamentoAux[i]);
						i++;
					}
				}
			}

			/* Altera os caracteres da placa para ficarem maiusculos */
			i = 0;
			while (i < 11) {
				cDataFechamentoAux[i] = toupper(cDataFechamentoAux[i]);
				i++;
			}
			fflush(stdin);

			for (i = 1; i <= iCodFechamento; i++) {
				if ((cCharAux = strchr(cDataFechamentoAux, '\n')) != NULL)
					*cCharAux = '\0';

				if ((cCharAux = strchr(listaFechamentos[i].cDataFechamento, '\n')) != NULL)
					*cCharAux = '\0';

				if (strcasecmp(listaFechamentos[i].cDataFechamento, cDataFechamentoAux) == 0) {
					iCodFechamento = i;
					strcpy(cDataFechamento, listaFechamentos[iCodFechamento].cDataFechamento);
					strcpy(cHorarioFechamento, listaFechamentos[iCodFechamento].cHorarioFechamento);
					fValorTotalFechamento = listaFechamentos[iCodFechamento].fValorTotalFechamento;

					if ((cCharAux = strchr(cDataFechamento, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(cHorarioFechamento, '\n')) != NULL)
						*cCharAux = '\0';

					printf("\n\n ::: Fechamento Encontrado com Sucesso :::\n");
					printf("\n :: ID Fechamento:          %d\n", iCodFechamento);
					printf("    Data:                   %s\n", cDataFechamento);
					printf("    Horario:                %s\n", cHorarioFechamento);
					printf("    Valor Total Fechamento: %.2lf\n", fValorTotalFechamento);
					iFechamentoEncontrado = 1;
				} else {
					iFechamentoEncontrado = 0;
				}
			}

			if (iFechamentoEncontrado == 0) {
				printf("\n\n ::: Fechamento nao Encontrado. :::\n");
			}

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 7: /* Relatorio de todas as entradas */
			printf(" ::: RELATORIO DE TODAS AS ENTRADAS :::\n");
			if (iUltimoCodEntrada > 0) {
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
				printf(" ::: FIM DO RELATORIO :::\n\n");

				printf(" Deseja salvar o relatorio em um arquivo de texto?\n   0: Nao\n   1: Sim.\n");
				scanf("%d", &iPodeImprimirRelatorio);

				if (iPodeImprimirRelatorio == 1) {
					printf(" Informe o nome do novo arquivo: \n");
					scanf("%s", &cNomeArquivo);

					sprintf(cNomeArquivo, "%s%s", cNomeArquivo, cExtensaoArquivo);

                    /* Cria arquivo para gravação */
	                fRelatorio = fopen(cNomeArquivo, "w");

                    if (fRelatorio == NULL) /* Se nao conseguiu criar */
                        printf("Problemas na criacao do arquivo\n");

					/* Salvar Entradas de Veiculos */
					if (iCodEntrada >= iUltimoCodEntrada) {
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

								fprintf(fRelatorio, "\n :: ID Entrada: %d\n", listaEntradas[iCodEntrada].iCodEntrada);
								fprintf(fRelatorio, "    Placa:      %s\n", listaEntradas[iCodEntrada].cPlaca);
								fprintf(fRelatorio, "    Data:       %s\n", listaEntradas[iCodEntrada].cDataEntrada);
								fprintf(fRelatorio, "    Horario:    %s\n\n", listaEntradas[iCodEntrada].cHorarioEntrada);
							}
							fclose(fRelatorio);
						}
					}
				}
			} else
				printf("\n Nao ha dados para emissao do relatorio de entradas.");

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 8: /* Relatorio de todas as saidas */
			printf(" ::: RELATORIO DE TODAS AS SAIDAS :::\n");
			if (iUltimoCodSaida > 0) {
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
						printf("    Data:     %s\n", listaSaidas[i].cDataSaida);
						printf("    Horario:  %s\n\n", listaSaidas[i].cHorarioSaida);
					}
				}
				printf(" ::: FIM DO RELATORIO :::\n\n");

				printf(" Deseja salvar o relatorio em um arquivo de texto?\n   0: Nao\n   1: Sim.\n");
				scanf("%d", &iPodeImprimirRelatorio);

				if (iPodeImprimirRelatorio == 1) {
					printf(" Informe o nome do novo arquivo: \n");
					scanf("%s", &cNomeArquivo);

					sprintf(cNomeArquivo, "%s%s", cNomeArquivo, cExtensaoArquivo);

					/* Cria arquivo para gravação */
	                fRelatorio = fopen(cNomeArquivo, "w");

                    if (fRelatorio == NULL) /* Se nao conseguiu criar */
                        printf("Problemas na criacao do arquivo\n");


					/* Salvar Saidas de Veiculos */
					if (iCodSaida >= iUltimoCodSaida) {
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

								fprintf(fRelatorio, "\n :: ID Saida: %d\n", listaSaidas[iCodSaida].iCodSaida);
								fprintf(fRelatorio, "    Placa:    %s\n", listaSaidas[iCodSaida].cPlaca);
								fprintf(fRelatorio, "    Data:     %s\n", listaSaidas[iCodSaida].cDataSaida);
								fprintf(fRelatorio, "    Horario:  %s\n\n", listaSaidas[iCodSaida].cHorarioSaida);
							}
							fclose(fRelatorio);
						}
					}
				}
			} else
				printf("\n Nao ha dados para emissao do relatorio de saidas.");

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 9: /* Relatorio de todos os pagamentos */
			printf(" ::: RELATORIO DE TODOS OS PAGAMENTOS :::\n");
			if (iUltimoCodPagamento > 0) {
				for (i = 1; i <= iCodPagamento; i++) {
					if (listaPagamentos[i].iCodPagamento > 0) {
						if ((cCharAux = strchr(listaPagamentos[i].cPlaca, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaPagamentos[i].cDataPagamento, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaPagamentos[i].cHorarioPagamento, '\n')) != NULL)
							*cCharAux = '\0';

						printf("\n :: ID Pagamento:    %d\n", listaPagamentos[i].iCodPagamento);
						printf("    Placa:           %s\n", listaPagamentos[i].cPlaca);
						printf("    Data:            %s\n", listaPagamentos[i].cDataPagamento);
						printf("    Horario:         %s\n", listaPagamentos[i].cHorarioPagamento);
						printf("    Valor Pagamento: %2.lf\n\n", listaPagamentos[i].fValorPagamento);
					}
				}
				printf(" ::: FIM DO RELATORIO :::\n\n");

				printf(" Deseja salvar o relatorio em um arquivo de texto?\n   0: Nao\n   1: Sim.\n");
				scanf("%d", &iPodeImprimirRelatorio);

				if (iPodeImprimirRelatorio == 1) {
					printf(" Informe o nome do novo arquivo: \n");
					scanf("%s", &cNomeArquivo);

					sprintf(cNomeArquivo, "%s%s", cNomeArquivo, cExtensaoArquivo);

					/* Cria arquivo para gravação */
	                fRelatorio = fopen(cNomeArquivo, "w");

                    if (fRelatorio == NULL) /* Se nao conseguiu criar */
                        printf("Problemas na criacao do arquivo\n");

					/* Salvar Pagamentos */
					if (iCodPagamento >= iUltimoCodPagamento) {
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

								fprintf(fRelatorio, "\n :: ID Pagamento:    %d\n", listaPagamentos[iCodPagamento].iCodPagamento);
								fprintf(fRelatorio, "    Placa:           %s\n", listaPagamentos[iCodPagamento].cPlaca);
								fprintf(fRelatorio, "    Data:            %s\n", listaPagamentos[iCodPagamento].cDataPagamento);
								fprintf(fRelatorio, "    Horario:         %s\n", listaPagamentos[iCodPagamento].cHorarioPagamento);
								fprintf(fRelatorio, "    Valor Pagamento: %.2lf\n\n", listaPagamentos[iCodPagamento].fValorPagamento);
							}
							fclose(fRelatorio);
						}
					}
				}
			} else
				printf("\n Nao ha dados para emissao do relatorio de pagamentos.");

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 10: /* Relatório de todos os fechamentos */
			printf(" :::: RELATORIO DE TODOS OS FECHAMENTOS ::::\n");
			if (iUltimoCodFechamento > 0) {
				for (i = 1; i <= iCodFechamento; i++) {
					if (listaFechamentos[i].iCodFechamento > 0) {
						if ((cCharAux = strchr(listaFechamentos[i].cDataFechamento, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaFechamentos[i].cHorarioFechamento, '\n')) != NULL)
							*cCharAux = '\0';

						printf("\n :: ID Fechamento:          %d\n", listaFechamentos[i].iCodFechamento);
						printf("    Data:                   %s\n", listaFechamentos[i].cDataFechamento);
						printf("    Horario:                %s\n", listaFechamentos[i].cHorarioFechamento);
						printf("    Valor Total Fechamento: %.2lf\n\n", listaFechamentos[i].fValorTotalFechamento);
					}
				}
				printf(" ::: FIM DO RELATORIO :::\n\n");

				printf(" Deseja salvar o relatorio em um arquivo de texto?\n   0: Nao\n   1: Sim.\n");
				scanf("%d", &iPodeImprimirRelatorio);

				if (iPodeImprimirRelatorio == 1) {
					printf(" Informe o nome do novo arquivo: \n");
					scanf("%s", &cNomeArquivo);

					sprintf(cNomeArquivo, "%s%s", cNomeArquivo, cExtensaoArquivo);

					/* Cria arquivo para gravação */
	                fRelatorio = fopen(cNomeArquivo, "w");

                    if (fRelatorio == NULL) /* Se nao conseguiu criar */
                        printf("Problemas na criacao do arquivo\n");

					/* Salvar Fechamentos */
					if (iCodFechamento >= iUltimoCodFechamento) {
						if (iUltimoCodFechamento == 0)
							iAux = 1;
						else
							iAux = iUltimoCodFechamento + 1;

						iCodFechamentoAux = iCodFechamento;

						if (iCodFechamentoAux > 0) {
							for (i = iAux; i <= iCodFechamentoAux; i++) {
								iCodFechamento = i;

								if ((cCharAux = strchr(listaFechamentos[iCodFechamento].cDataFechamento, '\n')) != NULL)
									*cCharAux = '\0';

								if ((cCharAux = strchr(listaFechamentos[iCodFechamento].cHorarioFechamento, '\n')) != NULL)
									*cCharAux = '\0';

								fprintf(fRelatorio, "\n :: ID Fechamento:           %d\n", listaFechamentos[iCodFechamento].iCodFechamento);
								fprintf(fRelatorio, "    Data:                    %s\n", listaFechamentos[iCodFechamento].cDataFechamento);
								fprintf(fRelatorio, "    Horario:                 %s\n", listaFechamentos[iCodFechamento].cHorarioFechamento);
								fprintf(fRelatorio, "    Valor Total Fechamento:  %.2lf\n\n", listaFechamentos[iCodFechamento].fValorTotalFechamento);
							}
							fclose(fRelatorio);
						}
					}
				}
			} else
				printf("\n Nao ha dados para emissao do relatorio de fechamentos.");

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 11: /* Realizar Fechamento do Caixa */
			printf(" ::: FECHAMENTO DE CAIXA DO ESTACIONAMENTO :::\n");
			if (iRealizouFechamentoHoje == 1) {
				printf("\n Nao e possivel continuar.\n");
				printf(" Houve fechamento de caixa no dia de hoje.");
			}
			else {
				if ((iTotalEntradasHoje > iTotalSaidasHoje) && (iTotalEntradasHoje > 0)) {
					printf("\n Ainda ha %d veiculo(s) estacionado(s) no patio do estacionamento.", iTotalEntradasHoje - iTotalSaidasHoje);
					printf("\n Faca a saida com pagamento para realizar o fechamento de caixa.");
					iOpcaoFechamentoCaixa = 0;
				} else if ((iTotalEntradasHoje = iTotalSaidasHoje) && (iTotalEntradasHoje > 0))
                    iOpcaoFechamentoCaixa = 1;
				else {
					printf("\n Nao houve entradas de veiculos no patio do estacionamento.");
					iOpcaoFechamentoCaixa = 0;
				}

				if (iOpcaoFechamentoCaixa == 1) {
					fValorTotalFechamento = 0;

					strftime(cDataAberturaSistema, 20, "%d/%m/%Y", localtime(&dtDataHorarioAgora));

					for (i = 1; i <= iUltimoCodPagamento; i++) {
						if (strcmp(cDataAberturaSistema, listaPagamentos[i].cDataPagamento) == 0)
							fValorTotalFechamento = fValorTotalFechamento + listaPagamentos[i].fValorPagamento;
					}

					strftime(cDataAberturaSistema, 20, "%d%m%Y", localtime(&dtDataHorarioAgora));

					/* Registra o fechamento do caixa */
					if (iCodFechamento == 0)
						iCodFechamento = 1;

					iUltimoCodFechamento = iCodFechamento;
					listaFechamentos[iCodFechamento].iCodFechamento = iCodFechamento;
					strftime(cDataFechamento, 20, "%d/%m/%Y", localtime(&dtDataHorarioAgora));
					strftime(cHorarioFechamento, 10, "%H:%M:%S", localtime(&dtDataHorarioAgora));
					strcpy(listaFechamentos[iCodFechamento].cDataFechamento, cDataFechamento);
					strcpy(listaFechamentos[iCodFechamento].cHorarioFechamento, cHorarioFechamento);
					time(&listaFechamentos[iCodFechamento].dtDataHorarioFechamento);
					listaFechamentos[iCodFechamento].dDataHorarioFechamento = listaFechamentos[iCodFechamento].dtDataHorarioFechamento;
					listaFechamentos[iCodFechamento].fValorTotalFechamento = fValorTotalFechamento;

					/* Imprimir fechamento do caixa */
					strftime(cDataAberturaSistema, 20, "%d/%m/%Y", localtime(&dtDataAberturaSistema));
					printf(" _____________________________________________________________\n");
					printf(" ::::::::::: FECHAMENTO DO CAIXA DATA: %s ::::::::::::\n", cDataAberturaSistema);
					strftime(cDataAberturaSistema, 20, "%d%m%Y", localtime(&dtDataAberturaSistema));

					if ((cCharAux = strchr(listaFechamentos[i].cDataFechamento, '\n')) != NULL)
						*cCharAux = '\0';

					if ((cCharAux = strchr(listaFechamentos[i].cHorarioFechamento, '\n')) != NULL)
						*cCharAux = '\0';

					printf("   ID Fechamento:          %d\n", iCodFechamento);
					printf("   Data:                   %s\n", cDataFechamento);
					printf("   Horario:                %s\n", cHorarioFechamento);
					printf("   Valor Total Fechamento: %.2lf\n", fValorTotalFechamento);
                    printf(" :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
					printf(" _____________________________________________________________\n");

					iRealizouFechamentoHoje = 1;
				}
			}

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 12: /* Consulta de Status do Caixa */
			printf(" ::: CONSULTA DE STATUS DO CAIXA :::\n");
			if (iRealizouFechamentoHoje == 0)
				printf("\n Ainda nao houve fechamento do caixa no dia de hoje");
			else
				printf("\n Houve fechamento de caixa no dia de hoje.");

			printf("\n\n Tecle 'Enter' para voltar ao menu do sistema.");
			pausarMenuPrincipal();
			break;

		case 0: /* Salva todos os dados que foram gravados nas structs e sai do sistema */
			/********** ----------- Inicio da rotina de salvamento dos dados das structs -----------**********/
			/* Criacao ou abertura dos arquivos de textos */
			/* Cria o nome do arquivo de entradas */
			sprintf(cNomeArquivo, "%s%s%s", "entradas_", cDataAberturaSistema, cExtensaoArquivo);

			//fEntradas = fopen(cNomeArquivo, "a+");

			if (fEntradas == NULL)
				printf("Erro ao abrir arquivo de entradas de veiculos.");

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

			/* Cria o nome do arquivo de saídas */
			sprintf(cNomeArquivo, "%s%s%s", "saidas_", cDataAberturaSistema, cExtensaoArquivo);

			//fSaidas = fopen(cNomeArquivo, "a+");

			if (fSaidas == NULL)
				printf("Erro ao abrir arquivo de saidas de veiculos.");

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

			/* Cria o nome do arquivo de pagamentos */
			sprintf(cNomeArquivo, "%s%s%s", "pagamentos_", cDataAberturaSistema, cExtensaoArquivo);

			//fPagamentos = fopen(cNomeArquivo, "a+");

			if (fPagamentos == NULL)
				printf("Erro ao abrir arquivo de pagamentos.");

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
						fprintf(fPagamentos, "%.2lf", listaPagamentos[iCodPagamento].fValorPagamento);
					}
				}
			}

			/* Cria o nome do arquivo de fechamentos */
			sprintf(cNomeArquivo, "%s%s%s", "fechamentos_", cDataAberturaSistema, cExtensaoArquivo);

			//fFechamentos = fopen(cNomeArquivo, "a+");

			if (fFechamentos == NULL)
				printf("Erro ao abrir arquivo de fechamentos de caixa.");

			/* Salvar Fechamentos */
			if (iCodFechamento > iUltimoCodFechamento) {
				if (iUltimoCodFechamento == 0)
					iAux = 1;
				else
					iAux = iUltimoCodFechamento + 1;

				iCodFechamentoAux = iCodFechamento;

				if (iCodFechamentoAux > 0) {
					for (i = iAux; i <= iCodFechamentoAux; i++) {
						iCodFechamento = i;

						if ((cCharAux = strchr(listaFechamentos[iCodFechamento].cDataFechamento, '\n')) != NULL)
							*cCharAux = '\0';

						if ((cCharAux = strchr(listaFechamentos[iCodFechamento].cHorarioFechamento, '\n')) != NULL)
							*cCharAux = '\0';

						if (verTamanhoArquivoTexto(cNomeArquivo) == 0)
							fprintf(fFechamentos, "Fechamento\n");
						else
							fprintf(fFechamentos, "\n\nFechamento\n");

						fprintf(fFechamentos, "%d\n", listaFechamentos[iCodFechamento].iCodFechamento);
						fprintf(fFechamentos, "%s\n", listaFechamentos[iCodFechamento].cDataFechamento);
						fprintf(fFechamentos, "%s\n", listaFechamentos[iCodFechamento].cHorarioFechamento);
						fprintf(fFechamentos, "%.2lf", listaFechamentos[iCodFechamento].fValorTotalFechamento);
					}
				}
			}

			/* Cria o nome do arquivo de configurações */
			sprintf(cNomeArquivo, "%s%s", "configuracoes", cExtensaoArquivo);

			//fConfiguracoes = fopen(cNomeArquivo, "a+");

			if (fConfiguracoes == NULL)
				printf("Erro ao abrir arquivo de configuracoes.");

			/* Salvar Configuracoes */
			fprintf(fConfiguracoes, "%.2lf\n", fValorPrimeiraHora);
			fprintf(fConfiguracoes, "%.2lf\n", fValorDemaisHoras);
			fprintf(fConfiguracoes, "%d\n", iTotalVagasEstacionamento);
			fprintf(fConfiguracoes, "%d\n", iTotalVagasDisponiveis);
			fprintf(fConfiguracoes, "%d\n", iRealizouFechamentoHoje);

			fclose(fEntradas);
			fclose(fSaidas);
			fclose(fPagamentos);
			fclose(fFechamentos);
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
