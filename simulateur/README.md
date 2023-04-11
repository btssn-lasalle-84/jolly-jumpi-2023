# Simulateur Jolly Jumpi 2023

## Présentation du protocole implanté dans le simulateur ESP'ACE

Ce document présente rapidement le fonctionnement du simulateur ainsi que le protocole implémenté. Le protocole complet est disponible dans Google Drive. Actuellement, la version du protocole est la **0.x**.

## Configuration du simulateur

Valeur par défaut :

```cpp
#define NB_TROUS              5
```

```cpp
#define GPIO_SW1         12   //!< Pour simuler un tir
```

## Fonctionnement

- Trame de début de partie :

```

```

- Trames de détection :

```

```

- Trame de fin de partie :

```

```

## platform.ini

```ini
[env:lolin32]
platform = espressif32
board = lolin32
framework = arduino
lib_deps =
  thingpulse/ESP8266 and ESP32 OLED driver for SSD1306 displays @ ^4.2.0
upload_port = /dev/ttyUSB0
upload_speed = 115200
monitor_port = /dev/ttyUSB0
monitor_speed = 115200
```

## Auteur

- Thierry Vaira <<tvaira@free.fr>>
