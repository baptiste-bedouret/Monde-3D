# TD6-Textures

## 1-Introduction

Dans l'introduction il faut appliquer les fonctions expliquées dans le tp pour créer une texture en OpenGL. Il faut juste savoir ou bien les placer dans le fichier viewer.cpp. Les maillages fournis possèdent déja des coordonnées de textures il faut juste mettre à jour Mesh::draw pour les transmettre aux shaders. On fait comme vtx_color/vtx_normal ect... Puis modifier les fichiers simple.vert et simple.frag .
On obtient pour la première image:

![alt text](mds3d_td6/data/textures/image1.png)

## 2-Placage de texture 

La deuxieme partie consiste à utiliser la texture earth.jpg pour créer un shader qui affiche cette dernière.

![alt text](mds3d_td6/data/textures/nuit.png)


J'ai appliqué une source lumineuse directionnelle pour simuler l'éclairage du soleil. J'ai calculé cela dans le fragment shader.
L'image suivante est la planète terre avec l'effet de lumière:

![alt text](mds3d_td6/data/textures/image2.png)

Dans la deuxième étape, j'ai ajouté une deuxième texture celle de l'athmosphère et j'ai appliqué la fonction mix qui mélange les deux textures. Voici l'image suivante:

![alt text](mds3d_td6/data/textures/image3.png)

Pour la 3eme étape je n'ai pas réussi à afficher la planète vue de nuit dans une troisième unité de texture.

## 3-Filtrage

J'ai eu quelques problèmes pour modifier les variables _minFilter et _magFilter de façon a tester toutes les combinaisons possibles.
La minification accepte les valeurs GL_NEAREST_MIPMAP_NEAREST (.resp LINEAR)  or lorsque j'affichais le damier et appuyais sur les touches correspondantes à ces valeurs je n'avais que du noir. J'ai donc changé les paramètres de SOIL_load_OGL_texture. Finalement, ça fonctionne correctement.

J'ai donc pour GL_NEAREST_MIPMAP_NEAREST l'image suivante:

![alt text](mds3d_td6/data/textures/image4bis.png)

Pour GL_NEAREST_MIPMAP_LINEAR:

![alt text](mds3d_td6/data/textures/image5.png)

Pour GL_LINEAR_MIPMAP_NEAREST:

![alt text](mds3d_td6/data/textures/image6.png)

Pour GL_LINEAR_MIPMAP_LINEAR:

![alt text](mds3d_td6/data/textures/image7.png)

Pour _minFilter = GL_NEAREST:

![alt text](mds3d_td6/data/textures/image8.png)

pas de différence avec l'image8 avec _minFilter = GL_LINEAR;

Pour _magFilter = GL_NEAREST:

![alt text](mds3d_td6/data/textures/image9.png)

Enfin, pour _magFilter = GL_LINEAR:

![alt text](mds3d_td6/data/textures/image10.png)

Pour la deuxième partie, il faut générer procéduralement une texture et sa mipmap. Il fallait donc utiliser le code donné dans l'énoncé et l'ajouter dans Viewer::init.

Ensuite, j'ai appliqué les différents modes de filtre sur modèle de la vache. 
Je trouve clairement deux filtres différents. 

Ainsi pour  _minFilter = GL_NEAREST_MIPMAP_NEAREST et _minFilter = GL_LINEAR_MIPMAP_NEAREST je trouve le même filtre:

![alt text](mds3d_td6/data/textures/upleft.png)

Finalement, pour _minFilter = GL_NEAREST_MIPMAP_LINEAR et _minFilter = GL_LINEAR_MIPMAP_LINEAR je trouve:

![alt text](mds3d_td6/data/textures/downright.png)

Je trouve ce filtre plus en forme de dégradé.
