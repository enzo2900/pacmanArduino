# pacmanArduino 🕹️

Une réadaptation du célèbre jeu Pac-Man développée pour microcontrôleur Arduino. Ce projet intègre une architecture matérielle distribuée (Émetteur/Récepteur) et une logique de jeu programmée en C++.

---

## 🛠️ Stack Technique & Matériel

* **Langage :** C / C++ (Programmation Orientée Objet)
* **Plateforme :** Arduino (Code source `.ino` et bibliothèques `.cpp`/`.h`)

---

## 🏗️ Fonctionnement du Projet

Le projet sépare la gestion des commandes et le moteur de jeu sur deux cartes différentes pour optimiser les performances :

1. **L'Arduino Transmetteur (Commandes) :**
   * Récupère les entrées du joueur via 4 boutons poussoirs.
   * Envoie instantanément les directions via la liaison série.
   * *Câblage :* Boutons Gauche/Droite sur les pins 2 et 3. Boutons Haut/Bas sur les pins 18 et 19.

2. **L'Arduino Receveur (Moteur de Jeu) :**
   * Reçoit les ordres de direction sur son port de communication (Pin 17 RX).
   * Gère la matrice du labyrinthe (`map.cpp`), les collisions et les déplacements de Pac-Man.
   * Intègre un algorithme autonome (`FantomeRechercheChemin.cpp`) pour permettre aux fantômes de traquer le joueur sur la grille.

---

## 🚀 Installation et Configuration

### Câblage de la Communication
* Relier la broche **TX (Pin 16)** de l'Arduino Transmetteur à la broche **RX (Pin 17)** de l'Arduino Receveur.
* Relier les masses (**GND**) des deux cartes ensemble.

### Lancement
1. Cloner le dépôt : `git clone https://github.com`
2. Ouvrir le dossier `commande/` et téléverser le code sur l'Arduino Transmetteur.
3. Ouvrir le fichier principal `pacmanArduino.ino` et le téléverser sur l'Arduino Receveur.
