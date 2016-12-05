#include <stdio.h>
#include <stdlib.h>
#include "diamond.h"

int nbConfigurations;

/**********************************
   functions to mangage the board
***********************************/
board_t* createBoard() {
    board_t* b = NULL;
    board_t plateau;
    b=&plateau;
    clearBoard(b);
    for(int k=0;k<13;k++){
        plateau.board[k]=NO_NEIGHBOR;
    }

    // define neighbors for cell 0
    plateau.neighbors[0][0] = 1;
    plateau.neighbors[0][1] = 3;
    plateau.neighbors[0][2] = 4;
    // define neighbors for cell 1
    plateau.neighbors[1][0] = 0;
    plateau.neighbors[1][1] = 2;
    plateau.neighbors[1][2] = 4;
    plateau.neighbors[1][3] = 5;
    // define neighbors for cell 2
    plateau.neighbors[2][0] = 1;
    plateau.neighbors[2][1] = 5;
    plateau.neighbors[2][2] = 6;
    // define neighbors for cell 3
    plateau.neighbors[3][0] = 0;
    plateau.neighbors[3][1] = 4;
    plateau.neighbors[3][2] = 7;
    // define neighbors for cell 4
    plateau.neighbors[4][0] = 0;
    plateau.neighbors[4][1] = 1;
    plateau.neighbors[4][2] = 3;
    plateau.neighbors[4][3] = 5;
    plateau.neighbors[4][4] = 7;
    plateau.neighbors[4][5] = 8;
    // define neighbors for cell 5
    plateau.neighbors[5][0] = 1;
    plateau.neighbors[5][1] = 2;
    plateau.neighbors[5][2] = 4;
    plateau.neighbors[5][3] = 6;
    plateau.neighbors[5][4] = 8;
    plateau.neighbors[5][5] = 9;
    // define neighbors for cell 6
    plateau.neighbors[6][0] = 2;
    plateau.neighbors[6][1] = 5;
    plateau.neighbors[6][2] = 9;
    // define neighbors for cell 7
    plateau.neighbors[7][0] = 3;
    plateau.neighbors[7][1] = 4;
    plateau.neighbors[7][2] = 8;
    plateau.neighbors[7][3] = 10;
    // define neighbors for cell 8
    plateau.neighbors[8][0] = 4;
    plateau.neighbors[8][1] = 5;
    plateau.neighbors[8][2] = 7;
    plateau.neighbors[8][3] = 9;
    plateau.neighbors[8][4] = 10;
    plateau.neighbors[8][5] = 11;
    // define neighbors for cell 9
    plateau.neighbors[9][0] = 5;
    plateau.neighbors[9][1] = 6;
    plateau.neighbors[9][2] = 8;
    plateau.neighbors[9][3] = 11;
    // define neighbors for cell 10
    plateau.neighbors[10][0] = 7;
    plateau.neighbors[10][1] = 8;
    plateau.neighbors[10][2] = 11;
    plateau.neighbors[10][3] = 12;
    // define neighbors for cell 11
    plateau.neighbors[11][0] = 8;
    plateau.neighbors[11][1] = 9;
    plateau.neighbors[11][2] = 10;
    plateau.neighbors[11][3] = 12;
    // define neighbors for cell 12
    plateau.neighbors[12][0] = 10;
    plateau.neighbors[12][1] = 11;

    return b;
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
            if(b->board[b->neighbors[idVoid][i]]<=6){
                b->blueScore += b->board[b->neighbors[idVoid][i]];
            }
            else{
                b->redScore += (b->board[b->neighbors[idVoid][i]]-6);
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

    n->turn=turn;
    n->idCell=idCell;

    node_t** children=NULL;
    if (n->turn == 1){
        node_t** children = malloc(sizeof(node_t)*1);
    }
    else if (n->turn < 12){
        node_t** children = malloc(sizeof(node_t)*(13-turn));
    }

    n->nbChildren=0;
    n->result=NO_RESULT;
    n->children=children;
    return n;
}

node_t* addChild(node_t* n, int idCell) {
    node_t* child = createNode(idCell, n->turn+1);
    n->children[n->nbChildren]=child;
    n->nbChildren+=1;
    return child;
}

/**********************************
   functions to mangage the tree
***********************************/

tree_t* createTree() {

    tree_t* t= malloc(sizeof(tree_t));
    node_t* root = malloc(sizeof(node_t));
    /* A COMPLETER :
     - allouer l'espace mémoire pour un tree_t et mettre son adresse dans t
     - initialiser le champ root
  */
    tree_t tree;
    t = &tree;

    /*************************
        Definiton de root...
  **************************/
    node_t n;
    n.idCell = 1;
    n.turn = 0;
    node_t** c=malloc(sizeof(node_t*));
    n.children=c;
    n.nbChildren=0;

    tree.root=&n;

    return t;
}


void setFirstBlueChoice(tree_t* t, board_t* b, int idCell) {
    //A REFAIRE
/**************************
    Definition de node
**************************/
    node_t node;
    node.idCell=idCell;
    node.turn=1;

    t->root=&node;
    setPawn(b,idCell,1);

}

void setFirstRedChoice(tree_t* t, board_t* b, int idCell) {
    // A REFAIRE
/**************************
    Definition de node
**************************/
    node_t node;
    node.idCell=idCell;
    node.turn=1;

    t->root=&node;
    setPawn(b,idCell,1);

}

void buildTree(tree_t* t, board_t* b) {
    /* A VERIFIER
  */
    nbConfigurations=5;
    node_t n;
    t->root->children[0]=&n;
    computePossibilities(&n,b);
}

void computePossibilities(node_t* n, board_t* b) {

    if(n->turn == 12){
        computeScore(b);
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

}

int computeBlueVictories(node_t* n) {
    int nb = 0;
    if (n->nbChildren==0){
        if(n->result==1){
            nb++;
        }
    }
    else {
        for (int k=0 ; k < n ; k++){
            nb += computeBlueVictories(n->children[k]);
        }
    }
    return nb;
}

int computeRedVictories(node_t* n) {
    int nb = 0;
    if (n->nbChildren==0){
        if(n->result==2){
            nb++;
        }
    }
    else {
        for (int k=0 ; k < n ; k++){
            nb += computeBlueVictories(n->children[k]);
        }
    }
    return nb;
}

int computeDraws(node_t* n) {
    int nb = 0;
    if (n->nbChildren==0){
        if(n->result==0){
            nb++;
        }
    }
    else {
        for (int k=0 ; k < n ; k++){
            nb += computeBlueVictories(n->children[k]);
        }
    }
    return nb;
}