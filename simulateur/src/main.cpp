/**
 * @file src/main.cpp
 * @brief Programme principal Jolly Jumpi 2023
 * @author Thierry Vaira
 * @version 0.12
 */
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <afficheur.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#define DEBUG

// Brochages
#define GPIO_LED_ROUGE   5    //!<
#define GPIO_LED_ORANGE  17   //!< Trame OK
#define GPIO_LED_VERTE   16   //!< Trame START
#define GPIO_SW1         12   //!< Pour simuler un tir
#define GPIO_SW2         14   //!< Pour simuler joueur suivant
#define ADRESSE_I2C_OLED 0x3c //!< Adresse I2C de l'OLED
#define BROCHE_I2C_SDA   21   //!< Broche SDA
#define BROCHE_I2C_SCL   22   //!< Broche SCL

// Protocole (cf. Google Drive)
#define EN_TETE_TRAME    "$"
#define DELIMITEUR_CHAMP ";"
#define DELIMITEURS_FIN  "\r\n"
#define DELIMITEUR_DATAS ';'
#define DELIMITEUR_FIN   '\n'

#define NB_TROUS 5

#define BLUETOOTH
#ifdef BLUETOOTH
#define BLUETOOTH_SLAVE //!< esclave
// #define BLUETOOTH_MASTER //!< maître
BluetoothSerial ESPBluetooth;
#endif

/**
 * @enum TypeTrame
 * @brief Les differents types de trame
 */
enum TypeTrame
{
    Inconnu = -1,
    START   = 0,
    PAUSE,
    PLAY,
    STOP,
    RESET,
    CONFIG,
    ACK,
    NB_TRAMES
};

/**
 * @enum EtatPartie
 * @brief Les differents états d'une partie
 */
enum EtatPartie
{
    Finie = 0,
    EnCours,
    EnPause,
    Terminee
};

/**
 * @enum CouleurTrou
 * @brief Les couleurs des trous
 */
enum CouleurTrou
{
    Jaune = 0,
    Bleu  = 1,
    Rouge,
    NbCouleurs
};

const String nomsTrame[TypeTrame::NB_TRAMES] = {
    "START", "PAUSE", "PLAY", "STOP", "RESET", "CONFIG", "ACK"
}; //!< nom des trames dans le protocole
const String codeCouleur[CouleurTrou::NbCouleurs] = {
    "J",
    "B",
    "R"
};                                //!< nom des trames dans le protocole
EtatPartie etatPartie = Finie;    //!< l'état de la partie
bool       tirEncours = false;    //!<
int        numeroTrou = 0;        //!< de 0 à NB_TROUS
int        nbTrous    = NB_TROUS; //!< le nombre de trous détectables
bool       refresh    = false;    //!< demande rafraichissement de l'écran OLED
bool       antiRebond = false;    //!< anti-rebond
Afficheur  afficheur(ADRESSE_I2C_OLED,
                    BROCHE_I2C_SDA,
                    BROCHE_I2C_SCL);                //!< afficheur OLED SSD1306
String     entete        = String(EN_TETE_TRAME);    // caractère séparateur
String     separateur    = String(DELIMITEUR_CHAMP); // caractère séparateur
String     delimiteurFin = String(DELIMITEURS_FIN);  // fin de trame

String extraireChamp(String& trame, unsigned int numeroChamp)
{
    String       champ;
    unsigned int compteurCaractere  = 0;
    unsigned int compteurDelimiteur = 0;
    char         fin                = '\n';

    if(delimiteurFin.length() > 0)
        fin = delimiteurFin[0];

    for(unsigned int i = 0; i < trame.length(); i++)
    {
        if(trame[i] == separateur[0] || trame[i] == fin)
        {
            compteurDelimiteur++;
        }
        else if(compteurDelimiteur == numeroChamp)
        {
            champ += trame[i];
            compteurCaractere++;
        }
    }

    return champ;
}

/**
 * @brief Envoie une trame d'acquittement via le Bluetooth
 *
 */
void envoyerTrameAcquittement()
{
    char trameEnvoi[64];

    sprintf((char*)trameEnvoi,
            "%s%s\r\n",
            entete.c_str(),
            nomsTrame[TypeTrame::ACK].c_str());

    ESPBluetooth.write((uint8_t*)trameEnvoi, strlen((char*)trameEnvoi));
#ifdef DEBUG
    String trame = String(trameEnvoi);
    trame.remove(trame.indexOf("\r"), 1);
    Serial.print("> ");
    Serial.println(trame);
#endif
}

/**
 * @brief Envoie une trame via le Bluetooth
 *
 */
void envoyerTrameDetection(int numeroTrou, CouleurTrou couleurTrou)
{
    char trameEnvoi[64];

    // Format : ${NUMERO};{COULEUR}\r\n

    sprintf((char*)trameEnvoi,
            "%s%d;%s\r\n",
            entete.c_str(),
            numeroTrou,
            codeCouleur[(int)couleurTrou].c_str());
    ESPBluetooth.write((uint8_t*)trameEnvoi, strlen((char*)trameEnvoi));
#ifdef DEBUG
    String trame = String(trameEnvoi);
    trame.remove(trame.indexOf("\r"), 1);
    Serial.print("> ");
    Serial.println(trame);
#endif
}

