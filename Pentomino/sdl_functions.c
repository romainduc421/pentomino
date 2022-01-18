//
// Created by nathand on 31/10/2020.
//
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "sdl_functions.h"

void initColor(struct piece (*piece)[12]){
    (*piece)[0].color.r = 30; (*piece)[0].color.g = 144; (*piece)[0].color.b = 255; //dodgerblue
    (*piece)[1].color.r = 0; (*piece)[1].color.g = 0; (*piece)[1].color.b = 139; //darkblue
    (*piece)[2].color.r = 255; (*piece)[2].color.g = 0; (*piece)[2].color.b = 0; //red
    (*piece)[3].color.r = 219; (*piece)[3].color.g = 112; (*piece)[3].color.b = 147; //palevioletred
    (*piece)[4].color.r = 0; (*piece)[4].color.g = 0; (*piece)[4].color.b = 0; // black
    (*piece)[5].color.r = 255; (*piece)[5].color.g = 69; (*piece)[5].color.b = 0; //orangered
    (*piece)[6].color.r = 255; (*piece)[6].color.g = 0; (*piece)[6].color.b = 255; //fuchsia
    (*piece)[7].color.r = 75; (*piece)[7].color.g = 0; (*piece)[7].color.b = 130; //indigo
    (*piece)[8].color.r = 255; (*piece)[8].color.g = 192; (*piece)[8].color.b = 203; //pink
    (*piece)[9].color.r = 165; (*piece)[9].color.g = 42; (*piece)[9].color.b = 42; //brown
    (*piece)[10].color.r = 222; (*piece)[10].color.g = 184; (*piece)[10].color.b = 135; //burlywood
    (*piece)[11].color.r = 188; (*piece)[11].color.g = 143; (*piece)[11].color.b = 143; //rosybrown
}

void displayPieces(SDL_Window (**window),struct piece (*partPiece)[12],int (**pieces)[NUMBER_PART_PIECE][NUMBER_PART_PIECE],int numberPieces, int rankSelectedPiece)//affiche toutes les pièces
{
    initColor(partPiece);

    //Déclaration
    SDL_Renderer* renderer = SDL_GetRenderer(*window);
    //TODO: malloc à faire pour le nombre de pièce

    //Obtention du renderer à partir de la window
    renderer=SDL_GetRenderer(*window);

    if(renderer==NULL){
        printf("Erreur lors de l'initialisation du renderer : %s", SDL_GetError());
    }

    int shiftOrdinate=10;//décallage des ordonnées
    int shiftAbscissa=20;//décallage des abscisses

    for(int i=0;i<12;i++){//TODO  12 doit être dynamique ici

        SDL_SetRenderDrawColor(renderer,(*partPiece)[i].color.r,(*partPiece)[i].color.g,(*partPiece)[i].color.b,255);//couleur des cases

        if (rankSelectedPiece != i && (*partPiece)[i].onGrid == 0){
            displayPiece(pieces,i,numberPieces,partPiece,shiftOrdinate,shiftAbscissa);
            SDL_RenderFillRects(renderer, (*partPiece)[i].rects, NUMBER_PART_PIECE);
        }
        shiftOrdinate+=0;
        shiftAbscissa+=100;
    }

    if (rankSelectedPiece != -1) {
        for (int k = 0; k < NUMBER_PART_PIECE; ++k) {
            SDL_SetRenderDrawColor(renderer, (*partPiece)[rankSelectedPiece].color.r, (*partPiece)[rankSelectedPiece].color.g, (*partPiece)[rankSelectedPiece].color.b, 0);
            SDL_RenderFillRect(renderer, &((*partPiece)[rankSelectedPiece].rects[k]));
        }
    }
}

