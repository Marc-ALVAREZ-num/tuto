											
# Fonctionnement Zybo


La zybo est un System on Chip (SoC), cela signifie qu'elle est composée d'une partie FPGA et d'une partie uP. Cette SoC est en colaboration avec Xilinx de ce fait nous utilisons Vivado l'IDE 
créé par Xilinx pour développer des code VHDL/Verilog (FPGA). 

Ainsi nous utiliserons Vivado afin d'implémenter le code VHDL sur la partie logique de la SoC puis nous exporterons ce design afin de codé grace à Vitis la partie software.

Version Vivado     : 2020.1
Version Vitis      : 2020.1
carte electronique : Zybo Z7 Zynq-7010


# Démarche à suivre: 
## Créer un Projet:

- Ouvrir vivado
- cliquer sur : Create Project
- next
- nommer le projet puis donner une localisation du dossier 
- choisir : RTL Project
- Pas nécessaire de créer des sources pour le moment
- verifier en bas à droite que dans la mention : Target source, cela soit bien VHDL.
- Pas besoin de créer de fichier contrainte non plus 
- cliquer sur : board        en haut à gauche
- choix de la board utilisée : Zybo Z7-10
- Finish 

A ce moment là, cela ouvre une fentere avec les sources éventuelles que l'on a créé, un résumé des paramètres de notre projet et une console.



Il faut maintenant créer le design FPGA dont on va avoir besoin.

Deux possibilité pour cela:
- Soit nous créons l'entièrete du design en créant des sources puis on les codes en VHDL pour ensuite les implantées dans le design global du projet --> chronophage 
- Soit nous prenons des IPs, propriétés intelectuelles gratuites fournies avec le logiciel. (méthode utilisée ici) 

##### Remarque: 
Dans le cas d'une fonction particulière il est évidemment nécessaire de créer le module par nous même mais 90% du temps il sera possible d'utiliser les IPs.


## Créer un Design    
#### (IP catalog peut être utile à ouvrir)

- Dans IP Integrator, cliquer sur Create Block Design
- Donner un nom au design, la direction de la ou vous voulez le stocker, souvent local to Project est le plus adequat
- une fenetre vide s'ouvre
- appuyer sur + ou sinon clique droit puis "add IP" 
- Rajouter les IPs dont vous avez besoin, nécessairement le 'ZYNQ7 Processing System' qui correspond à la modélisation FPGA du microP (partie PS)
- Une bande verte avec écrit : "Run Connection Automation" apparait cliquer dessus permet de faire les connexions automatiquement entre les differents blocks.
	
##### Remarques: 
en fonction des différents blocks utilisés il sera nécessaire de modifier cette connexion automatique elle permet de gérer les bases des connexions mais pas les particularités du design
		
- Pour modifier certains paramères des IPs double cliquer sur la boite de l'IP correspondant. Les options comme "enable interrupt" sont à modifier manuellement.
			
##### Remarques: 
Pour activer les interruptions de la zynq : double clique sur l'IP zynq, colonne de gauche "interrupts", selectionner "Fabric interrupts", dérouler et choisir lesquels sont nécessaires.
		
