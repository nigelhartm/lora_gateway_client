# lora_gateway_client
In the following I provide a working example to build your own LoRa (https://lora-alliance.org/) Gateway and client system. It consists basically of two files (one configuration for the client and one for the gateway)
The gateway additional takes the task to forward the received data to a IoT-Middleware (in this case Thingsboard https://thingsboard.io/)

Here are some more background information in german:

sorry :(

# Internet of Things
Der Begriff „Internet of Things“ (kurz: IoT) bezeichnet die Vernetzung von Endgeräten
mit dem Internet, dabei muss das Endgerät autark Aufgaben für den Besitzer erledigen
können. Dazu können sowohl das Sammeln von Daten zählen, sowie auch das Ausführen
von Aktionen. Der Einsatzbereich für solche Geräte kann sehr vielseitig sein, Beispiele
dafür sind automatisierte Bestellungen oder Warn- und Notfallfunktionen (Lackes/
Siepermann, 2018). Laut einer Prognose von Forbes aus dem Jahr 2016 soll die Anzahl der
vernetzten IoT-Geräte im Jahr 2020 eine Marke von 30,73 Milliarden Stück knacken, dies
entspricht einer Verdopplung innerhalb von fünf Jahren (Statista Research Department,
2016). Zum Vergleich in diesem Jahr 2020 soll die Weltbevölkerung ca. 7,79 Milliarden
Menschen umfassen (Rudnicka, 2019). Um die Anzahl der IoT-Geräte bildlicher
darzustellen, dies entspricht für das Jahr 2020 vier IoT-Geräte auf eine Person der
Weltbevölkerung. Mit dieser hohen Anzahl an IoT-Geräten zeigt sich, die für die
Informatik hohe Relevanz des Themas IoT. Die von den Endgeräten gesammelten Daten
werden in einer Middleware gesammelt und verarbeitet. Es können auch Daten von der
Middleware an die Endgeräte übertragen werden, um zum Beispiel Updates anzustoßen
oder andere Aktionen zu forcieren. Im Bereich „Internet of Things“ wird eine solche
Middleware als IoT-Plattform bezeichnet. Für Entwickler bieten IoT-Plattformen eine
schnelle Möglichkeit skalierbare IoT-Anwendungen zu entwickeln und damit unabhängig
von unterschiedlichen Standards zu sein (KaaIoT Technologies LLC., 2019).

# IoT-Plattformen
Die IoT-Plattform ist eine Middleware. Sie ermöglicht eine Kommunikation zwischen den
einzelnen Endgeräten und der eigentlichen IoT Anwendung. Die Plattform bietet die
Möglichkeit, die Endgeräte zu verwalten, sowie den Datenverkehr zu verwalten. Für die
Kommunikation mit der Anwendung stehen oft Application Programming Interfaces (kurz:
API) zur Verfügung. Als Benutzer besteht im Normalfall der Zugriff auf die Anwendung
über eine Benutzeroberfläche. (Vogel/ Dong/ Emruli/ Davidsson/ Spalazzese, 2020).

![alt text](https://github.com/nigelhartm/lora_gateway_client/blob/main/img/network.jpg)

# LPWAN
„Low-Power Wide Area Network“ (kurz: LPWAN) steht für Netzwerkarten, die es
Endgeräten ermöglichen, bei niedrigem Energieverbrauch über große Distanzen zu
kommunizieren. Diese Netzwerkarten ermöglichen es, vor allem Endgeräten im Internet of
Things, trotz eines Batteriebetriebes über lange Zeiträume, Sensordaten zu sammeln und
an IoT-Plattformen zu übermitteln, deswegen ist oft keine Echtzeitkommunikation mit
hohen Bandbreiten möglich (Farrell, 2018). Als hauptsächliche Architektur für solche
Netzwerkarten wird das Single-Sink- oder auch Multi-Sink-Prinzip gewählt. Eine solche
Senke kann auch als Gateway bezeichnet werden. Dabei werden die von den Sensoren
gesammelten Daten zuerst an eine Senke geschickt, bevor sie an die Middleware 
weitergeleitet werden können. Die Senke übernimmt hierbei die Verwaltung der Sensoren,
sowie die Umwandlung der Pakete in Pakete, die für das Internet-Gateway geeignet sind.
Auf Grund der geringen Datenraten im LPWAN kommen platzsparende
Netzwerkprotokolle zum Einsatz, welche sich für die herkömmliche Kommunikation im
Internet über den IP-Standard nicht immer eignen. Beim Multi-Sink-Prinzip kommen im
Vergleich zum Single-Sink-Prinzip mehrere Senken zum Einsatz. Mehrere Senken sorgen
hierbei für Redundanz und effektives Routing (Buratti / Dardari / Verdone, 2009). Die
LPWAN Technologien auf die man bei einer Internetrecherche am häufigsten trifft, sind
LoRa, Sigfox und NB-IOT (Patrick, 2020; Zeh, 2019). 

# LORA
„The LoRa Alliance” ist eine offene, nicht-gewinnorientierte Organisation, welche seit
ihrer Gründung im März 2015 auf mehr als 500 Mitglieder gewachsen ist. Die
Organisation versucht ihr eigenes LPWAN-Protokoll „LORAWAN“, mit eigener
Hardware zu etablieren (LoRa Alliance, o. J.).
Bei LoRa handelt es sich um eine Antennenhardware, welche hohe Reichweiten
ermöglichen kann. Viele alte kabellose Systeme nutzen als Kommunikationsalgorithmus
„Frequenz shifting keying modulation“ (kurz: FSKM), der aufgrund seiner
Energieeffizienz beliebt ist. LoRa hingegen basiert auf „Chirp spread spectrum
modulation“ (kurz: CSSM), welcher die gleiche Energieeffizienz wie FSKM erreicht, mit
dem positiven Zusatz noch höhere Reichweiten zu schaffen. Die CSSM ist früher bereits
von Militär und Weltraumorganisationen zur Kommunikation genutzt worden. Dies bot
sich vor allem aufgrund der Reichweite und Robustheit gegen Interferenzen an. Das
Besondere an LoRa ist, dass sie die erste günstige Implementation für den kommerziellen
Gebrauch ist. Ein LoRa-Gateway kann es ermöglichen, ganze Städte abzudecken. Die von
den Partnern der „LoRa Alliance“ vertriebenen Antennen können die Endkunden sowohl
für ihre Endgeräte als auch für eigene Basisstationen nutzen (LoRa Alliance, 2015). Um
dies zu ermöglichen, nutzt LoRa Frequenzbänder, die frei zugänglich sind. Damit diese
nicht überlastet werden, ist die Sendezeit eingeschränkt. In der EU beträgt die
Sendefrequenz 868 MHz und in den USA 915 MHz (LoRa Alliance, 2018-a).
Das LoRaWan-Protokoll baut auf die LoRa-Technologie auf und ist nur mit dieser nutzbar
(Techplayon, 2018). Pakete im LoRaWan können in zwei Richtungen versendet werden.
Uplink-Nachrichten werden vom Endgerät in Richtung Netzwerk versendet und Downlink
Nachrichten vom Netzwerk in Richtung Endgerät. Dabei können die Endgeräte in eine von
drei Klassen (A-, B- oder C-Klasse) eingeordnet werden. Bei Klasse A ist ein Downlink in
Richtung Endgerät erst nach einem Uplink des Endgerätes möglich. Bei Klasse B ist ein
Downlink immer in vorher konfigurierten Zeitschlitzen möglich. In Klasse C ist das
Endgerät jederzeit für Downlink-Nachrichten verfügbar. LoRaWAN ist zustandslos,
reserviert somit keine Ressourcen für die Verbindung. Es kann Nutzerdaten von bis zu 255
Bytes pro Paket übertragen. Dabei wird auf zwei Sicherheitsebenen gesetzt, zum einen gibt
es den „Unique End-Device Identifier“, welcher vom Antennenhersteller auf die Hardware
konfiguriert wird und diese wie eine herkömmliche MAC-Adresse einzigartig macht. Zum
anderen gibt es noch den softwareseitigen Application Key, welcher für Interaktionen mit
bestimmten Services genutzt wird (LoRa Alliance, 2018-b). 