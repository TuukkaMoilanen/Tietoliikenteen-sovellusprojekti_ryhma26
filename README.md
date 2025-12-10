# Tietoliikenteen-sovellusprojekti

## Projektin kuvaus

Projektin tarkoituksena oli toteuttaa sulautettu järjestelmä, joka mittaa kiihtyvyysdataa nRF5340-kehitysalustalla ja välittää sen langattomasti Raspberry Pi:lle Bluetooth Low Energy -yhteyden avulla. Raspberry Pi vastaanottaa datan ja siirtää sen edelleen palvelimelle, jossa Ubuntu Server -ympäristö ja MySQL-tietokanta tallentavat tiedot jatkokäsittelyä varten.

Kerättyä mittausdataa hyödynnetään Pythonilla toteutetussa analytiikassa ja koneoppimisessa (K-means). Kokonaisuus yhdistää sulautetun laiteohjelmoinnin, tietoliikenteen, palvelinpuolen ohjelmoinnin ja data-analytiikan.

## Projektin tavoitteet

Projektin päätavoitteet olivat:

- Suunnitella ohjelmisto nRF5340-alustalle kiihtyvyysdatan mittaamiseen SAADC-moduulilla  
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
| Koneoppiminen | K-means klusterointi |
| Versionhallinta | Git ja GitHub |


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

### **5. NRF5340DK – K-means-toteutus ja suorituskyvyn mittaus**

**Ohjelman toiminta:**
- Toteuttaa kevyen K-means-algoritmin sulautetulle laitteelle  
- Lataa esimerkkidatasarjan laitteelle  
- Suorittaa klusteroinnin nRF5340:n prosessorilla  
- Mittaa suorituskyvyn (aika, muistinkäyttö)  
- Arvioi, soveltuuko algoritmi reaaliaikaisesti ajettavaksi laitteessa  

---
## Projektin tulokset

Projektissa onnistuttiin rakentamaan toimiva end-to-end -ratkaisu:

- nRF5340 mittaa kiihtyvyysdatan ja lähettää sen BLE-yhteydellä Raspberry Pi:lle  
- Raspberry Pi vastaanottaa datan ja välittää sen HTTP POST -pyynnöllä palvelimelle  
- Ubuntu Server tallentaa datan MySQL-tietokantaan ja tarjoaa REST API -rajapinnan  
- Python-ohjelmat hakevat datan API:n kautta ja suorittavat esikäsittelyn sekä K-means -klusteroinnin  
- Lopulliset klusterikeskipisteet voidaan viedä takaisin sulautetulle laitteelle C-header -tiedostona  

Kokonaisjärjestelmä toimii vakaasti ja täyttää projektin tavoitteet.

## 9. Johtopäätökset

Projektissa opittiin yhdistämään useita eri teknologia-alueita:

- sulautetun laitteen signaalinkäsittely ja BLE-tiedonsiirto  
- palvelin- ja tietokantaohjelmointi  
- datan analytiikka, esikäsittely ja koneoppiminen  
- järjestelmäarkkitehtuurin suunnittelu sensorista tietokantaan ja analytiikkaan  

Kokonaisuutena projekti onnistui hyvin ja tarjosi käytännön kokemusta IoT-järjestelmien rakentamisesta. Tulokset osoittavat, että kevyitä koneoppimismenetelmiä kuten K-means voidaan hyödyntää sulautetuissa ympäristöissä tietyin rajoittein. Projekti muodostaa vahvan pohjan mahdolliselle jatkokehitykselle, kuten reaaliaikaiselle liikkeentunnistukselle tai laajemmalle datankeruujärjestelmälle.

   