- Une fois que le design semble adequat dans la partie source : clique droit sur le design (en orange) puis 'generate output product'
- Ensuite clique droit puis 'Create HDL Wrapper' ( cette partie permet de créer l'equivalent VHDL de votre design)
- Une fenetre s'ouvre, choisir 'Let Vivado manage wrapper and auto-upload'. Cette fonctionalité permet dès lors que vous changer le design alors le wrapper sera changé.
Cette action crée un fichier .vhd dans les sources avec le fichier du design associé indenté dessous.
- Une fois que le design est créer en .vhd; il est possible de voir les signaux entrant et sortant afin de completer le fichier contraintes.	
- En bas de la colonne de gauche cliquer sur "generate Bitstream" 
- Une fenetre s'ouvre indiquant que les étapes précedante n'ont pas été réalisées, cliquer sur YES
- une nouvelle fenetre s'ouvre, ne rien changer et appuyer sur OK
- Une fois que le bitstream est créé en haut à droite il y aura écrit : 

Voila cette étape à permis de créer un design. Il est maintenant possible de voir le design implémenté sur la carte FPGA à l'aide de "open implemented design".

Il est aussi possible, dans la mesure ou nous utiliserions uniquement la partie VHDL de la SoC, d'implémenter le code sur la carte et d'utiliser la carte électronique directement.
Pour cela il, est nécessaire de créer un design "manuellement", c'est à dire que l'on code. Il permettrait de créer le comportement voulu entre les différents signaux.

Néanmoins dans notre cas nous utilisons la puissance de calcul du microprocesseur et nous devons donc coder cette partie là. 



## Créer et compléter un fichier contraintes

Le fichier contraintes permet de lié les signaux créés dans le design et les pins/broches de la carte électronique. C'est la partie FPGA (PL) qui permet de choisir facilement l'emplacement des différents signaux.
		
- Télécharger le fichier contraintes correspondant à la carte Xilinx que l'on a, ici : Zybo-Z7-Master.xdc        
Trouvable sur internet. Ce fichier montre toutes les broches activable sur notre board.
- Pour activer une broche il suffit de décommenter la ligne de code correspondante. 
- Pour lié le signal à la broche il faut récupérer le nom du signal à partir du fichier .vhd précédemment créé et remplacer la partie précédée de "get_ports"

###### exemple: affilier un signal à un switch
###### avant : #set_property -dict { PACKAGE_PIN G15   IOSTANDARD LVCMOS33 } [get_ports { sw[0] }]; #IO_L19N_T3_VREF_35 Sch=sw[0]
###### après :  set_property -dict { PACKAGE_PIN G15   IOSTANDARD LVCMOS33 } [get_ports { mon_signal}]; #IO_L19N_T3_VREF_35 Sch=sw[0]

## Crétion de projet Vitis

- A partir de Vivado cliquer sur file -> export -> export harware -> fixed -> next -> include bitstream
- Donner le nom et l'emplacement du fichier .XSA aisni créé.
- cliquer sur 'Tools' -> launch Vitis IDE
		Cela lance un nouveau logiciel : Vitis IDE
- Une fenetre demande l'emplacement du workspace d'où les fichier Vitis seront stockés
		Il faut maintenant créer la plateforme sur laquelle on va coder, celle précédemment créer avec vivado. 
- File -> new -> platform project 
- nommer la platform créée 
- browse -> choisir le fichier .XSA précédement exporté de Vivado -> Finish
		Si la platform existe déja alors : 'select a platform from repository' -> chosir la plateforme existante dans les choix proposés 
##### Remarque: 
Cette plateforme doit être stocké dan le même workspace pour l'utiliser dans un nouveau projet.

Une fois la plateforme créé il faut créer l'application, c'est l'application qui est le coeur du code du uP.

- File -> new -> application project
- Selectionner la plateforme précédement créée
- Nommer l'application automatiquement le nom du système sera créé comme : nomapplication_system
- next 
- select domain : standalone on ps7_cortexa9_0
- Choisir le template "Hello World" -> Finish
		Un fichier helloworld pour coder en #C apparait. 
		Dans la partie 'Explorer' à gauche de l'écran on retrouve le système, l'application et la plateforme. 
##### Astuce:
Par mesure de simpliciter il est préférable de leur donner des noms cohérent comme:
- Nomprojet_platform
- Nomprojet_system
- Nomprojet_app

De même dans cette partie nous trouverons tous les fichiers .h et .c en lien avec notre uP sélectionner dans les IPs de Vivado. N'ayant pas eu besoin d'importer un fichier .h ou .c supplémentaire je ne sais pas comment en rajouter.

C'est à partir de ces fichiers que l'on trouvera les fonctions créées pour le projet. Il est évident qu'il est toujours possible de ne pas les utiliser et de vouloir créer ses propres fonctions -> chronophage la plupart du temps


# Fonctions importantes:

Dans cette partie je ne chercherais pas à expliquer comment fonctionnent les fonctions mais plutôt expliquer quelles fonctions sont indispensables et auquelle on ne penserait pas naturellement.

Ce qu'il faut savoir et bien comprendre avec Vitis ou la carte Zybo, c'est que tout périphériques ou interfaces doit être initialiser. (Cela est vrai pour n'importe quel système électronique)
Si l'on utilise une interface I2C ou des GPIOs alors il est nécessaire d'utiliser les fonctions:
- XIic_Initialize(); initialisation de l'interface IIC
- XGpio_Initialize(); initialistaion des ports GPIOs

Ensuite une fois que les interfaces sont initialiser il faut se demander si l'on a besoin d'interruptions dans notre processus. Dans notre cas nous en avons besoin en I2C.
Mais nous pourrions en avoir besoin avec les GPIOs aussi en utilisant un bouton 'emergency' par exemple. Pour cela nous utilisons le GIC, General Interrupts Controller, du microprocesseur.
Pour cela il faut l'initialiser et le configurer:

- XScuGic_LookupConfig();
- XScuGic_CfgInitialize();
- InterruptSystemSetup();   Cette fonction je l'ai trouvé en ligne donc la voici ci dessous:
					
voir code [ici](code.h)



Une fois que la configuration du GIC a été effectuée il nous faut connecter un gestionnaire d'interruption au controleur d'interruption (GIC). En effet chaque processus et code étant 
différent il est impossible de coder un gestionnaire commun à toutes les interruptions. De plus certaines fois il est intéressant de pouvoir rajouter ou supprimer des interruptions 
dans un protocol particulier.
Ici comme nous utilisons un protocole connu et déjà employé il est simple de trouver parmis les fonctions fournis un gestionnaire déjà créé: XIic_InterruptHandler();

Pour connecter le GIC au gestionnaire d'interruptions on utilise les fonctions suivantes:
- XScuGic_Connect();
- XScuGic_Enable();

Ces fonctions connectent puis activent des interruptions au GIC.

Finalement une fois que les initialisations sont faites, que les connections sont faites et que le GIC est en route il est possible de démarrer l'interface IIC (ou autres si besoin): 
XIic_Start();

Pour les GPIOs une étape est nécessaire à l'instar du démarrage d'une interface; configurer le GPIO, c'est à dire déterminer si c'est une entrée ou une sortie: 
XGpio_SetDataDirection();


## Approche séquentielle: 
C'est à dire toutes les commandes sont dans la boucle infinie et sont envoyées les unes après les autres.


VOIR CODE [ici](zybo_code/GPIO_sequentielle.h)

Ce code va donc demander une information au STM32 dont l'adresse est 100 ou 0x64 puis récupérer l'info afin d'allumer les LEDs.

Ce code fonctionne bien mais un problème majeur apparait: Il est impossible de faire des décalages temporels sinon le receveur n'est pas sur d'intercepté la demande de message.
Pour cela il faudrait une approche synchrone en machine d'état utilisant des interruptions.

#### ATTENTION:
Les paramètres nécessaires se trouvent principalement dans le fichier "xparameters.h". De plus pour connaitre ce dont on a besoin : clique droit sur la fonction -> open declaration

## Approche synchrone:

VOIR CODE [ici](zybo_code/GPIO_asynchrone.h)
