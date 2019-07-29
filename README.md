# IlvesBotti2019 :robot:
Ilves 2019 Arduinobotti
## Asennus
1. Asenna Arduino IDE v1.8.9: https://www.arduino.cc/en/main/software
2. Lataa IlvesBotti2019 -kirjasto tästä reposta ja pura se Arduino/libraries -kansion alle.
3. Lataa NeoPixelBus -kirjasto Arduinosta
4. Lisää Arduinon board manageriin seuraava url (File -> Preferences -> Additional Boards Manager URLs):
```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
5. Avaa Board manager (Tools -> Board -> Boards Manager)
6. Hae ja asenna: "esp8266 by ESP8266 Community"
7. Valitse Tools -> board -valikosta: "LOLIN (Wemos) D1 mini Pro"
8. Kytke Wemos D1 mini pro usb -porttiin ja valitse oikea portti Tools -> port -valikosta
9. Hae botille tyhjä templaatti täältä: https://github.com/kungfo0/IlvesBotti2019/blob/master/example/IlvesBotti2019-tyhja.ino
10. Koodaa botti ja nauti tuloksista :heart_eyes:

## Ohjeet
### Esimerkkiratkaisun lataaminen botille
1. Avaa esimerkkiratkaisu kansiosta Arduino/IlvesBotti2019/IlvesBotti2019-esimerkkiratkaisu.ino tuplaklikkaamalla sitä.
2. Valitse Arduinon Tools -valikon Board -alasvetovalikosta arvo "LOLIN (Wemos) D1 mini Pro"
3. Kytke botti usb-porttin ja odota pieni hetki, että kone tunnistaa sen
4. Valitse Arduinon Tools -valikon Port -alasvetovalikosta oikea portti. Portteja pitäisi olla yksi ja sen nimi pitäisi olla COM1 - COM9.
5. Käännä ja lataa esimerkkiratkaisu bottiin klikkaamalla Upload -nappia ja odota kunnes Arduino tulostaa näytölle "Done uploading".
6. Nyt esimerkkiratkaisu on botissa ja voit irroittaa usb -kaapelin ja testata bottia radalla.

### Oman ratkaisun lataaminen botille


