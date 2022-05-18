# ProjetHuffman
Projet de deuxième année


Compilation d'Huffman
Velay Lucas	Gautier Jordan	   Etrillard Yann
10/12/2021

Informations générales :

Ce logiciel permet de coder et de décoder des fichiers en utilisant l'algorithme d'Huffman, qui est basé le fait que certains caractères apparaissent 
souvent dans un fichier. 
Il s'agit alors de changer la taille mémoire prise par chaque caractère afin qu'il ne soient plus tous coder sur un octet mais sur moins de bits pour les 
caractères qui se répètent beaucoup et sur plus de bits pour ceux qui sont plus rares.


Démarrage :

Afin de démarrer le logiciel, il faut aller dans un éxécutable ou un invité de commandes, puis en premier faire le chemin jusqu'au dossier contenant tous les 
fichiers fournis.
Ensuite, il faut compiler le programme en utilisant la commande : $make all
Après cela, afin de lancer l'éxecutable obtenue, il faut effectuer la commande suivante : ./[chemin]/huffman [option] [fichierA] [fichierB].
Avec le chemin jusqu'au dossier, une option parmi -c, sui permet de compresser le fichier; -d qui permet de décompresser le fichier .hfzip obtenue. 
Ou encore -help qui permet d'afficher les instructions permettant l'éxécution du logiciel.
Enfin, fichierA qui correspond au fichier que l'on souhaite encoder ou décoder et fichierB qui correspond au fichier de sortie, 
fichier qui n'est pas obligé d'être créer avant de lancer l'éxécutable et qui pour la compression ne necessite pas de mettre d'extension puisque les extensions 
des fichiers créés (".hfzip" et ".hftxt") sont ajouté automatiquement.


Exemple :

-compréssion : ./huffman -c harry_potter.txt harry_potter

-décompression : ./huffman -d harry_potter.hfzip harry_potterv2.txt
