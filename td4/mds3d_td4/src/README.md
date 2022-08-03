
# TD4-Passage à la 3D

 Dans la partie correspondant à la transformation dans le plan x-y j'ai fait en sorte d'affciher les chaises mais j'ai enlevé le code pour pouvoir essentiellement me pencher et travailler sur le td5. J'avais les 3 chaises dans la fenetre graphique dont une où j'appliquais une rotation en son centre.
 Je n'ai malheureusement pas pris de capture d'écran donc je ne peux pas vous montrer le résulat. J'ai tout de même réecrit une partie du travail que j'ai perdu dans le fichier viewer.cpp.

Pour de ce qui est de la partie sur la caméra j'ai normalement tout fais correctement. En revanche, je n'ai pas eu le temps de me pencher sur la partie du système solaire.

# TD5-Éclairage et normales

L'objectif de ce TP est de mettre en place le modèle d'éclairage direct de Blinn-Phong (similaire au TD1) en GLSL.

## 1- Calcul de l'éclairage

1- Dans un premier temps il fallait mettre à jour la fonction Mesh::draw pour transmettre les normales des sommets au shader. Cette fonction a déja été modifié dans le td précédent. En résumé il faut obtenir l'id de l'attribut "vtx_normal" déclaré comme "in vec3 vtx_normal" dans le vertex shader.

2- La deuxième étape nous explique comment mettre en oeuvre la fonction vec3 blinn() évaluant le modèle d'éclairage Blinn-Phong. J' ai donc ajouté cette fonction dans le fragment shader et j'ai appellé cette derniere dans le main de sorte qu'on est: vec3 blinn=blinn(N,v,L,dCol,sCol,s);

3-  Pour pouvoir évaluer la fonction blinn précédente en un sommet donné, nous devons calculer et exprimer les trois premiers vecteurs dans le même repère. Je n'ai pas réussi à afficher la sphère avec le modèle de blinn-Phong. Je n'ai qu'une sphère de couleur rouge sans l'éclairement diffus et spéculaire(voir dossier src). Pourtant, je ne vois pas ou pourrait se trouver le problème. J'ai calculé les trois premiers vecteurs dans le même repère c'est-à-dire v, l et n.

Le vecteur v se calcul à partir de vtx_position + transformation dans l'espace caméra dans le vertex shader (c'est presque comme le calcul de gl_Position sauf qu'il ne faut pas appliquer la projection perspective). Ensuite, pour le vecteur l , j'ai initialisé un vecteur constant dans drawScene() grâce à glUniform3f() et je l'ai appelé dans le vertex shader : uniform vec3 lightPos. Enfin, pour le vecteur normal j'ai utilisé l'attribut de sommet vtx_normal et j'ai fais la transformation avec la matrice normal tel qu'elle est calculé dans drawScene().

## 2- Calcul des normales

Dans la partie 1 on suppose que les normales aux sommets sont inclues dans le fichier du maillage. 
Le but de Mesh::ComputeNormals() est de calculer les normales pour les maillages ne disposant pas de cette information.

Je n'ai pas eu le temps d'écrire cette fonction. 

