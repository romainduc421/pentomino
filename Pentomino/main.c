#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "main.h"
#include "sdl_functions.h"
#include <unistd.h>

int main(void) {

  int currentLevel=0;
  changeLevel:;

  bool exit=false;//sortir de la boucle
  SDL_Event event;//évènement SDL par exemple saisie clavier
  SDL_Init(SDL_INIT_VIDEO);
  struct piece SDL_Pieces[12];

  for (int i = 0; i < 12; ++i) {
    SDL_Pieces[i].onGrid = 0;
    for(int j=0;j<NUMBER_PART_PIECE;j++){
      SDL_Pieces[i].rects[j].h = 0;
      SDL_Pieces[i].rects[j].w = 0;
      SDL_Pieces[i].rects[j].x = -100;
      SDL_Pieces[i].rects[j].y = -100;

      SDL_Pieces[i].initialPosition[j].h = 0;
      SDL_Pieces[i].initialPosition[j].w = 0;
      SDL_Pieces[i].initialPosition[j].x = -100;
      SDL_Pieces[i].initialPosition[j].y = -100;
    }
  }

  //création de la fenêtre
  SDL_Window* window=SDL_CreateWindow("Pentomino", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH_SCREEN, HEIGHT_SCREEN, 0);
  if(window == NULL){//Erreur fenetre
    printf("Erreur lors de la creation d'une fenetre : %s",SDL_GetError());
  }
  //Création du renderer
  SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

  if(SDL_Init(SDL_INIT_VIDEO) < 0){// Initialisation de la SDL
    printf("Erreur d’initialisation de la SDL: %s",SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }// Créer la fenêtre

  // Mettre en place un contexte de rendu de l’écran
  int** grid = NULL;
  int isWin=0;
  int (*pieces) [MAX_SIZE][MAX_SIZE] = NULL; //Tableau dynamique de tableau 2D d'int, chaque rang du tableau correspond à une pièce
  char numberLevel[3];

  sprintf(numberLevel,"%d",currentLevel);

  grid = (int**) malloc(COL_GRID*sizeof(int*));
  //*grid = (int**) malloc(row*col*sizeof(int) + row*sizeof(int*));
  for (int i = 0; i < COL_GRID; i++){
    //pas faire de malloc dans une boucle sinon on perd les pointeurs pour free -> fuite mémoire
    //TODO : gestion fuite mémoire
    grid[i] = (int*) malloc(ROW_GRID*sizeof(int));
    for(int j = 0; j < ROW_GRID; j++){
      grid[i][j]=0;
    }
  }

  createPieces(&pieces, numberLevel, &grid);

  SDL_Rect selectedPieceSavedCord[NUMBER_PART_PIECE];
  SDL_Point mousePosition;
  SDL_Point clickedPoint;

  //debug
  /*if(currentLevel!=0){
      for(int i=0;i<12;i++){
          for(int j=0;j<5;j++){
              for(int k=0;k<5;k++){
                  printf("%d",pieces[i][j][k]);
              }
              printf("\n");
          }
      }
  }*/

  int rankPieceSelected = -1;
  struct gridSquare *gridSquares = NULL;
  int isPieceBig = 0;

  SDL_Rect* levels=malloc(getNumberLevel()*sizeof (SDL_Rect));
  SDL_Surface** images=(SDL_Surface**)malloc(getNumberLevel()*sizeof (SDL_Surface*));
  SDL_Texture** textures=(SDL_Texture**)malloc(getNumberLevel()*sizeof(SDL_Texture*));
  int posLevelX=WIDTH_SCREEN*0.1;
  int posLevelY=HEIGHT_SCREEN*0.8;

  char nameFile[5];
  for(int i=0;i<getNumberLevel();i++){
    levels[i].x=posLevelX;
    levels[i].y=posLevelY;
    //posLevelX+=WIDTH_SCREEN*0.1;
    posLevelX+=100;

    levels[i].w=levels[i].h=70;

    memset(nameFile,0,sizeof (nameFile));
    //strcat(nameFile,"");
    nameFile[0]=i+49;
    strcat(nameFile,".bmp");
    images[i]=SDL_LoadBMP(nameFile);
    textures[i]=SDL_CreateTextureFromSurface(renderer,images[i]);
  }

  int flagLevelChoice=0;//faux car on prend le niveau de base -> level0
  while(!exit && !isWin){//boucle principale du jeu
      //TODO: il faut que numberPieces soit dynamique, utiliser la fonction findPiecesNumber() ?

    while(SDL_PollEvent(&event))
      switch(event.type){
        case SDL_QUIT:exit = true;
          break;
        case SDL_KEYDOWN:
          switch(event.key.keysym.sym){
            case SDLK_ESCAPE:
              exit=true;
              break;
          }
          break;

        case SDL_MOUSEMOTION:
          mousePosition.x = event.motion.x;
          mousePosition.y = event.motion.y;
          if(rankPieceSelected != -1){
            SDL_Pieces[rankPieceSelected].rects[0].x = mousePosition.x;
            SDL_Pieces[rankPieceSelected].rects[0].y = mousePosition.y;

            for (int j = 0; j < NUMBER_PART_PIECE; ++j)
            {
              if (selectedPieceSavedCord[j].x < selectedPieceSavedCord[0].x)
              {
                SDL_Pieces[rankPieceSelected].rects[j].x = SDL_Pieces[rankPieceSelected].rects[0].x -
                                      (selectedPieceSavedCord[0].x - selectedPieceSavedCord[j].x);
              } else if (selectedPieceSavedCord[j].x >= selectedPieceSavedCord[0].x) {
                SDL_Pieces[rankPieceSelected].rects[j].x = SDL_Pieces[rankPieceSelected].rects[0].x +
                                      (selectedPieceSavedCord[j].x - selectedPieceSavedCord[0].x);
              }

              if (selectedPieceSavedCord[j].y < selectedPieceSavedCord[0].y) {
                SDL_Pieces[rankPieceSelected].rects[j].y = SDL_Pieces[rankPieceSelected].rects[0].y -
                                      (selectedPieceSavedCord[0].y - selectedPieceSavedCord[j].y);
              } else if (selectedPieceSavedCord[j].y >= selectedPieceSavedCord[0].y) {
                SDL_Pieces[rankPieceSelected].rects[j].y = SDL_Pieces[rankPieceSelected].rects[0].y +
                                      (selectedPieceSavedCord[j].y - selectedPieceSavedCord[0].y);
              }
            }
          }

          if (gridSquares != NULL){
            setGrid(gridSquares, &(SDL_Pieces[rankPieceSelected]), 60, SDL_Pieces[rankPieceSelected].color,
                    rankPieceSelected);
          }

          break;

        case SDL_MOUSEBUTTONDOWN:
          /*printf("Souris : %d,%d\n",event.motion.x,event.motion.y);
          printf(" Rectangle : %d,%d",SDL_Pieces[3].rects[0].x,SDL_Pieces[3].rects[0].y);*/

          //VERIFICATION DE SI LA GRILLE EST REMPLIE
          isWin=1;//true
          for(int i=0;i<ROW_GRID*COL_GRID;i++)
            if(gridSquares[i].pieceOver==-1)//toutes les parties de la grille ne sont pas complets
              isWin=0;//false
          //si isWin reste à 1 on a gagné

          if (rankPieceSelected == -1){
            clickedPoint.x=mousePosition.x;
            clickedPoint.y=mousePosition.y;
            //boutons cliquables
            for(int i=0;i<getNumberLevel();i++){
              if(clickedPoint.x>=levels[i].x && clickedPoint.x<=(levels[i].x+levels[i].w)){
                if(clickedPoint.y>=levels[i].y && clickedPoint.y<=(levels[i].y+levels[i].h)){
                  //on est sur un bouton de niveau
                  if(i!=currentLevel){//on ne peut pas sélectionner le niveau déjà en jeu
                    flagLevelChoice=1;//on choisit un niveau
                    currentLevel=i;//on set le niveau
                    exit=1;
                  }
                }
              }
            }

            for (int i = 0; i < 12; ++i) {
              for (int j = 0; j < NUMBER_PART_PIECE; ++j) {
                if(SDL_PointInRect(&mousePosition,&(SDL_Pieces[i].rects[j]))){
                  rankPieceSelected = i;
                  clickedPoint.y = SDL_Pieces[i].rects[0].y;
                  clickedPoint.x = SDL_Pieces[i].rects[0].x;
                  if (getGridSquareWithPiece(gridSquares,60,SDL_Pieces[rankPieceSelected].rects[0]) != -1){
                    setSizePiece(&(SDL_Pieces[rankPieceSelected]), 1);
                  } else {
                    setSizePiece(&(SDL_Pieces[rankPieceSelected]), 0);
                  }
                  for (int k = 0; k < NUMBER_PART_PIECE; ++k) {
                    selectedPieceSavedCord[k].x = SDL_Pieces[rankPieceSelected].rects[k].x;
                    selectedPieceSavedCord[k].y = SDL_Pieces[rankPieceSelected].rects[k].y;
                  }
                }
              }
            }
          } else {
            putPieceOnGrid(gridSquares,COL_GRID*ROW_GRID,&SDL_Pieces,SDL_Pieces[rankPieceSelected].color,&rankPieceSelected);
          }

          break;
      }

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);//couleur du fond
    SDL_RenderClear(renderer);

    SDL_Rect partDisplayPiece={0,0,WIDTH_SCREEN,HEIGHT_SCREEN/9};//partie haute de l'écran qui contient l'affichage des pièces
    SDL_SetRenderDrawColor(renderer, 125, 125, 125, 0);
    SDL_RenderFillRect(renderer,&partDisplayPiece);

    SDL_Rect partDispalyScore={0,HEIGHT_SCREEN-(HEIGHT_SCREEN/4),WIDTH_SCREEN,HEIGHT_SCREEN/4};//partie basse de la fenêtre, affichage des scores
    SDL_SetRenderDrawColor(renderer, 125, 125, 125, 0);
    SDL_RenderFillRect(renderer,&partDispalyScore);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);//affichage des niveaux
    //TODO contour à faire
    //SDL_RenderFillRects(renderer,levels,getNumberLevel());
    for(int i=0;i<getNumberLevel();i++){
        SDL_RenderCopy(renderer,textures[i],NULL,&levels[i]);
    }

    displayPieces(&window,&SDL_Pieces, &pieces, 12, rankPieceSelected);

    displayGrid(COL_GRID, ROW_GRID, &window, &gridSquares, SDL_Pieces);


    //Définition du texte
    /*char numero[3];
    SDL_Rect text_pos;//position
    text_pos.x=20;
    text_pos.y=HEIGHT_SCREEN-100;*/

    /*fonction inspiré de l'algo de :
    https://waytolearnx.com/2019/09/lister-les-fichiers-dans-un-repertoire-en-c.html*/
    /*char* nameLevel[getNumberLevel()];
    struct dirent *dir;
    DIR *d=opendir("../");
    if (d){
        int i=0;
        while ((dir = readdir(d)) != NULL){
            if(strstr(dir->d_name,"level")!=NULL){//s'il y a un fichier level...
                nameLevel[i]=dir->d_name;
                i++;
            }
        }
        closedir(d);
    }*/

    /*char msg[15]="Niveau ";
    int numberLevel=getNumberLevel();
    for(int i=0;i<numberLevel;i++){
        msg[7]=i+49;//i+49 -> code ascii
        SDL_Texture* text=loadText(msg,renderer,font,color);
        int wText=0;
        int hText=0;
        SDL_QueryTexture(text,NULL,NULL,&wText,&hText);
        text_pos.w=wText;
        text_pos.h=hText;
        SDL_RenderCopy(renderer,text,NULL,&text_pos);//affichage du texte
        text_pos.x+=200;
    }*/

    SDL_RenderPresent(renderer);
  }

  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  SDL_Surface* win=SDL_LoadBMP("../win.bmp");
  SDL_Texture* winText=SDL_CreateTextureFromSurface(renderer,win);

  while(!exit){
      while(SDL_PollEvent(&event)){
          if(event.type==SDL_QUIT){
              exit=true;
          } else if(event.type==SDL_KEYDOWN) {
              if(event.key.keysym.sym==SDLK_ESCAPE){
                  exit=true;
              }
          }
      }

      if(isWin){
          isWin=0;//pour éviter une boucle infinie
          //fenêtre de fin
          SDL_RenderCopy(renderer,winText,NULL,NULL);
          SDL_RenderPresent(renderer);
      }
  }

  SDL_DestroyTexture(winText);
  SDL_FreeSurface(win);

  //TODO libération mémoire free texture & surface
  for(int i=0;i<getNumberLevel();i++){
      SDL_DestroyTexture(textures[i]);
      SDL_FreeSurface(images[i]);
  }
  free(textures);
  free(images);

  free(levels);
  free(pieces);
  for(int i=0;i<COL_GRID;i++){
      free(grid[i]);//alloué dans createPieces
  }
  free(grid);
  free(gridSquares);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  if(flagLevelChoice){//on change de niveau
      goto changeLevel;
  }

  return 0;
}