void displayPiece(int (**pieces)[NUMBER_PART_PIECE][NUMBER_PART_PIECE],int pieceAfficher,int numberPiece,struct piece(*partPiece)[12],int shiftOrdinate,int shiftAbscissa)//affiche une pièce
{
    int countTemporary=0;
    int countReturnLine=0;//peut être pas indispensable, à voir
    int l=0;//compteur interne pour numéroter partPiece[l]
    bool isEndOfLine=false;
    bool flag=false;//utilisé pour bloquer le while() et passer ou non à la ligne suivante de l'affichage
    int countSpaceInPiece=0;//espace de x blocs entre différentes parties d'une pièce

    for(int i=0;i<NUMBER_PART_PIECE;i++){
        for(int j=0;j<NUMBER_PART_PIECE;j++){
            if(!isEndOfLine){
                if((*pieces)[pieceAfficher][i][j]==1) {//si on doit afficher un carré de couleur
                    if(l==0){//toute première itération, il faut donc set partPiece[0] puisqu'on va ensuite se servir de partPiece[l-1]
                        if(countSpaceInPiece!=0){//si on a une pièce qui commence par un vide en haut à gauche
                            (*partPiece)[pieceAfficher].rects[0].x=j*(SPACING_PX+PIECE_SIZE_PX)+shiftAbscissa;
                            (*partPiece)[pieceAfficher].rects[0].y=0+shiftOrdinate;
                        }else{//on a une pièce qui commence par un plein en haut à gauche
                            (*partPiece)[pieceAfficher].rects[0].x=0+shiftAbscissa;
                            (*partPiece)[pieceAfficher].rects[0].y=0+shiftOrdinate;
                        }
                        (*partPiece)[pieceAfficher].rects[0].w=(*partPiece)[pieceAfficher].rects[0].h=PIECE_SIZE_PX;
                    } else{
                        if(j==0 || countSpaceInPiece!=0){//on est sur une nouvelle ligne
                            //nouvelle ligne -> j==0 ou alors si countSpaceInPice!=0 ça veut dire qu'on a un espace en début de ligne
                            if(j==0){//si c'est vraiment le début d'une ligne
                                (*partPiece)[pieceAfficher].rects[l].x = 0+shiftAbscissa;
                                (*partPiece)[pieceAfficher].rects[l].y = i*(SPACING_PX+PIECE_SIZE_PX)+shiftOrdinate;
                            } else{//si la ligne a déjà commencé (j>0)
                                (*partPiece)[pieceAfficher].rects[l].x = j*(SPACING_PX+PIECE_SIZE_PX)+shiftAbscissa;
                                (*partPiece)[pieceAfficher].rects[l].y = i*(SPACING_PX+PIECE_SIZE_PX)+shiftOrdinate;
                            }
                        } else{//on est pas sur un nouvelle ligne, on continue
                            (*partPiece)[pieceAfficher].rects[l].x = j*(SPACING_PX+PIECE_SIZE_PX)+shiftAbscissa;
                            (*partPiece)[pieceAfficher].rects[l].y = i*(SPACING_PX+PIECE_SIZE_PX)+shiftOrdinate;
                        }
                        (*partPiece)[pieceAfficher].rects[l].w = (*partPiece)[pieceAfficher].rects[l].h = PIECE_SIZE_PX; //taille d'une case de la piece
                    }
                    l++;
                } else{//=0, deux possibilités encore ...
                    countTemporary=j+1;//reboot du compteur temporaire à la case suivante
                    countSpaceInPiece=1;//il y a au moins 1 espace
                    while(countTemporary<NUMBER_PART_PIECE && !flag){//on vérifie qu'il existe encore un 1 dans la ligne
                        if((*pieces)[pieceAfficher][i][countTemporary]==1){
                            isEndOfLine=false;//il existe encore un 1, la ligne n'est pas terminée d'être affichée
                            flag=true;
                        } else{//si ==0
                            countSpaceInPiece++;//on a sauté un espace en plus
                        }
                        countTemporary++;
                    }
                    if(!flag){//si flag reste à faux alors il n'y a aucun 1 dans le restant de la ligne
                        isEndOfLine=true;//c'est donc qu'on doit passer à la ligne suivante
                    }
                    if(isEndOfLine){//... s'il n'y a plus de 1 jusqu'à la fin de la ligne
                        countReturnLine++;
                    }
                }
            }
            //TODO
            (*partPiece)[pieceAfficher].initialPosition[0].x=(*partPiece)[pieceAfficher].rects[0].x;
            (*partPiece)[pieceAfficher].initialPosition[0].y=(*partPiece)[pieceAfficher].rects[0].y;

            (*partPiece)[pieceAfficher].initialPosition[0].w=(*partPiece)[pieceAfficher].rects[0].w;
            (*partPiece)[pieceAfficher].initialPosition[0].h=(*partPiece)[pieceAfficher].rects[0].h;
        }
        countSpaceInPiece=0;//reboot du compteur
        isEndOfLine=false;//il faut la remettre à false puisqu'on passe à la ligne suivante
    }

    //setDisplayPiece(partPiece, pieceAfficher, shiftOrdinate, shiftAbscissa);//réglage de l'affichage des pièces sur le côté gauche de l'écran
}

void setSizePiece(struct piece * piece, int isPieceBig) {
    int ratio = PIECE_SIZE_GRID_PX/PIECE_SIZE_PX;
    if (!isPieceBig){
        for (int i = 0; i < NUMBER_PART_PIECE; ++i) {
            (*piece).rects[i].x *= ratio;
            (*piece).rects[i].y *= ratio;
            (*piece).rects[i].h *= ratio;
            (*piece).rects[i].w *= ratio;
        }
    }
}

