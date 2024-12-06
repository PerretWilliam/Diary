# SAÉ S1.01 - Implémentation de besoin client

> La saé S1.01 consiste en la création d'un programme permettant la gestion d'agendas.
Pour cette SAÉ, la gestion d'agendas a dû être programmée.

---
## Extraire le dossier.
Le code vous est donné dans l'archive source_code.tar.gz, vous devez tout d'abord le décompresser.

tar -xzf source_code.tar.gz

## Contenu
Dans le fichier extrait, vous avez trois dossiers :
  - *src* contenant les fichiers du code C++ : *main.cpp*, *date.cpp* et *aganda.cpp*.
  - *includes* contenant les fichiers d'en-tête : *date.hpp* *aganda.hpp*.
  - *tests* contenant les jeux de tests que nous détaillerons plus bas.
  - *CMakeLists.txt* permettant de crée les fichiers nécessaire à la compilation du programme.

---
## Compilation  

Un cmake est à votre disposition pour créer tous les fichiers permettants la compilation du programme.

Pour Linux : 
```bash
$ cmake
```

Puis, vous devez ensuite compiler le programme avec le Makefile qui vous sera crée.

Pour Linux : 
```bash
$ make
```

Pour Windows
```bash
$ make win
```

Une fois fait, vous devriez avoir soit agenda pour Linux soit agenda.exe pour Windows.

---  

### Le menu d'arrivé
> Lors du lancement du programme, un dossier export sera crée à la racine du projet s'il n'existe pas (dans le cas où vous le supprimez par erreur), il contiendra tous ce qui est exporté par le programme.

Lorsque vous allez lancer le programme pour la première fois, vous arriverez sur ce menu :

Bienvene, que souhaitez-vous faire ?
[1] Crée un nouvel agenda
[2] Charger un agenda
[3] Supprimer un agenda
[q] Quitter

> Lors de la création du titre de votre agenda, vous n'avez pas le droit d'utiliser des '/'.

### Le menu principale
Voici le menu dans lequel vous serez après avoir crée ou chargé l'agenda.

Bienvenue dans l'agenda.
[1] Modifier l'agenda
[2] Afficher l'agenda
[3] Ajouter un événement
[4] Supprimer un événement
[5] Exporter au format HTML
[6] Enregistrer l'agenda
[q] Quitter
> 

Lorsque vous modifiez l'agenda, il ne s'enregistrera que si vous le faites.

> Dans le cas où vous quittez l'application après une modification, il vous sera demandé si oui ou non vous voulez enregistrer l'agenda, faites '1' pour enregistrer.

### Les dates

> Le programme vous indiquera toujours quoi faire et vous affichera des erreurs si la réponse attendut n'est pas la bonne

Saisissez la date de début (jj/mm/aaaa hh:mm) > 21042006 21:59
"Erreur : format de la date incorrect !"

> Vous avez deux possibilités pour la date soit : jj/mm/aaaa hh:mm ou jj mm aaaa hh mm
Saisissez la date de début (jj/mm/aaaa hh:mm) > 21/04/2006 21:59 OU Saisissez la date de début (jj/mm/aaaa hh:mm) > 21 04 2006 21 59

---
## Jeu de tests  
Vous avez à votre disposition, un jeu de tests fait avec *expect*.

Dans un dossier *tests*, trois tests seront à votre disposition : 
  - *test_menu.exp* : Pour le test des menus.
  - *test_evenement.exp* : Pour le test des évènements.
  - *test_file.exp* : Pour le test de la gestion de fichier (sauvegarde, chargement et suppression).

> Tous les tests créeront un fichier {nom du test}.log dans un dossier crée nommé 'log'.
