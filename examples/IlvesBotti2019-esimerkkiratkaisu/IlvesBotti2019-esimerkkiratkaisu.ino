#include <IlvesBotti2019.h>

IlvesBotti2019 botti;

void setup() {
    botti.setup();
}

// botti yrittää olla noin 15cm päästä oikean puolisesta seinästä
int haluttuEtaisyys = 15;

int maksimiNopeus = 1023;

// kulkunopeus on puolet maksiminopeudesta
float vasenNopeus = maksimiNopeus / 2;
float oikeaNopeus = maksimiNopeus / 2;

void loop() {
    // kutsu aina tätä ensin
    botti.loop();

    if(!botti.stopped) {
        int etuSensorinArvo = botti.lueEtuSensori();
        delay(5);
        int sivuSensorinArvo = botti.lueSivuSensori();
        
        if(sivuSensorinArvo <= haluttuEtaisyys && etuSensorinArvo > haluttuEtaisyys) {
            // seuraa seinää
            vasenNopeus = maksimiNopeus * 0.5;
            oikeaNopeus = maksimiNopeus * 0.5;
            if(sivuSensorinArvo <= 0.5 * haluttuEtaisyys) {
                vasenNopeus = maksimiNopeus * 0.3;
                oikeaNopeus = maksimiNopeus * 0.7;
            }
        }
        else if(etuSensorinArvo <= haluttuEtaisyys) {
            // käänny jyrkästi vasemmalle
            vasenNopeus = -(maksimiNopeus / 2);
            oikeaNopeus = maksimiNopeus;
        } else if(sivuSensorinArvo > haluttuEtaisyys * 0.5) {
            // käänny lievästi oikealle
            vasenNopeus = maksimiNopeus * 0.6;
            oikeaNopeus = maksimiNopeus * 0.5;
        }

        botti.asetaMoottorin1Nopeus(vasenNopeus);
        botti.asetaMoottorin2Nopeus(oikeaNopeus);
    }

    // venaa hetki
    delay(10);
}