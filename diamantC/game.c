#include <stdio.h>
#include <stdlib.h>
#include "diamond.h"
#include <time.h>
void printBoard(board_t* b){
    int lines[5];
    lines[0]=3;
    lines[1]=4;
    lines[2]=3;
    lines[3]=2;
    lines[4]=1;

    int spaceNumber[5];
    spaceNumber[0]=2;
    spaceNumber[1]=0;
    spaceNumber[2]=2;
    spaceNumber[3]=4;
    spaceNumber[4]=6;

    int cellNumber[5];
    cellNumber[0]=0;
    cellNumber[1]=3;
    cellNumber[2]=7;
    cellNumber[3]=10;
    cellNumber[4]=12;

    int count=0;

    printf("\n");

    for(int k=0;k<5;k++){
        for(int i=0;i<3;i++){
            for(int j=0;j<lines[k];j++){
                if(i==0){
                    if(j==0)
                        for(int x=0;x<spaceNumber[k];x++){
                            printf(" ");
                    }
                    printf("#---");
                }
                if(i==1){
                    if(j==0){
                        printf("#");
                        for(int x=0;x<(spaceNumber[k]*2+7);x++){
                            printf(" ");
                        }
                        for (int l = 0; l < lines[k]; ++l) {
                            printf("#---");
                        }
                        printf("#");
                        printf("\n");
                        for(int x=0;x<spaceNumber[k];x++){
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
                        for(int x=0;x<(spaceNumber[k]*2+7);x++){
                            printf(" ");
                        }
                        if (k<3) {
                            for (int l = 0; l < lines[k]; ++l) {
                                printf("| %d ", cellNumber[k] + l);
                            }
                            printf("|");
                        }
                        else{
                            for (int l = 0; l < lines[k]; ++l) {
                                printf("| %d", cellNumber[k] + l);
                            }
                            printf("|");
                        }
                        printf("\n");
                        for(int x=0;x<spaceNumber[k];x++){
                            printf(" ");
                        }
                    }
                    printf("#---");
                }
            }
        }
        printf("#");
        for(int x=0;x<(spaceNumber[k]*2+7);x++){
            printf(" ");
        }
        for (int l = 0; l < lines[k]; ++l) {
            printf("#---");
        }
        printf("#\n");
    }

}
void exo1(){
    board_t* b = createBoard();
    tree_t* t = createTree();

    int idCellBlue = 1;
    int idCellRed = 0;


    setFirstBlueChoice(t,b,idCellBlue);
    setFirstRedChoice(t,b,idCellRed);
    buildTree(t,b);
    int nbBlueVictories = computeBlueVictories(t->root);
    int nbRedVictories = computeRedVictories(t->root);
    int nbDraws = computeDraws(t->root);

    freeMemories(t->root);
    printf("\nnb configuration: %d, nb blue victories: %d, nb red victories: %d, nb draws: %d\n",nbConfigurations,nbBlueVictories, nbRedVictories, nbDraws);

}

int whereEasyAIHasToPlay(board_t* b,int turn){
    unsigned int t=time(NULL);
    int testRand= rand_r(&t)%13;
    return  testRand;
}

void playWithArtificialIntelligenceLevel1(){
    board_t* plateau = createBoard();

    int turn=0;

    char blueToken=1;
    char redToken=7;

    //Lancement de la partie
    printBoard(plateau);
    while(turn<6){

        //Joueur
        int playedCell=-1;
        do{
            printf("Où veux-tu jouer ?\n");
            int test = scanf("%d",&playedCell);
            if(test!=1)exit(0);
        }while( playedCell<0 || playedCell>=13 || plateau->board[playedCell]!=VOID_CELL);

        setPawn(plateau,playedCell,blueToken++);

        //IA
        do{
            playedCell = whereEasyAIHasToPlay(plateau, turn);
        }while(plateau->board[playedCell]!=VOID_CELL);
        printf("Tour de l'IA\n%d\n",playedCell);
        setPawn(plateau,playedCell,redToken++);

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

node_t* whereHardAIHasToPlay(int blueLastPlay,int turn, node_t* actual,board_t* plateau,tree_t* tree){
    if (turn==1){
        setFirstBlueChoice(tree,plateau,blueLastPlay);
        int redPlay;
        do{
            unsigned int t=time(NULL);
            redPlay= rand_r(&t)%13;
        }while(plateau->board[redPlay]!=VOID_CELL);

        setFirstRedChoice(tree,plateau,redPlay);

        return tree->root->children[0];
    }
    else if (turn == 2){
        board_t board=*plateau;
        addChild(actual, blueLastPlay);
        computePossibilities(actual->children[0],&board);
        actual=actual->children[0];
    } else{
        for(int k=0;k<actual->nbChildren;k++) {
            if(actual->children[k]->idCell==blueLastPlay)
                actual=actual->children[k];
        }
    }
    node_t* nextPlay=NULL;
    int nbChanceWin=0;
    int nbChanceDraw=0;
    for(int k=0;k<actual->nbChildren;k++) {
        int nbChanceWinForThisNode = computeRedVictories(actual->children[k]);
        int nbChanceDrawForThisNode = computeDraws(actual->children[k]);
        if (nbChanceWinForThisNode > nbChanceWin) {
            nbChanceWin=nbChanceWinForThisNode;
            nextPlay = actual->children[k];
        }else if(nbChanceWinForThisNode==nbChanceWin && nbChanceDrawForThisNode>nbChanceDraw){
            nbChanceDraw=nbChanceDrawForThisNode;
            nextPlay = actual->children[k];
        }
    }
    if(nextPlay==NULL){
        nextPlay=actual->children[0];
    }
        return nextPlay;

}

void playWithArtificialIntelligenceLevel2(){
    board_t* plateau = createBoard();

    int turn=1;

    node_t* actualPositionOfIA=NULL;

    char blueToken=1;
    char redToken=7;

    tree_t* t = createTree();

    //Lancement de la partie
    printBoard(plateau);
    while(turn<=6){

        //Joueur
        int playedCell=-1;
        do{
            printf("Où veux-tu jouer ?\n");
            int test = scanf("%d",&playedCell);
            if(test!=1)exit(0);
        }while( playedCell<0 || playedCell>=13 || plateau->board[playedCell]!=VOID_CELL);

        setPawn(plateau,playedCell,blueToken++);
        //printBoard(plateau);

        //IA
        actualPositionOfIA=whereHardAIHasToPlay(playedCell,turn,actualPositionOfIA,plateau,t);
        setPawn(plateau,actualPositionOfIA->idCell,redToken++);

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
    //freeMemories(t->root);
}




void playWithTwoFriends(){
    board_t* plateau = createBoard();

    int turn=1;

    char blueToken=1;
    char redToken=7;

    //Lancement de la partie
    printBoard(plateau);
    while(turn<=6){

        //Joueur
        int playedCell=-1;
        do{
            printf("Où veux-tu jouer (j1) ?\n");
            int test = scanf("%d",&playedCell);
            if(test!=1)exit(0);
        }while( playedCell<0 || playedCell>=13 || plateau->board[playedCell]!=VOID_CELL);
        setPawn(plateau,playedCell,blueToken++);

        printBoard(plateau);

        //IA
        playedCell=-1;

        do{
            printf("Où veux-tu jouer (j2) ?\n");
            int test = scanf("%d",&playedCell);
            if(test!=1){
                printf("Erreur de saisie ! Fin du programme...");
                exit(0);
            }
        }while( playedCell<0 || playedCell>=13 || plateau->board[playedCell]!=VOID_CELL);
        setPawn(plateau,playedCell,redToken++);

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

    if(argc!=1){
        printf("Usage : Aucun argument nécessaire pour le lancement de ce programme.\n"
                       "Pour lancer : $ diamond\n");
        exit(-1);
    }

    printf("  _____  _                                 _   _ \n"
                   " |  __ \\(_)                               | | | |\n"
                   " | |  | |_  __ _ _ __ ___   ___  _ __   __| | | |\n"
                   " | |  | | |/ _` | '_ ` _ \\ / _ \\| '_ \\ / _` | | |\n"
                   " | |__| | | (_| | | | | | | (_) | | | | (_| | |_|\n"
                   " |_____/|_|\\__,_|_| |_| |_|\\___/|_| |_|\\__,_| (_)\n"
                   "                                                 \n");
    int choice=0;
    while(1){
        printf("\n   ####################################\n");
        printf("   #               MENU               #\n");
        printf("   ####################################\n");
        printf("   # 1) Calcul configuration          #\n");
        printf("   # 2) Jouer avec un ami             #\n");
        printf("   # 3) Jouer avec une IA facile      #\n");
        printf("   # 4) Jouer avec une IA difficile   #\n");
        printf("   # 0) Quitter                       #\n");
        printf("   ####################################\n");

        do{
            printf("Choix :");
            scanf("%d",&choice);
        }while(choice>5 && choice<0);

        switch(choice){
            case 1:
                exo1();
                break;

            case 2:
                playWithTwoFriends();
                break;

            case 3:
                playWithArtificialIntelligenceLevel1();
                break;

            case 4:
                playWithArtificialIntelligenceLevel2();
                break;

            default:
                return 0;
        }

    }
}

