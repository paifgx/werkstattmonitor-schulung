# Zielvertrag für den synthetischen Import

Dieser Vertrag ist die fachliche Vorgabe der Übung. Er ist bewusst klein und
keine Anforderung an ein reales Produkt. U2 trennt Strukturänderung von den
beabsichtigten Fehlerkorrekturen in U3 und U4.

## Dateiformat

UTF-8-Text, Trennzeichen Semikolon. Die erste Zeile lautet exakt
`sequence;rpm;temperature_c;state`. Ein UTF-8-BOM am Dateianfang ist zulässig.
LF und CRLF werden akzeptiert. Leere oder nur aus Leerraum bestehende Datenzeilen
werden übersprungen, zählen aber für Fehlerzeilen mit. Eine Datei ohne Messwerte
ist ungültig. Es gibt kein CSV-Quoting und keine Kommentarzeilen.

Jede nichtleere Datenzeile hat genau vier Felder. Führender und nachfolgender
Leerraum in Datenfeldern ist zulässig. Leere Felder sind ungültig.

| Feld | Gültiger Bereich beziehungsweise Syntax |
| --- | --- |
| sequence | ASCII-Ziffern, positiver int, streng aufsteigend; Lücken sind erlaubt |
| rpm | ASCII-Ziffern, ganze Zahl 0 bis 24000 einschließlich |
| temperature_c | Optionales Minus, ASCII-Ziffern, optional Punkt plus Nachkommastellen; -20 bis 150 einschließlich |
| state | Exakt IDLE, RUN oder ALARM, Großschreibung |

Bei Temperatur sind Komma, Tausendertrennzeichen, Exponent, führendes Plus,
NaN und Unendlich ungültig. Die Konvertierung darf nicht von der System-Locale abhängen.
Ein int-Überlauf ist ein Eingabefehler.

## Fehler und Zustandsübergänge

`ImportResult::ok()` ist nur ohne Fehler wahr. Bei irgendeinem Fehler ist
`samples` leer. Es wird nichts teilweise übernommen. Für jede fehlerhafte
Datenzeile genügt ein verständlicher Fehler mit physischer Zeilennummer ab 1.
Weitere Zeilen werden geprüft. Bei falscher Kopfzeile darf sofort abgebrochen werden.
Datei- und Lesefehler haben Zeile 0. Eine Datei über 1 MiB wird zurückgewiesen.

Ein neuer Wiedergabeversuch leert die bisherige Tabelle. Ein ungültiger Import
zeigt eine Fehlermeldung und keine alten oder teilweise gültigen Werte.

## Bestehendes Verhalten, das erhalten bleibt

`normal.csv` ergibt fünf Samples, maximale Drehzahl 1800, eine Alarmzeile und
mittlere Temperatur 40,0 °C. Die Mittelwertbildung nutzt alle fünf Werte,
nicht nur RUN-Zeilen. Eine leere Summary hat überall 0.

## Zielverhalten der Wiedergabe

Eine Zeile erscheint ungefähr alle 120 ms. Timer sind keine Echtzeitgarantie.
Während der simulierten Wartezeit kann Qt weitere Ereignisse verarbeiten.
Stopp beendet die Wiedergabe; danach erscheinen keine zusätzlichen Zeilen.
Neustart beginnt wieder mit leerer Tabelle. Nach dem letzten Sample ist der Zustand
„Fertig“. Die Zerstörung des Fensters beendet die gebundenen Timer.

Die kleine lokale Datei wird weiterhin synchron gelesen und begrenzt eingelesen.
Das Labor belegt keine Reaktionszeit für langsame Datenträger, Netzwerkdateien
oder große reale Datenmengen. Ein QTimer macht blockierende I/O nicht asynchron.
