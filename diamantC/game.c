#include <stdio.h>
#include "diamond.h"

void printBoard(board_t* b){
    int lignes[5];
    lignes[0]=3;
    lignes[1]=4;
    lignes[2]=3;
    lignes[3]=2;
    lignes[4]=1;

    int nbEspace[5];
    nbEspace[0]=2;
    nbEspace[1]=0;
    nbEspace[2]=2;
    nbEspace[3]=4;
    nbEspace[4]=6;


    int count=0;

    for(int k=0;k<5;k++){
        for(int i=0;i<3;i++){
            for(int j=0;j<lignes[k];j++){
                if(i==0){
                    if(j==0)
                        for(int x=0;x<nbEspace[k];x++){
                            printf(" ");
                    }
                    printf("#---");
                }
                if(i==1){
                    if(j==0){
                        printf("#");
                        printf("\n");
                        for(int x=0;x<nbEspace[k];x++){
                            printf(" ");
                        }
                    }
                    printf("| ");
                }
                if(i==1){
                    if(b->board[count]==VOID_CELL) printf("  ");
                    else if(b->board[count]<=6)printf("\033[34m%d \033[37m",b->board[count]);
                    else{
                        printf("\033[31m%d \033[37m",b->board[count]-6);
                    }
                    count++;
                }
                if(i==2){
                    if(j==0){
                        printf("|");
                        printf("\n");
                        for(int x=0;x<nbEspace[k];x++){
                            printf(" ");
                        }
                    }
                    printf("#---");
                }
            }
        }
        printf("#\n");
    }

}
void exo1(){
    board_t* b = createBoard();
    tree_t* t = createTree();

    int idCellBlue = 1;
    int idCellRed = 0;


    setFirstRedChoice(t,b,idCellRed);
    setFirstBlueChoice(t,b,idCellBlue);
    buildTree(t,b);
    int nbBlueVictories = computeBlueVictories(t->root);
    int nbRedVictories = computeRedVictories(t->root);
    int nbDraws = computeDraws(t->root);

    printf("nb configuration: %d, nb blue victories: %d, nb red victories: %d, nb draws: %d\n",nbConfigurations,nbBlueVictories, nbRedVictories, nbDraws);

}

void playWithTwoFriends(){
    board_t* plateau = createBoard();

    int turn=0;

    char blueToken=1;
    char redToken=7;

    //Lancement de la partie
    printBoard(plateau);
    while(turn<6){
        printf("bleu : %d\n",blueToken);
        printf("rouge : %d\n",redToken);


        //Joueur
        int jouer;
        do{
            printf("Où veux-tu jouer ?\n");
            scanf("%d",&jouer);
        }while(plateau->board[jouer]!=VOID_CELL);
        setPawn(plateau,jouer,blueToken++);


        printBoard(plateau);


        //IA
        do{
            printf("Où veux-tu jouer (ia)?\n");
            scanf("%d",&jouer);
        }while(plateau->board[jouer]!=VOID_CELL);
        setPawn(plateau,jouer,redToken++);


        printBoard(plateau);

        turn++;
    }

    computeScore(plateau);

    if(plateau->redScore<plateau->blueScore)
        printf("\033[31mRed Victories \033[37m\n");
    else if(plateau->blueScore<plateau->redScore)
        printf("\033[34mBlue Victories \033[37m\n");
    else
        printf("Egalité\n");
}

int main(int argc, char** argv){

    printf("  _____  _                                 _   _ \n"
                   " |  __ \\(_)                               | | | |\n"
                   " | |  | |_  __ _ _ __ ___   ___  _ __   __| | | |\n"
                   " | |  | | |/ _` | '_ ` _ \\ / _ \\| '_ \\ / _` | | |\n"
                   " | |__| | | (_| | | | | | | (_) | | | | (_| | |_|\n"
                   " |_____/|_|\\__,_|_| |_| |_|\\___/|_| |_|\\__,_| (_)\n"
                   "                                                 \n");
    int choice=0;
    while(1){
        printf("\n   ############################\n");
        printf("   #            MENU          #\n");
        printf("   ############################\n");
        printf("   # 1) Calcul configuration  #\n");
        printf("   # 2) Jouer avec un ami     #\n");
        printf("   # 3) Jouer avec une IA     #\n");
        printf("   # 0) Quitter               #\n");
        printf("   ############################\n");

        do{
            printf("Choix :");
            scanf("%d",&choice);
        }while(choice<4 && choice>=0);

        switch(choice){
            case 1:
                exo1();
                break;

            case 2:
                playWithTwoFriends();
                break;

            case 3:
                printf("Work in progress...");

            default:
                return 0;
        }

    }
}

