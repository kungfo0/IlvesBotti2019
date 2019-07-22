#include <IlvesBotti2019.h>

IlvesBotti2019 botti;

int etaisyysLiianLahella = 10;
int etaisyysLiianKaukana = 25;

void setup() {
  botti.wifi("ssid", "salasana", "botti123");
  botti.setup();
}

void liikuVasemmalleJaEteenpain() {
    botti.asetaMoottorin2Nopeus(600);
    botti.asetaMoottorin1Nopeus(0);
    delay(700);
    botti.asetaMoottorienNopeus(600);
    delay(700);
    botti.asetaMoottorienNopeus(0);
}

void liikuOikealleJaEteenpain() {
    botti.asetaMoottorin1Nopeus(600);
    botti.asetaMoottorin2Nopeus(0);
    delay(600);
    botti.asetaMoottorienNopeus(600);
    delay(700);
    botti.asetaMoottorienNopeus(0);
}

void liikuTaakseJaKaannyVasemmalle() {
    // nopeus on välillä -1023 - 1023
    botti.asetaMoottorienNopeus(-600);
    delay(700);
    botti.asetaMoottorin1Nopeus(600);
    botti.asetaMoottorin2Nopeus(0);
    delay(700);
    botti.asetaMoottorienNopeus(0);
}

void liikuEteenpain() {
    botti.asetaMoottorienNopeus(600);
    delay(700);
    botti.asetaMoottorienNopeus(0);
}

void loop() {
  // kutsu aina tätä ensin
  botti.loop();

  // asettaa ledi 1:n värin siniseksi
  botti.asetaLedinVari(0, 0, 0, 255, 255);

  int etuSensorinArvo = botti.lueEtuSensori();
  int sivuSensorinArvo = botti.lueSivuSensori();
  botti.asetaHtml(String("Etusensori: <b>") + String(etuSensorinArvo) + String("</b><br/>") + String("Sivusensori: <b>") + String(sivuSensorinArvo) + String("</b>"));

  if(etuSensorinArvo < etaisyysLiianLahella) {
    liikuTaakseJaKaannyVasemmalle();
  } else {
    liikuEteenpain();
  }
  
  if(sivuSensorinArvo < etaisyysLiianLahella) {
    liikuVasemmalleJaEteenpain();
  } else if(sivuSensorinArvo > etaisyysLiianKaukana) {
    liikuOikealleJaEteenpain();
  }
  
  // odota 100ms eli 0.1s
  delay(100);
}
