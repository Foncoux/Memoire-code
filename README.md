# Algorithme Direct Search pour la calibration de modèle épidémiologique

Ce code fonctionne sur des OS basé sur linux. Il devrait également fonctionner sur l'OS d'Apple. Aucune garantie cependant pour Windows.

## outils utilisées
- C++
- CMake
- Un compilateur c++ (testé avec G++ (GCC for c++))

## Librairie utilisée

- [GSL](https://www.gnu.org/software/gsl/) (2.7.1) : pour certaines fonctions mathématiques.
- [NOMAD](https://www.gerad.ca/fr/software/nomad) (4.3.1) : pour l'utilisation de MADS. Il est nécessaire de compiler la librairie en suivant la procédure disponible sur le site (https://nomad-4-user-guide.readthedocs.io/en/v.4.3.1/Installation.html). Le dossier contenant le résultat de la librairie compilée doit se trouver à côté du dossier de ce projet et porter le nom NOMAD_lib. Vous êtes censé avoir ceci : 
- 📁 NOMAD_lib
- 📁 MMMI
  - 📁 sources
  - 📁 headers
  - ...
  - 📄 main.cpp




## Découpe des fichiers

- 📁 config contient la configuration du projet. 
  - 📄 config.cpp définit des tableaux définis globalement.
  - 📄 config.hpp définit l'ensemble des variables préprocesseurs du projet.
  - 📄 setup.cpp définit les variables préprocesseur du projet que l'utilisateur peut modifier.
- 📁 data contient toutes les données utilisées et produites par le projet
- 📁 headers contient tous les fichiers .hpp du projet associés aux fichiers .cpp
- 📁 sources contient tous les fichiers fichiers .cpp
  - 📄 Data.cpp contient les fonctions permettant la lecture, l'écriture et également la création des structures de données pour celle-ci.
  - 📄 fonction_discret.cpp contient les fonctions concernant le modèle SIRQD multi-âge
  - 📄 fonction_obj.cpp contient les fonctions permettant le calcul de la fonction objectif
  - 📄 MCMC.cpp contient les fonctions relatives à MCMC
  - 📄 nomad_file.cpp contient les fonctions relatives à NOMAD
  - 📄 optimisation_algo.cpp contient la fonction permettant le choix d'algo pour MCMC
  - 📄 Parametres.cpp contient les fonctions s'occupant de la génération des paramètres ainsi que la validation de ceux-ci pour MCMC
  - 📄 Stats.cpp contient les fichers s'occupant de la génération des fichiers stats 
- 📄 main.cpp est le point de départ du programme 


### Lancer le programme

Après avoir installé les librairies, les outils et correctement placé le dossier NOMAD_lib.
Placez-vous dans le dossier MMMI et effectuer les commandes suivantes :

```bash
mkdir build
cd build 
cmake .. -DNB_CLASSE_AGE_CMAKE=4 #peut être 1,2,4 ou 8
make
./Main_classe_4 0

```
Noter que l'algo démarrera du point enregistré dans ./data/save_dir