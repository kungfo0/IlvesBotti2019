#include <IlvesBotti2019.h>

int moottori1PWMPin = 15;
int moottori1SuuntaPin = 13;
int moottori2PWMPin = 14;
int moottori2SuuntaPin = 12;

int sensori1TrigPin = D2;
int sensori1EchoPin = D1;
int sensori2TrigPin = D4;
int sensori2EchoPin = D3;

// Jos pinnit ovat samat kuin yllä, riittää: IlvesBotti2019 botti;
IlvesBotti2019 botti = IlvesBotti2019(moottori1SuuntaPin, moottori2SuuntaPin, moottori1PWMPin, moottori2PWMPin,
             sensori1TrigPin, sensori2TrigPin, sensori1EchoPin, sensori2EchoPin);

void setup() {
  botti.wifi("Wlan ssid", "wlan salasana", "botti"); // viimeinnen arvo on botin nimi, vaihda se yksilölliseksi
  botti.setup();
}

void loop() {
  // kutsu aina tätä ensin
  botti.loop();

  // asettaa ledi 1:n värin siniseksi
  botti.asetaLedinVari(0, 0, 0, 255, 255);

  // sensoriarvojen luku
  int etuSensorinArvo = botti.lueEtuSensori();
  int sivuSensorinArvo = botti.lueSivuSensori();
  // botti vastaa osoitteessa http://botti.local
  // sivulle voi lisätä html-koodia seuraavalla tavalla:
  botti.asetaHtml(String("Etusensori: <b>") + String(etuSensorinArvo) + String("</b><br/>") + String("Sivusensori: <b>") + String(sivuSensorinArvo) + String("</b>"));

  // moottorien ohjaus seuraavasti
  // -1023 on täysillä taakse ja 1023 täysillä eteen
  // botti.asetaMoottorin1Nopeus(600);
  // botti.asetaMoottorin2Nopeus(600);
  // odottelee 700ms
  delay(700);
  // sammuttaa moottorit
  // botti.asetaMoottorienNopeus(0);

}
