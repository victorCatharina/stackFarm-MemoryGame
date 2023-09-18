#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <malloc.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

#define VACA_IMG_PATH "img/vaca.txt"
#define GALINHA_IMG_PATH "img/galinha.txt"
#define PORCO_IMG_PATH "img/porco.txt"
#define CACHORRO_IMG_PATH "img/cachorro.txt"

#define CAPA_IMG "img/capa.txt"

#define VACA_SOM_PATH "som/vaca.wav"
#define GALINHA_SOM_PATH "som/galinha.wav"
#define PORCO_SOM_PATH "som/porco.wav"
#define CACHORRO_SOM_PATH "som/cachorro.wav"

#define RANKING_PATH "ranking.txt"

#define UP_ARROW 72
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define ENTER_KEY 13

#define SIZE_PLAYER_NAME 100

typedef struct
{
    char playerName[SIZE_PLAYER_NAME];
    int score;

} tRanking;

typedef struct {
    int id;
    char nome[20];
    char *img;
    char *som;
} Animal;

typedef struct No {
    Animal *animal;
    struct No *proximo;
} No;

typedef struct {
    No *topo;
    int tamanho;
} Pilha;

// -------------------- FUNÇÕES ------------------------

void fillPlayerRanking(tRanking *ranking, char *playerName, int score)
{
    for(int i = 0; i < SIZE_PLAYER_NAME; i++)
    {

        ranking->playerName[i] = playerName[i];
    }

    ranking->score = score;

}

void showRanking()
{
    tRanking playerRanking;
    FILE *ranking;

    ranking = fopen(RANKING_PATH, "r");

    if(ranking == NULL)
    {
        printf("Nenhum ranking para exibir");
    }
    else
    {

        system("cls");

        printf("\n\n******* RANKING *******\n\n");

        fread(&playerRanking, sizeof(tRanking), 1, ranking);

        if(ferror(ranking) != 0)
        {
            printf("Erro ao gravar ranking");
        }

        while(!feof(ranking))
        {
            printf("\nPlayer: %s", playerRanking.playerName);
            printf("Score: %d\n\n", playerRanking.score);

            fread(&playerRanking, sizeof(tRanking), 1, ranking);
        }

    }

    system("pause");

    fclose(ranking);

}

void savePlayerPoints(char *playerName, int sizeName, int points)
{
    tRanking playerRanking;
    FILE *ranking;

    ranking = fopen(RANKING_PATH, "a");

    if(ranking == NULL)
    {
        printf("Não foi possível salvar o ranking");
    }
    else
    {
        fillPlayerRanking(&playerRanking, playerName, points);

        fwrite(&playerRanking, sizeof(tRanking), 1, ranking);

        if(ferror(ranking) != 0)
        {
            printf("Erro ao gravar ranking");
        }

    }

    fclose(ranking);


}

void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}

char esperaQlqTecla()
{
	int pressed;

	while(!kbhit());

	pressed = getch();
	return((char)pressed);
}

void setup(Pilha *p)
{
    p->topo = NULL;
    p->tamanho = 0;
}

void empilhar(Pilha *p, Animal *animal) {
    No *no = malloc(sizeof(No));
    no->animal = animal;
    no->proximo = p->topo;
    p->topo = no;
    p->tamanho++;
}

No* desempilhar(Pilha *p)
{
    No* no = NULL;

    no = p->topo;
    p->topo = no->proximo;

    return no;
}

void inverterPilha(Pilha *p, Pilha *invertida)
{
    while(p->topo){
        No *atual = desempilhar(p);
        empilhar(invertida, atual->animal);
    }
}

void pontuacao(int nivel, int *pontos)
{
    if(nivel == 1){
        *pontos = 0;
    }
    else if(nivel > 1 && nivel <= 5){
        *pontos = *pontos + 5;
    }
    else if(nivel > 5 && nivel <= 10){
        *pontos = *pontos + 10;
    }
    else{
        *pontos = *pontos + 20;
    }
}

void addAnimalAleatorio(Pilha *animais, Pilha *animaisRodada, int tamanhoPilha)
{
    srand(time(NULL));

    int indice = rand() % tamanhoPilha;
    No *atual = animais->topo;
    for (int i = 0; i < indice; i++) {
        atual = atual->proximo;
    }
    empilhar(animaisRodada, atual->animal);
}

