# PENTAMINO
## Présentation du jeu
Le programme est une adaptation du jeu de plateau Pentamino en jeu vidéo.
Le joueur doit remplir une grille avec des pièces qui lui sont imposées. Le jeu fonctionne sur la base du Drag&Drop c'est à dire
que le joueur devra glisser les différentes pièces sur le plateau afin de remplir la grille.

### Comment jouer ?
Le but du jeu est de compléter entièrement la grille de 6 lignes par 10 colonnes. Pour cela, vous disposez de 12 pièces, non pivotables, que vous devez disposer sur la grille en utilisant votre souris. Un clic sur une pièce, et vous vous en saisissez, un deuxième clic à un emplacemnt valide pour cette pièce et vous la déposez sur la grille. Pour vous saisir d'une autre pièce, cliquez à côté de la grille pour redéposer la pièce en haut de la fenêtre. Trois niveaux sont disponibles, tous sont réalisables, vous disposez de la correction du premier niveau dans les fichiers sources. Pour sélectionner un autre niveau, cliquez sur son numéro. Une fois que vous avez complété un niveau, une fenêtre de victoire s'affiche. Si vous voulez tenter un autre niveau, il faut fermer cette fenêtre et relancer le jeu.

## Langages et librairies
Le programme est exclusivement développé en C avec la librairie SDL2 (qu'il convient d'installer).

## Installation et Lancement
Se positionner dans le dossier de votre choix (le jeu sera installé ici).<br>
```
sudo apt-get install libsdl2-dev
git clone https://github.com/romainduc421/pentomino.git
cd Pentomino/
make && ./main
```