/**
 * @brief Déclenchée par interruption sur le bouton SW1
 * @fn tirerPanier()
 */
void IRAM_ATTR tirer()
{
    if(etatPartie != EnCours || antiRebond || tirEncours)
        return;

    tirEncours = true;
    antiRebond = true;
}

/**
 * @brief Déclenchée par interruption sur le bouton SW2
 * @fn terminerTir()
 */
void IRAM_ATTR terminerTir()
{
}

/**
 * @brief Lit une trame via le Bluetooth
 *
 * @fn lireTrame(String &trame)
 * @param trame la trame reçue
 * @return bool true si une trame a été lue, sinon false
 */
bool lireTrame(String& trame)
{
    if(ESPBluetooth.available())
    {
#ifdef DEBUG
        Serial.print("Disponible : ");
        Serial.println(ESPBluetooth.available());
#endif
        trame.clear();
        trame = ESPBluetooth.readStringUntil(DELIMITEUR_FIN);
#ifdef DEBUG
        Serial.print("< ");
        Serial.println(trame);
#endif
        trame.concat(DELIMITEUR_FIN); // remet le DELIMITEUR_FIN
        return true;
    }

    return false;
}

/**
 * @brief Vérifie si la trame reçue est valide et retorune le type de la trame
 *
 * @fn verifierTrame(String &trame)
 * @param trame
 * @return TypeTrame le type de la trame
 */
TypeTrame verifierTrame(String& trame)
{
    String format;

    for(int i = 0; i < TypeTrame::NB_TRAMES; i++)
    {
        // Format : ${TYPE}\r\n
        format = entete + nomsTrame[i]; // + delimiteurFin;
#ifdef DEBUG
        Serial.print("Verification trame : ");
        Serial.print(format);
#endif
        // if(trame == format)
        if(trame.indexOf(format) != -1)
        {
            return (TypeTrame)i;
        }
        else
        {
#ifdef DEBUG
            Serial.println("");
#endif
        }
    }
#ifdef DEBUG
    Serial.println("Type de trame : inconnu");
#endif
    return Inconnu;
}

void reinitialiserAffichage()
{
    afficheur.setMessageLigne(Afficheur::Ligne1, "");
    afficheur.setMessageLigne(Afficheur::Ligne2, "");
    afficheur.setMessageLigne(Afficheur::Ligne3, "");
    refresh = true;
}

/**
 * @brief Initialise les ressources du programme
 *
 * @fn setup
 *
 */
void setup()
{
    Serial.begin(115200);
    while(!Serial)
        ;

    pinMode(GPIO_LED_ROUGE, OUTPUT);
    pinMode(GPIO_LED_ORANGE, OUTPUT);
    pinMode(GPIO_LED_VERTE, OUTPUT);
    pinMode(GPIO_SW1, INPUT_PULLUP);
    pinMode(GPIO_SW2, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(GPIO_SW1), tirer, FALLING);
    // attachInterrupt(digitalPinToInterrupt(GPIO_SW2), terminerTir, FALLING);

    digitalWrite(GPIO_LED_ROUGE, HIGH);
    digitalWrite(GPIO_LED_ORANGE, LOW);
    digitalWrite(GPIO_LED_VERTE, LOW);

    afficheur.initialiser();

    String titre  = "";
    String stitre = "=====================";

#ifdef BLUETOOTH
#ifdef BLUETOOTH_MASTER
    String nomBluetooth = "iot-esp-maitre";
    ESPBluetooth.begin(nomBluetooth, true);
    const uint8_t* adresseESP32 = esp_bt_dev_get_address();
    char           str[18];
    sprintf(str,
            "%02x:%02x:%02x:%02x:%02x:%02x",
            adresseESP32[0],
            adresseESP32[1],
            adresseESP32[2],
            adresseESP32[3],
            adresseESP32[4],
            adresseESP32[5]);
    stitre = String("== ") + String(str) + String(" ==");
    titre  = nomBluetooth;
#else
    String nomBluetooth = "jolly-jumpi-1";
    ESPBluetooth.begin(nomBluetooth);
    const uint8_t* adresseESP32 = esp_bt_dev_get_address();
    char           str[18];
    sprintf(str,
            "%02x:%02x:%02x:%02x:%02x:%02x",
            adresseESP32[0],
            adresseESP32[1],
            adresseESP32[2],
            adresseESP32[3],
            adresseESP32[4],
            adresseESP32[5]);
    stitre = String("== ") + String(str) + String(" ==");
    titre  = nomBluetooth;
#endif
#endif

#ifdef DEBUG
    Serial.println(titre);
    Serial.println(stitre);
#endif

    afficheur.setTitre(titre);
    afficheur.setSTitre(stitre);
    afficheur.afficher();

    // initialise le générateur pseudo-aléatoire
    // Serial.println(randomSeed(analogRead(34)));
    Serial.println(esp_random());
}