void createPieces(int (**pieces)[MAX_SIZE][MAX_SIZE], char* levelNumber, int*** grid){

  //ouverture du fichier et allocation mémoire du tableau de pièce
  char fileName[14];
  getFileNameFromLevel(levelNumber, fileName);
  FILE* file = fopen(fileName, "r");
  //TODO
  //soucis avec fileName, il faut pas allouer fileName dynamiquement dans la fonction getFileName..() sinon on perds le pointeur pour libérer ensuite
  *pieces = malloc(sizeof(int[MAX_SIZE][MAX_SIZE])*findPiecesNumber(levelNumber));

  char readChar = 0; //Pour lire le charactère du fichier
  int numPiece = 0; //numéro de la pièce lu

  int endGrid = 0;
  int row = 0;
  int col = 0;

  readChar = fgetc(file);
  while (!endGrid){
    while (readChar != '\n'){
      col++;
      readChar = fgetc(file);
    }
    readChar = fgetc(file);
    if (readChar == '\n'){
      endGrid = 1;
    } else {
      row++;
    }
  }

  readChar = fgetc(file);
  do{
    for (int x = 0; x < MAX_SIZE; ++x) {
      for (int y = 0; y < MAX_SIZE; ++y) {
        if(readChar == '\n'){ // si on arrive à la fin de la ligne
          while(y < MAX_SIZE){//Si on atteint la fin de la ligne et que on a pas atteint MAX_SIZE on rempli par des 0
            (*pieces)[numPiece][x][y] = 0;
            y++;
          }
          readChar = fgetc(file);
          if(readChar == '\n'){ // si on est encore en fin de ligne -> fin de la pièce
            x++;
            while(x < MAX_SIZE){ //on rempli de 0 les lignes restantes.
              for (y = 0; y < MAX_SIZE; ++y) {
                (*pieces)[numPiece][x][y] = 0;
              }
              x++;
            }
            numPiece++; // on passe à la pièce suivante
            readChar = fgetc(file); // on lit la première partie de la pièce suivante
          }

        } else if (readChar == '#'){ // 1 correspond à la présence d'une pièce
          readChar = fgetc(file);
          (*pieces)[numPiece][x][y] = 1;
        } else { // si readChar == ' ' || readChar == EOF
          readChar = fgetc(file);
          (*pieces)[numPiece][x][y] = 0;
        }
      }
    }
  }while (readChar != EOF);

  if(file!=NULL){
    fclose(file);
  }
  else{
    printf("Le fichier n'a pas pu etre ouvert fonction createPieces");
  }
}
/*fonction inspiré de l'algo de :
https://waytolearnx.com/2019/09/lister-les-fichiers-dans-un-repertoire-en-c.html*/
int getNumberLevel(){
  int numberLevel=0;
  struct dirent *dir;
  DIR *d=opendir(".");
  if (d){
    while ((dir = readdir(d)) != NULL){
      if(strstr(dir->d_name,"level")!=NULL){//s'il y a un fichier level...
        numberLevel++;
      }
    }
    closedir(d);
  }
  return numberLevel;
}

//Parcours le fichier pour trouver le nombre de pièces
int findPiecesNumber(char *levelNumber){
  int readChar = 0;
  int nbPiece = 0;
  bool emptyRow = true;
  char fileName[14];
  getFileNameFromLevel(levelNumber,fileName);
  FILE* file = fopen(fileName, "r");

  do {
    readChar = fgetc(file);

    if(readChar == '#'){
      emptyRow = false;
    } else if(readChar == '\n'){
      if(emptyRow){ // si à la fin de la ligne emptyRow = true alors il n'y a pas/plus de pièce.
        nbPiece++;
        emptyRow = false;
      } else {
        emptyRow = true;
      }
    }
  }while(readChar != EOF);

  fclose(file);
  return nbPiece; // Le code compte à partir de la première pièce, inutile de retirer la grille en faisant -1
}

void getFileNameFromLevel(char* levelNumber, char fileName[14]) {
  //concaténation pour le nom du fichier à parcourir
  strcpy(fileName,"level");// ../ car l'exe se créer dans CMakeFiles/
  strcat(fileName, levelNumber);
  strcat(fileName, ".txt");
}
