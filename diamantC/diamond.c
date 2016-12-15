#include <stdio.h>
#include <stdlib.h>
#include "diamond.h"

int nbConfigurations;

/**********************************
   functions to mangage the board
***********************************/
board_t* createBoard() {

    board_t* plateau = malloc(sizeof(board_t));

    clearBoard(plateau);
    for(int i=0;i<13;i++)
        for(int j=0;j<6;j++){
            plateau->neighbors[i][j]=NO_NEIGHBOR;
        }

    // define neighbors for cell 0
    plateau->neighbors[0][0] = 1;
    plateau->neighbors[0][1] = 3;
    plateau->neighbors[0][2] = 4;
    // define neighbors for cell 1
    plateau->neighbors[1][0] = 0;
    plateau->neighbors[1][1] = 2;
    plateau->neighbors[1][2] = 4;
    plateau->neighbors[1][3] = 5;
    // define neighbors for cell 2
    plateau->neighbors[2][0] = 1;
    plateau->neighbors[2][1] = 5;
    plateau->neighbors[2][2] = 6;
    // define neighbors for cell 3
    plateau->neighbors[3][0] = 0;
    plateau->neighbors[3][1] = 4;
    plateau->neighbors[3][2] = 7;
    // define neighbors for cell 4
    plateau->neighbors[4][0] = 0;
    plateau->neighbors[4][1] = 1;
    plateau->neighbors[4][2] = 3;
    plateau->neighbors[4][3] = 5;
    plateau->neighbors[4][4] = 7;
    plateau->neighbors[4][5] = 8;
    // define neighbors for cell 5
    plateau->neighbors[5][0] = 1;
    plateau->neighbors[5][1] = 2;
    plateau->neighbors[5][2] = 4;
    plateau->neighbors[5][3] = 6;
    plateau->neighbors[5][4] = 8;
    plateau->neighbors[5][5] = 9;
    // define neighbors for cell 6
    plateau->neighbors[6][0] = 2;
    plateau->neighbors[6][1] = 5;
    plateau->neighbors[6][2] = 9;
    // define neighbors for cell 7
    plateau->neighbors[7][0] = 3;
    plateau->neighbors[7][1] = 4;
    plateau->neighbors[7][2] = 8;
    plateau->neighbors[7][3] = 10;
    // define neighbors for cell 8
    plateau->neighbors[8][0] = 4;
    plateau->neighbors[8][1] = 5;
    plateau->neighbors[8][2] = 7;
    plateau->neighbors[8][3] = 9;
    plateau->neighbors[8][4] = 10;
    plateau->neighbors[8][5] = 11;
    // define neighbors for cell 9
    plateau->neighbors[9][0] = 5;
    plateau->neighbors[9][1] = 6;
    plateau->neighbors[9][2] = 8;
    plateau->neighbors[9][3] = 11;
    // define neighbors for cell 10
    plateau->neighbors[10][0] = 7;
    plateau->neighbors[10][1] = 8;
    plateau->neighbors[10][2] = 11;
    plateau->neighbors[10][3] = 12;
    // define neighbors for cell 11
    plateau->neighbors[11][0] = 8;
    plateau->neighbors[11][1] = 9;
    plateau->neighbors[11][2] = 10;
    plateau->neighbors[11][3] = 12;
    // define neighbors for cell 12
    plateau->neighbors[12][0] = 10;
    plateau->neighbors[12][1] = 11;

    return plateau;
}

void clearBoard(board_t* b) {

    for(int i=0;i<13;i++)b->board[i]=VOID_CELL;
    b->blueScore=0;
    b->redScore=0;

}

int voidCellIndex(board_t* b) {

    int id = -1;
    for(int i=0;i<13;i++)if(b->board[i]==VOID_CELL)id=i;
    return id;
}

void computeScore(board_t* b) {


    b->blueScore=0;
    b->redScore=0;

    int idVoid = voidCellIndex(b);

    for(int i=0;i<6;i++){
        if(b->neighbors[idVoid][i] != NO_NEIGHBOR){
            if(b->board[(int)b->neighbors[idVoid][i]]<=6){
                b->blueScore += b->board[(int)b->neighbors[idVoid][i]];
            }
            else{
                b->redScore += (b->board[(int)b->neighbors[idVoid][i]]-6);

            }
        }
    }
}

