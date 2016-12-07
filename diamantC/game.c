#include <stdio.h>
#include "diamond.h"

int main(int argc, char** argv){

  board_t* b = createBoard();


  tree_t* t = createTree();
  int idCellBlue = 1;
  int idCellRed = 0;  

  //Bug ici
  setFirstBlueChoice(t,b,idCellBlue);
  perror("main-line-15");
  setFirstRedChoice(t,b,idCellRed);
  perror("main-line-17");
  buildTree(t,b);
  perror("main-line-19");
  int nbBlueVictories = computeBlueVictories(t->root);
  perror("main-line-21");
  int nbRedVictories = computeRedVictories(t->root);
  perror("main-line-22");
  int nbDraws = computeDraws(t->root);
  perror("main-line-25");
  printf("nb configuration: %d, nb blue victories: %d, nb red victories: %d, nb draws: %d\n",nbConfigurations,nbBlueVictories, nbRedVictories, nbDraws);
  return 0;
}
