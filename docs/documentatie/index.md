# Inleiding

AI (Artificiële Intelligentie) en ML (Machine Learning) nemen een steeds
prominentere plaats in binnen de IT-wereld. Door velen worden deze
technologieën gezien als de toekomst. Andere industrieën zien het
voordeel ervan in en beginnen het steeds meer toe te passen. Veel
processen kunnen immers geautomatiseerd, versneld of op andere manieren
verbeterd worden met deze technologieën. Dit resulteerde de afgelopen
jaren dan ook in een stijgende vraag naar AI-oplossingen.

De vraag uit deze andere industrieën is steeds complexer en specifieker.
Als resultaat ontwikkeld de IT-wereld complexere algoritmes om aan deze
vraag te kunne voldoen. Deze complexere algoritmes vragen om meer en
betere rekenkracht van de machines die ze draaien. Om te voorkomen dat
de tijd die er nodig is om dit alles te verwerken exponentieel groeit,
doet men onderzoek naar geoptimaliseerde hardware. Dit onderzoek naar
optimalisatie van hardware resulteerde in de ontwikkeling van TPU’s
(Tensor Processing Units).

Achterliggend hebben heel veel AI en ML algoritmes matrix berekeningen
als basis. Deze matrix berekeningen kunnen geoptimaliseerd worden in de
hardware. De architectuur van een processor kan aangepast worden zodat
deze matrixberekeningen sneller gedaan worden. Als resultaat is worden
berekeningen die op een TPU gedaan worden nog sneller volbracht als dat
deze op een CPU of GPU gedaan worden. Deze TPU’s worden door bedrijven
verkocht in de vorm van een minicomputer. Dit is zodat ze gebruikt
kunnen worden binnen IOT-oplossingen, in deze gevallen wordt er
gesproken over edgeTPU’s.

In dit onderzoek willen wij nog een stap verder gaan in deze
optimalisatie. We willen namelijk een cluster opstellen van deze
edge-TPU's

**Het doel van de cluster**

Door het maken van een beowulf cluster van TPU’s willen we een omgeving
voorzien die om kan gaan met de vraag naar meer rekenkracht voor AI en
ML software. We willen erin te slagen een AI-trainingsalgoritme te
verdelen over de verschillende TPU’s binnen de cluster, en op deze
manier sneller tot een resultaat te komen.

Omdat het onderzoek naar AI-hardware relatief nieuw is, is er veel nog
niet ontdekt of gedaan. Met dit onderzoek komen we in nieuw territorium.
De enige referentie die we terugvinden naar een cluster van TPU’s, is
een project van google[^1]. Hierover is weliswaar weinig tot geen info
terug te vinden. Wel weten we dat het resultaat van dit project wordt
aangeboden als shared cloudcomputing. Het doel van deze cluster is dus
anders als die van ons.

Het is dus reeds bewezen dat het maken van een cluster van TPU’s
mogelijk is. Wij willen met dit project bewijzen dat het ook mogelijk is
om een cluster te maken op kleinere schaal. Deze willen we dan gebruiken
om AI en ML algoritmes sneller te verwerken.

# Wat is een cluster?

**Cluster**

Een cluster is een groep van computers of servers die samenwerken om een
gezamenlijk doel te bereiken. Het kan worden gebruikt voor verschillende
doeleinden, zoals het verbeteren van de beschikbaarheid, prestaties of
schaalbaarheid van een systeem.

Er zijn verschillende soorten clusters, zoals:

- Hoge beschikbaarheidsclusters die zorgen dat een systeem blijft werken
  als een van de componenten uitvalt.

- Prestatieclusters die meerdere computers gebruiken om een taak sneller
  te voltooien.

- Schaalbare clusters die meerdere computers toevoegen om meer
  verwerking te kunnen aan, als de belasting van een systeem toeneemt.

- Cloud clusters die meerdere servers in een cloud-omgeving gebruiken om
  een systeem te laten schalen.

Een cluster kan ook bestaan uit een groep van specifieke
hardware-apparaten zoals GPUs of FPGAs die samenwerken om specifieke
taken zoals Machine learning of Deep Learning te verwerken.

**Beowulf Cluster**

Een beowulf cluster is een high-performance cluster die aan parallel
computing doet. Dit wil zeggen dat de cluster verschillende
berekeningen/processen op verschillende CPU’s tegelijk zal uitvoeren.
Dit is om complexe berekeningen sneller uit te voeren. Beowulf clusters
hebben Linux als besturingssysteem voor al zijn nodes. Meestal is een
Beowulf cluster ook gebouwd uit open source software. De verschillende
computers of nodes zijn verbonden met elkaar door gebruik te maken van
LAN (lokaal netwerk). Voordat we kunnen spreken over een Beowulf cluster
zijn er wel verschillende programma’s dat we moeten installeren. We
moeten zorgen voor de communicatie van de nodes en de verdeling van het
vermogen van de cluster. Hierna volgt er nog de configuratie en kan de
beowulf gebruikt worden.

Hier onder is een diagram van een beowulf cluster

