# Lokale Modelle einsetzen

## Pflichtweg: Browser und bestehende Umgebung

Nutze nur einen tatsächlich vorhandenen, für die Schulung freigegebenen Zugang.
Wähle das bestätigte lokale Modell in Open WebUI, füge Prompt 01 und die beiden
kleinen Kontextdateien ein und bewerte die Antwort anhand von Tests.
Modellnamen, Zugangsdaten und Serveradresse werden hier absichtlich nicht vorgegeben.

Ein Open-WebUI-Frontend beweist allein nicht, dass die Inferenz lokal läuft.
Modellrouting und aktivierte Funktionen müssen in der tatsächlichen Umgebung
geklärt sein. Für die Übung genügen Textantworten; keine Tools, RAG-Sammlungen,
Shellzugriffe oder automatischen Repositoryberechtigungen aktivieren.

## Optional: API-Brücke ohne IDE-Erweiterung

`tools/ai_review.py` verwendet nur die Python-Standardbibliothek. Es liest genau
`src/telemetry.h`, `src/legacy.cpp` und den Analyseprompt. Keine rekursive Projektsuche.
Es ist für den Teilnehmer-Ausgangsstand gedacht. Nach der Aufteilung in neue
Dateien den Browserweg verwenden oder die Kontextliste bewusst selbst anpassen.

```sh
python tools/ai_review.py
```

Der Standard erzeugt ausschließlich JSON auf stdout und führt keinen HTTP-Aufruf aus.
Prüfe den vollständigen Text vor einem Versand. Ein optionaler Modellaufruf braucht
den vom Betreiber bestätigten **vollständigen Chat-Endpunkt** und die echte Modell-ID.
Bei üblichen Open-WebUI-Versionen ist der Chatpfad `/api/chat/completions`, bei
direkten OpenAI-kompatiblen Backends oft `/v1/chat/completions`. Die installierte
Version und Konfiguration entscheidet. Diese beiden Pfade nicht blind vertauschen.

Beispiel ausschließlich für einen bereits laufenden lokalen Testserver:

```sh
python tools/ai_review.py --send --url http://127.0.0.1:8080/api/chat/completions --model BESTAETIGTE_MODELL_ID
```

Der Token wird verdeckt abgefragt. Bei einem administrativ bestätigten HTTPS-Server
dessen Endpunkt angeben. Keine Zugangsdaten in Argumente, Dateien oder Chatverläufe
kopieren. Für eine bereits eingerichtete Prozessumgebung sind `LAB_AI_URL`,
`LAB_AI_MODEL` und `LAB_AI_TOKEN` optional; neue Schlüssel werden vom Skript nicht angelegt.
`LAB_AI_TOKEN` nicht per Klartextbefehl in die Shellhistorie schreiben.

Das Skript folgt keinen HTTP-Redirects, verwendet keine Umgebungs-Proxys und
deaktiviert keine TLS-Prüfung. HTTP ist ausschließlich für Loopback erlaubt.
Es erwartet eine nicht gestreamte Antwort in `choices[0].message.content` und
führt Modellantworten nie aus. Antwortformat und `max_tokens` können je nach
Modellserver abweichen. Bei Fehlern nichts an Zertifikatsprüfung oder Freigaben
umgehen; auf den Browserweg wechseln.

## Visual Studio 2022

Verlässlicher Grundweg: Codeauswahl im Editor lesen, in den freigegebenen Browserchat
übernehmen, Antwort als Diff prüfen. Das benötigt keine Erweiterung.

Für eine schlichte Werkzeuganbindung kann ein vorhandenes Python über
**Tools → External Tools → Add** eingetragen werden:

| Feld | Beispiel, lokal anpassen |
| --- | --- |
| Title | Lab – Analyse-Payload |
| Command | Vollständiger Pfad zur vorhandenen python.exe |
| Arguments | `"C:\Training\werkstattmonitor\tools\ai_review.py"` |
| Initial directory | `C:\Training\werkstattmonitor` |
| Use Output window | Aktivieren, wenn verfügbar |

Dies ist zunächst nur die Payload-Ausgabe. Für einen API-Aufruf das Skript in
einem separaten Terminal starten, damit die verdeckte Tokenabfrage funktioniert.
Die IDE-Ausgabe eignet sich nicht für eine interaktive Passwortabfrage.
Das ist eine kontrollierte Werkzeugbrücke, keine Inline-Codevervollständigung.
Wir setzen keine direkte lokale Modellunterstützung von Copilot und keine
unbestätigte Erweiterungsfreigabe voraus. Die Menüanbindung unter Windows ist
in dieser Lieferung nicht ausgeführt worden.

## Review statt Vertrauensvorschuss

| Ergebnis | Prüfung |
| --- | --- |
| Modell nennt einen Fehler | Mit Symbol und Gegenprobe reproduzieren |
| Modell behauptet erfolgreiche Tests | Lokales Testprotokoll verlangen; selbst ausführen |
| Modell schlägt neue Architektur vor | Nutzen, Aufwand und Scope gegen kleine Änderung abwägen |
| Modell liefert Patch | Erst lesen, dann klein übernehmen, bauen, testen und committen |

Keine reale Modellantwort ist dieser Lieferung als ausgeführt beigefügt.
API-Clienttests mit simuliertem Server belegen nur das Clientverhalten.
