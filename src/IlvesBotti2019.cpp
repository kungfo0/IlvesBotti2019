#include "IlvesBotti2019.h"

#define WIFI_ENABLED 1

#ifdef WIFI_ENABLED

#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

std::string IlvesBotti2019::nimi;
std::string IlvesBotti2019::wifiSsid;
std::string IlvesBotti2019::wifiPass;
std::string IlvesBotti2019::extraHtml;

uint16_t IlvesBotti2019::ledienLkm = 1;
uint8_t IlvesBotti2019::ledienPin = 3;

NeoPixelBus<NeoRgbFeature, Neo400KbpsMethod>* IlvesBotti2019::pixels = NULL;

void IlvesBotti2019::wifi(char* ssid, char *passphrase, char *name)
{
    wifiSsid = ssid;
    wifiPass = passphrase;
    nimi = name;
}

#endif

IlvesBotti2019::IlvesBotti2019()
{
    if (pixels) delete pixels;
    pixels = new NeoPixelBus<NeoRgbFeature, Neo400KbpsMethod>(ledienLkm, ledienPin);
    pixels->ClearTo(VIHREA);
}

IlvesBotti2019::IlvesBotti2019(int moottori1SuuntaPin, int moottori2SuuntaPin, int moottori1PWMPin, int moottori2PWMPin,
                               int sensori1TrigPin, int sensori2TrigPin, int sensori1EchoPin, int sensori2EchoPin, uint16_t ledCount, uint8_t ledPin)
{
    motorPWMPin1 = moottori1PWMPin;
    motorDirPin1 = moottori1SuuntaPin;

    motorPWMPin2 = moottori2PWMPin;
    motorDirPin2 = moottori2SuuntaPin;

    trigPin = sensori1TrigPin;
    echoPin = sensori1EchoPin;

    trigPin2 = sensori2TrigPin;
    echoPin2 = sensori2EchoPin;

    ledienLkm = ledCount;
    ledienPin = ledPin;

    if (pixels) delete pixels;
    pixels = new NeoPixelBus<NeoRgbFeature, Neo400KbpsMethod>(ledienLkm, ledienPin);
    pixels->ClearTo(VIHREA);
}

int maxDistance = 200;
int maxPulseTime = 30000;

#define swap(a,b) a ^= b; b ^= a; a ^= b;
#define sort(a,b) if(a>b){ swap(a,b); }

int median(int a, int b, int c, int d, int e)
{
    sort(a,b);
    sort(d,e);
    sort(a,c);
    sort(b,c);
    sort(a,d);
    sort(c,d);
    sort(b,e);
    sort(b,c);

    return c;
}

// The cyclic buffer
int frontReadings = 0;
int sideReadings = 0;
int frontSensorReadings[5];
int sideSensorReadings[5];

int IlvesBotti2019::motorPWMPin1 = D8;
int IlvesBotti2019::motorDirPin1 = D4;

int IlvesBotti2019::motorPWMPin2 = D2;
int IlvesBotti2019::motorDirPin2 = D3;

// from -1023 to 1023
int IlvesBotti2019::minSpeed = -1023;
int IlvesBotti2019::maxSpeed = 1023;
int IlvesBotti2019::speedStopped = 0;
int IlvesBotti2019::speedAdjustRight = 1.0;
int IlvesBotti2019::speedAdjustLeft = 1.0;

int IlvesBotti2019::wifiConnectMaxRetries = 20;

// defines pins numbers
int IlvesBotti2019::trigPin = D6;
int IlvesBotti2019::echoPin = D7;

int IlvesBotti2019::trigPin2 = D1;
int IlvesBotti2019::echoPin2 = D5;

long IlvesBotti2019::durationFront;
long IlvesBotti2019::durationSide;

int IlvesBotti2019::distanceFront;
int IlvesBotti2019::distanceSide;

int IlvesBotti2019::delayTime = 750;

bool IlvesBotti2019::OTAStarted;
bool IlvesBotti2019::stopped;
bool IlvesBotti2019::shouldTurnLeft;
bool IlvesBotti2019::shouldTurnRight;

bool IlvesBotti2019::shouldGoForward;
bool IlvesBotti2019::shouldGoBackward;

void IlvesBotti2019::onOTAStart() {
    OTAStarted = true;
}

void IlvesBotti2019::setupOta() {
    ArduinoOTA.setHostname(nimi.c_str());
    ArduinoOTA.setPort(8266);

    ArduinoOTA.begin();
    ArduinoOTA.onStart(onOTAStart);
}

#ifdef WIFI_ENABLED

void IlvesBotti2019::handleRoot() {
    String baseHtml("<form action=\"/toggle-motors\" method=\"POST\"><input type=\"submit\" value=\"Toggle motors on/off\"></form><br><form action=\"/turn-left\" method=\"POST\"><input type=\"submit\" value=\"Turn left\"></form><br><form action=\"/turn-right\" method=\"POST\"><input type=\"submit\" value=\"Turn right\"></form><br><form action=\"/go-forward\" method=\"POST\"><input type=\"submit\" value=\"Go forward\"></form><br><form action=\"/go-backward\" method=\"POST\"><input type=\"submit\" value=\"Go backward\"></form>");
    String extra(extraHtml.c_str());
    server.send(200, "text/html",
                baseHtml + extra);
}