<img src="./assets/media/image2.png" style="width:4.18261in;height:2.13644in"
alt="Diagram of a Beowulf cluster of computers. | Download Scientific Diagram" />

Figuur 1: Diagram van een beowulf cluster

# Coral DevBoard

In dit project maken we gebruik van Coral DevBoards. Een Coral Dev Board
is een ontwikkelbord dat is ontworpen voor het ontwikkelen van machine
learning-applicaties op het edge. Het bord is uitgerust met een Google
Edge TPU-accelerator, een krachtige chip die is ontworpen om neural
networks te versnellen op de rand van het netwerk. Het bord is ook
uitgerust met andere componenten zoals RAM, opslag, WiFi en Bluetooth,
zodat ontwikkelaars een compleet systeem hebben om mee te werken.

## Architectuur 

De architectuur van het Coral Dev Board is gebaseerd op een
systeem-op-a-chip (SoC) met een ARM Cortex-A53 CPU en een Google Edge
TPU co-processor. De Edge TPU is een speciale chip die is ontworpen om
neural networks efficiënt te versnellen op de rand van het netwerk. Het
bord heeft ook een aantal andere componenten zoals RAM, opslag, WiFi en
Bluetooth, die het een compleet systeem maken voor het ontwikkelen van
machine learning-applicaties.

De Edge TPU co-processor is gekoppeld aan de CPU via een high-speed
interconnect genaamd PCIe, die het mogelijk maakt om grote hoeveelheden
gegevens snel te verplaatsen tussen de twee chips. De Edge TPU
co-processor heeft toegang tot de gehele RAM van het bord, zodat het
grote hoeveelheden gegevens kan verwerken zonder beperkingen.

De software stack van het bord bestaat uit een Linux-besturingssysteem,
dat wordt ondersteund door een aantal machine learning-bibliotheken en
frameworks zoals TensorFlow Lite en PyTorch. Deze bibliotheken en
frameworks maken het mogelijk om machine learning-modellen te
ontwikkelen, te trainen en uit te voeren op het bord.

## OS

Standaard is er ‘Mendel’ geïnstalleerd op de Devboards. Een
besturingssysteem speciaal ontworpen voor deze bordjes. Alleen met
mendel kan je alle hardware aansturen van de Coral Devboards. Mendel is
een lichtere versie van Debian. Het is standaard niet geoptimaliseerd
voor clusters, daarom zijn we eerst op zoek gegaan naar een OS die dit
wel zou zijn. Al snel botsten we op problemen met ondersteuning van de
architectuur. Daarbij komt ook nog het feit dat je de nodige packages
zou moeten installeren op de Devboards om de Coral hardware correct aan
te sturen. Uiteindelijk hebben we besloten om het bestaande OS te
gebruiken. Het is mogelijk om een ander besturingssysteem te
installeren. Er moet echter wel rekening gehouden worden met dat het
aanspreken van de TPU’s niet meer mogelijk zal zijn. Dit komt omdat je
als een ‘mendel-user’ aangemeld moet zijn. Daarom hebben we gekozen voor
mendel os.

Mendel OS is een Linux-gebaseerd besturingssysteem dat is ontwikkeld
voor het Coral Dev Board. Het is geoptimaliseerd voor het uitvoeren van
machine learning-modellen op het bord en voorziet in een eenvoudige
manier om modellen te ontwikkelen, testen en implementeren op
randapparaten. Mendel OS biedt ontwikkelaars een eenvoudige en
consistente manier om toegang te krijgen tot de Edge TPU van het bord en
de modellen uit te voeren.

Mendel OS is ontwikkeld door Google's Edge TPU-team en gelanceerd in
2019 samen met het Coral Dev Board. Het besturingssysteem is
geoptimaliseerd voor snelle prestaties en eenvoudig gebruik, waardoor
ontwikkelaars hun modellen snel kunnen testen en implementeren op
randapparaten. Mendel OS is gericht op veiligheid en betrouwbaarheid, en
biedt ook ondersteuning voor updates en onderhoud.

De geschiedenis van Mendel OS is kort, aangezien het pas in 2019 is
gelanceerd. Het is echter een populaire keuze geworden onder
ontwikkelaars die machine learning willen uitvoeren op randapparaten,
vanwege de geoptimaliseerde prestaties en eenvoud van gebruik. Het wordt
gebruikt in combinatie met het Coral Dev Board, een populaire keuze
onder ontwikkelaars die machine learning willen toepassen op
randapparaten, en het is deel van een grotere trend waarbij bedrijven en
ontwikkelaars steeds meer hun aandacht richten op het toepassen van
machine learning op randapparaten.

<img src="./assets/media/image3.png" style="width:2.6087in;height:1.70167in"
alt="Pico 20 Google Coral Dev Board Cluster PicoCluster Desktop DataCenter – PicoCluster LLC" />

Figuur 2: Foto van een coral dev board

# Board setup

**Set-up van de Coral dev board**

**Benodigdheden**

1.  Een host computer met Linux ,Mac of Windows

2.  Micro SD-card (8gb)

3.  2 usb-c kabels 1 voor de voeding en 1 voor de verbinding met de
    computer,1 micro usb

