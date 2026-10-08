/*
 * ESP32 : Portail Wi-Fi éducatif
 * Démonstration locale : point d'accès, DNS et serveur HTTP.
 * Aucun formulaire, aucune collecte ni journalisation des visiteurs.
 * Utilisation uniquement sur un réseau de démonstration autorisé.
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

//Configuration du réseau local 
const char* SSID = "Atelier-WiFi-Educatif";
const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress SUBNET(255, 255, 255, 0);
const byte DNS_PORT = 53;

//Services réseau
WebServer webServer(80);
DNSServer dnsServer;

//Page locale (stockée en mémoire Flash)
const char PAGE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Wi-Fi éducatif</title>

<style>
* {
  box-sizing: border-box;
}

html, body {
  margin: 0;
  min-height: 100%;
}

body {
  background: #030703;
  color: #66ff99;
  font-family: "Courier New", monospace;
  font-size: 15px;
  font-weight: normal;
  padding: 35px 18px;
  line-height: 1.7;
}

.line {
  display: block;
  width: 0;
  max-width: 100%;
  white-space: nowrap;
  overflow: hidden;
  animation: typing var(--time)
             steps(var(--n), end)
             var(--delay) both;
}

.line:nth-child(3),
.line:nth-child(7) {
  margin-top: 24px;
}

@keyframes typing {
  from {
    width: 0;
  }
  to {
    width: var(--w);
  }
}

@media (prefers-reduced-motion: reduce) {
  .line {
    animation: none;
    width: auto;
    white-space: normal;
  }
}
</style>
</head>

<body>

<div class="line" style="--n:20;--w:20ch;--time:1s;--delay:.3s;">Faut pas appuyer sur</div>

<div class="line" style="--n:15;--w:15ch;--time:.75s;--delay:1.3s;">n'importe quoi...</div>

<div class="line" style="--n:28;--w:28ch;--time:1.4s;--delay:2.2s;">Vous êtes sur un faux réseau</div>

<div class="line" style="--n:23;--w:23ch;--time:1.15s;--delay:3.6s;">Wi-Fi de démonstration.</div>

<div class="line" style="--n:27;--w:27ch;--time:1.35s;--delay:4.8s;">Son apparence ne prouve pas</div>

<div class="line" style="--n:17;--w:17ch;--time:.85s;--delay:6.2s;">qu'il est fiable.</div>

<div class="line" style="--n:26;--w:26ch;--time:1.3s;--delay:7.2s;">Aucun identifiant demandé.</div>

<div class="line" style="--n:28;--w:28ch;--time:1.4s;--delay:8.55s;">Aucun accès Internet fourni.</div>

</body>
</html>
)rawliteral";

//Réponse HTTP
void afficherPortail() {
  webServer.sendHeader("Cache-Control", "no-store");
  webServer.send_P(200, "text/html; charset=UTF-8", PAGE_HTML);
}

void setup() {
  Serial.begin(115200);

  //Point d'accès Wi-Fi (pas de connexion à une box)
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAPConfig(AP_IP, AP_IP, SUBNET)) {
    Serial.println("Erreur configuration IP du point d'accès");
    return;
  }
  if (!WiFi.softAP(SSID)) {
    Serial.println("Erreur création du point d'accès Wi-Fi");
    return;
  }

  //DNS : les noms demandés pointent vers l'ESP32
  dnsServer.start(DNS_PORT, "*", AP_IP);

  //HTTP : page locale
  webServer.on("/", HTTP_GET, afficherPortail);
  webServer.onNotFound(afficherPortail);
  webServer.begin();

  Serial.println("PORTAIL WIFI ESP32");
  Serial.print("Réseau Wi-Fi : ");
  Serial.println(SSID);
  Serial.print("Page locale : http://");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  //Fonctions tournent régulièrement
  dnsServer.processNextRequest();
  webServer.handleClient();
}
