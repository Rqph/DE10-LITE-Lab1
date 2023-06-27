# DEMAGALHAES_Lab1

# Hardware Description
## Architecture
On souhaite implémenter un chenillard ou "led chaser" sur notre DE10-Lite, pour se faire on commence tout d'abord par décrire l'architecture de notre système.
Il sera composé des éléments suivants : 
* Nios 2
* RAM
* CLK
* LEDs
* Boutons
* Interrupteurs
* JTAG

Les élements seront connectés les uns aux autres à travers le bus Avalon et décriront l'architecture suivante :
![SPRK_default_preset_name_custom – 1](https://user-images.githubusercontent.com/77203492/212548863-949409b8-0408-4952-8b09-1f0e715da83a.png)


## Platform Designer
Réalisons maintenant cette architecture sur **Platform Designer** :
![image](https://user-images.githubusercontent.com/77203492/211787624-8a138f02-0409-4d70-b56d-855e1f2428e8.png)
On peut voir les différentes connections entre les éléments de notre architecture, à noter les connections importantes :

* Les broches **irq** des switchs,buttons et jtag sont reliées à la broche **irq** du NIOS, pour activer les interruptions.
* On peut noter également les broches **s1** des *switchs,buttons et leds* qui sont reliées à la broche **data_master** du NIOS (canal de donnée).
* Dans la configuration des boutons et des switchs, on peut définir comment les interruptions seront détectées, notamment sur les fronts montant/descendant.
Dès que l'architecture du système est bien configurée, on peut assigner les addresses et générer le HDL de notre système.

## Top Level design
Passons désormais à la description du design VHDL :
Sur l'image ci-dessous vous pouvez constater le fichier VHDL qui décrit les entrées sorties du système et le mapping sur le descriptif réalisé sur plateform designer.
![image](https://user-images.githubusercontent.com/77203492/211788882-e9fb8c02-f705-4f8d-902c-ebbc174964a9.png)

## Pin Assignement
Sur les 2 images qui suivent vous retrouverez, l'association entre les entrées sorties et les pin physique du FPGA.
On peut voir les différentes spécifications associées à chaque pin, le niveau de tension etc..

![image](https://user-images.githubusercontent.com/77203492/211789038-0e8fe222-ab16-44d8-8a10-a56bc091b9f2.png)
![image](https://user-images.githubusercontent.com/77203492/211788984-3e8c7cf4-0290-4472-b32b-f233fd7b02e2.png)

# Software Description
Dans cette partie je vais m'attarder sur la description du fonctionnement de chacun des programmes :
## Chenillard Simple
Le chenillard simple comme son nom l'indique, allume automatiquement les LEDs de la façon souhaitée sans pouvoir modifier la vitesse ou arrêter le chenillard.
On utilise la fonction **IOWR_ALTERA_AVALON_PIO_DATA(PIO_BASE,DATA)** Pour allumer les LEDs une à une dans une boucle for.

## Chenillard Bouton (Polling)
Le chenillard bouton fonctionne selon le principe du polling, c'est à dire qu'on va venir interroger continuellement le périphérique pour savoir si le bouton a été pressé ou non. C'est sans doute la façon la moins optimisée, puisqu'on vient surcharger le processeur pour rien. C'est là qu'intervient l'intérêt d'utiliser des interruptions qui vont permettre de déclencher le chenillard seulement lorsque le bouton est pressé. Pour mettre en place ce chenillard, il suffit simplement d'utiliser la fonction suivante : **IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE)** qui permet de lire la valeur du bouton. Ensuite, selon cette valeur, on démarre le chenillard.

## Chenillard Final (Interruptions)
Le chenillard final, pour commencer, j'ai d'abord défini une routine d'interruption pour le bouton, pour être sûr de comprendre le principe et de démarrer le chenillard avec.
La fonction **static void handle_button_interrupts(void context, alt_u32 id)** permet de définir l'interruption du bouton. Elle est appelée lorsque le bouton est pressé.
Et la fonction **static void init_button_pio()** permet d'initialiser le bouton avec son interruption. Il est important qu'elles soient définies en static pour que le compilateur ne les optimise pas afin d'être sûr qu'elles sont utilisées comme prévu.


Ceci réalisé, j'ai d'abord commencé par implémenter les switchs par la méthode du polling, comme vu précédemment, pour définir la vitesse du chenillard.
Pour se faire, j'ai utilisé la même fonction que pour le chenillard bouton, pour lire la valeur des switchs.

_N.B._ **Le Switch[9] sert de Switch reset, il reset le programme à l'état bas.**

J'ai donc, lors de la lecture des switchs, pris en compte ce Switch[9], dans le programme de la vitesse.
Je réalise donc un Switch case sur la valeur des switchs en partant de 0x200 (0b10 0000 0000).
La lecture se faisant ainsi :

* 0b10 0000 0000
* 0b10 0000 0001
* 0b10 0000 0010
* 0b10 0000 0100
* 0b10 0000 1000
* 0b10 0001 0000 pour la dernière vitesse.

Pour les vitesses, on peut régler 5 niveaux de vitesses à l'aide des switchs 1 à 5. Pour qu'une vitesse soit prise en compte, il faut qu'un seul switch soit à l'état haut à la fois.

Ensuite, j'ai ajouté un flag stop pour permettre de démarrer le chenillard avec l'appui sur le bouton et de l'éteindre avec un appui à nouveau.
Le programme n'est peut-être pas le plus optimisé surtout au niveau de la lecture des switchs et du calcul de la vitesse, mais cela suffit pour comprendre le fonctionnement des interruptions.

# Résultat
Voici une courte vidéo démontrant le principe de fonctionnement.

https://user-images.githubusercontent.com/77203492/211807035-a3131f97-3379-4ab3-bd41-5f9093403c06.mp4


# Conclusion
J'ai bien compris le principe et l'intérêt d'utiliser les interruptions, ainsi que l'approche particulière que requièrent les FPGA, en définissant d'abord l'architecture **Hardware** puis la partie **Software** et enfin l'aspect **Compilation** sur le shell de nios. Cela m'a permis de mieux comprendre comment fonctionnent les FPGA et comment les utiliser pour créer des systèmes embarqués efficaces et réactifs.