4.  Wifi verbinding

**Werkwijze**

De set-up is relatief gemakkelijk het bestaat uit enkele stappen. Ik zal
ze hier kort opsommen en als u meer info wilt over deze stappen kunt u
gaan kijken in de bronvermelding.

1.  Download de Mendel OS-image van de officiële website.

2.  Sluit een Micro-SD kaart aan op uw computer en maak de kaart
    bootable met behulp van een tool zoals balenaEtcher.

3.  Flash de Mendel OS-image naar de Micro-SD kaart.

4.  Steek de Micro-SD kaart in de Coral Dev Board.

5.  Zet de Coral Dev Board aan en selecteer de Micro-SD kaart als
    opstartmedium.  
    <img src="./assets/media/image4.png" style="width:4.08716in;height:3.57294in"
    alt="A picture containing text, electronics, screenshot Description automatically generated" />

6.  Volg de stappen op het scherm om Mendel OS te installeren op het
    bord.

7.  Nadat de installatie is voltooid, kunt u het bord opnieuw opstarten
    en aan de slag gaan met de Mendel OS.  
    <img src="./assets/media/image5.png" style="width:4.83401in;height:4.17767in"
    alt="A picture containing graphical user interface Description automatically generated" />

8.  Herstart de Coral dev board en verbind met serial communicatie.

**Verbinden met Coral dev board**

Om een Coral Dev Board te verbinden met seriële communicatie, kunt u de
volgende stappen volgen:

1.  Sluit een seriële kabel aan op de seriële poort van het Coral Dev
    Board en de seriële poort van uw computer. De seriële poort van het
    bord is gewoonlijk te vinden aan de achterkant of onderkant van het
    bord, en is gemarkeerd met een label zoals "SERIAL" of "UART".

2.  Start een seriële terminal-applicatie op uw computer, zoals PuTTY of
    screen.

3.  Configureer de seriële terminal-applicatie met de juiste seriële
    poortinstellingen voor uw bord. Deze instellingen zouden moeten
    zijn:

- Baudrate: 115200

- Data bits: 8

- Pariteit: None

- Stopbits: 1

- Flow control: None

4.  Klik op verbinden om een verbinding te maken met het bord.

5.  Als de verbinding is gemaakt, zou u een login-prompt moeten zien.
    Log in met de standaard gebruikersnaam en wachtwoord voor uw bord
    (login: mendel, password: mendel)

6.  Als u succesvol bent ingelogd, kunt u de seriële terminal-applicatie
    gebruiken om commando's uit te voeren op het bord, zoals het
    bekijken van systeemlogs of het configureren van
    netwerkinstellingen.

# Network setup

Om een cluster van kaarten te creëren, die met elkaar verbonden zouden
zijn via netwerkswitches, is het noodzakelijk om ervoor te zorgen dat ze
altijd hetzelfde IP-adres hebben bij het herstarten of afsluiten. Dit is
van het grootste belang voor het goed functioneren van het cluster en
zorgt voor een naadloze communicatie tussen de boards. Hier onderstaat
hoe ze met elkaar verbonden zijn.

<img src="./assets/media/image6.png" style="width:6.12872in;height:3.44348in"
alt="Diagram Description automatically generated" />

Figuur 3: Coral dev board verbonden met switch

Hier onder zijn de stappen op vaste IP-adres toewijzen.

> 1\. Download de WinBox-software via deze link.
>
> 2\. Verbind uw laptop met Wi-Fi van Mikrotech, als Wi-Fi niet werkt,
> gebruik dan een Ethernet-kabel.
>
> 3\. Open de winbox-software om de verbinden met de switch heb je
> IP-adres nodig. Dit hun je vinde als je een Ethernet-kabel hebt, dan
> kun je IP vinden via ipconfig. Als je verbonden bent met wifi. Ziet u
> uw apparaat op het tabblad Neighbors? Dit zie je op onderstaande
> afbeelding.
>
> <img src="./assets/media/image7.png" style="width:5.17032in;height:2.85668in"
> alt="Graphical user interface, text, application, email Description automatically generated" />
>
> 4\. Dubbelklik op het apparaat of tik op het ip-adres en klik op
> “Connect”.
>
> 5\. Links zie je de verschillende menu's. Klik op IP DHCP Server, dan
> zie je een menu, klik op Lease.
>
> <img src="./assets/media/image8.png" style="width:6.26806in;height:1.44444in"
> alt="Graphical user interface, application Description automatically generated" />
>
> 6\. Alle apparaten die al IP hebben kun je zien. Dubbelklik op je
> apparaat. Dan zie je een menu als je IP-wil veranderen kun je doen.
> Dan moet je op “Make Static” drukken.
>
> <img src="./assets/media/image9.png" style="width:3.43622in;height:2.7549in"
> alt="Graphical user interface Description automatically generated" />
>
> 7\. Dan ben je klaar. Als je nu gaat rebooten blijft je appraten
> dezelfde IP-adres houden.

