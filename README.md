# RFID Relay

Ein mit RFID-Karten steuerbares Relay. Es verfügt über die Möglichkeit eine Entgerät zum Beispiel einen Fernseher **ein** und **aus** zu schalten. Nach dem verifizeren durch eine Chipkarte wird `{revalidateDelay}` gewartet, bis die Session ausgelaufen ist und eine neue Verifizerung angefordert wird. Wenn dies in `{turnOffDelay}` nicht passiert, schaltet sich das Relay aus.

> **Offline-Modus** Wenn der Arduino die Verbindung zum Internet verliert, wird er alle `{retryWifiDelay}` versuchen sich erneut zu verbinden, sollte der Arduino keine UUID's im RAM haben wird das Relay auf EIN geschalten bis die Verbindung wiederhergestellt werden kann.
>
## Inhaltsverzeichnis

1. [Hardware Setup](#hardware-setup)
2. [Quellcode Installation](#quellcode-installation)
3. [Konfiguration](#konfiguration)

## Hardware Setup

## Bauteile

Benötigt werden **4 Module** und eine Möglichkeit diese miteinander zu verkabeln, entweder mit **Jumper wires** oder per Kabel und Löten.

### ESP32

Ein ESP32 Mikrocontroller. (Der Computer der das ganze System steuert)
> Auf [Amazon](https://www.amazon.de/dp/B071P98VTG/ref=twister_B07Z6CSD9K?_encoding=UTF8&psc=1) für **8,36 €**.

### RC522

Ein RC522 RFID Modul. (Scannt die Chips)
> Auf [Amazon](https://www.amazon.de/AZDelivery-Reader-Arduino-Raspberry-gratis/dp/B01M28JAAZ/ref=sr_1_1_sspa?dchild=1&keywords=rc522&qid=1618571460&sr=8-1-spons&psc=1&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyVjBRQkNQMUdFUlEyJmVuY3J5cHRlZElkPUEwMjQ0MjgyVElJOUxOOUVWWTZBJmVuY3J5cHRlZEFkSWQ9QTAzNjIyNzgxT0dMVTEzNDhGTE5WJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==) für **4,84 €**.

### Relay

Ein Relay, am besten/einfachsten als Modul. (Steuert den Fernseher)
> Auf [Amazon](https://www.amazon.de/AZDelivery-1-Relais-High-Level-Trigger-Arduino-inklusive/dp/B07TYG14N6/ref=sr_1_7_sspa?__mk_de_DE=ÅMÅŽÕÑ&dchild=1&keywords=Relay+Module&qid=1618571881&sr=8-7-spons&psc=1&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyMlY4QVgwS0pYT05VJmVuY3J5cHRlZElkPUEwNDU3MzgyM1RSQzNBV0daSTVYSSZlbmNyeXB0ZWRBZElkPUEwMzgzNDk0M0pXMThFVU5HUTRZViZ3aWRnZXROYW1lPXNwX210ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=) für **4,33 €**.

### Buzzer

Ein Buzzer. (Optional, für das Audio-Feedback)
> Auf [Amazon](https://www.amazon.de/AZDelivery-KY-006-Passives-Buzzer-Arduino/dp/B089QHLRSG/ref=sr_1_3_sspa?__mk_de_DE=ÅMÅŽÕÑ&dchild=1&keywords=Buzzer&qid=1618572043&sr=8-3-spons&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyT05TWExRTDZRVFBPJmVuY3J5cHRlZElkPUEwOTIyNzczMjVGODJLMVVURTQ5SiZlbmNyeXB0ZWRBZElkPUEwOTQ3NzMwMUdEUjBUSEdLSFNSWCZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU&th=1) für **4,84 €**.

> **Wichtig** Alle Module sind bei den meisten Online-Shops und Elektronik-Läden erhältlich. Der Preis beträgt fast ein Drittel wenn die Module in höherer Stückzahl gekauft werden.

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

## Quellcode Installation

## Arduino IDE

### ESP32 Support

Zuerst muss in der Arduino IDE, das ESP32 als Board hinzugefügt werden, hierfür muss man in die **Einstellungen**, dann zu **Additional Boards Manager URLs** und dort den Link einfügen.

<pre>
<code><a href="https://dl.espressif.com/dl/package_esp32_index.json" style="color: #4183c4">https://dl.espressif.com/dl/package_esp32_index.json</a></code>
</pre>

Danach die IDE Neustarten und in **Tools/Boards/Boards Manager** nach esp32 suchen und installieren.

### CP210 Treiber

Um eine Verbindung mit dem ESP32 zu ermöglichen werden die **CP210** Treiber benötigt **(auf MacOS Systemen sind diese vorinstalliert)** auf Linux und Windows Geräten wird dieser Treiber aber benötigt diesen bekommt man <a href="https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers">hier</a>.

## Python

Um den ESP32 verwenden zu können muss Python installiert sein das sollte normalerweise aber schon auf deinem Computer installiert sein falls nicht ist hier eine [Anleitung](https://www.python.org/downloads/) zur installation.
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
Wenn alles installiert ist kann fortgefahren werden. Als nächstes kann das GIT-Repository entweder geklont werden oder einfach als zib heruntergeladen werden. Danach muss man `/rfid_02/rfid_02.ino` öffnen. In der Arduino IDE bei `Tools/Board/ESP32 Arduino/ESP32 Dev Module` auswählen und bei `Tools/Port` den Port des angeschlossenen Arduinos auswählen. Jetzt kann der Sketch mit Upload auf den Arduino geladen werden. Weiter Konfiguration können wie unten erklärt am Sketch vorgenommen werden.

> **Wichtig** Bei Problemen kontaktiert mich bitte entweder über Discord `Toqix#5435` oder per Issue.

## Konfiguration

### Lokale Einstellungen

Kann nur in der `rfid_02.ino` geändert werden. (/rfid_02/rfid_02.ino)

| Name                                             | Typ                    | Standartwert                                | Information |
| ------------------------------------------------ | ---------------------- | ------------------------------------------- | ----------- |
| <code style=" color: #4183c4;">ssid</code>       | <code >string</code>   | <code ><strong>erforderlich</strong></code> |Wlan Name.|
| <code style=" color: #4183c4;">password</code>   | <code >string</code>   | <code ><strong>erforderlich</strong></code> |Wlan Passwort.|
| <code style=" color: #4183c4;">server</code>     | <code >string</code>   | <code ><strong>erforderlich</strong></code> | Link zur JSON-Datei mit den UUIDs.|
| <code style=" color: #4183c4;">mastercard</code> | <code >string</code> | <code >""</code>                          |UUID einer Chip-Karte, welche immer funktioniert.|
| <code style=" color: #4183c4;">relay</code>   | <code >int</code> | <code >16</code> |GPIO Pin an den das Relay angeschlossen wird.|
| <code style=" color: #4183c4;">buzzer</code>   | <code >int</code> | <code >12</code> |GPIO Pin an den der Buzzer angeschlossen wird.|
| <code style=" color: #4183c4;">retryWifiDelay</code>   | <code >long int</code> | <code >12000</code> |Interval in millisekunden, in welchem der Arduino versucht sich erneut mit dem WLAN zu verbinden.|



## Globale Einstellungen

Alle Globalen Einstellungen werden in einer JSON-Datei, welche auf einem Webserver hinterlegt wird gesetzt. \
 *(Eine Anleitung um einen solchen Server einzurichten, welcher diese Datei hostet gibt es auf diesem Github aktuell nicht.)*

```json
{
    "turnOffDelay": 45,
    "revalidate": true,
    "revalidateDelay": 5,
    "notify": true,
    "automaticReloadDelay": 6,
    "cards": [
        {
            "owner": "Vorname Nachname",
            "uuid": "ID"
        }
    ]
}
```

`"owner"` ist optional und nur zur wiedererkennung der UUIDs gedacht.

### Funktion der Variablen

Werden in der JSON gesetzt und vom jedem Arduino im Netzwerk gespeichert.
| Name                                                  | Typ                   | Standartwert       | Information |
| ----------------------------------------------------- | --------------------- | ------------------ | ----------- |
| <code style=" color: #4183c4;">turnOffDelay</code>           | <code >int</code>     | <code >45</code>   |Zeit in Minuten bis zur Warnung vor dem Ausschalten.|
| <code style=" color: #4183c4;">revalidate</code>      | <code >boolean</code> | <code >true</code> |Ob nach `turnOffDelay` eine Warnung vor dem nach `revalidateDelay` ausschalten mit der möglichkeit zum einbehalten gesendet werden soll. |
| <code style=" color: #4183c4;">revalidateDelay</code> | <code >int</code>     | <code >5</code>   |Zeit in Minuten nach der ausgeschalten wird.|
| <code style=" color: #4183c4;">notify</code>      | <code >boolean</code>  | <code >false</code>                         |Ob Sound Benachrigungen aktiv sein sollen. Z.B.: Beim ein/aus schalten, warnungen, ... .|
| <code style=" color: #4183c4;">automaticReloadDelay</code>      | <code >int</code>  | <code >6</code>                         |Zeitinterval in Stunden in dem der Arduino sich mit dem Server synchronisiert. *(Neue UUIDs ladet.)*|
| <code style=" color: #4183c4;">uuids</code>|JsonArray| <code >{}</code> |Liste von JsonObject/Card mit Benutzer und UUID. |
| <code style=" color: #4183c4;">card/uuid</code>|JsonObject| <code >erforderlich</code> |JsonObject/Card mit Benutzer und UUID. (gespeichert in uuids). |

## ToDo

------

- [x] Readme
- [x] JSON processing
- [x] Sound-Notification
- [x] Test system

> **Note:** Bei Problem oder Fragen können Sie mich auf **Discord** erreichen oder ein **Issue** erstellen. (Toqix#5435)

> Credits: <a href="https://github.com/toqix" style="color: #4183c4;">toqix</a>
