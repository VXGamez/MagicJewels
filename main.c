/************************************************
* @Proposit: Crear joc Magic Jewels amb sistema de ranking i un sistema de desat de partida
* @Autor/s: Victor Xirau Guardans
* @Data creacio: 15 de febrer de 2019
* @Data ultima modificacio: 20 de març de 2019
* ************************************************/

//Llibreries
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include "LS_allegro.h"

//Defeixo les constants que fare servir al llarg del codi.
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 800

//Defineixo els meus structs.
//Aquest primer struct servirà pel rànking
typedef struct{
	char nom[150];
	int puntuacio;
}Jugador;

//Aquest segon struct es el emprat per desar partida.
typedef struct{
	char nom[100];
	int puntos;
	int tauler[18][10];
	int x;
	int y;
	int indexColors[3];
}Partida;

void contarTemps(float segons){
	/***********************************************
	* @Finalitat: Contar el temps en segons que se li indiqui
	* @Parametres: in: segons = #segons que es volen contar
	* @Retorn: ------
	************************************************/
	int times=0; //Declaració de les variables emprades en la funció
	float time1, time0;
	time0 = (float) clock();
	while( times < segons * 4){
		time1  = (float) clock();
		if( ((time1-time0) / (float) CLOCKS_PER_SEC) >= 0.25 ){

			times++;
			time0 = (float) clock();
		}
	}
}
void menu1(int choice[2]){
	/***********************************************
	* @Finalitat: Mostrar el primer menu i demanar i llegir una opcio.
	* @Parametres: in: choice = array que conté les tries realitzades en els dos menus
	* @Retorn: ------
	************************************************/
	//Declaració de les variables emprades en aquesta funció
	char tria[100];
	//Mostro el menu i les seves opcions.
	printf("---------- JEWELS ----------\n");
	printf("\t1. Jugar partida\n");
	printf("\t2. Veure ranking\n");
	printf("\t3. Sortir\n");
	printf("Opcio: ");
	fflush(stdin); //Buido el buffer per poder agafar correctament la opció triada.
	fgets(tria, 100, stdin); //Realitzo un fgets per poder llegir l'input tot i que aquest no sigui una de les opcions.
	tria[ strlen(tria)-1 ]= '\0'; //Corregeixo el resultat del fgets per eliminar el \n que ve per defecte quan realitzem un fgets.

	choice[0] = atoi(tria); //Realitzo un atoi per convertir el input a número i el guardo a la primera posició del array choice.

	//Com que ara choice és un numero, el bucle a continuació s'encarrega de comprovar si el numero introduit és un dels demanats.
	//En cas que no ho sigui es torna a mostrar el menu i a demanar la tria del usuari.
	while(choice[0] != 1 && choice[0] != 2 && choice[0] != 3){
			printf("\nERROR: Opcio no valida!\n\n");
			printf("\t1. Jugar partida\n");
			printf("\t2. Veure ranking\n");
			printf("\t3. Sortir\n");
			printf("Opcio: ");
			fflush(stdin);
			fflush(stdin);
			fgets(tria, 100, stdin);
			tria[ strlen(tria)-1 ]= '\0';
			choice[0] = atoi(tria);
	}
}
void menu2(int choice[2]){
	/***********************************************
	* @Finalitat: Mostrar el segon menu i demanar i llegir una opcio.
	* @Parametres: in: choice = array que conté les tries realitzades en els dos menus
	* @Retorn: ------
	************************************************/
	char tria[100]; //Declaració de les variables emprades en la funció

	printf("\n\n---------- JUGAR PARTIDA ----------\n");
	printf("\t1. Nova partida\n");
	printf("\t2. Carregar partida\n");
	printf("\t3. Sortir\n");
	printf("Opcio: ");

	fflush(stdin); //Buido el buffer per poder agafar correctament la opció triada.
	fgets(tria, 100, stdin); //Realitzo un fgets per poder llegir l'input tot i que aquest no sigui una de les opcions.
	tria[ strlen(tria)-1 ]= '\0'; //Corregeixo el resultat del fgets per eliminar el \n que ve per defecte quan realitzem un fgets.

	choice[1] = atoi(tria); //Realitzo un atoi per convertir el input a número i el guardo a la primera posició del array choice.


	//Com que ara choice és un numero, el bucle a continuació s'encarrega de comprovar si el numero introduit és un dels demanats.
	//En cas que no ho sigui es torna a mostrar el menu i a demanar la tria del usuari.
	while(choice[1] != 1 && choice[1] != 2 && choice[1] != 3){
			printf("\nERROR: Opcio no valida\n\n");
			printf("\t1. Nova partida\n");
			printf("\t2. Carregar partida\n");
			printf("\t3. Sortir\n");
			printf("Opcio: ");
			fflush(stdin);
			fgets(tria, 100, stdin);
			tria[ strlen(tria)-1 ]= '\0';
			choice[1] = atoi(tria);
	}
}
void initPartida(int tauler[18][10]){
	/***********************************************
	* @Finalitat: Inicialitzar el valor tauler de la partida a 0.
	* @Parametres: in: tauler = array matricial de 2 dimensions que inicialitzem a 0.
	* @Retorn: ------
	************************************************/
	int i, j; //Declaració de les variables emprades en la funció

	//El bucle s'encarrega de recorrer totes les posicions possibles del tauler i inicialitzar-les a 0.
	for(i=0; i<18 ;i++){
		for(j=0; j<10 ;j++){
			tauler[i][j]= 0;
		}
	}
}
void dissenyInterficie(char nom_jugador[100], int punts, int tauler[18][10]){
	/***********************************************
	* @Finalitat: Disseny tota la interficie gràfica
	* @Parametres: in: nom_jugador = string que conté el nom del jugador
								 in: punts = puntuació de la partida
								 in: tauler = array matricial de 2 dimensions que conté el valor dels colors dels jewels en aquelles posicions.
	* @Retorn: ------
	************************************************/
	int i, j; //Declaració de les variables emprades en la funció

	LS_allegro_clear_and_paint(LIGHT_GRAY);

	//Linees Verticals negres
	for(i=1; i<=720; i=i+40){
		al_draw_line(i, 0, i, 800, LS_allegro_get_color(BLACK), 1);
	}
	//Linees Horitzontals negres
	for(i=1; i<=800; i=i+40){
		al_draw_line(0, i, 760, i, LS_allegro_get_color(BLACK), 1);
	}

	al_draw_filled_rectangle(40, 40, 440, 760, LS_allegro_get_color(BLACK));
	al_draw_filled_rectangle(480, 40, 680, 760, LS_allegro_get_color(BLACK));

	//Linees Verticals Grises
	for(i=80; i<=400; i=i+40){
		al_draw_line(i, 0, i, 760, LS_allegro_get_color(GRAY), 1);
	}
	//Linees Horitzontals Grises
	for(i=1; i<=760; i=i+40){
		al_draw_line(40, i, 440, i, LS_allegro_get_color(GRAY), 1);
	}

	//Info Jugador
	al_draw_textf(LS_allegro_get_font(LARGE),LS_allegro_get_color(WHITE),480,42,0,"%s"," JUGADOR");
	al_draw_textf(LS_allegro_get_font(NORMAL),LS_allegro_get_color(WHITE),490,80,0,"%s", nom_jugador);
	//-------

	//Punts
	al_draw_textf(LS_allegro_get_font(LARGE),LS_allegro_get_color(WHITE),490,160,0,"%s"," PUNTS");
	al_draw_textf(LS_allegro_get_font(NORMAL),LS_allegro_get_color(WHITE),490,200,0,"%d", punts);
	//-------

	//Controls
	al_draw_textf(LS_allegro_get_font(LARGE),LS_allegro_get_color(WHITE),470,240,0,"%s"," CONTROLS");
	//-------

	//Fletxa dreta
	al_draw_filled_rectangle(520, 280, 560, 300, LS_allegro_get_color(WHITE));
	al_draw_filled_triangle( 560, 270, 560, 310, 600, 290, LS_allegro_get_color(WHITE));
	al_draw_textf(LS_allegro_get_font(SMALL),LS_allegro_get_color(WHITE),500,330,0,"%s"," MOVE RIGHT");
	//-------

	//Fletxa esquerra
	al_draw_filled_rectangle(560, 360, 600, 380, LS_allegro_get_color(WHITE));
	al_draw_filled_triangle( 560, 350, 560, 390, 520, 370, LS_allegro_get_color(WHITE));
	al_draw_textf(LS_allegro_get_font(SMALL),LS_allegro_get_color(WHITE),500,410,0,"%s"," MOVE LEFT");
	//-------

	//Fletxa abaix
	al_draw_filled_rectangle(560, 430, 580, 470, LS_allegro_get_color(WHITE));
	al_draw_filled_triangle( 550, 470, 590, 470, 570, 510, LS_allegro_get_color(WHITE));
	al_draw_textf(LS_allegro_get_font(SMALL),LS_allegro_get_color(WHITE),500,530,0,"%s"," MOVE DOWN");
	//-------

	//Order colors
	al_draw_filled_rectangle(500, 570, 620, 610, LS_allegro_get_color(WHITE));
	al_draw_textf(LS_allegro_get_font(SMALL),LS_allegro_get_color(BLACK),500,570,0,"%s"," SPACEBAR");
	al_draw_textf(LS_allegro_get_font(SMALL),LS_allegro_get_color(WHITE),500,630,0,"%s"," ORDER COLORS");
	//-------

	//Exit
	al_draw_filled_rectangle(500, 680, 620, 720, LS_allegro_get_color(WHITE));
	al_draw_textf(LS_allegro_get_font(SMALL),LS_allegro_get_color(BLACK),500,680,0,"%s"," EXIT");
	al_draw_textf(LS_allegro_get_font(SMALL),LS_allegro_get_color(WHITE),500,740,0,"%s"," EXIT");
	//-------

	//Pinta jewels, inicalment negres ja que tauler s'ha inicialitzat a 0, a tota la interficie.
	//En actualitzar tauler, es mostraran els jewels dels colors que calguin.
	for(i=0; i<18 ;i++){
		for(j=0; j<10 ;j++){
			al_draw_filled_circle( (j*40)+60 , (i*40)+60, 19, LS_allegro_get_color( tauler[i][j]));
		}
	}
	//--------------------

}
int iniciPintaJewels(int tauler[18][10], int colors[5], int c[3],int c2[3]){
	/***********************************************
	* @Finalitat: Assignar colors aleatoris als nous jewels.
	* @Parametres: in: colors = array que conté els colors emprats en el joc
								 in: c = array que conté l'index que es farà servir per accedir a colors.
								 in: c2 = array que conté l'index que s'ha fet servir per accedir a colors anteriorment, per no coincidir colors amb els anteriors jewels.
								 in: tauler = array matricial de 2 dimensions que conté el valor dels colors dels jewels en aquelles posicions.
	* @Retorn: Retorna la variable final que determinarà si s'ha arribat al final de la partida o no.
	************************************************/
	int final=1, i, j; //Declaració de les variables emprades en la funció
	srand(time(0)); //Canviem la semilla del rand() per el instant de temps actual. S'ha emprat aquesta eina per evitar que rand generés sempre els mateixos valors.

	//declarem c2 com a c abans de canviar-ho per poder tenir en 2 arrays separats, els colors dels anteriors jewels (c2) i dels nous jewels (c)
	c2[0] = c[0];
	c2[1] = c[1];
	c2[2] = c[2];

	//Comprovo que no hi ha cap jewel a la filera de dalt de tot. En cas afirmatiu canvi-ho la variable final a 2 per que la funció la retorni.
	//Aquesta variable final serà la que dicti si s'ha acabat la partida o no.
	for(i=0; i<10; i++){
			//El if verifica que en cas que trobi un jewel a la filera superior, que aquest sigui dels colors que s'han triat pel joc, per evitar errors.
			if(tauler[0][i] != 0 && (tauler[0][i] == colors[0] || tauler[0][i] == colors[1] || tauler[0][i] == colors[2] || tauler[0][i] == colors[3] || tauler[0][i] == colors[5])){
				final=2;
			}
	}

	//En cas que no s'hagi detectat cap jewel a la filera suprior, es genera un nou index per el array c.
	if( final == 1){
		//Com que ha donat problemes assignar un valor de 0 a c, m'asseguro que mai sigui 0 afegint 1 al resultat del rand.
		c[0] = (rand() % 4) +1;
		c[1] = (rand() % 4) +1;
		c[2] = (rand() % 4) +1;

		//El bucle a continuació s'encarrega de que els colors no siguin iguals entre si i de que no sigui tot idèntic al trio de jewels anterior.
		while( (c[1] == c[0] || c[2] == c[0] || c[2] == c[1]) || (c[1]==c2[1] || c[2] == c2[2] || c[0] == c2[0])){
			c[0] = (rand() % 4) +1;
			c[1] = (rand() % 4) +1;
			c[2] = (rand() % 4) +1;
		}

	}

	return final; //Retorna la variable final. En cas que tot hagi anat bé hauria de ser 1. En cas que la partida hagi de finalitzar serà 2.
}
int buscaTrios(int tauler[18][10], int punts){
	/***********************************************
	* @Finalitat: Buscar trios de jewels en tota la partida
	* @Parametres: in: tauler = array matricial de 2 dimensions que conté el valor dels colors dels jewels en aquelles posicions.
								 in: punts = puntuació de la partida
	* @Retorn: Retorna la puntuació final després d'haver trobat els trios.
	************************************************/
	int i, j, k, c; //Declaració de les variables emprades en la funció

	//El bucle a continuació s'encarrega de recorrer el tauler.
	for(i=17; i>=0 ;i--){
		for(j=9; j>=0 ;j--){
			//Aquest condicional verifica si existeixen jewels als costats del actual que siguin del mateix color, i que aquest no sigui negre.
			//Cercant aixi tots els trios de jewels horitzontals.
			if( (tauler[i][j] == tauler[i][j+1] && tauler[i][j] == tauler[i][j-1]) && tauler[i][j]!=0  ){
				//Un cop detectat un trio, es canvia el color dels jewels detectats a 0
				tauler[i][j] = 0;
				tauler[i][j+1] = 0;
				tauler[i][j-1] = 0;
				punts++; //S'incrementa la puntuació en 1.
				bajaJewels(tauler); //I es crida la funció bajaJewels, encarregada de desplaçar tots els jewels cap a baix, si ho requereixen.
			//Aquesta segon if verifica si existeixen jewels per sobre i per sota del actual que siguin del mateix color, i que aquest no sigui negre.
			//Cercant aixi tots els trios de jewels horitzontals.
			}else{
					if( (tauler[i+1][j] == tauler[i][j] && tauler[i-1][j] == tauler[i][j]) && tauler[i][j]!=0   ){
						//Un cop detectat un trio, es canvia el color dels jewels detectats a 0.
						tauler[i][j] = 0;
						tauler[i-1][j] = 0;
						tauler[i+1][j] = 0;
						punts++; //S'incrementa la puntuació en 1.
						bajaJewels(tauler); //Es crida la funció bajaJewels, encarregada de desplaçar tots els jewels cap a baix, si ho requereixen.
					}
			}
		}
	}

	return punts; //Retorna la puntuació calculad final després d'haver actualitzat el tauler.

}
void bajaJewels(int tauler[18][10]){
	/***********************************************
	* @Finalitat: Baixar tots els jewels que tinguin negre a sota. Es fa servir per canviar el tauler després de trobar trios.
	* @Parametres: in: tauler = array matricial de 2 dimensions que conté el valor dels colors dels jewels en aquelles posicions.
	* @Retorn: ------
	************************************************/
	int i,j; //Declaració de les variables emprades en la funció

	//El bucle a continuació s'encarrega de recorrer el tauler sencer.
	for(i=17; i>=0 ;i--){
		for(j=9; j>=0 ;j--){
			//En cas que es trobin jewels que a sota tenen negre, i aquests no son negres, es desplaçen cap a baix.
			if(tauler[i+1][j] == 0 && tauler[i][j] != 0 && i<18  ){
				tauler[i+1][j] = tauler[i][j];
				tauler[i][j] = 0;
			}
		}
	}

}
void Joc(int v_control[3], char nom_jugador[100], int tauler[18][10], int punts, int jewelMov[6]){
	/***********************************************
	* @Finalitat: La finalitat d'aquesta funció és la de executar el joc.
	* @Parametres: in: v_control = array que conté les variables de control que s'han emprat en el programa.
								 in: nom_jugador = string que conté el nom del jugador.
								 in: tauler = array matricial de 2 dimensions que conté el valor dels colors dels jewels en aquelles posicions.
								 in: punts = punts acumulats en la partida.
								 in: jewelMov = Array que conté la informació de si s'està carregant la partida i, en cas afirmatiu, la informació del jewel en moviment de la partida a carregar.
	* @Retorn: ------
	************************************************/
	 //Declaració de les variables emprades en la funció
	int colors[5] = { 14, 10, 14, 4, 3, 5};
	int falling = 0;
	int i, j, x=5, y=2, temp, k;
	int c[3], c2[3];
	char nom[100];
	v_control[0]=0;

	//El array jewelMov serveix per carregar les dades de el jewel que estava caient en cas que es vulgui carregar partida.
	//Per saber si estem carregant partida o no hi ha un valor guardat a jewelMov[5]. Si aquest val 1 vol dir que s'està carregant partida.
	if(jewelMov[5] == 1){
		//Es per això que extrec els valors de jewelMov i els assigno a les variables pertinents.
		x = jewelMov[0];
		y = jewelMov[1];
		c[0] = jewelMov[2];
		c[1] = jewelMov[3];
		c[2] = jewelMov[4];
		falling = jewelMov[5];
	}

	strcpy(nom, nom_jugador); //Deso en nom el nom del jugador per evitar problemes que s'han experimentat.
	LS_allegro_init(SCREEN_WIDTH,SCREEN_HEIGHT,"VICTOR XIRAU - MAGIC JEWELRY");	//Obro la interficie gràfica
	dissenyInterficie(nom, punts, tauler); //Pinto la interficie gràfica cridant a la funció dissenyInterficie.

	//El bucle a continuació es repeteix mentre v_control[0] sigui 0. Aquest v_control[0] correspon a si s'ha premut escape.
	while(!v_control[0]){
		dissenyInterficie(nom, punts, tauler); //Torno a pintar la interficie gràfica per que es pinti constantment dintre del bucle del joc.
		//La variable falling s'ha emprat per saber si hi ha un trio de jewels baixant o no i per saber si s'ha acabat la partida.
		//Està inicialitzada a 0 pel que en iniciar el programa entra en el if a continuació.
		if( falling == 0){
			falling = iniciPintaJewels(tauler, colors, c, c2); //En entrar al if crido a la funció iniciPintaJewels que retorna el valor de falling.
		}else{ //En cas que no sigui 0
			if(falling == 2){ //Primer verifico si enlloc de ser 0 és 2, que es el valor que s'ha emprat per saber si s'ha acabat la partida.
				//De ser aixi mostro per pantalla un menu on indica que la partida s'ha acabat
				printf("\n\n---------- GAME OVER ----------\n");
				printf("%s - %d punts\n", nom, punts);
				guardarRanking(nom_jugador, punts); //A més, crido la funcio guardarRanking que desa els resultats i printa un missatge de que el ranking s'ha guardat correctament si tot ha anat bé
				printf("Presiona ESC per sortir del joc.\n");
				printf("-------------------------------\n\n");
				v_control[0] = 1; //Canvio les variables de control
			}else{
				//En cas que no sigui ni 0 ni 2, el joc s'executa normalment.
				//Pinta els 3 jewels a la posició x i y assignades.
				//Els colors dels jewels son els colors que corresponguin al index c del array colors.
				al_draw_filled_circle( ((x*40)+60) , ((y-2)*40)+60, 19, LS_allegro_get_color( colors[c[0]] ) );
				al_draw_filled_circle( ((x*40)+60) , ((y-1)*40)+60, 19, LS_allegro_get_color( colors[c[1]] ));
				al_draw_filled_circle( ((x*40)+60) , (y*40)+60, 19, LS_allegro_get_color(  colors[c[2]] ));

				contarTemps(1); //Conto 1 segon

				if(LS_allegro_key_pressed(ALLEGRO_KEY_RIGHT)){ //En cas que es premi la fletxa dreta
					//Si el jewel no està el més a la dreta possible incremento la x
					//També s'ha de complir la condició de que no hi hagi jewels al costat on es preté anar.
					if(x<9 && tauler[y][x+1] == 0 && tauler[y+1][x+1] == 0){
						x++;
					}
				}
				if(LS_allegro_key_pressed(ALLEGRO_KEY_LEFT)){ //En cas que es premi la fletxa esquerra
					//Si el jewel no està el més a l'esquerra possible disminueixo la x.
					//També s'ha de complir la condició de que no hi hagi jewels al costat on es preté anar.
					if(x>0 && tauler[y][x-1] == 0 && tauler[y+1][x-1] == 0){
						x--;
					}
				}
				if(LS_allegro_key_pressed(ALLEGRO_KEY_SPACE)){ //En cas que es premi la tecla espai
					//Realitzo un switch dels valors de c.
					temp = c[0];
					c[0] = c[2];
					c[2] = c[1];
					c[1] = temp;
				}
				if(LS_allegro_key_pressed(ALLEGRO_KEY_DOWN)){ //En cas que es premi la tecla down.
					if(y<17 && tauler[y+1][x] == 0){ //Si la y és inferior a 17, no ha arribat al final, incremento la y.
						y++;
					}
				}
				if(LS_allegro_key_pressed(ALLEGRO_KEY_ESCAPE)){ //En cas que es premi la tecla esc.
					v_control[0] = 1; //Canvio la variable de control[0] a 1
				}
				if(LS_allegro_key_pressed(ALLEGRO_KEY_S)){ //En cas que es premi la tecla s.
					printf("\n\nGuardant partida...\n"); //Es mostra un missatge conforme s'està desant la partida.
					//Actualitzo el array jewelMov per que tingui els valors del jewel que estava caient en aquest instant.
					jewelMov[0] = x;
					jewelMov[1] = y;
					jewelMov[2] = c[0];
					jewelMov[3] = c[1];
					jewelMov[4] = c[2];
					guardarPartida(tauler, punts, nom, jewelMov); //Crido la funció guardarPartida.
				}

				//Aquest condicional verifica si ha arribat al final del seu recorregut. Si ha arribat al final o si el jewel a continuació no es negre.
				if(y==17 || tauler[y+1][x] != 0  ){
					//En cas que hagi arribat al final:
					falling = 0; //Canvio-ho la variable falling per que torni a pintar nous jewels de uns colors nous.
					tauler[y][x] = colors[c[2]]; //Deso en el tauler els jewels que acaben de baixar a la posició on estàven
					tauler[y-1][x] = colors[c[1]];
					tauler[y-2][x] = colors[c[0]];
					y=2; //Torno a inicialitzar la y a dalt de tot
					x= (rand() % 5) + 1 ; //Torno a inicialitzar la x a un valor aleatori a la part superior del mapa. No pren tots els valors per que no començi a un extrem i dificulti la jugabilitat.
					//El següent bucle executa la funció buscaTrios 3 vegades, per buscar fins a 3 trios seguits, si en baixar es van acumulant.
					for(k=3; k>=0 ;k--){
						punts = buscaTrios(tauler, punts); //Crido la funció buscaTrios que retorna els punts que s'han obtingut.
					}
				}else{
					y++; //En cas que no hagi arribat al final, incrementa la y.
				}
			}
		}
	}

	LS_allegro_exit(); //Si s'ha sortit del while, s'ha premut esc, tanca la finestra gràfica.

}
void mostrarRanking(){
		/***********************************************
		* @Finalitat: Llegir el arixu del ranking i mostrar els jugadors en ordre de puntuació.
		* @Parametres: ------
		* @Retorn: ------
		************************************************/
		 //Declaració de les variables emprades en la funció
		FILE *ranking;
		int punts;
		Jugador player[100];
		int i=0, j=0, tmp, k, c;
		char kk, temp2[100], puntuacio[10], kkk[50];
		int total;

		//El format del ranking es: nom-puntuació

		printf("\n\n---------- RANKING ----------\n"); //Mostro un banner de ranking per pantalla.
		ranking = fopen("ranking.txt", "r"); //Obro el arxiu ranking.txt en mode lectura.
		if(ranking == NULL){ //En cas que no s'hagi pogut obrir el arxiu mostro el missatge d'error.
			printf("\nERROR: Per motius desconeguts no es pot obrir el fitxer\n\n");
		}else{  //en cas que el fitxer s'hagi pogut obrir correctament es procedeix a la lectura del ranking.

			//En començar a escriure el codi no havia contat amb que al inici del document de ranking s'indicaria el nombre de persones en el ranking.
			//Per tal de corregir aquest error simplement he ignorat aquesta primera fila i el meu codi ja està adaptat per llegir tot el ranking i mostrar-lo ordenat.
			fscanf(ranking, "%s", kkk);
			kkk[strlen(kkk)] = '\0';
			fscanf(ranking, "%s", kkk);
			kkk[strlen(kkk)] = '\0';



			//Per fer la lectura del ranking, llegim el valor que hi detecta i el desem a player[i].nom
			//Aquesta primera lectura es la prelectura per poder recorrer el arxiu fins al seu final.
			fscanf(ranking, "%s", player[i].nom);
			player[i].nom[strlen(player[i].nom)] = '\0';  //Corregeixo el \n del fgets.
			//El bucle a continuació es repeteix fins arribar al final del arxiu.
			while( !feof(ranking) ){
				k=0; //Inicialitzo la variable auxiliar k.
				//Recorro la lectura fins a el guió perque ho ha llegit com a un conjunt: nom-puntuacio.
				//A continuació procedeixo a separar aquesta lectura en nom i puntuació per separat.
				//Recorro la lectura fins el -
				while(player[i].nom[k]!='-'){
					k++;
				}
				player[i].nom[k] = '\0'; //Canvio el valor del - per un \0 per finalitzar la cadena nom en aquesta posició
				k++; //Igualment augmento k per poder accedir a la puntuació llegida.
				//Recorro la cadena restant fins al \0 original per poder guardar el valor de la puntuació
				for(c=k; player[i].nom[c]!='\0' ;c++){
					puntuacio[c-k] = player[i].nom[c]; //Guardo en el array puntuació, la puntuació detectada.
				}
				player[i].puntuacio = atoi(puntuacio); //Converteixo el array puntuació en integer mitjançant un atoi i la guardo a player[i].puntuacio.
				i++; //Augmento la i per passar a introduir el següent jugador
				j++; //Augmento j per saber quin és el total de jugadors detectats.
				fscanf(ranking, "%s", player[i].nom); //Realitzo la lectura del següent jugador.
				player[i].nom[strlen(player[i].nom)] = '\0'; //Corregeixo el \n del fgets.
			}
			total = j; //Assigno el valor de j a la variable total per ajudar a la comprensió del codi i per poder emprar j.
			//El següent bucle s'encarrega de recorrer totes les dades introduides i les ordena per ordre de puntuació.
			for (i = 0; i < total; i++) {
	            for (j = i + 1; j < total; j++) {
									//En cas que la puntuació del jugador i, que és el anterior al jugador j, sigui inferior es canvia el ordre d'aquestes 2.
	                if (player[i].puntuacio < player[j].puntuacio) {
											//Intercanvio el valor de player[i] per player[j]
	                    tmp = player[i].puntuacio;
											strcpy(temp2, player[i].nom);
	                    player[i].puntuacio = player[j].puntuacio;
											strcpy(player[i].nom, player[j].nom);
	                    player[j].puntuacio = tmp;
											strcpy(player[j].nom, temp2);
	                }
	          }
	  	}
			//El bucle a continuació es repeteix tantes vegades com persones s'ha detectat al ranking.
			for (i = 0; i < total; i++){
				printf("%d. %s --> %d punts\n",i+1, player[i].nom, player[i].puntuacio); //Mostro les persones, que s'han ordenat anteriorment, i la seva puntuació
			}

			fclose(ranking);
			printf("\n\n");
		}




}
void guardarRanking(char nom_jugador[100], int punts){
	/***********************************************
	* @Finalitat: Desar el ranking en un arxiu.
	* @Parametres: in: nom_jugador = string que conté el nom del jugador.
								 in: punts = punts acumulats pel jugador en la partida.
	* @Retorn: ------
	************************************************/
	 //Declaració de les variables emprades en la funció
	FILE *r;

	r = fopen("ranking.txt", "a");
	//Obro el fitxer ranking.txt en mode append per poder afegir el resultat al final del document.
	//Com ja s'ha comentat en mostraRankin, inicalment el codi estava escrit different perque havia entès el enunciat diferent.
	//Simplement afegeix el nou valor al final i en mostrar el ranking la funció mostrarRanking ja se'n encarrega de ordenar-ho.

	if(r != NULL){ //En cas que s'hagi llegit correctament
		fprintf(r, "%s", nom_jugador); //Deso el nom del jugador.
		fprintf(r, "%c", '-'); //Deso el guió que separa els dos valors
		fprintf(r, "%d", punts); //Deso els punts obtinguts.
		fprintf(r, "%c", '\n'); //Afegeixo el \n al final per que les dades s'introdueixin a continuació
		fclose(r); //Tanco el document
		printf("Les dades s'han desat al ranking.\n"); //Mostro un missatge un cop s'han desat les dades al fitxer.
	}else{ //En cas de error obrint l'arxiu es mostra un missatge d'error.
		printf("Hi ha hagut un error guardant la teva posició al ranking.");
	}
}
void guardarPartida(int tauler[18][10], int punts, char nom_player[100], int jewelMov[6]){
	/***********************************************
	* @Finalitat: Guardar la partida per poder carregarla posteriorment en cas que es vulgui.
	* @Parametres: in: tauler = array matricial de 2 dimensions que conté el valor dels colors dels jewels en aquelles posicions.
								 in: punts = punts acumulats pel jugador en la partida.
								 in: nom_jugador = string que conté el nom del jugador.
								 in: jewelMov = Array que conté la informació de si s'està carregant la partida i, en cas afirmatiu, la informació del jewel en moviment de la partida a carregar.
	* @Retorn: ------
	************************************************/
	//Declaració de les variables emprades en la funció
	FILE *p;
	Partida game;
	int i, j, k, n, valor[190];
	//Inicialitzo la cadena game.nom
	strcpy(game.nom, "");

	game.puntos = punts; //Guardo els punts al struct game, al parametre puntos.
	strcpy(game.nom, nom_player); //Deso el nom del jugador al struct game en el parametre nom
	//El bucle a continuació s'encarrega de recorrer totes les posicions del tauler

	//Guardo en el struct la informació rebuda sobre el jewel que estava en moviment en el moment de guardar partida.
	//Aquesta informació estava guardada al struct jewelMov.
	game.x = jewelMov[0];
	game.y = jewelMov[1];
	game.indexColors[0] = jewelMov[2];
	game.indexColors[1] = jewelMov[3];
	game.indexColors[2] = jewelMov[4];

	//El bucle a continuació s'encarrega de desar el tauler actual en el tauler del struct.
	for(i=0; i<18; i++){
		for(j=0; j<10; j++){
			game.tauler[i][j] = tauler[i][j];
		}
	}

	p = fopen("partida.bin", "wb"); //Obro el arxiu partida.bin per escriptura binaria
	if(p == NULL){ //En cas que hi hagi error obrint el arxiu es mostra un missatge de error.
		printf("\n\nError: El fitxer no existeix!\n");
	}else{ //En cas que s'hagi obert correctament es procedeix a desar les dades.
		fwrite(&game, sizeof(game), 1, p); //Deso el struct game al arxiu binari.
		printf("\nPartida Guardada\n"); //Mostro un missatge per indicar que tot ha anat correctament.

		fclose(p); //Tanco el fitxer binari.
	}
}
int cargarPartida(int tauler[18][10], char nom_jugador[100], int v_control[3], int jewelMov[6]){
	/***********************************************
	* @Finalitat: Carregar la partida que s'ha desat anteriorment. En cas que no existeixi avisar-ho.
	* @Parametres: in: tauler = array matricial de 2 dimensions que conté el valor dels colors dels jewels en aquelles posicions.
								 in: nom_jugador = string que conté el nom del jugador.
								 in: v_control = array que s'ha fet servir per guardar un seguit de variables de control.
								 in: jewelMov = Array que conté la informació de si s'està carregant la partida i, en cas afirmatiu, la informació del jewel en moviment de la partida a carregar.
	* @Retorn: Retorna els punts que s'han acumulat en la partida carregada.
	************************************************/
	 //Declaració de les variables emprades en la funció
	FILE *p;
	Partida game;
	int i, j, points = 0;
	v_control[2]=1;

	p = fopen("partida.bin", "rb"); //Obre el arxiu partida.bin per lectura binaria.

	if(p == NULL){ //En cas que hagi un error obrint el arxiu es mostra un missatge d'error.
		printf("\n\nError: El fitxer no existeix!\n");
		v_control[2]=0;
	}else{ //En cas que tot hagi anat bé es llegeixen les dades del arxiu.
		if(feof(p)){
	    printf("\n\nEror: El fitxer esta buit\n");
		}else{
			fread(&game, sizeof(game), 1, p); //Llegeixo les dades del arxiu binari i ho deso al struct game.
			//Deso la informació del struct game a les variables emprades en el programa.
			points = game.puntos; //Deso els puntos del struct game a la variable points.
			//El bucle a continuació recorre tot el tauler.
			for(i=0; i<19 ;i++){
				for(j=0; j<10 ;j++){
					tauler[i][j] = game.tauler[i][j]; //Deso tot el tauler del struct game al tauler emprat al joc.
				}
			}

			strcpy(nom_jugador, game.nom); //Deso el nom del struct game a la variable nom_jugador emprada en tot el programa.

			//Deso en el array jewelMov la informació del jewel que està en moviment per que en carregar la partida segueixi des d'el mateix lloc.
			jewelMov[0] = game.x;
			jewelMov[1] = game.y;
			jewelMov[2] = game.indexColors[0];
			jewelMov[3] = game.indexColors[1];
			jewelMov[4] = game.indexColors[2];
			jewelMov[5] = 1; //Canvi-ho la ultima posició del array per indicar que s'està carregant partida. Més detalls a la funció joc.

		}
		fclose(p); //Tanco el fitxer p.
	}

	return points; //Retorno els punts llegits per carregar-los en el joc.

}

