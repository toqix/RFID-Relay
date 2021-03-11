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
| <code style=" color: #4183c4;">ssid</code>       | <code >string</code>   | <code ><strong>erforderlich</strong></code> |             |
| <code style=" color: #4183c4;">password</code>   | <code >string</code>   | <code ><strong>erforderlich</strong></code> |             |
| <code style=" color: #4183c4;">server</code>     | <code >string</code>   | <code ><strong>erforderlich</strong></code> |             |
| <code style=" color: #4183c4;">pathToJSON</code> | <code >string</code>   | <code ><strong>erforderlich</strong></code> |             |
| <code style=" color: #4183c4;">mastercard</code> | <code >object[]</code> | <code >'{}'</code>                          |             |

# Globale Konfiguration

## UUID hinzufügen/ entfernen

Inteface:

```json
"cards": [
    {
        "owner": "",
        "uuid": ""
    }
]
```

`"owner"` ist optional und nur zur wiedererkennung der UUIDs gedacht.

## Haupteinstellungen

| Name                                                  | Typ                   | Standartwert       | Information |
| ----------------------------------------------------- | --------------------- | ------------------ | ----------- |
| <code style=" color: #4183c4;">delay</code>           | <code >int</code>     | <code >45</code>   |             |
| <code style=" color: #4183c4;">revalidate</code>      | <code >boolean</code> | <code >true</code> |             |
| <code style=" color: #4183c4;">revalidateDelay</code> | <code >int</code>     | <code >45</code>   |             |
| <code style=" color: #4183c4;">turnOff</code>         | <code >boolean</code> | <code >true</code> |             |
| <code style=" color: #4183c4;">turnOffDelay</code>    | <code >int</code>     | <code >0.5</code>  |             |

## Pins

| Name                                          | Typ               | Standartwert     | Information |
| --------------------------------------------- | ----------------- | ---------------- | ----------- |
| <code style=" color: #4183c4;">SS_PIN</code>  | <code >int</code> | <code >21</code> |             |
| <code style=" color: #4183c4;">RST_PIN</code> | <code >int</code> | <code >22</code> |             |
| <code style=" color: #4183c4;">relay</code>   | <code >int</code> | <code >16</code> |             |

## Notify

| Name                                              | Typ                    | Standartwert                                | Information |
| ------------------------------------------------- | ---------------------- | ------------------------------------------- | ----------- |
| <code style=" color: #4183c4;">audio</code>       | <code >object[]</code> | <code >{}</code>                            |             |
| <code style=" color: #4183c4;">notify</code>      | <code >boolean</code>  | <code >false</code>                         |             |
| <code style=" color: #4183c4;">pathToSound</code> | <code >string</code>   | <code ><strong>erforderlich</strong></code> |             |
| <code style=" color: #4183c4;">volume</code>      | <code >int</code>      | <code >1</code>                             |             |

\*Erforderlich falls 'notify' auf true gesetzt ist.

> **Note:** Bei Problem oder Fragen können Sie mich auf **Discord** erreichen oder ein **Issue** erstellen.

> Credits: <a href="https://github.com/toqix" style="color: #4183c4;">Toqix</a>