void IlvesBotti2019::goForwardHandler() {
    shouldGoForward = true;
    server.sendHeader("Location", "/");
    server.send(303);
}

void IlvesBotti2019::goBackwardHandler() {
    shouldGoBackward = true;
    server.sendHeader("Location", "/");
    server.send(303);
}

void IlvesBotti2019::turnLeftHandler() {
    shouldTurnLeft = true;
    server.sendHeader("Location", "/");
    server.send(303);
}

void IlvesBotti2019::turnRightHandler() {
    shouldTurnRight = true;
    server.sendHeader("Location", "/");
    server.send(303);
}

void IlvesBotti2019::toggleMotors() {
    stopped = !stopped;
    server.sendHeader("Location", "/");
    server.send(303);
}

void IlvesBotti2019::handleNotFound() {
    server.send(404, "text/plain", "404: Not found");
}

void IlvesBotti2019::setupWifi() {
    WiFi.mode(WIFI_STA);
    // Set Hostname.
    String hostname(nimi.c_str());
    hostname += String(ESP.getChipId(), HEX);
    WiFi.hostname(hostname);
    WiFi.begin(wifiSsid.c_str(), wifiPass.c_str());
    int tried = 0;
    while (WiFi.status() != WL_CONNECTED && tried < wifiConnectMaxRetries) {
        delay(500);
        tried++;
    }

    if(tried >= wifiConnectMaxRetries) {
          asetaLedienVari(PUNAINEN);
    } else {
        asetaLedienVari(VIHREA);
    }

    // http server handlers
    server.on("/", HTTP_GET, handleRoot);
    server.on("/toggle-motors", HTTP_POST, toggleMotors);
    server.on("/turn-left", HTTP_POST, turnLeftHandler);
    server.on("/turn-right", HTTP_POST, turnRightHandler);
    server.on("/go-forward", HTTP_POST, goForwardHandler);
    server.on("/go-backward", HTTP_POST, goBackwardHandler);
    server.onNotFound(handleNotFound);

    server.begin();
}

#endif

void IlvesBotti2019::setupMotors() {
    digitalWrite(motorPWMPin1, LOW);
    pinMode(motorPWMPin1, OUTPUT);
    digitalWrite(motorPWMPin1, LOW);
    digitalWrite(motorPWMPin2, LOW);
    pinMode(motorPWMPin2, OUTPUT);
    digitalWrite(motorPWMPin2, LOW);
    digitalWrite(motorDirPin1, LOW);
    pinMode(motorDirPin1, OUTPUT);
    digitalWrite(motorDirPin1, LOW);
    digitalWrite(motorDirPin2, LOW);
    pinMode(motorDirPin2, OUTPUT);
    digitalWrite(motorDirPin2, LOW);
}

void IlvesBotti2019::setupSensors() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input

    pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
}

void IlvesBotti2019::asetaMoottorin1Nopeus(int nopeus) {
    int restrictedSpeed = constrain(nopeus, minSpeed, maxSpeed);
    if (restrictedSpeed >= 0) {
        analogWrite(motorPWMPin1, restrictedSpeed);
        digitalWrite(motorDirPin1, LOW);
    } else {
        analogWrite(motorPWMPin1, -restrictedSpeed);
        digitalWrite(motorDirPin1, HIGH);
    }
}

void IlvesBotti2019::asetaMoottorin2Nopeus(int nopeus) {
    int restrictedSpeed = constrain(nopeus, minSpeed, maxSpeed);
    if (restrictedSpeed >= 0) {
        analogWrite(motorPWMPin2, restrictedSpeed);
        digitalWrite(motorDirPin2, LOW);
    } else {
        analogWrite(motorPWMPin2, -restrictedSpeed);
        digitalWrite(motorDirPin2, HIGH);
    }
}

void IlvesBotti2019::asetaMoottorienNopeus(int nopeus) {
    asetaMoottorin1Nopeus(nopeus);
    asetaMoottorin2Nopeus(nopeus);
}

void IlvesBotti2019::asetaLedienVari(RgbColor vari) {
    Serial.println(ledienLkm);
    for(int i=0;i<ledienLkm;i++) {
        pixels->SetPixelColor(i, vari);
    }
    pixels->Show();
}

void IlvesBotti2019::asetaLedinVari(int ledinNumero, RgbColor vari) {
    pixels->SetPixelColor(ledinNumero, vari);
    pixels->Show();
}

void IlvesBotti2019::setSpeed(motor motor, int speed) {
    if (motor == LEFT_MOTOR) {
        asetaMoottorin1Nopeus(speed * speedAdjustLeft);
    } else {
        asetaMoottorin2Nopeus(speed * speedAdjustRight);
    }
}