void setPawn(board_t* b, int idCell, char value) {

    b->board[idCell] = value;
}


/**********************************
   functions to mangage the nodes
***********************************/

node_t* createNode(int idCell, int turn) {

    node_t* n=malloc(sizeof(node_t));

    n->turn=(char)turn;
    n->idCell=(char)idCell;

    node_t** children;
    if (n->turn == 1){
        children = malloc(sizeof(node_t*)*1);

    }
    else if (n->turn < 12){
        children = malloc(sizeof(node_t*)*(13-turn));

    }else{
        children=NULL;

    }

    n->nbChildren=0;
    n->result=NO_RESULT;
    n->children=children;
    return n;
}

node_t* addChild(node_t* n, int idCell) {

    node_t* child = createNode(idCell,(int)n->turn+1);

    n->nbChildren+=1;

    n->children[(n->nbChildren)-1]=child;

    return child;
}

/**********************************
   functions to mangage the tree
***********************************/

tree_t* createTree() {

    tree_t* t= malloc(sizeof(tree_t));
    t->root=malloc(sizeof(node_t));
    return t;
}


void setFirstBlueChoice(tree_t* t, board_t* b, int idCell) {
/**************************
    Definition de node
**************************/
    t->root=createNode(idCell,1);
    setPawn(b,idCell,1);

}

void setFirstRedChoice(tree_t* t, board_t* b, int idCell) {
/**************************
    Definition de node
**************************/
    addChild(t->root,idCell);
    setPawn(b,idCell,7);
}

void buildTree(tree_t* t, board_t* b) {

    nbConfigurations=0;
    node_t* n = t->root->children[0];
    computePossibilities(n,b);
}

void computePossibilities(node_t* n, board_t* b) {


    if(n->turn == 12){
        computeScore(b);//
        int red = b->redScore;
        int blue = b->blueScore;
        if(blue == red){
            n->result = DRAW_PARTY;
        }
        else if(blue<red){
            n->result = BLUE_WINS;
        }
        else{
            n->result = RED_WINS;
        }
        nbConfigurations+= 1;
        if((nbConfigurations%1000000)==0)printf(".");
        return;
    }
    int nextPawnValue= ((int) n->turn+2)/2;
    if((n->turn+1)%2==0){nextPawnValue+=6;}

    for(int i=0;i<13;i++){
        if(b->board[i]==VOID_CELL){
            setPawn(b,i,(char)nextPawnValue);
            node_t* child = addChild(n,i);

            computePossibilities(child,b);
            setPawn(b,i,VOID_CELL);
        }
    }

}

int computeBlueVictories(node_t* n) {

    int nb = 0;
    if (n->nbChildren==0){
        if(n->result==BLUE_WINS){
            nb++;
        }
    }
    else {
        for (int k=0 ; k < n->nbChildren ; k++){
            nb += computeBlueVictories(n->children[k]);
        }
    }
    return nb;
}

int computeRedVictories(node_t* n) {


    int nb = 0;
    if (n->nbChildren==0){
        if(n->result==RED_WINS){
            nb++;
        }
    }
    else {
        for (int k=0 ; k < n->nbChildren ; k++){
            nb += computeRedVictories(n->children[k]);
        }
    }
    return nb;
}

int computeDraws(node_t* n) {


    int nb = 0;
    if (n->nbChildren==0){
        if(n->result==DRAW_PARTY){
            nb++;
        }
    }
    else {
        for (int k=0 ; k < n->nbChildren ; k++){
            nb += computeDraws(n->children[k]);
        }
    }
    return nb;
}

void freeMemories(node_t* n){
    if(n->nbChildren==0){
        return;
    }else{
        for(int k=0;k<n->nbChildren;k++){
            freeMemories(n->children[k]);
            free(n->children[k]);
            n->children[k]=NULL;
        }
    }
    if(n->turn==1){
        free(n);
    }
}