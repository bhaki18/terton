#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#define RIGHE 20
#define COLONNE 32
#define FPS (1000000/4) // 4fps 
#define MELA 6000
#define BORDO_LATERALE 5000
#define BORDO_ORIZZONTALE 4000
#define LEN_MAX ((RIGHE - 2) * (COLONNE - 2))
void prepara_griglia(char griglia[RIGHE][COLONNE]);
void printa_griglia(char griglia[RIGHE][COLONNE]);
void pulisci_terminale();
void aggiorna_griglia();
void muovi_snake();
void mossa_utente();
void spawna_mela();
void game_loop();
int check_vittoria();
void stampa_vittoria();
void stampa_sconfitta();

int snake_pos[RIGHE][COLONNE];
int snake_x = 1;
int snake_y = 0;
int game_running = 1;
int len = 1;
int input_recived = 0;
int mela_esiste = 0;

char griglia[RIGHE][COLONNE];





// legenda dei numeri di snake_pos:
// 0:(' ') equivale allo spazio vuoto
// 1:('@') equivale alla testa del serpente
// 2:('#') equivale a una parte di mezzo del serpente
// 3:('#') equivale alla coda finale del serpente
// 4000:('=') equivale ai bordi orizzontali della mappa
// 5000:('|') equivale ai bordi laterali della mappa
// 6000:('a') equivale alla mela


int main(){
    game_loop();

return 0;
}

void prepara_griglia(char griglia[RIGHE][COLONNE]){
    for(int i = 0;i<RIGHE;i++){
        for(int j = 0;j<COLONNE;j++){
            snake_pos[i][j] = 0;
            if(i == 0 || i == RIGHE - 1){
                griglia[i][j] = '=';
                snake_pos[i][j] = BORDO_ORIZZONTALE;
            }else{
                if(j == 0 || j == COLONNE - 1){
                    griglia[i][j] = '|';
                    snake_pos[i][j] = BORDO_LATERALE;
                }else{
                    griglia[i][j] = ' ';
                }
            }
        }
    }
    snake_pos[4][4] = 1;
}

void printa_griglia(char griglia[RIGHE][COLONNE]){
    for(int i = 0;i<RIGHE;i++){
        for(int j = 0;j<COLONNE;j++){
            printf("%c",griglia[i][j]);
            if(j==COLONNE-1){
                printf("\n");
            }
        }
    }
}

void pulisci_terminale() {
    printf("\033[2J\033[H");
}

void aggiorna_griglia(){
    for(int i = 0;i<RIGHE;i++){
        for(int j = 0;j<COLONNE;j++){
                if(snake_pos[i][j] == 1){
                griglia[i][j] = '@';
                }else if(snake_pos[i][j] == 2 || snake_pos[i][j] == 3){
                griglia[i][j] = '#';
                }else if(snake_pos[i][j] == BORDO_ORIZZONTALE){
                    griglia[i][j] = '=';
                }else if(snake_pos[i][j] == BORDO_LATERALE){
                    griglia[i][j] = '|';
                }else if(snake_pos[i][j] == 0){
                    griglia[i][j] = ' ';
                }else if(snake_pos[i][j] == MELA){
                    griglia[i][j] = 'a';
                }
        }
    }
}

void muovi_snake(){
    
    int testa_x;
    int testa_y;
    // calcoliamo le coordinate della testa
    for(int i = 0;i<RIGHE;i++){
        for(int j = 0;j<COLONNE;j++){
            if(snake_pos[i][j] == 1){
                testa_x = j;
                testa_y = i;
            }
        }
    }
    // calcolo la posizione futura
    if(snake_x == 1){
        testa_x++;
    }else if(snake_x == -1){
        testa_x--;
    }
    if(snake_y == 1){
        testa_y++;
    }else if(snake_y == -1){
        testa_y--;
    }

    // controllo se lo snake mangia
    if(snake_pos[testa_y][testa_x] == MELA){
        for(int i = 0;i<RIGHE;i++){
            for(int j = 0;j<COLONNE;j++){
                if(snake_pos[i][j] != BORDO_ORIZZONTALE && snake_pos[i][j] != 0 && snake_pos[i][j] != BORDO_LATERALE && snake_pos[i][j] != MELA){
                    snake_pos[i][j]++;
                }
            }
        }
        snake_pos[testa_y][testa_x] = 1;
        mela_esiste = 0;
    }else if(snake_pos[testa_y][testa_x] != 0 && snake_pos[testa_y][testa_x] != MELA){
        game_running = 0;
    }else{
        for(int i = 0;i<RIGHE;i++){
            for(int j = 0;j<COLONNE;j++){
                if(snake_pos[i][j] < BORDO_ORIZZONTALE && snake_pos[i][j] > 0){
                snake_pos[i][j]++;
                }
            }
        }
        snake_pos[testa_y][testa_x] = 1;
        int coda_x = 0;
        int coda_y = 0;
        int temp = 0;
        for(int i = 0;i<RIGHE;i++){
            for(int j = 0;j<COLONNE;j++){
                if(snake_pos[i][j] < BORDO_ORIZZONTALE && snake_pos[i][j] > 0){
                    if(snake_pos[i][j]>temp){
                        temp = snake_pos[i][j];
                        coda_x = j;
                        coda_y = i;
                    }
                }
            }
        }
        snake_pos[coda_y][coda_x] = 0;
    }
    
}

