# Configuration du projet Arduino pour Firebase et le capteur SCD30

Ce projet utilise une carte ESP32 ou ESP8266 pour envoyer des données environnementales (CO2, température, humidité) issues d'un capteur SCD30 à une base de données Firebase. Voici les étapes pour modifier les paramètres principaux du projet, notamment le SSID Wi-Fi, le mot de passe Wi-Fi, la clé API et l'URL de la base de données.

## Paramètres modifiables

Les paramètres principaux se trouvent au début du code sous forme de macros `#define`. Voici leur emplacement et comment les modifier :

### Modifier le SSID Wi-Fi et le mot de passe
Ces informations sont nécessaires pour connecter l'ESP32/ESP8266 à votre réseau Wi-Fi.

```cpp
#define WIFI_SSID "MYPC 0617"
#define WIFI_PASSWORD "U2<y4160"
```

- **SSID Wi-Fi** : Remplacez `"MYPC 0617"` par le nom (SSID) de votre réseau Wi-Fi.
- **Mot de passe Wi-Fi** : Remplacez `"U2<y4160"` par le mot de passe de votre réseau Wi-Fi.

### Modifier la clé API Firebase
La clé API permet d'autoriser les communications avec votre projet Firebase.

```cpp
#define API_KEY "AIzaSyDRfqBnf_vz0clPPxrEcUjNUnqtY2YABkE"
```

- Remplacez `"AIzaSyDRfqBnf_vz0clPPxrEcUjNUnqtY2YABkE"` par la clé API générée dans les paramètres de votre projet Firebase.

### Modifier l'URL de la base de données Firebase
L'URL identifie l'emplacement de votre base de données dans Firebase.

```cpp
#define DATABASE_URL "https://workshop-3395b-default-rtdb.europe-west1.firebasedatabase.app/"
```

- Remplacez `"https://workshop-3395b-default-rtdb.europe-west1.firebasedatabase.app/"` par l'URL de votre base de données Firebase. Vous pouvez la trouver dans les paramètres de votre base de données.

## Étapes pour modifier les paramètres

1. **Ouvrez le fichier source** : Importez le code dans votre IDE Arduino.
2. **Localisez les paramètres** : Les paramètres sont situés en haut du fichier dans les lignes `#define`.
3. **Effectuez les modifications** :
   - Remplacez les valeurs existantes par celles correspondant à votre configuration.
4. **Téléversez le code** : Compilez et téléversez le code sur votre carte ESP32/ESP8266.

## Exemple de modification
Si votre réseau Wi-Fi est nommé "MonWiFi", votre mot de passe est "MotDePasse123", votre clé API est "NouvelleAPIKey", et l'URL de votre base de données est `https://example.firebaseio.com/`, vos modifications seront :

```cpp
#define WIFI_SSID "MonWiFi"
#define WIFI_PASSWORD "MotDePasse123"
#define API_KEY "NouvelleAPIKey"
#define DATABASE_URL "https://example.firebaseio.com/"
```

## Vérification
Après modification, connectez votre carte au port série via l'IDE Arduino. Surveillez la sortie pour vérifier que :

1. La carte se connecte correctement au Wi-Fi.
2. Les données sont envoyées avec succès à Firebase.

## Remarque
Assurez-vous de garder vos informations sensibles (comme la clé API et le mot de passe Wi-Fi) privées et sécurisées.