int main(void){

	 //Declaració de les variables emprades en la funció
	int v_control[3]={0,0,0},sortir[3]={0,0, 0}, i, choice[2];
	char nom_jugador[100];
	int tauler[18][10];
	int punts=0, ok=0;
	int jewelMov[6];
	//Inicialitzo jewelMov a els valors per defecte que vull que tingui aquest array.
	//La utilitat del array jewelMov està explicada a la funció joc.
	jewelMov[5] = 0;
	jewelMov[0] = 5;
	jewelMov[1] = 2;

	//El següent bucle es repeteix mentre la variable desada al index 0 de sortir sigui 0.
	//El array sortir s'ha emprat per controlar si es vol sortir dels dos menus.
	while( !sortir[0] ){
		sortir[1]=0;
		menu1(choice); //Crido la funció menu1 i li passo el array choice
    switch(choice[0]){ //En funcio del valor que prengui choice[0] realitzo el switch següent.
        case 1: //En cas de que el valor sigui 1, jugar partida, es mostra un altre menu.
							while( !sortir[1] ){ //Es fa servir el index 1 del array sortir per controlar si es vol sortir d'aquest segon menu.
								menu2(choice); //Crido la funció menu2 i li passo el array choice.
								switch(choice[1]){ //En funció del valor que prengui choice[1] realitzo el switch següent.
									case 1: //Pel primer cas, en que choice[1] val 1, nova partida.

											initPartida(tauler); //Crido la funció initPartida per inicialitzar el tauler.

											printf("\nNom del jugador?: "); //Demano el nom del jugador.
											fflush(stdin);
											fgets(nom_jugador, 100, stdin);
											nom_jugador[strlen(nom_jugador)-1]='\0'; //Elimino el \n de la cadena.

											Joc(v_control, nom_jugador, tauler, punts, jewelMov); //Crido la funció Joc.

											break;
									case 2: //El segon cas, en que choice[1] val 2, correspon a carregar una partida.
											printf("Carregant partida...\n"); //Es mostra un missatge per indicar que s'està carregant la partida
											punts = cargarPartida(tauler, nom_jugador, v_control, jewelMov); //Crido la funció cargarPartida i li assigno el seu valor a punts, ja que retorna els punts de la partida carregadda
											//En cas que no hi hagi hagut cap problema carregant la partida s'inicia el joc.
											//Per realitzar aquest control faig servir l'index 2 del array v_control, array on he guardat algunes variables de control emprades al llarg del codi.
											if(v_control[2]){
												Joc(v_control, nom_jugador, tauler, punts, jewelMov); //Criddo a la funció Joc, ara te el tauler, els punts, i el nom_jugador carregats pel que carregarà la partida pertinent.
											}
											break;
									case 3: //El tercer cas, en que choice[1] val 3, correspon a sortir d'aquest segon menu.
										printf("\n\n"); //Mostro un \n per poder separar els dos menus entre si en sortir d'aquest.
										sortir[1]=1; //Canvio la variable sortir[1] a 1 per que surti del menu2
										break;
								}
							}
							break;
				case 2: //El segon cas, en que choice[0] val 2, correspon a mostrar el ranking
							mostrarRanking(); //Crido a la funció Ranking.
							break;
				case 3: //El tercer cas, en quee choice[0] val 3, correspon a sortir del programa.
							printf("\n\nEns veiem a la seguent partida!\n"); //Mostro un missatge d'acomiaadment
							contarTemps(2); //Conto 2 segons per poder donar temps a llegir aquest missatge.
							sortir[0]=1; //Canvio la variable sortir[0] a 1 per sortir del bucle que fa que el programa segueixi executant-se
				    	break;
    }
	}
	return 0;
}
