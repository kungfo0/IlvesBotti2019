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
7. Valitse Tools -> board -valikosta: "Wemos D1 mini Pro"
8. Kytke Wemos D1 mini pro usb -porttiin ja valitse oikea portti Tools -> port -valikosta
9. Hae botille tyhjä templaatti täältä: https://github.com/kungfo0/IlvesBotti2019/blob/master/example/IlvesBotti2019-tyhja.ino
10. Koodaa botti ja nauti tuloksista :heart_eyes:
