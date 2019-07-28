#include <IlvesBotti2019.h>

IlvesBotti2019 botti;

void setup() {
  botti.wifi("Wlan ssid", "wlan salasana", "botti"); // viimeinnen arvo on botin nimi, vaihda se yksilölliseksi
  botti.setup();
}

void loop() {
  // kutsu aina tätä ensin
  botti.loop();

  // sensoriarvojen luku
  int etuSensorinArvo = botti.lueEtuSensori();
  int sivuSensorinArvo = botti.lueSivuSensori();

  // moottorien ohjaus seuraavasti
  // -1023 on täysillä taakse ja 1023 täysillä eteen
  // botti.asetaMoottorin1Nopeus(600);
  // botti.asetaMoottorin2Nopeus(600);
  // odottelee 700ms
  delay(700);
  // sammuttaa moottorit
  // botti.asetaMoottorienNopeus(0);

}