Nadat je dit hebt gedaan, is het een goed moment om de hostnaam van het
Coral-bord te wijzigen. Je kunt dit doen door verbinding te maken met
het bord via seriële communicatie of SSH. Gebruik hiervoor het commando
'hostnamectl set-hostname \<naam\>', waarbij je de naam van het bord
vervangt door één van de volgende: TPU0, TPU1, TPU2, TPU3 of TPU4.
Vergeet niet om het bord opnieuw op te starten met het commando 'sudo
reboot' nadat je de hostnaam hebt gewijzigd.

Om het Coral-bord zonder wachtwoord te laten werken met SSH, moet je SSH
configureren.

Hier onder staat de commando’s die je moet uitvoeren om SSH te gebruiken
zonder password in te geven.

**TPU0**

- ssh-keygen

- cd ~

- cd .ssh

- cp id_rsa.pub TPU0

**TPU1**

- ssh mendel@192.168.88.13

- ssh-keygen

- cd .ssh

- cp id_rsa.pub TPU1

- scp 192.168.88.12:/home/mendel/.ssh/TPU0 .

- cat TPU0 \>\> authorized_keys

- exit

**TPU2**

- ssh mendel@192.168.88.14

- ssh-keygen

- cd .ssh

- cp id_rsa.pub TPU2

- scp 192.168.88.12:/home/mendel/.ssh/TPU0 .

- cat TPU0 \>\> authorized_keys

- exit

**TPU3**

- ssh mendel@192.168.88.15

- ssh-keygen

- cd .ssh

- cp id_rsa.pub TPU3

- scp 192.168.88.12:/home/mendel/.ssh/TPU0 .

- cat TPU0 \>\> authorized_keys

- exit

**TPU4**

- ssh mendel@192.168.88.16

- ssh-keygen

- cd .ssh

- cp id_rsa.pub TPU4

- scp 192.168.88.12:/home/mendel/.ssh/TPU0 .

- cat TPU0 \>\> authorized_keys

- exit

**TPU0 (Voor alle TPUS)**

- scp 192.168.88.13:/home/mendel/.ssh/TPU1 .

- cat TPU1 \>\> authorized_keys

- scp 192.168.88.14:/home/mendel/.ssh/TPU2 .

- cat TPU2 \>\> authorized_keys

<!-- -->

- scp 192.168.88.15:/home/mendel/.ssh/TPU3 .

- cat TPU3 \>\> authorized_keys

- scp 192.168.88.16:/home/mendel/.ssh/TPU4 .

- cat TPU4 \>\> authorized_keys

Met deze commando kun je checken of de SSH werkt zonder password

- SSH mendel@\<ip van de coaral board\>

Of als de MPI geïnstalleerd is.

- mpiexec -H 192.168.88.13 -n 1 hostname

# NFS

## Ganesha

NFS-Ganesha is de serverside van NFS. Er wordt gebruik gemaakt van
Ganesha, dit is een alternatieve implementatie van NFS. De gewoonlijke
implementatie van NFS had problemen met het opstarten van de service.

NFS-Ganesha is ook niet perfect. Deze versie van Ganesha wilt enkel
werken als er een log file aanwezig is, dit is op zich niet onlogisch.
Het jammere is dat Ganesha deze log file niet zelf aanmaakt en dat deze
log file verdwijnt het moment dat het systeem een reboot doet.

Om dit probleem op te lossen is er een nieuwe service aangemaakt om zo
te garanderen dat de log file gemaakt wordt bij opstarting van het
systeem. Deze service werkt als deze uitgevoerd wordt met een commando,
bij het opstarten wilt deze service niet werken jammer genoeg. Dit is
een probleem dat niet opgelost is geraakt uiteindelijk. De service is
waarschijnlijk wat fout aangemaakt.

De uiteindelijke oplossing is om het commando voor de de log file
creatie in crontab te zetten. Zo wordt er een logfile aangemaakt bij
opstarten van het systeem.

De installatie van NFS-ganesha is heel gelijkaardig aan installatie van
eender welke andere NFS versie. Het opstellen is wat anders.

Er moet in de config file van gasnesha (te vinden op
/etc/ganesha/ganesha/conf) een aantal dingen aangepast worden. In Path
en Pseudo moet een path komen naar de gedeelde directory. In Clients
moet een lijst komen van alle clients hun IP-adres.

Met deze aanpassingen samen met een crontab commando dat bij reboot een
log file aanmaakt zal NFS-Ganesha perfect werken.

EXPORT

{

\# Export Id (mandatory, each EXPORT must have a unique Export_Id)

Export_Id = 77;

\# Exported path (mandatory)

Path = \[path to shared dir\]

\# Pseudo Path (required for NFS v4)

Pseudo = \[path to shared dir\]

\# Required for access (default is None)

\# Could use CLIENT blocks instead

Access_Type = RW;

\# Exporting FSAL

FSAL {

Name = VFS;

}

CLIENT

{

Clients = \[list of clients\]

}

}

## Common

NFS-Common is de service die geïnstalleerd wordt op de slave nodes om
NFS succesvol op te zetten. Er zijn echter wel een paar vereisten waar
je cluster/nodes aan moeten voldoen.

