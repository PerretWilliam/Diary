# SAÉ S1.01 - Implémentation de besoin client

> Pour cette SAÉ, la gestion d'agendas a dû être programmée.

---

## Extraire le dossier.
Le code vous est donné dans l'archive perret_william_agenda.tar.gz, vous devez tout d'abord le décompresser.

tar -xzf perret_william_agenda.tar.gz

## Contenu
Dans le fichier extrait, vous avez trois dossiers :
  - *src* contenant les fichiers du code C++ : *main.cpp*, *date.cpp* et *aganda.cpp*.
  - *includes* contenant les fichiers d'en-tête : *date.hpp* *aganda.hpp*.
  - *tests* contenant les jeux de tests que nous détaillerons plus bas.
  - *CMakeLists.txt* permettant de crée les fichiers nécessaire à la compilation du programme.

---

## Compilation  

Un cmake est à votre disposition pour créer tous les fichiers permettants la compilation du programme.

Pour Windows et Linux : 
```bash
$ cmake .
```

Pour Linux, vous devez ensuite compiler le programme avec le Makefile qui vous sera crée.

```bash
$ make
```

> Ce qui vous crée un exécutable nommé **agenda**.

Pour Windows, vous devez ouvrir le fichier **agenda.sln** avec Visual Studio et compiler le programme.

---  

### Le menu d'arrivé
> Lors du lancement du programme, un dossier export sera crée à la racine du projet s'il n'existe pas (dans le cas où vous le supprimez par erreur), il contiendra tous ce qui est exporté par le programme.

Lorsque vous allez lancer le programme, vous arriverez sur ce menu :

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

Lorsque vous modifiez l'agenda, il ne s'enregistrera que si vous le faites.

> Dans le cas où vous quittez l'application après une modification, il vous sera demandé si oui ou non vous voulez enregistrer l'agenda, faites '1' pour enregistrer.

### Les dates

> Vous devez respecter ce format pour la date : jj/mm/aaaa hh:mm

Saisissez la date de début (jj/mm/aaaa hh:mm) > 21042006 21:59
> Dans le cas où votre saisi est incorrect, le programme vous le signalera.
"Erreur : format de la date incorrect !"

Saisissez la date de début (jj/mm/aaaa hh:mm) > 21/04/2006 21:59

---

## Jeu de tests  
Tous d'abords, il est important que vous vous situez dans le dossier de test pour lancer les tests.

```bash
cd tests
```

Vous avez à votre disposition, des jeux de tests fait avec *expect*.

Dans un dossier *tests*, trois tests seront à votre disposition : 
  - *test_menu.exp* : Pour le test des menus.
  - *test_evenement.exp* : Pour le test des évènements.
  - *test_file.exp* : Pour le test de la gestion de fichier (sauvegarde, chargement et suppression).

> Tous les tests créeront un fichier {nom du test}.log dans un dossier crée nommé 'log'.
> Sachez que le fichier export sera crée dans le dossier 'tests'.

Pour lancer les tests, vous avez un menu à votre disposition, faite : 

```bash
./launch.sh
```

> Si les tests où le script bash n'ont pas les droits d'exécution, faites : 

```bash
chmod +x launch.sh test_menu.exp test_evenement.exp test_file.exp
```