void displayGrid(int x, int y, SDL_Window **window, struct gridSquare **grid, struct piece *pieces) {

    //Déclaration
    SDL_Renderer* renderer = SDL_GetRenderer(*window);
    int width = x;
    int gridStartX;
    SDL_GetWindowSize(*window, &gridStartX, NULL);
    int gridSize = x * y;

    gridStartX /= 5;
    if (*grid == NULL){
        *grid = malloc(gridSize * sizeof(struct gridSquare));
        for (int i = 0; i <x*y; ++i) {
            (*grid)[i].color.r = 182;
            (*grid)[i].color.g = 182;
            (*grid)[i].color.b = 182; //couleur de base de la grille
            (*grid)[i].pieceOver = -1;
        }
    }

    (*grid)[0].rect.x = gridStartX;
    (*grid)[0].rect.y = HEIGHT_SCREEN / 8;
    (*grid)[0].rect.w = PIECE_SIZE_GRID_PX;
    (*grid)[0].rect.h = (*grid)[0].rect.w;
    SDL_SetRenderDrawColor(renderer,(*grid)[0].color.r,(*grid)[0].color.g,(*grid)[0].color.b,255); //Couleur des cases du plateau
    SDL_RenderFillRect(renderer,&(*grid)[0].rect);

    for (int i = 1; i < gridSize; ++i) {
        if(i == x){ //retour à la ligne
            x += width;
            (*grid)[i].rect.x = gridStartX;
            (*grid)[i].rect.y = (*grid)[i - 1].rect.h + (*grid)[i - 1].rect.y + SPACING_PX_GRID;
        } else {
            (*grid)[i].rect.x = (*grid)[i - 1].rect.x + (*grid)[i - 1].rect.w + SPACING_PX_GRID;
            (*grid)[i].rect.y = (*grid)[i - 1].rect.y;
        }
        (*grid)[i].rect.w = (*grid)[0].rect.w;
        (*grid)[i].rect.h = (*grid)[0].rect.h;
        
        SDL_SetRenderDrawColor(renderer,(*grid)[i].color.r,(*grid)[i].color.g,(*grid)[i].color.b,255); //Couleur des cases du plateau
        SDL_RenderFillRect(renderer,&(*grid)[i].rect);
    }
}

void setGrid(struct gridSquare *grid, struct piece *selectedPiece, int gridSize, struct color pieceColor,
             int rankSelectedPiece) {

    int gX, gY, gH, gW;
    int cursorOut = 1;
    for (int i = 0; i < gridSize; ++i) {
        gX = grid[i].rect.x;
        gY = grid[i].rect.y;
        gH = grid[i].rect.h;
        gW = grid[i].rect.w;
    if ((*selectedPiece).rects[0].x >= gX && (*selectedPiece).rects[0].x <= gX + gW) {
        if ((*selectedPiece).rects[0].y > gY && (*selectedPiece).rects[0].y < gY + gH) {
            placePiece(grid, gridSize, rankSelectedPiece, selectedPiece, pieceColor);
            cursorOut = 0;
        }
    }
}

    if (cursorOut){
        for (int i = 0; i < gridSize; ++i) {
            if (grid[i].pieceOver == rankSelectedPiece){
                grid[i].pieceOver = -1;
            }
        }
        razGrid(grid, gridSize);
    }
}