/**
 * @brief Boucle infinie d'exécution du programme
 *
 * @fn loop
 *
 */
void loop()
{
    String    trame;
    TypeTrame typeTrame;

    if(refresh)
    {
        afficheur.afficher();
        refresh = false;
    }

    if(antiRebond)
    {
        afficheur.afficher();
        delay(300);
        antiRebond = false;
    }

    if(tirEncours)
    {
        char strMessageDisplay[24];
        int  tir =
          random(0, (NB_TROUS * 2)) + 1; // 1 chance sur 2 : entre 1 et 12
        if(tir >= 1 && tir <= NB_TROUS)
        {
            CouleurTrou couleurTrou = (CouleurTrou)random(0, NbCouleurs);
            envoyerTrameDetection(tir, couleurTrou);
            sprintf(strMessageDisplay,
                    "-> Trou %d %s",
                    tir,
                    codeCouleur[(int)couleurTrou].c_str());
        }
        else
        {
            sprintf(strMessageDisplay, "-> loupé !");
        }

        afficheur.setMessageLigne(Afficheur::Ligne1, String(strMessageDisplay));
        String message = afficheur.getMessageLigne(Afficheur::Ligne2);
        message.remove(0, 3);
        afficheur.setMessageLigne(Afficheur::Ligne2, message);
        tirEncours = false;
#ifdef DEBUG
        Serial.println(strMessageDisplay);
#endif
    }

    if(lireTrame(trame))
    {
        typeTrame = verifierTrame(trame);
        if(typeTrame != Inconnu)
            afficheur.setMessageLigne(Afficheur::Ligne4, nomsTrame[typeTrame]);
        refresh = true;
#ifdef DEBUG
        if(typeTrame >= 0)
            Serial.println("\nTrame : " + nomsTrame[typeTrame]);
#endif
        switch(typeTrame)
        {
            case Inconnu:
                break;
            case TypeTrame::START:
                if(etatPartie == Finie)
                {
                    /*
                    String type = extraireChamp(trame, 2);
#ifdef DEBUG
                    Serial.print("type : ");
                    Serial.println(type);
#endif
                    */
                    reinitialiserAffichage();
                    envoyerTrameAcquittement();
                    etatPartie = EnCours;
                    digitalWrite(GPIO_LED_ROUGE, LOW);
                    digitalWrite(GPIO_LED_ORANGE, LOW);
                    digitalWrite(GPIO_LED_VERTE, HIGH);
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              String("En cours"));
                    afficheur.afficher();
#ifdef DEBUG
                    Serial.println("Nouvelle partie");
#endif
                }
                break;
            case TypeTrame::PAUSE:
                if(etatPartie == EnCours)
                {
                    envoyerTrameAcquittement();
                    etatPartie = EnPause;
                    digitalWrite(GPIO_LED_ROUGE, LOW);
                    digitalWrite(GPIO_LED_ORANGE, HIGH);
                    digitalWrite(GPIO_LED_VERTE, LOW);
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              String("En pause"));
                    afficheur.afficher();
                }
                break;
            case TypeTrame::PLAY:
                if(etatPartie == EnPause)
                {
                    envoyerTrameAcquittement();
                    etatPartie = EnCours;
                    digitalWrite(GPIO_LED_ROUGE, LOW);
                    digitalWrite(GPIO_LED_ORANGE, LOW);
                    digitalWrite(GPIO_LED_VERTE, HIGH);
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              String("En cours"));
                    afficheur.afficher();
                }
                break;
            case TypeTrame::STOP:
                if(etatPartie > Finie)
                {
                    reinitialiserAffichage();
                    envoyerTrameAcquittement();
                    etatPartie = Finie;
                    digitalWrite(GPIO_LED_ROUGE, HIGH);
                    digitalWrite(GPIO_LED_ORANGE, LOW);
                    digitalWrite(GPIO_LED_VERTE, LOW);
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              String("Finie"));
                    afficheur.afficher();
                }
                break;
            case TypeTrame::RESET:
                reinitialiserAffichage();
                envoyerTrameAcquittement();
                etatPartie = Finie;
                digitalWrite(GPIO_LED_ROUGE, HIGH);
                digitalWrite(GPIO_LED_ORANGE, LOW);
                digitalWrite(GPIO_LED_VERTE, LOW);
                afficheur.setMessageLigne(Afficheur::Ligne3, String("Finie"));
                reinitialiserAffichage();
                break;
            case TypeTrame::CONFIG:
                envoyerTrameAcquittement();
                digitalWrite(GPIO_LED_ORANGE, HIGH);
                delay(150);
                digitalWrite(GPIO_LED_ORANGE, LOW);
                break;
            case TypeTrame::ACK:
                digitalWrite(GPIO_LED_ORANGE, HIGH);
                delay(150);
                digitalWrite(GPIO_LED_ORANGE, LOW);
                break;
            default:
#ifdef DEBUG
                Serial.println("Trame invalide !");
#endif
                break;
        }
    }
}
