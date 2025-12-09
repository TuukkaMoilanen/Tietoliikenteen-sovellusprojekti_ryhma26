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

---

### **1. Kiihtyvyysdatan mittaus ja lähetys (nRF5340DK + GY-61)**

**Ohjelman toiminta:**
- Lukee X-, Y- ja Z-akselien analogiset arvot SAADC-moduulilla  
- Muuntaa arvot digitaaliseen muotoon ja normalisoi  
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

   
