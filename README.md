# RFID Relay

Ein mit RFID-Karten steuerbares Relay. Es verfügt über die Möglichkeit einen zum Beispiel Fernseher **ein** und **aus** zu schalten. Nach dem verifizeren durch eine Chipkarte wird `{revalidateDelay}` gewartet, bis die Session ausgelaufen ist und eine neue Verifizerung angefordert wird. Wenn dies in `{turnOffDelay}` nicht passiert, schaltet das Relay sich aus.

# Inhaltsverzeichnis

1. <a href="#setup" style="color: #4183c4;">Installation/ Setup</a>
2. <a href="#setup2" style="color: #4183c4;">Installation für die Verwendung des Codes</a>
3. <a href="#config" style="color: #4183c4;">Konfiguration</a>

<br>

# Installation/ Setup<a name="setup"></a>

## Voraussetzungen

1. <a href="#first" style="color: #4183c4;">first</a>
2. <a href="#first" style="color: #4183c4;">second</a>
3. <a href="#first" style="color: #4183c4;">third</a>

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
| VCC     | VCC  |

> **Missing:** Sound-Modul, Relay

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

# Lokale Konfiguration<a name="config"></a>

Kann nur in der 'rfid_02.ino' geändert werden. (/rfid_02/rfid_02.ino)

| Name                                             | Typ                    | Standartwert                                | Information |
| ------------------------------------------------ | ---------------------- | ------------------------------------------- | ----------- |
| <code style=" color: #4183c4;">ssid</code>       | <code >string</code>   | <code ><strong>erforderlich</strong></code> |Wlan Name.|
| <code style=" color: #4183c4;">password</code>   | <code >string</code>   | <code ><strong>erforderlich</strong></code> |Wlan Passwort.|
| <code style=" color: #4183c4;">server</code>     | <code >string</code>   | <code ><strong>erforderlich</strong></code> | Link zur JSON-Datei mit den UUIDs.|
| <code style=" color: #4183c4;">mastercard</code> | <code >object[]</code> | <code >'{}'</code>                          |UUID einer Chip-Karte, welche immer funktioniert.|
| <code style=" color: #4183c4;">relay</code>   | <code >int</code> | <code >16</code> |GPIO Pin an den das Relay angeschlossen wird.|

# Globale Konfiguration

## UUID hinzufügen/ entfernen

Inteface:

```json
{
    "delay": 45,
    "revalidate": true,
    "revalidateDelay": 5,
    "",
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
### Abschallt-Zeiten
| Name                                                  | Typ                   | Standartwert       | Information |
| ----------------------------------------------------- | --------------------- | ------------------ | ----------- |
| <code style=" color: #4183c4;">delay</code>           | <code >int</code>     | <code >45</code>   |Zeit in Minuten bis zur Warnung vor dem Ausschalten.|
| <code style=" color: #4183c4;">revalidate</code>      | <code >boolean</code> | <code >true</code> |Ob nach `delay` eine Warnung vor dem nach `revalidateDelay` ausschalten mit der möglichkeit zum einbehalten gesendet werden soll. |
| <code style=" color: #4183c4;">revalidateDelay</code> | <code >int</code>     | <code >5</code>   |Zeit in Minuten nach der ausgeschalten wird.|

### Notifikation

| Name                                              | Typ                    | Standartwert                                | Information |
| ------------------------------------------------- | ---------------------- | ------------------------------------------- | ----------- |
| <code style=" color: #4183c4;">notify</code>      | <code >boolean</code>  | <code >false</code>                         |Ob die Warnung mit einem Sound gemacht werden soll.|
| <code style=" color: #4183c4;">pathToSound</code> | <code >string</code>   | <code ><strong>erforderlich</strong></code> |Der Dateipfad zum Sound auf der eingesteckten SD-Karte|
| <code style=" color: #4183c4;">volume</code>      | <code >int</code>      | <code >1</code>                             |Lautstärke des Sounds, 0-100.|

\*Erforderlich falls 'notify' auf true gesetzt ist.

# ToDo
[x] Readme
[x] JSON processing
[ ] Store Settings in EEPROM
[ ] Sound-Notification
[ ] Test system
> **Note:** Bei Problem oder Fragen können Sie mich auf **Discord** erreichen oder ein **Issue** erstellen.

> Credits: <a href="https://github.com/toqix" style="color: #4183c4;">Toqix</a>