void exibirConteudoArquivo(char *arquivo) {

    FILE *arq;
    char Linha[100];
    char *result;
    int i;

    arq = fopen(arquivo, "r");

    if (arquivo == NULL) {

        printf("Erro! Arquivo nulo");
        return;
    }

    i = 1;
    while (!feof(arq))
    {
	// Lê uma linha (inclusive com o '\n')
      result = fgets(Linha, 100, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
      if (result)  // Se foi possível ler
	  printf("%s",Linha);
      i++;
    }

    fclose(arq);
}

void emiteSom(char *somArq)
{
    PlaySound(somArq, NULL, SND_SYNC);
}

void exibeAnimais(No *noAnimal) {

    No* noAux = noAnimal;

    while(noAux) {
        printf("Mostrando animal: %s\n", noAux->animal->nome);

        exibirConteudoArquivo(noAux->animal->img);
        emiteSom(noAux->animal->som);

        //sleep(2);
        system("clear || cls");

        noAux = noAux->proximo;
    }

}

void sair()
{
	char pressed;
	gotoxy(9,8);
	printf("Você tem certeza que deseja sair? (S/N)\n");

	do
	{
		pressed = esperaQlqTecla();
		pressed = tolower(pressed);
	} while (!(pressed == 's' || pressed == 's'));

	if (pressed == 's')
	{
		system("cls");
		exit(1);
	}
	return;
}


int menuSelector(int x, int y, int yInicio)
{
	char key;
	int i = 0;
	x = x - 2;
	gotoxy(x,yInicio);

	printf(">");

	gotoxy(1,1);


	do
	{
		key = esperaQlqTecla();
		if ( key == (char)UP_ARROW )
		{
			gotoxy(x,yInicio+i);
			printf(" ");

			if (yInicio >= yInicio+i )
				i = y - yInicio - 2;
			else
				i--;
			gotoxy(x,yInicio+i);
			printf(">");
		}
		else
			if ( key == (char)DOWN_ARROW )
			{
				gotoxy(x,yInicio+i);
				printf(" ");

				if (i+2 >= y - yInicio )
					i = 0;
				else
					i++;
				gotoxy(x,yInicio+i);
				printf(">");
			}
	} while(key != (char)ENTER_KEY);
	return(i);
}

int mainMenu(void)
{
	int x = 10, y = 5;
	int yStart = y;

	int selected;

	system("cls");

	gotoxy(x,y++);
	printf("Novo Jogo\n");
	gotoxy(x,y++);
	printf("Recordes\n");
	//gotoxy(x,y++);
	//printf("Controls\n");
	gotoxy(x,y++);
	printf("Exit\n");
	gotoxy(x,y++);

	selected = menuSelector(x, y, yStart);

	return(selected);
}

int selecaoReposta(void)
{
	int x = 10, y = 5;
	int yStart = y;

	int selected;

	system("cls");

	printf("Selecione a opção que correspondente ao animal:\n\n");

	gotoxy(x,y++);
	printf("Vaca\n");
	gotoxy(x,y++);
	printf("Galinha\n");
	gotoxy(x,y++);
	printf("Porco\n");
	gotoxy(x,y++);
	printf("Cachorro\n");
	gotoxy(x,y++);

	selected = menuSelector(x, y, yStart);

	return(selected);
}

int verificaResposta(Pilha *invertida, int numAnimais, int pontos)
{
    No *atual = invertida->topo;
    for (int i = 0; i < numAnimais; i++) {

        int resposta = selecaoReposta();
//        printf("Digite o numero correspondente ao animal %d: (VACA - 0, GALINHA - 1, PORCO - 2, CACHORRO - 3)", i + 1);
        //scanf("%d", &resposta);

        system("clear || cls");

        if (resposta != atual->animal->id) {
            printf("Resposta incorreta. Fim de jogo!\n");
            printf("PONTOS: %d\n\n", pontos);
            return 0;
        }

        atual = atual->proximo;
    }

    printf("Resposta correta! Proximo nivel.\n");
    return 1;
}

void jogo(Pilha *listaAnimais, Pilha *animaisRodada, Pilha *invertida)
{

    char playerName[SIZE_PLAYER_NAME];

    int jogando = 1;
    int numAnimais = 1;
    int pontos = 0;
    int tamanho = 0;

    //criaAnimais(listaAnimais);

    // Preciso colocar essa matriz aqui para que no processo de voltar para a função jogo não seja perdido nenhum elemento como o nome
    Animal animais[4] = {
        {0, "Vaca", VACA_IMG_PATH, VACA_SOM_PATH}, // ---------------- colocar o PATH dos animais aqui -----------------------------
        {1, "Galinha", GALINHA_IMG_PATH, GALINHA_SOM_PATH},
        {2, "Porco", PORCO_IMG_PATH, PORCO_SOM_PATH},
        {3, "Cachorro", CACHORRO_IMG_PATH, CACHORRO_SOM_PATH}
    };

    for (int i = 0; i < 4; i++) {
        empilhar(listaAnimais, &animais[i]);
    }

    tamanho = listaAnimais->tamanho;

    while (jogando) {
        system("clear || cls");
        printf("Nivel %d\n", numAnimais);

        addAnimalAleatorio(listaAnimais, animaisRodada, tamanho);

        inverterPilha(animaisRodada, invertida);

        // Mostra os animais individualmente
        exibeAnimais(invertida->topo);

        // Mostra a pontuação do jogador
        pontuacao(numAnimais, &pontos);
        printf("PONTOS: %d\n\n", pontos);

        // Peça ao jogador para repetir a sequência
        jogando = verificaResposta(invertida, numAnimais, pontos);

        numAnimais++;
        // Volto a pilha ao normal para que seja adicionado outro animal no topo dela
        inverterPilha(invertida, animaisRodada);
        sleep(2);  // Atraso de 2 segundo
    }

    printf("\nDigite seu nome para o Ranking: ");
    fflush(stdin);
    fgets(playerName, SIZE_PLAYER_NAME, stdin);

    savePlayerPoints(playerName, SIZE_PLAYER_NAME, pontos);
    showRanking();

}

int main() {

    setlocale(LC_ALL, "Portuguese");
    SetConsoleTitle("Stack Farm Memory Game");
    ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);

    Pilha listaAnimais;
    Pilha animaisRodada;
    Pilha invertida;

    printf("Jogo de Memória de Animais!\n\n");
    exibirConteudoArquivo(CAPA_IMG);
    printf("\n");
    system("pause");

      do
	{
	    setup(&listaAnimais);
        setup(&animaisRodada);
        setup(&invertida);

		switch(mainMenu())
		{
			case 0:
				jogo(&listaAnimais, &animaisRodada, &invertida);
				break;
			case 1:
				showRanking();
				break;
			case -1:
				//controles();
				break;
			case 2:
				sair();
				break;
		}
	} while(1);

    return 0;
}
