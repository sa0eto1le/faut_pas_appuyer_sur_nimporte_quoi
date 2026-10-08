# Faut pas appuyer sur n'importe quoi...

**Démonstration pédagogique d'un portail Wi-Fi local sur ESP32**, destinée à expliquer pourquoi le nom ou l'apparence d'un réseau ne garantit pas son authenticité.

Ce dépôt partage **uniquement la partie infrastructure** du projet de sensibilisation : création d'un point d'accès, résolution DNS locale et service d'une petite page web via HTTP. Il ne contient pas la version graphique complète de l'atelier.

## Objectif

Comprendre, dans un environnement contrôlé, comment un microcontrôleur peut héberger un réseau Wi-Fi et une page d'information accessible depuis un téléphone. Le projet illustre certains mécanismes utilisés par les portails captifs, sans fournir d'accès Internet ni chercher à récupérer les informations des visiteurs.

## Fonctionnalités

- **Point d'accès ESP32** : réseau de démonstration `Atelier-WiFi-Educatif`.
- **Serveur DNS local** : redirige les résolutions DNS reçues vers `192.168.4.1`.
- **Serveur HTTP** : affiche une page d'information embarquée dans la mémoire Flash.
- **Autonome** : pas de box, d'hébergement web ni d'accès Internet nécessaire.
- **Sans collecte applicative** : aucun formulaire, mot de passe ou identifiant demandé ; aucun historique de visiteurs enregistré par ce programme.

## Architecture

```text
Téléphone / ordinateur
         |
         | connexion au Wi-Fi de démonstration
         v
  ESP32 — point d'accès
         |
         +--> DNS (port 53)  --> 192.168.4.1
         |
         +--> HTTP (port 80) --> page HTML pédagogique
```

## Matériel et logiciel

- Carte **ESP32** compatible avec Arduino IDE (ex. ESP32 DevKit).
- Câble USB pour la programmation et l'alimentation.
- **Arduino IDE** avec le package de cartes **esp32 by Espressif Systems**.
- Bibliothèques incluses avec le package ESP32 : `WiFi.h`, `WebServer.h`, `DNSServer.h`.

## Installation

1. Ouvrir `PortailEducatif/PortailEducatif.ino` dans Arduino IDE.
2. Sélectionner la carte ESP32 et le port série correspondant.
3. Téléverser le programme.
4. Ouvrir le moniteur série à **115200 bauds** pour voir le nom du réseau et l'adresse locale.
5. Sur un appareil de test, se connecter à **`Atelier-WiFi-Educatif`**.
6. Si aucune fenêtre de portail n'apparaît, ouvrir **`http://192.168.4.1`** dans un navigateur.

> Le téléphone peut afficher « Pas d'accès Internet » : c'est normal. Selon l'appareil et son système, la fenêtre de portail captif peut ne pas s'ouvrir automatiquement.

## Partie du code présentée

| Composant | Fonction Arduino | Rôle |
|---|---|---|
| Point d'accès | `WiFi.softAP()` | Crée le réseau local |
| DNS | `dnsServer.start()` | Résout les noms vers l'ESP32 |
| HTTP | `webServer.on()` / `webServer.begin()` | Sert le contenu pédagogique |
| Traitement | `processNextRequest()` / `handleClient()` | Répond aux demandes des clients |

## Limites

- Cette démonstration repose sur **DNS + HTTP**, pas sur une interception HTTPS.
- Il s'agit d'un **portail captif pédagogique simplifié** : son ouverture automatique n'est pas garantie sur tous les téléphones.
- Le programme ne fournit aucun accès Internet et n'enregistre pas les clics ou les appareils connectés.
- L'ESP32 utilise ici un **réseau Wi-Fi ouvert**, prévu pour un test local.

## Usage responsable

Projet conçu pour des ateliers et essais **sur du matériel autorisé et avec des participants informés**. Ne pas reprendre le nom ou l'identité graphique d'un réseau réel sans autorisation, ne pas demander de données personnelles et ne pas utiliser ce code pour tromper des tiers.

## Pistes d'évolution

- Réaliser une interface pédagogique plus détaillée.
- Afficher une fiche « Reconnaître un faux réseau Wi-Fi » et les bons réflexes.
- Tester l'affichage du portail sur différents systèmes mobiles.

---
 
##  Licence
 
Ce projet est distribué sous licence **MIT**.  
Voir le fichier [LICENSE](LICENSE) pour le détail.

---

> *sa0*
 
