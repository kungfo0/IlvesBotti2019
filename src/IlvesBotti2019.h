#ifndef IlvesBotti2019_h
#define IlvesBotti2019_h

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

enum directions {
    FORWARD,
    BACK
};

enum motor {
    RIGHT_MOTOR,
    LEFT_MOTOR
};

class IlvesBotti2019
{
public:
    IlvesBotti2019();
    IlvesBotti2019(int moottori1SuuntaPin, int moottori2SuuntaPin, int moottori1PWMPin, int moottori2PWMPin,
            int sensori1TrigPin, int sensori2TrigPin, int sensori1EchoPin, int sensori2EchoPin);
    static void asetaMoottorin1Nopeus(int nopeus);
    static void asetaMoottorin2Nopeus(int nopeus);
    static void asetaMoottorienNopeus(int nopeus);
    static void asetaLedinVari(int ledi, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    static void asetaLedinVari(int ledi, uint32_t vari);
    static void loop();
    static int lueEtuSensori();
    static int lueSivuSensori();
    static void wifi(char* ssid, char *passphrase, char *name);
    static void asetaNopeuksienRajaArvot(int minNopeus, int maxNopeus, int pysahtynyt, int oikeaSaato, int vasenSaato);
    static void asetaNeoPixel(uint16_t ledienLkm, uint8_t ledPin, neoPixelType pixelinTyyppi);
    static void setup();
    static void asetaToimintojenKesto(int kestoMs);
    static void asetaHtml(String html);

private:
    static long durationFront;
    static long durationSide;
    static int distanceFront;
    static int distanceSide;
    static bool OTAStarted;
    static bool stopped;
    static bool shouldTurnLeft;
    static bool shouldTurnRight;
    static bool shouldGoForward;
    static bool shouldGoBackward;
    static int trigPin;
    static int echoPin;
    static int trigPin2;
    static int echoPin2;
    static int delayTime;
    static int motorPWMPin1;
    static int motorPWMPin2;
    static int motorDirPin1;
    static int motorDirPin2;
    static int minSpeed;
    static int maxSpeed;
    static int speedStopped;
    static int speedAdjustRight;
    static int speedAdjustLeft;
    static std::string wifiSsid;
    static std::string wifiPass;
    static std::string nimi;
    static std::string extraHtml;
    static void onOTAStart();
    static void setupOta();
    static void handleRoot();
    static void goForwardHandler();
    static void goBackwardHandler();
    static void turnLeftHandler();
    static void turnRightHandler();
    static void toggleMotors();
    static void handleNotFound();
    static void setupWifi();
    static void setupMotors();
    static void setupSensors();
    static void setSpeed(motor motor, int speed);
    static void run(motor motor, directions dir, int speed);
    static void run(motor motor, directions dir);
    static void stopMotors();
    static void readSensors();
    static void turnRight();
    static void turnLeft();
    static void goForward();
    static void goBackward();
};
#endif