- Minimum 2 computers/nodes verbonden via een TCP/IP netwerk

- Elke machine runt op dezelfde architectuur om het sharen van program
  executables makkelijk te maken

- Elke machine runt op Linux

- Elke machine heeft een gedeelde gebruikersnaam geïnstalleerd à
  Hierdoor kunnen we shared files opzetten via NFS

Installatie:

1.  Installeer NFS-Common:  
    Sudo apt install -y nfs-common

2.  Unmask de service indien nodig à
    <https://askubuntu.com/questions/804946/systemctl-how-to-unmask>

3.  Start de service:  
    sudo systemctl start nfs-common.service

4.  Mount de directory:

> Sudo mount.nfs -o \<options\> \<ip addr\>:\<source directory\>
> \<target directory\>

NFS-Common zou nu correct geïnstalleerd & geconfigureerd moeten zijn.

# MPI

Nu kunnen we MPICH installeren op ons coral dev boaard. MPICH is de
belangrijkste tool die we nodig hebben om het cluster uit te voeren. Het
is een gratis implementatie van de MPI-standaard (Message Passing
Interface), die bedoeld is om parallelle computerarchitecturen te
beheren. Kortom, dit stelt ons in staat om scripts tegelijkertijd op
meerdere Coral-dev-borden uit te voeren. We hebben gekozen voor MPICH
omdat het populair is en goed werkt met python. Bovendien duurt het
minder lang om te installeren dan open-MPI (ongeveer 25-30 minuten in
plaats van 40-50 minuten). Dit bespaart veel tijd, aangezien je dit op
vijf Coral-dev-borden moet installeren.

Om MPICH te installeren, kunnen we niet gewoon 'sudo apt install mpich'
gebruiken, omdat de mpich-versie niet compatibel is met het
Coral-dev-bord. Daarom moeten we MPICH vanuit de bron installeren. Volg
hiervoor de stappen om MPICH vanaf de source te instaleren.

> Dit moet je voor alle Coral dev board doen.

- sudo apt-get update

- sudo mkdir mpich2

- cd ~/mpich2

- sudo wget https://www.mpich.org/static/downloads/3.2/mpich-3.2.tar.gz

- sudo tar xfz mpich-3.2.tar.gz

- sudo mkdir /home/rpimpi/

- sudo mkdir /home/rpimpi/mpi-install

- sudo apt-get install gfortran

- sudo /home/mendel/mpich2/mpich-3.2/configure
  > -prefix=/home/rpimpi/mpi-install

- sudo make

- sudo make install

- cd ..

- nano .bashrc

- PATH=\$PATH:/home/rpimpi/mpi-install/bin

> Test de MPI met deze commando

- mpiexec -n 1 hostname

> De output zijn de naam van je Coral dev board v.b. TPU0

Om verder te gaan met ons cluster hebben we een bibliotheek nodig die we
in onze scripts kunnen gebruiken om te communiceren tussen alle
knooppunten om ons programma efficiënt uit te voeren. Helaas kunnen
MPI's alleen rechtstreeks worden gebruikt in Fortran- en C-scripts op
het Coral-bord. Omdat de Coral-borden echter in Python draaien, moeten
we Python-mogelijkheden toevoegen aan ons cluster. Hiervoor moeten we de
Python-bibliotheek MPI4PY installeren. Hier onder staat de stappen die
je moet volgen

> Instaleren van MPI4PY

- sudo apt install python-pip python-dev libopenmpi-dev

- sudo pip install mpi4py

Hier onderstaat hoe je aan de slag kunt met Coral-borden door een
Python-script te maken. Om cross-board communicatie te laten werken,
moeten we eerst één hostbestand maken met het IP-adres van elk board. U
kunt deze informatie vinden door de Micro Tech-software te openen.

> Hier is een voorbeeld van een hostbestand. Met naam als machinefile.

- 192.168.88.12

- 192.168.88.13

- 192.168.88.14

- 192.168.88.15

- 192.168.88.16

Daarnaast gaan we een python script maken die met mpi4py alle Coral
board ga communiceren.

Maak een test.py file met touch en plak de code onder via nano test.py

> from mpi4py import MPI
>
> if \_\_name\_\_ == "\_\_main\_\_":
>
> world_comm = MPI.COMM_WORLD
>
> node_size = world_comm.Get_size()
>
> my_rank = world_comm.Get_rank()
>
> print("Node: " + str(node_size) + " " + "Rank: " + str(my_rank))

Voor dat je de code kunt runnen moet je eerst de test.py kopiëren naar
de rest van je coral dev board. Dat kun je doen via” scp test.py
mendel@\<ip-adres\>:\<op zelfde locatie als van master opslaan\>”. Dan
kun je de commando onder uit voeren. Je kunt ook via NFS je bestand
delen.

> mpiexec -n 5 –machinefile machinefile python test.py
>
> Node: 1 Rank: 1
>
> Node: 2 Rank: 0
>
> Node: 3 Rank: 2
>
> Node: 4 Rank: 3
>
> Node: 5 Rank: 4

Hierdoor zien we dat de cluster werkt. Met simpel python code.

