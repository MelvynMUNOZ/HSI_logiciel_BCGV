# HSI LOGICIEL BCGV

Le BCGV (Boitier Central de Gestion de la Voiture) est responsable du bon fonctionnement du système global : la voiture.
Le BCGV est en charge d’assurer les fonctionnalités
suivantes décrites dans le document de spécification :
- Interagir avec les équipements suivants : Tableau de bord, commodos, Feux, Moteur,
Batterie et Châssis
- Recevoir et traiter les contrôles des feux et des essuis glaces de l’utilisateur
- Recevoir et traiter les problèmes et mesures des autres équipements telles que les
problèmes moteurs, le kilométrage, le niveau du réservoir, ou la charge batterie
- Contrôler les différents feux de la voiture
- Contrôler les voyants du tableau de bord de la voiture


# TABLE DES MATIERES

* [Compilation](#compilation)
* [Utilisation](#utilisation)
* [Explications](#explications)
* [Difficultés](#difficultes)


# COMPILATION

Compilation complète (librairie statique + application)
```
make
```

Compiler la librairie statique
```
make lib
```

Compiler l'application
```
make app
```

Nettoyer le projet
```
make clean
```

Nettoyer l'application
```
make cleanapp
```

Nettoyer la librarie statique
```
make cleanlib
```


# UTILISATION

Pour utiliser le docker de test, se réferrer au fichier `docker/readme.md`.

Lancer le docker de test
```
cd docker
sudo docker-compose up
```

Lancer le driver et l'application
```
chmod +x run.sh
./run.sh
```


# EXPLICATIONS


### Question 1

Nous avons analysé les annexes et les tableaux de représentation des différents messages du système BCGV pour établir les deux tableaux essentiels de notre application :
tableaux des types applicatifs et tableaux des données applicatives.
Chaque type applicatif que nous avons défini répond à une utilisation précise. Prenons l'exemple des types `cmd_t` et `flag_t` : ce sont tous deux des typedef du type primitif `bool` mais leur utilisation diffère dans le programme. Le premier représente les commandes reçues par le COMODO, tandis que le second sert à gérer les états d'activation des voyants ou des sous-systèmes.

Le choix d'un fichier tableur (.xlsx) à été fait pour des raisons de collaboration en ligne sur un drive partagé. Bien que ça ne soit pas la solution la plus pratique pour
l'analyse du fichier et l'extraction des données de ce dernier, elle présente l'avantage d'être facilement lisible et modifiable par les membres de l'équipe.


### Question 2 (Roland Cédric TAYO)

La librairie statique que nous avons réalisée se nomme `bcgv_api.a`. Elle est générée à partir des fichiers `bcgv_api.h` et `bcgv_api.c` qui définissent les types et les données applicatives ainsi que leurs getters et setters. Ces fichiers sont générés via un script python de génération qui prend en entrée notre fichier tableur (.xlsx) et lit successivement les 2 tableaux qu'il contient. On lance ce scripte python dans un environnement virtuel python pour gérer les packages à installer.

Les données applicatives ont été mises dans une structure `context_t` interne au fichier `bcgv_api.c` et donc inaccessible ailleurs. Cette structure représente le contexte de l'application pour son exécution et est un singleton. Une fonction d'initialisation du contexte permet de remplir la structure avec les valeurs initiales pour chacun de ses membres.


### Question 3 (Raphaël CAUSSE)

Avant de coder notre premier main, nous avons défini l'arborescence du projet :
```
projet/
│
├── Makefile
├── README.md
├── run.sh
│
├── app/
│   ├── bin/
│   ├── build/
│   ├── lib/
│   │   └── bcgv_api/
│   │       ├── build/
│   │       ├── include/
│   │       ├── lib/
│   │       ├── src/
│   │       └── Makefile
│   └── src/
│       ├── *.c / *.h
│       └── Makefile
│
├── driver/
│   ├── bin/
│   ├── include/
│   └── lib/
│
└── generator/
  └── .venv/
```

Le code du premier main est simple et évident. Nous avons réalisé un Makefile pour compiler l'application, en se basant sur un Makefile générique fait par un membre de l'équipe et stocké dans un repository github [RaphaelCausse/makefile](https://github.com/RaphaelCausse/makefile/tree/v2). Cela nous a permi de facilement link la librairie statique du driver `drv_api.a` avec les variables suivantes :
```
CPPFLAGS := -I../driver/include
LDFLAGS := -L../driver/lib
LDLIBS := -l:drv_api.a
```


### Question 4

Pour chaque FSM, l'interface visible via chaque fichier header est une fonction d'exécution du FSM qui traite, pour l'état actuel, les transitions d'état possibles à effectuer selon des événements, qui dépendent de conditions sur les données internes et applicatives.


* **State machine des feux classiques (Melvyn MUNOZ)**

Le fsm des feux de position, de croisement et de route permet de gérer leur allumage de façon cohérente par rapport aux autres feux. C'est à dire que lors de l'allumage d'un feu particulier, les autres s'éteignent automatiquement. 
Pour être assurer de dire que le feux est allumé on va envoyer un message encodé au bgf contenant les flags correspondant aux états actuels de tous les feux. Si le retour du bgf est le même message alors l'état de feu allumé est vérifié. Par contre, si le bgf ne renvoie pas le message à l'identique dans les 10 secondes après l'envoi du message par le bcgv alors la commande est considérée comme non prise en compte et on passe à l’état d'erreur.


* **State machine des feux clignotants et warnings (Raphaël CAUSSE)**

Pour gérer les feux clignotants et warnings, on vérifie les commandes reçues par le COMODO et on lève ou non les flags associés, qui représentent les états d’activation de feux clignotants et warnings à envoyer au BGF.
L’acquittement est attendu sous 1 seconde et le cycle ON/OFF se fait toutes les 1 seconde, en comptant le temps d’acquittement. L’acquittement est porté par des bits dans un uint8_t et un masque binaire est réalisé pour savoir si le bit d’acquittement correspondant est présent.


* **State machine des essuie-glace et lave-glace (Roland Cédric TAYO)**

Pour gérer les essuie-glaces et le lave-glace, on implémente une machine à états finis (FSM) qui contrôle les états des essuie-glaces et du lave-glace en fonction des commandes reçues par le COMODO et du chronométrage.
La FSM commence dans l'état ST_INIT où elle initialise tous les systèmes à OFF. Ensuite, elle transite entre les différents états en fonction des commandes reçues et du chronométrage. Par exemple, à partir de ST_ALL_OFF, si une commande pour activer les essuie-glaces est reçue (EV_CMD_WIPER_ON), la FSM passe à l'état ST_WIPER_ON. Si une commande pour activer le lave-glace est reçue dans cet état, la FSM passe à l'état ST_BOTH_ON.
La gestion des transitions et des états assure que les essuie-glaces et le lave-glace fonctionnent correctement selon les commandes utilisateurs et les délais du chronomètre.


### Question 5 (Melvyn MUNOZ)

Le Makefile à la racine du projet nous permet d’appeler les règles du Makefile de l'application et du Makefile de la librairie statique (voir [l’arborescence du projet](#question-3)), tous deux basés sur le Makefile générique [RaphaelCausse/makefile](https://github.com/RaphaelCausse/makefile/tree/v2).


### Question 6

* **Décodage du COMODO (Raphael CAUSSE, Roland Cédric TAYO)**
La trame COMODO est une trame série de 1 octet de données, soit 8 bits. À chaque bit correspond une commande spécifique (feux de position, clignotants, essuie-glaces, etc.). 

Afin de la représenter, nous avons implémenté trois principales opérations : La lecture de la trame (comodo_read_frame_500ms), le décodage de la trame (comodo_decode_frame), et l’affichage de la trame décodée (comodo_print_decoded). 

Pour décoder efficacement la trame, nous avons développé une macro GET_BIT qui extrait chaque bit de l’octet reçu. Cette macro  utilise un masque binaire (1 << bit) pour accéder à des bits individuels dans un octet.


* **Décodage du MUX (Raphael CAUSSE, Roland Cédric TAYO)**

Nous avons utilisé la macro MUX_100MS_GET_UINT32_AT qui utilise un décalage de bits pour extraire des valeurs à la position indiquée en paramètre, dans la trame.
Le module crc8 (crc8.c et crc8.h) a été développée et simplifiée en ce basant sur l’implémentation suivante : https://github.com/lammertb/libcrc/blob/master/src/crc8.c


### Question 7

* **Encodage du BGF (Melvyn MUNOZ)**

Le BGF nous permet de vérifier qu’une commande comme l’allumage de feux ou de clignotant a bien été prise en compte. Pour le BGF, la fonction d’écriture permet de récupérer les valeurs actuelles des flags envoyées pour les feux et les clignotants. Nous comparons les différences grâce aux valeurs des anciens flags que nous avons stocké dans des variables globales. Puis, dans le cas où celles-ci sont différentes de celles précédemment envoyées, alors on écrit un message au BGF. Ce message contiendra toutes les données à modifier. 

La fonction de lecture va récupérer le message que le BGF a renvoyé. Puis elle va comparer les valeurs reçues avec les valeurs envoyées juste avant. Nous avons donc stocké dans des variables globales les valeurs envoyées lors du dernier message du BCGV vers le BGF. Si elles sont identiques alors on set les bits qui vont nous permettre de valider le ack pour les fsm des feux et des clignotants.


* **Encodage du MUX (Raphael CAUSSE)**

Les données applicatives sont pour certaines formaté en Big endian à l’aide des macros MUX_200MS_SET_UINT32_AT et MUX_200MS_SET_UINT16_AT. Pour les bits individuels respectent les spécifications des messages.


### Question 8 (Melvyn MUNOZ, Raphael CAUSSE)

L'implémentation du code principale (main) dans app.c suit un ordre logique et structuré qui correspond étroitement au diagramme fourni : initialisation, attente et vérification des trames UDP, lecture et décodage des trames série, exécution des algorithmes (machines d'état), encodage et écriture des trames UDP et série.
Nous avons fait le choix de ne pas traiter les retours des fonctions principales de chaque système et sous-système, car nous souhaitons limiter l’impact des logs sur les performances, en ne faisant les logs que dans les couches basses. 


# DIFFICULTES

Les principales difficultés rencontrées ont été avec les interfaces séries du driver, notamment pour l’envoi et la réception de message du BGF. Lors des tests avec docker, nous ne sommes pas parvenu à afficher les états des feux sur le dashboard. Notre piste d’investigation nous à mené à penser que le BGF ne recevait pas nos trames séries et qu’il n’envoyait pas de message en réponse, pour l’acquittement. Nous ne pensons pas avoir eu une parfaite compréhension et interprétation de l’API du driver pour les communications séries.


# AUTEURS

Raphael CAUSSE – Melvyn MUNOZ – Roland Cédric TAYO

CY TECH ING3 INEM 2024-2025
