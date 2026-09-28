#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#define RIGHE 20
#define COLONNE 32
#define FPS 1
void prepara_griglia(char griglia[RIGHE][COLONNE]);
void printa_griglia(char griglia[RIGHE][COLONNE]);
void pulisci_terminale();
void aggiorna_griglia();
void muovi_snake();
void mossa_utente();
void spawna_mela();
void game_loop();

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
                snake_pos[i][j] = 4000;
            }else{
                if(j == 0 || j == COLONNE - 1){
                    griglia[i][j] = '|';
                    snake_pos[i][j] = 5000;
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
                }else if(snake_pos[i][j] == 4000){
                    griglia[i][j] = '=';
                }else if(snake_pos[i][j] == 5000){
                    griglia[i][j] = '|';
                }else if(snake_pos[i][j] == 0){
                    griglia[i][j] = ' ';
                }else if(snake_pos[i][j] == 6000){
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
    if(snake_pos[testa_y][testa_x] == 6000){
        for(int i = 0;i<RIGHE;i++){
            for(int j = 0;j<COLONNE;j++){
                if(snake_pos[i][j] != 4000 && snake_pos[i][j] != 0 && snake_pos[i][j] != 5000 && snake_pos[i][j] != 6000){
                    snake_pos[i][j]++;
                }
            }
        }
        snake_pos[testa_y][testa_x] = 1;
        mela_esiste = 0;
    }else if(snake_pos[testa_y][testa_x] != 0 && snake_pos[testa_y][testa_x] != 6000){
        game_running = 0;
    }else{
        for(int i = 0;i<RIGHE;i++){
            for(int j = 0;j<COLONNE;j++){
                if(snake_pos[i][j] < 4000 && snake_pos[i][j] > 0){
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
                if(snake_pos[i][j] < 4000 && snake_pos[i][j] > 0){
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
    while(!mela_esiste){
        for(int i = 0;i<RIGHE;i++){
            for(int j = 0;j<COLONNE;j++ ){
                if(snake_pos[i][j] == 0 && !mela_esiste){
                    snake_pos[i][j] = 6000;
                    mela_esiste = 1;
                    break;
                    
                }
            }
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
        sleep(FPS);

    }
}