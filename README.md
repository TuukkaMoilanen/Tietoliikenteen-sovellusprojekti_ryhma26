# Tietoliikenteen-sovellusprojekti

## Projektin kuvaus
Tehtävänä on suunnitella nRF5340:llä client, joka mittaa anturidataa kiihtyvyysanturilla ja välittää tietoa Raspberry Pi:lle. Raspberry välittää dataa omalle ja tai Oamkin MySQL-palvelimelle.
Tietokantaan tallentuvaan dataan on TCP-sokettirajapinta (jos käytössä Oamkin MySQL) ja yksinkertainen HTTP API (edelleen, jos Oamkin tietokanta käytössä). Kerättyä dataa haetaan HTTP-rajanpinnasta omaan kannettavaan koodatulla Python-ohjelmalla ja käsitellään koneoppimistarkoituksiin.


## Arkkitehtuurikuva projektista 
<img width="1680" height="804" alt="Blank diagram" src="https://github.com/user-attachments/assets/364c9aac-cb1c-4bfb-be7b-53c448e95b9c" />

## Projektissa käytetyt laitteet

| Laite | Käyttötarkoitus |
|-------|------------------|
| **nRF5340DK** | Kiihtyvyysdatan mittaus ja BLE-lähetys |
| **Kiihtyvyysanturi** | X/Y/Z signaali |
| **Raspberry Pi** | BLE-vastaanotin, HTTP Post → Ubuntu |
| **Ubuntu Server** | MySQL-tietokanta, REST API|
| **Kehitysläppäri / PC** | analytiikka, GitHub |


## Projektissa tehdyt ohjelmat

1. Ohjelma kiihtyvyysdatan mittaukseen ja lähetykseen (nRF5340DK + GY-61)
   
   Ohjelman toiminta:

   Lukee kiihtyvyysanturilta X-, Y- ja Z-akselien analogiset jännitearvot SAADC-moduulin avulla.

   Muuntaa arvot digitaaliseen muotoon ja normalisoi ne käsittelyä varten.

   Pakkaa mittaustulokset BLE-lähetyspakettiin.

   Lähettää kiihtyvyysdatan Bluetooth Low Energy -yhteydellä Raspberry Pi:lle.

3. Python TCP/socket ohjelma

   Ohjelman toiminta:

5. Pythonilla tietokannan luku ohjelma

   Ohjelman toiminta:

7. Python K-means algoritmi

   Ohjelman toiminta:

9. NRF5340DK:lle ohjelma K-means algoritmin toteutukseen ja algoritmin suorituskyvyn mittaukseen

    Ohjelman toiminta:

   
