<h1 id="rfid-relay">RFID Relay</h1>
<p>Ein mit RFID-Karten steuerbares Relay. Sicheres <strong>ein/aus</strong> schalten eines Fernsehers oder ähnliches. Nach dem einschalten wird ein Timer gestartet der nach 40 Minuten eine Warnung ausgibt und dann nach 5 Minuten das Relay wieder abschaltet, sofern die Chipkarte nicht ein weiteres mal vorgehalten.</p>
<h1 id="installation">Installation</h1>
<p>Wie alles was benötigt wird installiert wird.</p>
<h2 id="arduino-ide">Arduino IDE</h2>
<p>Um die Arduino IDE mit dem ESP32 zu verwenden muss man als ersten in die Einstellungen.<br>
<strong>Additional Boards Manager</strong></p>
<pre><code>https://dl.espressif.com/dl/package_esp32_index.json
</code></pre>
<p>Danach die IDE Neustarten und in <strong>Tools/Boards/Boards Manager</strong> hier dann nach esp32 suchen und installieren.</p>
<p><strong>CP210 Treiber</strong><br>
Um den ESP32 zu verwenden werden die <strong>CP210</strong> Treiber benötigt <em>(auf MacOS Systemen sind diese vorinstalliert)</em> Auf Linux-Geräten wird dieser Treiber aber benötigt diesen bekommt man  <a href="https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers">Hier</a>.</p>
<h2 id="python">Python</h2>
<p>Um den esp32 verwenden zu können muss Python installiert sein das sollte normalerweise aber schon auf deinem Computer installiert sein falls nicht ist hier eine Anleitung zur installation <a href=":</p>
<pre><code>https://www.python.org/downloads/">Hier</a
</code></pre>
<p><strong>PySerial</strong><br>
Wenn bem hochladen des codes Probleme auftreten, da Serial nicht gefunden wird liegt das mit hoher Wahrscheinlichkeit an einer fehlenden PySerial installation <em>(auch dies sollte vorinstalliert sein)</em><br>
Wenn python pip nicht installiert ist:</p>
<pre><code>sudo apt-get install pip3
</code></pre>
<p>Danch um pyserial zu installieren:</p>
<pre><code>pip3 install pyserial
</code></pre>
<p>Warten bis alles installiert ist und dann sollte der Code ohne Fehler kcompilieren.</p>
<blockquote>
<p>Wichtig hierbei sind Administrator-Rechte benötigt werden</p>
</blockquote>
<h2 id="connection">Connection</h2>
<p>So wird das RC522 an den ESP32 angeschlossen:<br>
<img src="https://hackster.imgix.net/uploads/attachments/704294/nfc_osc_bb_gBdcDg8Rq9.jpg" alt=""></p>
<p>Connection Table</p>
<table>
<thead>
<tr>
<th>ESP32</th>
<th>RC522</th>
</tr>
</thead>
<tbody>
<tr>
<td>Gpio 23</td>
<td>MOSI</td>
</tr>
<tr>
<td>Gpio 22</td>
<td>RST</td>
</tr>
<tr>
<td>Gpio 21</td>
<td>SDA</td>
</tr>
<tr>
<td>Gpio 19</td>
<td>MISO</td>
</tr>
<tr>
<td>Gpio 18</td>
<td>SCK</td>
</tr>
<tr>
<td>GND</td>
<td>GND</td>
</tr>
<tr>
<td>VCC</td>
<td>VCC</td>
</tr>
</tbody>
</table><h1 id="to-do">To Do</h1>
<p>Es fehlt noch die Kommunikation mit einem externen Server um neue UUIDs zu <strong>laden</strong> und alte zu <strong>löschen</strong><br>
<strong>Coming soon!</strong></p>
<blockquote>
<p><strong>Note:</strong> Bei <strong>Problemen</strong> einfach ein <strong>Issue</strong> erstellen ich beantworte Fragen gerne.</p>
</blockquote>
<p>By @<strong>toqix</strong></p>

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE3OTc4MTU4ODJdfQ==
-->