# AI Model

Om een AI-model te maken moeten we eerst uitzoeken hoe we
tensorflow-modellen op het Coral-bord kunnen uitvoeren. Hiervoor hebben
we de website van het Coral-bord bezocht, die alle benodigde informatie
bevat om het model uit te voeren. Er zijn ook koppelingen naar de
[sites](https://coral.ai/docs/edgetpu/models-intro/) beschikbaar voor
meer informatie.

Het basisprincipe is dat het Coral Development Board een vereenvoudigde
versie van tensorflow gebruikt, genaamd tensorflow lite (tflite), om het
model uit te voeren. Het kan dus alleen tflite-modellen uitvoeren en kan
het volledige tensorflow-model niet uitvoeren. Het onderstaande diagram
laat zien wat er moet gebeuren om het model uit te voeren.

<img src="./assets/media/image10.png" style="width:6.5in;height:2.67014in"
alt="Diagram Description automatically generated" />

Figuur 4: Stappen om model te maken voor tflite

Om een Tensorflow model te maken dat compatibel is met de EDGE TPU en
klaar is om uitgevoerd te worden, moeten er verschillende stappen worden
ondernomen. Allereerst moet het model getraind worden, wat betekent dat
het wordt blootgesteld aan data en de parameters worden aangepast om de
gewenste output te produceren. Vervolgens moet het model worden
geëxporteerd in een formaat dat geschikt is om te worden geconverteerd
naar tflite, wat een lichtere variant is van het Tensorflow model. Zodra
het model is omgezet naar tflite, moet het worden gecompileerd zodat het
compatibel is met de EDGE TPU en klaar is om uitgevoerd te worden op het
apparaat. Voor het maken en omzetten van het model naar tflite kun je de
code op GitHub vinden. Hier onder staan de stappen hoe je model moet
maken.

**Als je een python script gaat runnen op een Coral dev board moet je
*python3* gebruiken en niet *python* bij commandline.**

**Om een AI-model te maken voor het Coral Dev Board met TensorFlow Lite,
kunt u de volgende stappen volgen:**

1.  Installeer TensorFlow en TensorFlow Lite op uw computer. Dit kan
    gedaan worden met behulp van pip.

> pip install tensorflow
>
> pip install tensorflow-lite

2.  Kies een bestaand AI-model of ontwikkel uw eigen model met behulp
    van TensorFlow. Hier kunt u gebruik maken van de
    TensorFlow-bibliotheken zoals keras of de TensorFlow-API direct. Er
    is ook een code gemaakt om model te maken. Dat kun je bij de GitHub
    vinden bij het software map.

3.  Converteer het model naar TensorFlow Lite-formaat. Dit kan gedaan
    worden met behulp van de TensorFlow Lite Converter.

> tflite_convert --output_file=model.tflite --saved_model_dir=model

4.  Verplaats het model naar uw Coral Dev Board. Dit kan gedaan worden
    via de command line via scp

> scp model.tflite mendel@coral_dev_board_ip:~/

5.  Instaleer edgetpu compiler

    1.  curl https://packages.cloud.google.com/apt/doc/apt-key.gpg \|
        sudo apt-key add -

    2.  echo "deb https://packages.cloud.google.com/apt
        coral-edgetpu-stable main" \| sudo tee
        /etc/apt/sources.list.d/coral-edgetpu.list

    3.  sudo apt-get update

    4.  sudo apt-get install edgetpu-compiler

6.  Zet de tflite model om naar edgetpu met deze command line.  
    edgetpu_compiler \[options\] model_name.tflite

7.  Voer het model uit op het bord met behulp van TensorFlow
    Lite-bibliotheken. Hiervoor kunt u gebruik maken van Python en de
    TensorFlow Lite-API.

8.  Test het model op uw Coral Dev Board met behulp van testgegevens en
    optimaliseer het model indien nodig.

Het lukt om het model te laten draaien op een enkele Coral dev board,
maar toen we het model op vijf Coral dev boarden wilden laten draaien,
was het niet mogelijk om het te verdelen om zo de snelheid te verhogen.
In plaats daarvan werd het model vijf keer uitgevoerd. Daarna hebben we
geprobeerd om het model te segmenteren in vijf stukken om ze tussen de
vijf Coral dev boarden te verdelen, maar er was een probleem: het script
kon de edge tpu's van de Coral dev boarden niet vinden, behalve die van
de master node.

We hebben geprobeerd om de TPU PCI-filelocatie te delen via NFS zodat
deze herkend zou worden, maar we ontdekten dat dat niet mogelijk was
omdat het een fysieke component is en geen protocol, dus het kan niet
via het netwerk worden gedeeld. Als tweede oplossing probeerden we
gewoon andere softwarecode te maken en te testen of die werkte, maar dat
was ook niet gelukt. We hebben ook geprobeerd om contact te krijgen met
het PCI-bord via seriële communicatie, maar dat was ook niet succesvol.
Helaas zijn we niet in staat geweest om het AI-model te laten draaien op
een cluster, maar we hebben wel een cluster opgezet die werkt. En een Ai
model gemaakt die op Coral dev board werkt.

# Testresultaten 

Het is ons niet gelukt om model te runnen op een cluster. Het model dat
werd gebruikt om de testen is een image classificatie model. Als we het
model op een cluster via mpi laten runnen werkt het wel. Als je het
model wilden verdelen naar meerdere nodes of dev boarden wild het niet
werken. Daarom is er een andere code gemaakt om aan te tonnen dat
cluster echter werkt en efficiënter is. Voor dat hebben we een code
gemaakt die PI waarde gaat bereken. De code vindt u op GitHub:
https://github.com/NithujanSel/EdgeAI_Documentatie/tree/main/software/piCode

Dit stuk code gebruikt MPI (Message Passing Interface) om pi te
berekenen met behulp van Monte Carlo-methode. MPI is een
communicatieprotocol dat wordt gebruikt om te communiceren tussen
verschillende processoren in een clusteromgeving. In dit specifieke
voorbeeld wordt MPI gebruikt om de berekening van pi te verdelen over
meerdere devices.

Het script begint met het importeren van de MPI-bibliotheek en het
gebruik van de "comm" variabele om toegang te krijgen tot de
communicatie-interface. Vervolgens wordt de rank van de huidige device
bepaald en het aantal devices dat deelneemt aan de berekening.

Daarna wordt een for-loop uitgevoerd die begint bij de rank van de
huidige device en stap voor stap doorloopt tot het aantal punten dat
moet worden gebruikt in de berekening. In deze loop worden willekeurige
x en y-waarden gegenereerd en wordt gecontroleerd of deze binnen de
eenheidscirkel liggen. Als dit het geval is, wordt de binnenste_cirkel
variabele verhoogd.

Vervolgens wordt MPI gebruikt om de resultaten van alle devices te
verzamelen en te vermenigvuldigen met 4, vervolgens wordt het aantal
punten gedeeld en zo berekend pi. Als alles goed gaat, geeft de code
"MPI test successful." weer.

Hier onder staat de tijd die heeft gebruikt om te bereken met verschilde
cluster Nodes. Dit is sort van simulatie als het echte TPU zou gebruiken
bij een AI-model.

| Aantal TPU’ | Run Tijd van het script(s) |
|-------------|----------------------------|
| 1           | 675                        |
| 2           | 483                        |
| 3           | 364                        |
| 4           | 308                        |
| 5           | 102                        |

Deze tabel geeft de runtijd van een script weer in seconden voor
verschillende aantallen TPU's (Tensor Processing Units). Het aantal
TPU's loopt van 1 tot 5. Het is duidelijk te zien dat hoe meer TPU's er
worden gebruikt, hoe sneller het script uitgevoerd wordt. Bijvoorbeeld,
met 1 TPU duurt het script 675 seconden, terwijl het met 5 TPU's slechts
102 seconden duurt. Dit suggereert dat het script efficiënt
paralleliseerbaar is en kan profiteren van meerdere TPU's tijdens de
uitvoering.

Runnen van ai model op een TPU via MPI

| Aantal TPU’s | Ai Model                                                            |
|--------------|---------------------------------------------------------------------|
| 1            | Het model werkt wel het run time was 3 seconde                      |
| 2            | Het wilde de EDGE TPU niet herkennen van de andere Coral dev board  |
| 3            | Het wilde de EDGE TPU niet herkennen van de andere Coral dev board  |
| 4            | Het wilde de EDGE TPU niet herkennen van de andere Coral dev board  |
| 5            | Het wilde de EDGE TPU niet herkennen van de andere Coral dev board. |

Deze tabel geeft informatie weer over hoe een AI-model presteert met
verschillende aantallen TPU's (Tensor Processing Units). Het aantal
TPU's loopt van 1 tot 5. Als we kijken naar de resultaten, is het
duidelijk dat het model wel functioneert als er slechts 1 TPU wordt
gebruikt. De run-tijd is in dit geval 3 seconden. Echter, wanneer er
meer dan 1 TPU wordt gebruikt, lijkt het erop dat het model problemen
heeft met het herkennen van de EDGE TPU's van de andere Coral dev
boards. Dit suggereert dat er problemen zijn met de configuratie of
compatibiliteit van de TPU's bij gebruik van meerdere TPU's.

#  Behuizing/ Koeling

Om onze cluster goed te koelen en alle componenten ordelijk samen te
voegen is er een behuizing nodig met een goede koeloplossing.

Op een Coral dev board zit een grote koelplaat met geïntegreerde fan.
Deze fan wordt aangezet wanneer een interne temperatuur probe op de CPU
een temperatuur meet die hoger is dan 65C. Dit is handig omdat de boards
geen of weinig actieve koeling nodig hebben wanneer er geen hevig proces
bezig is. Moest de waarde veranderd worden, dan is dit mogelijk door een
waarde (in 1/1000 van een C) weg te schrijven naar
'/sys/devices/virtual/thermal/thermal_zone0/trip_point_4_temp'.

Om alle 5 bordjes goed te kunnen koelen tijdens het trainen van een
model is wel enig koelvermogen nodig. Dit aangezien er constant frisse
lucht moet worden gevoed aan de bordjes hun koeloplossing. We hebben
besloten om dit op te lossen door in onze behuizing een paar grotere
fans te verwerken. We hebben besloten om 3 fans te gebruiken met ieders
een diameter van 80mm. Dit soort fans zijn redelijk makkelijk te vinden.

De finale behuizing ziet er als volgt uit:

<img src="./assets/media/image11.png"
style="width:5.89565in;height:2.70217in" />

Figuur 5: Finale behuizing

<img src="./assets/media/image12.png"
style="width:5.89514in;height:2.70194in" />

Figuur 6: Finale behuizing, de 2 niveaus kunnen uit elkaar gehaald
worden om kabels te routen

De behuizing bestaat uit 2 niveaus. Vanonder zitten de 3 grote fans en
de 5 dev boards van onze cluster. Vanboven zit de voeding van de dev
boards en de netwerkswitch die het netwerkverkeer van onze cluster
reguleert. Er is een gleuf voorzien om kabels van de bovenkant naar de
onderkant te routen.

<img src="./assets/media/image13.png" title="Inserting image..."
style="width:5.77392in;height:2.64638in" />

Figuur 7: De dev board sleetjes in werking

De dev boards zitten vanonder op klein sleetjes die de boards op hun
plaats houdt en ze makkelijk te verwijderen maakt.

<img src="./assets/media/image14.png" title="Inserting image..."
style="width:5.79131in;height:2.65435in" />

Figuur 8: De voeding houder in het bovenste niveau.

Aangezien de voeding geen bevestigingspunten heeft hebben we een houder
ontworpen. De houder zal de voeding op zijn plaats houden zonder dat de
input en output poorten geblokkeerd worden.

<img src="./assets/media/image15.png"
style="width:5.8245in;height:2.66956in" />

Figuur 9: De achterkant van de behuizing. De fans trekken de lucht uit
de behuizing

<img src="./assets/media/image16.png" title="Inserting image..."
style="width:5.85218in;height:2.68225in" />

Figuur 10: De fans in de binnenkant van de behuizing

De 3 fans staan zo georiënteerd dat ze constant de lucht uit de
behuizing naar buiten zuigen. Omdat de fans van de dev boards dicht bij
de fans staan, krijgen ze constant frisse lucht.

# GitHub link

[LINK](https://github.com/NithujanSel/EdgeAI_Documentatie):
<https://github.com/NithujanSel/EdgeAI_Documentatie>

# Conclusie

In dit onderzoek is geprobeerd om een cluster op te zetten met behulp
van MPICH op een Coral dev board, met het doel om een AI-model te laten
draaien. Hiervoor is onderzocht hoe SSH, NFS, MPICH en MPI4PY moeten
worden geïnstalleerd en gebruikt. Het AI-model is gemaakt en werkte op
het Coral dev board, maar niet op de cluster. Er zijn verschillende
pogingen gedaan om het model te laten werken op de cluster, zoals het
segmenteren van het model en het delen van de TPU PCI-filelocatie via
NFS, maar deze pogingen zijn niet geslaagd. Uiteindelijk is het niet
gelukt om het AI-model te laten draaien op de cluster, maar het cluster
zelf is wel opgezet en werkt. Om toch te laten zien dat het cluster
werkt is een ander script gemaakt om PI te berekenen. Daarnaast is er
een behuizing gemaakt voor onze cluster om deze te beschermen en
voldoende koeling te bieden, zodat ze niet te warm worden. Concluderend
kan worden gesteld dat het is gelukt om een cluster op te zetten en een
AI-model te maken dat werkt op de Coral dev board, hoewel het niet is
gelukt om het model te laten draaien op de cluster. Er is ook een
behuizing gemaakt voor de cluster.

# Bronvermelding

Get started with the Dev Board. (z.d.). Geraadpleegd op 10 september
2020, van https://coral.ai/docs/dev-board/get-started

Fromaget, P. (z.d.). How to build your first Raspberry Pi cluster?
Geraadpleegd op 24 november 2022, van
https://raspberrytips.com/raspberry-pi-cluster/

TensorFlow models on the Edge TPU. (z.d.). Geraadpleegd op 8 oktober
2020, van https://coral.ai/docs/edgetpu/models-intro/

Edge TPU Compiler. (z.d.). Geraadpleegd op 25 november 2020, van
https://coral.ai/docs/edgetpu/compiler/

What is the difference between Mpich and OpenMPI? (z.d.). Geraadpleegd
op 8 november 2020, van
https://www.quora.com/What-is-the-difference-between-Mpich-and-OpenMPI

Just a moment. . . (z.d.). Geraadpleegd op 21 december 2022, van
<https://www.linuxquestions.org/questions/linux-networking-3/doing-nfs-mount-over-pci-713810/>

TechCrunch is part of the Yahoo family of brands. (2022, 11 mei).
Geraadpleegd op 20 januari 2023, van
https://techcrunch.com/2022/05/11/google-launches-a-9-exaflop-cluster-of-cloud-tpu-v4-pods-into-public-preview/