void placePiece(struct gridSquare *grid, int gridSize, int rankSelectedPiece, struct piece *selectedPiece, struct color pieceColor) {

    int maxY = 0, maxX = 0, minY = HEIGHT_SCREEN, minX = WIDTH_SCREEN;
    int maxXGrid = grid[gridSize-1].rect.x, maxYGrid = grid[gridSize-1].rect.y, minXGrid = grid[0].rect.x, minYGrid = grid[0].rect.y;
    int deltaX = 0, deltaY = 0;
    for (int i = 0; i < NUMBER_PART_PIECE; ++i) {
        if ((*selectedPiece).rects[i].x > maxX)
            maxX = (*selectedPiece).rects[i].x;

        if ((*selectedPiece).rects[i].y > maxY)
            maxY = (*selectedPiece).rects[i].y;

        if ((*selectedPiece).rects[i].x < minX)
            minX = (*selectedPiece).rects[i].x;

        if ((*selectedPiece).rects[i].y < minY)
            minY = (*selectedPiece).rects[i].y;
    }

    int rankGridSquare = getGridSquareWithPiece(grid,gridSize,(*selectedPiece).rects[0]);
    deltaX = (*selectedPiece).rects[0].x - grid[rankGridSquare].rect.x;
    deltaY = (*selectedPiece).rects[0].y - grid[rankGridSquare].rect.y;
    int allPieceOver = 1;
    int tempX[5];
    int tempY[5];
    for (int i = 0; i < NUMBER_PART_PIECE; ++i) {
        tempX[i] = (*selectedPiece).rects[i].x - deltaX;
        tempY[i] = (*selectedPiece).rects[i].y - deltaY;
        if (tempX[i] < minXGrid || tempX[i] > maxXGrid || tempY[i] < minYGrid || tempY[i] > maxYGrid){
            allPieceOver = 0;
        }
    }

    if (allPieceOver) {
        int slotAlreadyTaken = 0;
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < NUMBER_PART_PIECE; ++j) {
                if(tempX[j] == grid[i].rect.x && tempY[j] == grid[i].rect.y && (grid[i].pieceOver != rankSelectedPiece && grid[i].pieceOver != -1)){
                    slotAlreadyTaken = 1;
                }
            }
        }

        if (!slotAlreadyTaken){
            for (int i = 0; i < NUMBER_PART_PIECE; ++i) {
                (*selectedPiece).rects[i].x = tempX[i];
                (*selectedPiece).rects[i].y = tempY[i];
            }

            for (int i = 0; i < gridSize; ++i) {
                for (int j = 0; j < NUMBER_PART_PIECE; ++j) {
                    if ((*selectedPiece).rects[j].x == grid[i].rect.x && (*selectedPiece).rects[j].y == grid[i].rect.y ){
                        grid[i].color = pieceColor;
                        grid[i].pieceOver = rankSelectedPiece;
                        break;
                    }
                }
            }
        }

    } else {
        for (int i = 0; i < gridSize; ++i) {
            if (grid[i].pieceOver == rankSelectedPiece){
                grid[i].pieceOver = -1;
            }
        }
        razGrid(grid,gridSize);
    }

}

void razGrid(struct gridSquare *grid, int gridSize){
    for (int i = 0; i < gridSize; ++i) {
        if(grid[i].pieceOver == -1){
            grid[i].color.r = 182;
            grid[i].color.g = 182;
            grid[i].color.b = 182;
        }
    }
}

void setDisplayPiece(struct piece(*partPiece)[12], int numPiece, int shiftOrdinate, int abscissa){//réglage de l'affichage d'une pièce sur la partie prévue à cet effet, sinon elle déborde de l'affichage
        if(numPiece%3==0 && numPiece!=0){
                for(int j=0;j<NUMBER_PART_PIECE;j++){//toutes les partie d'une pièce
                    if(j==0){
                        abscissa=(*partPiece)[numPiece].rects[j].x-((numPiece%3)*100);
                    }
                    (*partPiece)[numPiece].rects[j].x-=abscissa;
                    (*partPiece)[numPiece].rects[j].y+=shiftOrdinate;
                }
        }

}

int getGridSquareWithPiece(struct gridSquare *grid, int gridSize ,SDL_Rect piece){
    int res = -1;
    for (int i = 0; i < gridSize; ++i) {
        if (piece.x >= grid[i].rect.x && piece.x <= grid[i].rect.x + grid[i].rect.w){
            if (piece.y >= grid[i].rect.y && piece.y <= grid[i].rect.y + grid[i].rect.h){
                res = i;
            }
        }
    }
    return res;
}

void putPieceOnGrid(struct gridSquare *grid, int gridSize, struct piece (*pieces)[12], struct color color, int *rankPieceSelected) {
    int flag = 0;
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < NUMBER_PART_PIECE; ++j) {
            if ((*pieces)[*rankPieceSelected].rects[j].x == grid[i].rect.x && (*pieces)[*rankPieceSelected].rects[j].y == grid[i].rect.y){//on est sur la grille
                grid[i].color = color;
                grid[i].pieceOver = *rankPieceSelected;
                flag = 1;
            }
        }
    }

    if (flag){
        (*pieces)[*rankPieceSelected].onGrid = 1;
    } else {//on remet l'affichage de la pièce en petit
        for(int i=0;i<NUMBER_PART_PIECE;i++){
            (*pieces)[*rankPieceSelected].rects[i].x = (*pieces)[*rankPieceSelected].initialPosition[i].x;
             (*pieces)[*rankPieceSelected].rects[i].y = (*pieces)[*rankPieceSelected].initialPosition[i].y;

             (*pieces)[*rankPieceSelected].rects[i].h = (*pieces)[*rankPieceSelected].initialPosition[i].h;
             (*pieces)[*rankPieceSelected].rects[i].w = (*pieces)[*rankPieceSelected].initialPosition[i].w;
        }
        (*pieces)[*rankPieceSelected].onGrid = 0;
    }
    *rankPieceSelected = -1;

}
