#include <stdio.h>
#include "diamond.h"

int main(int argc, char** argv){

  board_t* b = createBoard();


  tree_t* t = createTree();
  int idCellBlue = 1;
  int idCellRed = 0;  

  //Bug ici
  setFirstBlueChoice(t,b,idCellBlue);
  setFirstRedChoice(t,b,idCellRed);
  buildTree(t,b);
  int nbBlueVictories = computeBlueVictories(t->root);
  int nbRedVictories = computeRedVictories(t->root);
  int nbDraws = computeDraws(t->root);
  printf("nb configuration: %d, nb blue victories: %d, nb red victories: %d, nb draws: %d\n",nbConfigurations,nbBlueVictories, nbRedVictories, nbDraws);
  return 0;
}
