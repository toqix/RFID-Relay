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
<h2 id="python">Python</h2>
<p>Um den esp32 verwenden zu können muss Python installiert sein das sollte normalerweise aber schon auf deinem Computer installiert sein falls nicht ist hier eine Anleitung zur installation:</p>
<pre><code>https://www.python.org/downloads/
</code></pre>
<p><strong>PySerial</strong><br>
Wenn bem hochladen des codes Probleme auftreten, da Serial nicht gefunden wird liegt das mit hoher Wahrscheinlichkeit an einer fehlenden PySerial installation <em>(auch dies sollte vorinstalliert sein)</em><br>
Wenn python pip nicht installiert ist:</p>
<pre><code>python get-pip.py
</code></pre>
<p>Danch um pyserial zu installieren:</p>
<pre><code>install pyserial
</code></pre>
<p>Warten bis alles installiert ist und dann sollte der Code ohne Fehler compilieren</p>
<h2 id="connection">Connection</h2>
<p>So wird das RC522 an den ESP32 angeschlossen: https://hackster.imgix.net/uploads/attachments/704294/nfc_osc_bb_gBdcDg8Rq9.jpg<br>
<img src="https://hackster.imgix.net/uploads/attachments/704294/nfc_osc_bb_gBdcDg8Rq9.jpg" alt="ConnectionDiagram"></p>
<h1 id="to-do">To Do</h1>
<p>Es fehlt noch die Kommunikation mit einem externen Server um neue UUIDs zu <strong>laden</strong> und alte zu <strong>löschen</strong><br>
<strong>Coming soon!</strong></p>
<blockquote>
<p><strong>Note:</strong> Bei <strong>Problemen</strong> bitte ich sie herzlich ein <strong>Issue</strong> zu erstellen ich beantworte Fragen gerne.</p>
</blockquote>
<p>By @<strong>toqix</strong></p>

