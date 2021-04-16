# RFID Relay

Ein mit RFID-Karten steuerbares Relay. Es verfügt über die Möglichkeit einen zum Beispiel Fernseher **ein** und **aus** zu schalten. Nach dem verifizeren durch eine Chipkarte wird `{revalidateDelay}` gewartet, bis die Session ausgelaufen ist und eine neue Verifizerung angefordert wird. Wenn dies in `{turnOffDelay}` nicht passiert, schaltet das Relay sich aus.

# Inhaltsverzeichnis

1. <a href="#setup" style="color: #4183c4;">Installation/ Setup</a>
2. <a href="#setup2" style="color: #4183c4;">Installation für die Verwendung des Codes</a>
3. <a href="#config" style="color: #4183c4;">Konfiguration</a>

<br>

# Installation/ Setup<a name="setup"></a>

## Bauteile
Benötigt werden **4 Module** und eine Möglichkeit alles zu verbinden, entweder mit **Jumper wires** oder per Kabel und Löten. 

### ESP32
Ein ESP32 Mikrocontroller. (Der Computer der das ganze System steuert)
> Auf <a href="https://www.amazon.de/dp/B071P98VTG/ref=twister_B07Z6CSD9K?_encoding=UTF8&psc=1">Amazon</a> für **8,36 €**. 
### RC522
Ein RC522 RFID Modul. (Scannt die Chips)
> Auf <a href="https://www.amazon.de/AZDelivery-Reader-Arduino-Raspberry-gratis/dp/B01M28JAAZ/ref=sr_1_1_sspa?dchild=1&keywords=rc522&qid=1618571460&sr=8-1-spons&psc=1&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyVjBRQkNQMUdFUlEyJmVuY3J5cHRlZElkPUEwMjQ0MjgyVElJOUxOOUVWWTZBJmVuY3J5cHRlZEFkSWQ9QTAzNjIyNzgxT0dMVTEzNDhGTE5WJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==">Amazon</a> für **4,84 €**. 

### Relay
Ein Relay, am besten/einfachsten als Modul. (Steuert den Fernseher)
> Auf <a href="https://www.amazon.de/AZDelivery-1-Relais-High-Level-Trigger-Arduino-inklusive/dp/B07TYG14N6/ref=sr_1_7_sspa?__mk_de_DE=ÅMÅŽÕÑ&dchild=1&keywords=Relay+Module&qid=1618571881&sr=8-7-spons&psc=1&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyMlY4QVgwS0pYT05VJmVuY3J5cHRlZElkPUEwNDU3MzgyM1RSQzNBV0daSTVYSSZlbmNyeXB0ZWRBZElkPUEwMzgzNDk0M0pXMThFVU5HUTRZViZ3aWRnZXROYW1lPXNwX210ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=">Amazon</a> für **4,33 €**.

### Buzzer
Ein Buzzer. (Optional, für das Audio-Feedback)
> Auf <a href="https://www.amazon.de/AZDelivery-KY-006-Passives-Buzzer-Arduino/dp/B089QHLRSG/ref=sr_1_3_sspa?__mk_de_DE=ÅMÅŽÕÑ&dchild=1&keywords=Buzzer&qid=1618572043&sr=8-3-spons&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyT05TWExRTDZRVFBPJmVuY3J5cHRlZElkPUEwOTIyNzczMjVGODJLMVVURTQ5SiZlbmNyeXB0ZWRBZElkPUEwOTQ3NzMwMUdEUjBUSEdLSFNSWCZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU&th=1">Amazon</a> für **4,84 €**.

> <a style=" color: #4183c4;">**Wichtig** Alle Module sind bei den meisten Online-Shops und Elektronik-Läden erhältlich. Der Preis beträgt fast ein Drittel wenn die Module in höherer Stückzahl gekauft werden.

## Connection

So wird der ESP32 mit dem RC522 verbunden:
| ESP32   | RC22 |
| ------- | ---- |
| Gpio 23 | MOSI |
| Gpio 22 | RST  |
| Gpio 21 | SDA  |
| Gpio 19 | MISO |
| Gpio 18 | SCK  |
| GND     | GND  |
| 3v3     | VCC  |

Relay
|ESP32| Relay|
|-----|------|
|Gpio 16 |S|
|GND|-|
|3v3|+|

Buzzer
|ESP32|Buzzer|
|---|---|
|Gpio 12|+|
|GND|-|

# Installation für die Verwendung/ Bearbeitung des Quellcodes<a name="setup2"></a>

## Arduino IDE

Zuerst muss in der Arduino IDE, das ESP32 als Board hinzugefügt werden, hierfür in die **Einstellungen**, dann **Additional Boards Manager URLs** und dort den Link einfügen.

<pre>
<code><a href="https://dl.espressif.com/dl/package_esp32_index.json" style="color: #4183c4">https://dl.espressif.com/dl/package_esp32_index.json</a></code>
</pre>

Danach die IDE Neustarten und in **Tools/Boards/Boards Manager** hier dann nach esp32 suchen und installieren.
**CP210 Treiber**
Um eine Verbindung mit dem ESP32 zu ermöglichen werden die **CP210** Treiber benötigt **(auf MacOS Systemen sind diese vorinstalliert)** auf Linux und Windows Geräten wird dieser Treiber aber benötigt diesen bekommt man <a href="https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers">hier</a>.

## Python

