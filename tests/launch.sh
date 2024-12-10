#!/bin/bash

# Selection menu for tests
echo "Sélectionnez le test que vous voulez lancer :"
echo "1) Test des évènements"
echo "2) Test des fichiers"
echo "3) Test des menus"
echo "4) Quitter"

# Read user choice
read -p "> " choix

# Launch the selected test
case $choix in
  1)
    expect -f ./test_evenement.exp
    ;;
  2)
    expect -f ./test_file.exp
    ;;
  3)
    echo "Lancement du test des menus"
    expect -f ./test_menu.exp
    ;;
  4)
    exit 0
    ;;
  *)
    echo "Choix invalide, veuillez réessayer."
    ;;
esac