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
  <title>Atelier Wi-Fi educatif</title>
  <style>
    * { box-sizing: border-box; }
    body { margin: 0; min-height: 100vh; display: grid; place-items: center;
           padding: 20px; font-family: system-ui, Arial, sans-serif;
           background: #081c30; color: #f0f9ff; }
    main { max-width: 480px; padding: 30px; border-radius: 20px;
           background: #15314b; border: 1px solid #34627e; }
    .tag { color: #8cddff; font-weight: bold; font-size: .85rem; }
    h1 { font-size: 1.7rem; line-height: 1.2; }
    p { line-height: 1.6; }
    small { color: #b9cedb; }
  </style>
</head>
<body>
  <main>
    <div class="tag">ATELIER DE SENSIBILISATION</div>
    <h1>Faut pas appuyer sur n'importe quoi...</h1>
    <p>Vous êtes sur un réseau local de démonstration créé par un ESP32.
       Cette page montre le fonctionnement d'un point d'accès,
       d'un serveur DNS et d'un serveur HTTP.</p>
    <small>Aucun identifiant demandé. Aucun accès Internet fourni.</small>
  </main>
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
