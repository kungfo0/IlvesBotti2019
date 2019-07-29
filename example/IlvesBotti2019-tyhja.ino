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

  // Jos etusensorin arvo on alle 15cm käännytään vasemmalle
  if(etuSensorinArvo < 15) {
      // käännytään vasemmalle
      // vasen moottori taaksepäin nopeudella 600
      botti.asetaMoottorin1Nopeus(-600);
      // oikea moottori eteenpäin nopeudella 600
      botti.asetaMoottorin2Nopeus(600);
  }
  // jos etusensorin arvo on yli 15cm, yritetään kulkea suoraan
  else {
      // molemmat moottorit nopeudella 600, eli suoraan
      botti.asetaMoottorin1Nopeus(600);
      botti.asetaMoottorin2Nopeus(600);
  }

  // odotetaan hieman (0.05s) ennen kuin tehdään luuppi uudestaan
  delay(50);
}
