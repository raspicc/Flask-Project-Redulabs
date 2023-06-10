#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "CHOCO"
#define WIFI_PASSWORD "46994780"

#define API_KEY "AIzaSyBB5J5godIm94e01qShdUQUL3k1CTFGb5w"

#define DATABASE_URL "https://labremotouni-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

#define USER_EMAIL "luis.chanquetti.h@uni.pe"
#define USER_PASSWORD "luisalejandro"

#define LED_PIN 5 // Pin para el LED

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void setup()
{
  pinMode(LED_PIN, OUTPUT); // Configurar el pin del LED como salida

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

void loop()
{
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    int iVal = 0;
    Serial.printf(Firebase.getInt(fbdo, F("/onoffled/STATE"), &iVal) ? String(iVal).c_str() : fbdo.errorReason().c_str());
    Serial.println();

    if (iVal == 1)
    {
      digitalWrite(LED_PIN, HIGH); // Encender el LED
    }
    else if (iVal == 0)
    {
      digitalWrite(LED_PIN, LOW); // Apagar el LED
    }
  }
}
