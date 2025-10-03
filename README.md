# ft_irc - Documentation 🇫🇷

## Introduction
ft_irc est une implémentation du protocole IRC développée dans le cadre du cursus 42.
Ce projet a pour objectif d'approfondir les compétences en programmation réseau, gestion de processus, et conception logicielle.

## Fonctionnalités principales
- Serveur IRC basique respectant les RFC standards
- Gestion de multiples clients simultanés
- Prise en charge des commandes essentielles IRC (JOIN, PART, PRIVMSG, QUIT, etc.)
- Système de salons (channels)
- Authentification simple
- Gestion des erreurs et des déconnexions

## Installation
1. **Cloner le dépôt**
   ```bash
   git clone <repo_url>
   ```
2. **Compiler le projet**
   ```bash
   make
   ```
3. **Exécuter le serveur**
   ```bash
   ./ircserv <port> <password>
   ```

## Utilisation
- Connectez-vous au serveur via le client IRC reference Irssi
- Rejoignez un salon avec `/join #channel`
- Envoyez des messages privés avec `/msg <user> <message>`
- Quittez le serveur avec `/quit`

## Ressources
- [RFC 1459](https://tools.ietf.org/html/rfc1459) - Spécification IRC
- [RFC 2812](https://tools.ietf.org/html/rfc2812) - Mise à jour du protocole

---

Pour toute question ou problème, pleurez.

___________________________________________________________________________________________________________________________________

# ft_irc - Documentation 🇬🇧

## Introduction
ft_irc is an implementation of the IRC protocol developed as part of the 42 curriculum.
The goal of this project is to deepen skills in network programming, process management, and software design.

## Main Features
- Basic IRC server compliant with standard RFCs
- Management of multiple simultaneous clients
- Support for essential IRC commands (JOIN, PART, PRIVMSG, QUIT, etc.)
- Channel system
- Simple authentication
- Error handling and client disconnections

## Installation
1. **Clone the repository**
   ```bash
   git clone <repo_url>
   ```
2. **Compile the project**
   ```bash
   make
   ```
3. **Run the server**
   ```bash
   ./ircserv <port> <password>
   ```

## Usage
- Connect to the server using the reference IRC client Irssi
- Join a channel with `/join #channel`
- Send private messages with `/msg <user> <message>`
- Quit the server with `/quit`

## Resources
- [RFC 1459](https://tools.ietf.org/html/rfc1459) - IRC Specification
- [RFC 2812](https://tools.ietf.org/html/rfc2812) - Protocol Update

    ---

For any question or issue, cry.
