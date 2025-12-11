# Tietoliikenteen-sovellusprojekti

## Projektin kuvaus

Projektin tehtävänä oli suunnitella nRF5340-kehitysalustalla ajettava ohjelma, joka mittaa kiihtyvyysanturin dataa ja välittää mitatun datan Raspberry Pi:lle Bluetooth Low Energy -yhteyden kautta. Raspberry Pi välittää vastaanottamansa datan MySQL-tietokantaan.

Tietokantaan tallentuvaan dataan toteutettiin TCP-sokettirajapinta sekä yksinkertainen HTTP-rajapinta. Kerättyä dataa haetaan HTTP-rajapinnan kautta omaan kannettavaan tietokoneeseen Pythonilla koodatulla ohjelmalla, jossa dataa esikäsitellään ja hyödynnetään koneoppimisessa.

Projektissa yhdistyvät laiteohjelmointi, tietoliikenne, palvelinohjelmointi ja koneoppimisen perusteet.


## Projektin tavoitteet

Projektin päätavoitteet olivat:

- Suunnitella ohjelmisto nRF5340-alustalle kiihtyvyysdatan mittaamiseen 
- Toteuttaa BLE-pohjainen tiedonsiirto Raspberry Pi:lle  
- Rakentaa Raspberry Pi:lle vastaanottava BLE-klientti ja HTTP-lähetysjärjestelmä  
- Luoda Ubuntu Serverille MySQL-tietokanta ja REST API datan tallentamiseen ja hakemiseen  
- Toteuttaa Python-ohjelmistot datan esikäsittelyyn, analysointiin ja K-means -klusterointiin  
- Arvioida K-means -algoritmin suorituskykyä sulautetulla laitteella  
- Dokumentoida järjestelmä ja sen arkkitehtuuri selkeästi  


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

## Käytetyt teknologiat ja työkalut

| Osa-alue | Teknologiat |
|----------|-------------|
| Sulautettu laite | C, Zephyr RTOS, nRF Connect SDK, SAADC |
| Langaton tiedonsiirto | Bluetooth Low Energy (BLE) |
| Raspberry Pi | Python, Bleak |
| Palvelin | Ubuntu Server, MySQL, Python |
| Datan analytiikka | Python, NumPy, Pandas, Matplotlib |
| Koneoppiminen | K-means |
| Versionhallinta | GitHub |


## Projektissa tehdyt ohjelmat

---

### **1. Kiihtyvyysdatan mittaus ja lähetys (nRF5340DK + GY-61)**

**Ohjelman toiminta:**
- Lukee X-, Y- ja Z-akselien analogiset arvot SAADC-moduulilla  
- Muuntaa arvot digitaaliseen muotoon 
- Pakkaa mittaustulokset BLE-lähetyspakettiin  
- Lähettää datan BLE-yhteydellä Raspberry Pi:lle  

---

### **2. Python TCP/Socket -ohjelma**

**Ohjelman toiminta:**
- Vastaanottaa tai lähettää kiihtyvyysdataa TCP-yhteydellä  
- Tallentaa vastaanotetun datan analyysiä varten  
- Toimi testivaiheen yhteysratkaisuna ennen lopullista REST API -toteutusta  

---

### **3. Python-tietokantaohjelma (MySQL)**

**Ohjelman toiminta:**
- Yhdistyy MySQL-tietokantaan  
- Lukee kiihtyvyysdatan tauluista  
- Esikäsittelee datan (suodatus, normalisointi, segmentointi)  
- Syöttää datan analytiikkaohjelmille  

---

### **4. Python K-means -algoritmi**

**Ohjelman toiminta:**
- Lataa tietokannasta tai tiedostosta esikäsitellyt kiihtyvyysarvot  
- Alustaa satunnaiset keskipisteet  
- Sijoittaa datapisteet lähimpään klusteriin  
- Päivittää keskipisteet, kunnes tulos vakioituu  
- Visualisoi datan ja klusterit 3D-kuviona

  <img width="400" height="270" alt="image" src="https://github.com/user-attachments/assets/96ef006d-45d5-4153-84c1-047c4756632c" />

  <img width="400" height="270" alt="image" src="https://github.com/user-attachments/assets/757498ac-db44-49b3-a7c7-65627d418319" />

- Tallentaa lopulliset keskipisteet C-kieliseen header-tiedostoon (`keskipisteet.h`)  

---

### **5. nRF5340DK – K-means-luokittelijan toteutus**

**Ohjelman toiminta:**
- Toteuttaa kevyen K-means-algoritmin sulautetulle laitteelle  
- Lataa kiihtyvyysdatan laitteelle  
- Suorittaa klusteroinnin nRF5340DK kehitysalustalle
- Päivittää confusion matriisin

  **Miten confusion matriisi toteutettiin:**
  Confusion-matriisi lasketaan siten, että jokaisen luokittelun jälkeen päivitetään matriisin solu C[i][j], jossa  i on todellinen luokka ja j ennustettu luokka. Jos järjestelmä ennustaa oikein, kyseisen solun arvoa kasvatetaan yhdellä: C[i][i]=C[i][i]+1. Toistamalla    tämä kaikille näytteille saadaan matriisi, joka kuvaa, kuinka usein kukin luokka ennustettiin oikein tai väärin.
  Matriisin päädiagonaali sisältää kaikki oikein luokitellut näytteet, ja mitä tarkempi luokitin on, sitä suuremmiksi diagonaalin arvot kasvavat.
  Diagonaalin ulkopuoliset solut puolestaan kuvaavat virheellisiä ennusteita ja pysyvät pieninä, jos luokitin toimii hyvin.

  
  <img width="464" height="284" alt="image" src="https://github.com/user-attachments/assets/66929f37-2e7e-48fc-ae4e-e8c001b327eb" />


---
## Projektin tulokset

Projektissa onnistuttiin rakentamaan toimiva end-to-end -ratkaisu:

- nRF5340 mittaa kiihtyvyysdatan ja lähettää sen BLE-yhteydellä Raspberry Pi:lle  
- Raspberry Pi vastaanottaa datan ja välittää sen HTTP POST -pyynnöllä palvelimelle  
- Ubuntu Server tallentaa datan MySQL-tietokantaan ja tarjoaa REST API -rajapinnan  
- Python-ohjelmat hakevat datan API:n kautta ja suorittavat esikäsittelyn sekä K-means -klusteroinnin  
- Lopulliset klusterikeskipisteet voidaan viedä takaisin sulautetulle laitteelle C-header -tiedostona  

Kokonaisjärjestelmä toimii vakaasti ja täyttää projektin tavoitteet.

## Johtopäätökset

Projektissa opittiin yhdistämään useita eri teknologia-alueita:

- sulautetun laitteen signaalinkäsittely ja BLE-tiedonsiirto  
- palvelin- ja tietokantaohjelmointi  
- datan analytiikka, esikäsittely ja koneoppiminen  
- järjestelmäarkkitehtuurin suunnittelu sensorista tietokantaan ja analytiikkaan  

Kokonaisuutena projekti onnistui hyvin ja tarjosi käytännön kokemusta IoT-järjestelmien rakentamisesta. Tulokset osoittavat, että kevyitä koneoppimismenetelmiä kuten K-means voidaan hyödyntää sulautetuissa ympäristöissä tietyin rajoittein. Projekti muodostaa vahvan pohjan mahdolliselle jatkokehitykselle, kuten reaaliaikaiselle liikkeentunnistukselle tai laajemmalle datankeruu- ja käsittelyjärjestelmälle.

   