void mossa_utente(){
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);


        int c = getch();

        if (c == 'w' && snake_y == 0 && input_recived == 0){
            snake_y = -1;
            snake_x = 0;
            input_recived = 1;
        }

        if (c == 'a' && snake_x == 0 && input_recived == 0){
            snake_x = -1;
            snake_y = 0;
            input_recived = 1;
        }

        if (c == 's' && snake_y == 0 && input_recived == 0){
            snake_y = 1;
            snake_x = 0;
            input_recived = 1;
        }
        
        if (c == 'd' && snake_x == 0 && input_recived == 0){
            snake_x = 1;
            snake_y = 0;
            input_recived = 1;
        }
        
    

    endwin();
}

void spawna_mela(){
    srand(time(NULL));
    while(!mela_esiste){
    int mela_x = rand() % (COLONNE-2) + 1;
    int mela_y = rand() % (RIGHE - 2) + 1;
        if(snake_pos[mela_y][mela_x] == 0){
            snake_pos[mela_y][mela_x] = MELA;
            mela_esiste = 1;
        }
    }
}


void game_loop(){
    prepara_griglia(griglia);
    printa_griglia(griglia);
    while(game_running){
        spawna_mela();
        input_recived = 0;
        mossa_utente();
        pulisci_terminale();
        mossa_utente();
        muovi_snake();
        aggiorna_griglia();
        printa_griglia(griglia);
        mossa_utente();
        usleep(FPS);
    }
    if(check_vittoria()){
        pulisci_terminale();
        stampa_vittoria();
    }else{
        pulisci_terminale();
        stampa_sconfitta();
    }
}

int check_vittoria(){
    if(len == LEN_MAX){
        return 1;
    }else{
        return 0;
    }
}

void stampa_vittoria(){
    printf(
    "\n"
    "██╗   ██╗ ██████╗ ██╗   ██╗\n"
    "╚██╗ ██╔╝██╔═══██╗██║   ██║\n"
    " ╚████╔╝ ██║   ██║██║   ██║\n"
    "  ╚██╔╝  ██║   ██║██║   ██║\n"
    "   ██║   ╚██████╔╝╚██████╔╝\n"
    "   ╚═╝    ╚═════╝  ╚═════╝ \n"
    "\n"
    "██╗    ██╗ ██████╗ ███╗   ██╗\n"
    "██║    ██║██╔═══██╗████╗  ██║\n"
    "██║ █╗ ██║██║   ██║██╔██╗ ██║\n"
    "██║███╗██║██║   ██║██║╚██╗██║\n"
    "╚███╔███╔╝╚██████╔╝██║ ╚████║\n"
    " ╚══╝╚══╝  ╚═════╝ ╚═╝  ╚═══╝\n"
);
}

void stampa_sconfitta(){
    printf(
    "\n"
    "██╗   ██╗ ██████╗ ██╗   ██╗\n"
    "╚██╗ ██╔╝██╔═══██╗██║   ██║\n"
    " ╚████╔╝ ██║   ██║██║   ██║\n"
    "  ╚██╔╝  ██║   ██║██║   ██║\n"
    "   ██║   ╚██████╔╝╚██████╔╝\n"
    "   ╚═╝    ╚═════╝  ╚═════╝ \n"
    "\n"
    "██╗      ██████╗ ███████╗████████╗\n"
    "██║     ██╔═══██╗██╔════╝╚══██╔══╝\n"
    "██║     ██║   ██║███████╗   ██║   \n"
    "██║     ██║   ██║╚════██║   ██║   \n"
    "███████╗╚██████╔╝███████║   ██║   \n"
    "╚══════╝ ╚═════╝ ╚══════╝   ╚═╝   \n"
);

}