void IlvesBotti2019::run(motor motor, directions dir, int speed) {
    switch (dir) {
        case FORWARD:
            setSpeed(motor, speed);
            break;
        case BACK:
            setSpeed(motor, -speed);
            break;
    }
}

void IlvesBotti2019::run(motor motor, directions dir) {
    run(motor, dir, maxSpeed);
}

void IlvesBotti2019::stopMotors() {
    setSpeed(LEFT_MOTOR, speedStopped);
    setSpeed(RIGHT_MOTOR, speedStopped);
}

void IlvesBotti2019::turnRight() {
    // try to turn right
    run(LEFT_MOTOR, FORWARD);
    setSpeed(RIGHT_MOTOR, speedStopped);
    delay(delayTime);
}

void IlvesBotti2019::turnLeft() {
    // try to turn right
    run(RIGHT_MOTOR, FORWARD);
    setSpeed(LEFT_MOTOR, speedStopped);
    delay(delayTime);
}

void IlvesBotti2019::goForward() {
    run(RIGHT_MOTOR, FORWARD);
    run(LEFT_MOTOR, FORWARD);
    delay(delayTime);
}

void IlvesBotti2019::goBackward() {
    run(RIGHT_MOTOR, BACK);
    run(LEFT_MOTOR, BACK);
    delay(delayTime);
}

void IlvesBotti2019::loop() {
#ifdef WIFI_ENABLED
    ArduinoOTA.handle();
    // Skip all other logic if we're running an OTA update
    if (OTAStarted) {
        return;
    }

    server.handleClient();
#endif

    // readSensors();

    if (shouldGoForward) {
        goForward();
        shouldGoForward = false;
    }

    if (shouldGoBackward) {
        goBackward();
        shouldGoBackward = false;
    }

    if (shouldTurnLeft) {
        turnLeft();
        shouldTurnLeft = false;
    }

    if (shouldTurnRight) {
        turnRight();
        shouldTurnRight = false;
    }

    if (stopped) {
        stopMotors();
    }
}

void IlvesBotti2019::setup() {
    for(int i=0; i<5; i++) {
        sideSensorReadings[i] = maxDistance;
        frontSensorReadings[i] = maxDistance;
    }
#ifdef WIFI_ENABLED
    setupWifi();
    setupOta();
#endif

    setupMotors();
    setupSensors();
    pixels->Begin();
    // disable serial to enable led
    Serial.begin(115200);

    asetaLedienVari(VIHREA);
}

int IlvesBotti2019::lueEtuSensori() {
    for (int i=0;i<5;i++) {
        // Clears the trigPin
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);

        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // Reads the echoPin, returns the sound wave travel time in microseconds
        noInterrupts();
        long start = micros();
        durationFront = pulseIn(echoPin, HIGH, maxPulseTime);
        long timeTaken = micros() - start;
        Serial.print("Time taken: ");
        Serial.println(timeTaken);
        interrupts();
        if (timeTaken > maxPulseTime) {
            Serial.println("returning previous front median");
            return getMedianFront();
        }
        // Calculating the distance
        distanceFront = durationFront * 0.0343 / 2;
        // Prints the distance on the Serial Monitor
        Serial.print("Distance front: ");
        Serial.println(distanceFront);
        frontReadings++;

        frontSensorReadings[frontReadings++ % 5] = min(distanceFront, maxDistance);
    }

    return getMedianFront();
}

int IlvesBotti2019::lueSivuSensori() {
    for (int i=0;i<5;i++) {
        // Clears the trigPin
        digitalWrite(trigPin2, LOW);
        delayMicroseconds(2);

        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(trigPin2, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin2, LOW);

        // Reads the echoPin, returns the sound wave travel time in microseconds
        noInterrupts();
        long start = micros();
        durationSide = pulseIn(echoPin2, HIGH, maxPulseTime);

        long timeTaken = micros() - start;
        Serial.print("Time taken: ");
        Serial.println(timeTaken);
        interrupts();
        if (timeTaken > maxPulseTime) {
            Serial.println("returning previous side median");
            return getMedianSide();
        }
        // Calculating the distance
        distanceSide = durationSide * 0.0343 / 2;
        // Prints the distance on the Serial Monitor
        Serial.print("Distance side: ");
        Serial.println(distanceSide);
        sideReadings++;

        sideSensorReadings[sideReadings++ % 5] = min(distanceSide, maxDistance);
    }
    return getMedianSide();
}

int IlvesBotti2019::getMedianSide() {
    return median(sideSensorReadings[0], sideSensorReadings[1], sideSensorReadings[2], sideSensorReadings[3], sideSensorReadings[4]);
}

int IlvesBotti2019::getMedianFront() {
    return median(frontSensorReadings[0], frontSensorReadings[1], frontSensorReadings[2], frontSensorReadings[3], frontSensorReadings[4]);
}

void IlvesBotti2019::readSensors() {
    lueEtuSensori();
    delay(1);
    lueSivuSensori();
}

void IlvesBotti2019::asetaToimintojenKesto(int delayMs) {
    delayTime = delayMs;
}

void IlvesBotti2019::asetaHtml(String html) {
    extraHtml = html.c_str();
}