Um den ESP32 verwenden zu können muss Python installiert sein das sollte normalerweise aber schon auf deinem Computer installiert sein falls nicht ist hier eine Anleitung zur installation.

<pre>
<code><a href="https://www.python.org/downloads/" style="color: #4183c4">https://www.python.org/downloads/</a></code>
</pre>

### PySerial

Wenn bem hochladen des codes Probleme auftreten, da Serial nicht gefunden wird liegt das mit hoher Wahrscheinlichkeit an einer fehlenden PySerial installation <em>(auch dies sollte vorinstalliert sein)</em><br>
Wenn **pip** nicht installiert ist:

```console
$ sudo apt-get install pip3
```

Danch um **PySerial** zu installieren:

```console
$ pip3 install pyserial
```

Warten bis alles installiert ist und dann sollte der Code ohne Fehler kompilieren.

> Wichtig: Hierbei werden Administratorrechte benötigt.

## Installation
Wenn alles installiert ist kann fortgefahren werden. Als nächstes kann entweder das GIT-Repository geklont werden oder einfach als zib heruntergeladen werden. Danach muss man `/rfid_02/rfid_02.ino` öffnen. In der Arduino IDE bei `Tools/Board/ESP32 Arduino/ESP32 Dev Module` auswählen und bei `Tools/Port` den Port des angeschlossenen Arduinos auswählen. Jetzt kann der Sketch mit Upload auf den Arduino geladen werden. Weiter Konfiguration können wie unten erklärt am Sketch vorgenommen werden.

> **Wichtig** Bei Problemen kontaktiert mich bitte entweder über Discord `Toqix#5435` oder per Issue.

# Lokale Konfiguration<a name="config"></a>

Kann nur in der `rfid_02.ino` geändert werden. (/rfid_02/rfid_02.ino)

| Name                                             | Typ                    | Standartwert                                | Information |
| ------------------------------------------------ | ---------------------- | ------------------------------------------- | ----------- |
| <code style=" color: #4183c4;">ssid</code>       | <code >string</code>   | <code ><strong>erforderlich</strong></code> |Wlan Name.|
| <code style=" color: #4183c4;">password</code>   | <code >string</code>   | <code ><strong>erforderlich</strong></code> |Wlan Passwort.|
| <code style=" color: #4183c4;">server</code>     | <code >string</code>   | <code ><strong>erforderlich</strong></code> | Link zur JSON-Datei mit den UUIDs.|
| <code style=" color: #4183c4;">mastercard</code> | <code >string</code> | <code >""</code>                          |UUID einer Chip-Karte, welche immer funktioniert.|
| <code style=" color: #4183c4;">relay</code>   | <code >int</code> | <code >16</code> |GPIO Pin an den das Relay angeschlossen wird.|
| <code style=" color: #4183c4;">buzzer</cod>   | <code >int</code> | <code >12</code> |GPIO Pin an den der Buzzer angeschlossen wird.|



# Globale Konfiguration

## UUID hinzufügen/ entfernen

Inteface:

```json
{
    "turnOffDelay": 45,
    "revalidate": true,
    "revalidateDelay": 5,
    "notify": true,
    "cards": [
        {
            "owner": "Vorname Nachname",
            "uuid": "ID"
        }
    ]
}
```

`"owner"` ist optional und nur zur wiedererkennung der UUIDs gedacht.

## Haupteinstellungen
Werden in der JSON gesetzt und vom Arduino gespeichert.
| Name                                                  | Typ                   | Standartwert       | Information |
| ----------------------------------------------------- | --------------------- | ------------------ | ----------- |
| <code style=" color: #4183c4;">turnOffDelay</code>           | <code >int</code>     | <code >45</code>   |Zeit in Minuten bis zur Warnung vor dem Ausschalten.|
| <code style=" color: #4183c4;">revalidate</code>      | <code >boolean</code> | <code >true</code> |Ob nach `turnOffDelay` eine Warnung vor dem nach `revalidateDelay` ausschalten mit der möglichkeit zum einbehalten gesendet werden soll. |
| <code style=" color: #4183c4;">revalidateDelay</code> | <code >int</code>     | <code >5</code>   |Zeit in Minuten nach der ausgeschalten wird.|
| <code style=" color: #4183c4;">notify</code>      | <code >boolean</code>  | <code >false</code>                         |Ob Sound Benachrigungen aktiv sein sollen. Z.B.: Beim ein/aus schalten, warnungen, ... .|
| <code style=" color: #4183c4;">uuids</code>|JsonArray| <code >{}</code> |Liste von JsonObject/Card mit Benutzer und UUID. |
| <code style=" color: #4183c4;">card/uuid</code>|JsonObject| <code >erforderlich</code> |JsonObject/Card mit Benutzer und UUID. (gespeichert in uuids). |

# ToDo
- [x] Readme
- [x] JSON processing
- [ ] Store Settings in EEPROM
- [x] Sound-Notification
- [x] Test system

> **Achtung:** EEPROM hat nur 256 plätze für je 256bits, was das speichern von UUIDs unmöglich macht  -> Settings ohne UUIDs zu speichern ist nur begrenzt sinnvoll.
> **Note:** Bei Problem oder Fragen können Sie mich auf **Discord** erreichen oder ein **Issue** erstellen.

> Credits: <a href="https://github.com/toqix" style="color: #4183c4;">toqix</a>
