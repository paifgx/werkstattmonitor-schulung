# Praxisprojekt: Werkstattmonitor modernisieren

Ihr übernehmt eine kleine C++/Qt6-Anwendung aus einem gewachsenen Bestand.
Die Anzeige funktioniert mit Beispieldaten, ist aber weder gegen fehlerhafte
Importe noch gegen Bedienung während der Wiedergabe ausreichend abgesichert.
Euer Auftrag: Änderungen nachweisbar sicher machen, statt den Code neu zu schreiben.

## Arbeitsweise und Lernziele

Nach dem Labor könnt ihr Verhalten charakterisieren, Strukturänderungen von
Fehlerkorrekturen trennen, Importgrenzen testen, Qt-Lebensdauer und Ereignisse
begründen und KI-Vorschläge anhand eigener Nachweise bewerten.

- Arbeitet allein oder zu zweit. In Paaren wechseln Implementierung und Review
  nach jeder Teilaufgabe. Beide müssen die Entscheidung erklären können.
- Beginnt mit einer eigenen Hypothese und einer Gegenprobe. Nutzt erst danach
  die zugeordnete Promptdatei. KI ist ein Werkzeug, keine Abnahmeinstanz.
- Haltet Nachweise in `local/` fest: Eingabe, Erwartung, Beobachtung, Entscheidung.
  Keine Zugangsdaten, Kundendaten oder ungeprüften Modellantworten einchecken.
- Pro Schritt: Test oder Beobachtung → kleiner Diff → Gegenprobe → Review → Commit.
- **Kern** ist der gemeinsame Lernpfad. **Vertiefung** ist für früh fertige Teams
  und Selbststudium. Sie ist kein zusätzlicher Pflichtumfang für zwei Kurstage.
- Die Zeitspannen sind grobe Arbeitsbudgets für den Kern, ohne Theorie, Pausen
  und Diskussion. Der Trainer entscheidet über Tempo und Zwischenstände.
  U0–U4 bilden den ersten, U5–U7 den zweiten Lernabschnitt; bei Bedarf wechseln
  Teams mit dokumentierten offenen Punkten auf einen Trainer-Zwischenstand.

Der [Importvertrag](02_Importvertrag.md) ist verbindlich. Bestehende Tests dürfen
nicht abgeschwächt werden. Eine grüne Suite ist notwendig, aber kein Beweis für
Fehlerfreiheit. Die Musterlösung bleibt bis zur eigenen Abnahme geschlossen.

## U0 – Ausgangsstand und Auftrag klären · 15–25 Minuten

**Situation:** Ihr sollt Änderungen verantworten, kennt aber weder Build noch
Testaussage. Erst ein belastbarer Ausgangspunkt erlaubt einen fairen Vergleich.

### Kern

1. Richtet das Projekt nach [Einrichtung](00_Start.md) und [Git](03_Git.md) ein.
   Dokumentiert Compiler, Qt-Kit, Buildart und Commit in `local/umgebung.md`.
2. Baut und startet die Anwendung. Führt beide Bestandstestgruppen aus.
   Notiert, was `baseline.parser` und `baseline.widget` tatsächlich prüfen.
3. Spielt `normal.csv` ab. Prüft fünf Messwerte, maximale Drehzahl 1800,
   eine Alarmzeile und mittlere Temperatur 40,0 °C selbst nach.
4. Formuliert drei überprüfbare Qualitätsziele: Importkorrektheit, Bedienbarkeit
   und sichere Änderung. Grenzt das Labor von einer realen Maschinensteuerung ab.

**Abnahme:** Reproduzierbarer Build, zwei grüne Bestandstestgruppen, dokumentierter
Ausgangscommit und eine begründete Aussage, was damit noch nicht abgesichert ist.

**Vertiefung:** Ein Teammitglied reproduziert den Start nur anhand eurer Notizen.
Fehlende Schritte werden ergänzt, nicht durch mündliche Hilfe verdeckt.

## U1 – Fehler untersuchen statt Code bewerten · 30–45 Minuten

**Situation:** „Legacy“ ist kein Fehlerbericht. Das Team braucht belegbare Risiken,
keine Liste persönlicher Stilvorlieben. Prompt: `prompts/01_analyse.txt`.

### Kern

1. Verfolgt einen Messwert durch `telemetry.h`, `legacy.cpp` und `hmiwindow.cpp`:
   Datei → Felder → Sample → Summary → Tabellenzelle. Markiert Ressourcenbesitz
   und die Grenze zwischen fachlicher Verarbeitung und GUI.
2. Erstellt drei kleine Gegenproben: leeres Drehzahlfeld, unbekannter Zustand,
   Stopp während der Wiedergabe. Haltet Erwartung und tatsächliches Verhalten fest.
3. Erstellt höchstens fünf priorisierte Befunde. Jeder braucht Datei/Symbol,
   reproduzierbare Eingabe oder Bedienfolge, Auswirkung und kleinsten nächsten Schritt.
   Trennt fachlichen Fehler, Lebensdauerrisiko und Ereignisverarbeitungsproblem.
   Behauptet keinen Speicherfehler, den ihr nicht belegen könnt.
4. Lasst die KI unabhängig analysieren. Überprüft mindestens zwei Aussagen am
   Code oder per Gegenprobe. Kennzeichnet unbelegte Behauptungen ausdrücklich.

**Abnahme:** Datenflussskizze und Befundtabelle mit getrennter Bewertung von
Auswirkung und Änderungsaufwand. Ein anderes Team kann eure höchste Priorität reproduzieren.

**Vertiefung:** Wählt einen plausibel klingenden KI-Befund, den ihr widerlegen
oder mangels Kontext nicht entscheiden könnt. Nennt den fehlenden Nachweis.

## U2 – Sicherheitsnetz aufbauen, dann entflechten · 45–65 Minuten

**Situation:** Parser und Anzeige sollen wartbarer werden. Unbemerkte fachliche
Änderungen sind in diesem Schritt nicht erlaubt. Prompts: `02_tests`, `03_refactoring`.

### Kern

1. Ergänzt in `tests/characterization.cpp` einen gültigen Importfall mit selbst
   berechneter Summary. Nutzt andere Werte als `normal.csv`; prüft auch eine leere Summary.
2. Notiert vor dem Umbau die öffentlichen Funktionen und erwarteten Ergebnisse.
   Unterscheidet bestehendes Verhalten von später gewünschtem Zielverhalten.
3. Trennt Dateizugriff, Parser und Summary in fachlich zusammenhängende Dateien.
   Erhaltet die öffentliche API und passt die Quellliste in CMake an.
4. Ersetzt manuelles `new QFile`/`delete` durch automatische Lebensdauer.
   Erklärt, was bei vorzeitigem Rücksprung mit der Ressource passiert.
5. Prüft den Diff auf versteckte Verhaltensänderungen. Validierungsregeln gehören
   noch nicht hierher. Sichert Testergänzung und Refactoring in getrennten Commits.

**Abnahme:** Bestandstests einschließlich eigener Fälle grün; unveränderte API;
keine neue Fachregel im Refactoring-Commit. Der Reviewer erklärt den Ressourcenbesitz.

**Vertiefung:** Findet eine alternative Aufteilung mit weniger Schnittstellen.
Begründet anhand eines konkreten Änderungsfalls, welche Variante ihr verwerft.
Keine Klassenhierarchie ohne nachgewiesenen Bedarf.

## U3 – Einen Defekt mit einem roten Test festnageln · 35–50 Minuten

**Situation:** Fehlerhafte Messwerte dürfen nicht wie gültige Daten aussehen.
Prompt: `prompts/04_fehlerkorrektur.txt`.

### Kern

1. Aktiviert die Zieltests nach `00_Start.md`. Ordnet mindestens zwei rote Fälle
   einer konkreten Regel aus `02_Importvertrag.md` zu. Noch müssen nicht alle grün sein.
2. Schreibt eine minimale Eingabe mit leerem Drehzahlfeld und eine mit unbekanntem
   Zustand. Zeigt für beide einen fehlschlagenden Test vor der Korrektur.
3. Korrigiert die Ursache: Felder dürfen nicht verrutschen; unbekannte Zustände
   dürfen nicht still zu IDLE werden. Bewahrt gültiges Verhalten.
4. Ergänzt einen positiven Nachbarfall, damit nicht pauschal alle Eingaben
   abgewiesen werden. Prüft zusätzlich eine Datei mit gültiger und ungültiger Zeile:
   Bei einem Fehler darf kein Teilergebnis übernommen werden.
5. Gebt der KI nur Vertrag, betroffene Funktion und Test. Prüft, ob ihr Vorschlag
   die Ursache behebt oder lediglich das Beispiel umgeht.

**Abnahme:** Rot-Grün-Nachweis, unverändert grüne Bestandstests und ein eigener
Fehlerkorrektur-Commit. Noch offene Zieltests werden mit Vertragsregel notiert.

**Vertiefung:** Übertragt das Vorgehen auf ein führendes Plus oder einen
int-Überlauf der Sequenz. Erst die Regel herleiten, dann implementieren.

## U4 – Den ganzen Importvertrag absichern · 55–75 Minuten

**Situation:** Einzelne reparierte Beispiele ergeben noch keinen robusten Import.
Arbeitet mit Vertrag und `tests/parser_contract.cpp`, nicht nur nach Testnamen.

### Kern

1. Erstellt eine Testmatrix: Regel | gültiger Randfall | ungültiger Nachbarfall |
   erwarteter Fehler/Zeile | vorhandener oder neuer Test. Deckt Feldanzahl,
   Zahlenbereiche und Syntax, Sequenz, Header, Leerzeilen, BOM, CRLF und Dateigrenze ab.
2. Implementiert die fehlenden Regeln. Zahlenkonvertierung muss unabhängig von
   der System-Locale sein. Prüft denselben Dezimalpunkt unter deutscher Locale;
   ein Dezimalkomma bleibt ungültig. Stellt globale Locale-Änderungen im Test zurück.
3. Prüft atomare Fehlerbehandlung: mehrere fehlerhafte Zeilen, physische
   Zeilennummern einschließlich Leerzeilen und keine Samples bei irgendeinem Fehler.
   Datei-/Lesefehler haben Zeile 0. Begrenzt das Lesen auf den vereinbarten Umfang.
4. Ergänzt mindestens einen bislang nicht abgedeckten Grenzfall mit eigener
   Erwartung, etwa die 1-MiB-Grenze oder einen Fehler am Dateiende.
5. Lasst die KI nach Lücken in eurer Matrix suchen. Übernehmt eine Ergänzung nur,
   wenn sie aus dem Vertrag folgt. Markiert nicht reproduzierbare I/O-Fehlerpfade.

**Abnahme:** `contract.parser` und beide Bestandstestgruppen grün; nachvollziehbare
Matrix und mindestens ein eigener neuer Grenztest. Keine abgeschwächten Assertions.

**Vertiefung:** Messt mit selbst erzeugten gültigen Daten zwei Dateigrößen unter
identischen Buildbedingungen. Trennt Lesezeit und Parserzeit, soweit messbar.
Nennt Messgrenzen; aus einem Laborlauf folgt kein Produktionsnachweis.

## U5 – Bedienbarkeit als Zustandsproblem lösen · 45–65 Minuten

**Situation:** Während der Wiedergabe soll die Oberfläche bedienbar bleiben.
Prompt: `prompts/05_qt_review.txt` und der Wiedergabeabschnitt im Vertrag.

### Kern

1. Dokumentiert vor der Änderung Klickdauer, UI-Tick-Zähler und Verhalten bei Stopp.
   Skizziert Zustände und Übergänge für Start, Stopp, Fertig, Importfehler und Neustart.
2. Ersetzt das Schlafen im GUI-Thread durch timerbasierte Wiedergabe mit 120 ms
   Intervall. Kein `processEvents()` als Reparatur, kein eigener Thread für die Pause.
   Begründet QObject-Elternbeziehungen und gebundene Connections.
3. Prüft die Bedienfolgen: Start → Stopp → warten; Start → erneuter Start;
   Start → Fenster schließen; erfolgreicher Import → ungültiger Folgeimport.
   Nach Stopp dürfen keine Zeilen hinzukommen; Neustart leert die Tabelle.
4. Zeigt beim ungültigen Folgeimport keine alten oder teilweise gültigen Werte.
   Die Statuszeile nennt die Fehlerzeile; `invalidAfterSuccess` erwartet „Zeile 2“.
5. Ergänzt oder schärft einen Widgettest für eine dieser Folgen. Vermeidet exakte
   Millisekundenvergleiche: Timer sind keine Echtzeitgarantie. Prüft Zustände und Folgen.
6. Wiederholt die Ausgangsbeobachtung und lasst den Diff gegen den Vertrag prüfen.
   Eine KI-Aussage über Thread-Sicherheit zählt nur mit Codebeleg oder Test.

**Abnahme:** Alle vier Testgruppen grün, sichtbarer Ablauf mit funktionierendem
Stopp und Neustart, erklärter Timer-Lebenszyklus. Benennt ausdrücklich, warum
synchrones Lesen langsamer Dateien durch den Timer nicht asynchron wird.

**Vertiefung:** Entwerft einen überprüfbaren Auftrag für wirklich langsame I/O:
Abbruch, Ergebnisübergabe, Fehler und Fensterzerstörung. Noch keinen Worker implementieren.

## U6 – KI-Review als kontrolliertes Experiment · 20–35 Minuten

**Situation:** Ihr sollt entscheiden, ob ein Modell das Review wirklich verbessert,
nicht ob seine Antwort professionell klingt. Anleitung: [Lokale KI](04_Lokale_KI.md).

### Kern

1. Wählt denselben kleinen Diff für zwei getrennte Anfragen an dasselbe freigegebene
   Modell: einmal nur „Bitte reviewen“, einmal Prompt 05 plus Vertrag und Diff.
   Nutzt getrennte Chats, damit die erste Antwort nicht die zweite beeinflusst.
2. Protokolliert Modell-ID, bereitgestellten Kontext, Antwortzeit und vorhandene
   Einstellungen. Sendet ausschließlich freigegebenen synthetischen Inhalt.
3. Bewertet jede fachliche Aussage als belegt, widerlegt oder offen. Notiert
   mindestens zwei hilfreiche Aussagen und eine unbelegte Aussage, **falls vorhanden**.
   Fehlt eine Kategorie, dokumentiert das; erfindet keinen Befund für die Quote.
4. Wählt einen Vorschlag aus und entscheidet begründet über Übernahme oder Ablehnung.
   Übernommene Änderungen brauchen einen unabhängigen Test und menschliches Review.
5. Optional: Payload mit `tools/ai_review.py` erzeugen und vor jedem Versand prüfen.
   Endpunkt und Datenfreigabe bestätigen; keine automatische Patchübernahme.

**Abnahme:** Vergleichstabelle und konkrete Entscheidung samt Beleg. Ohne erreichbares
Modell führt ihr dasselbe Review paarweise durch. Kennzeichnet den KI-Vergleich
als nicht durchgeführt; das Ersatzreview ist kein Modellnachweis.

**Vertiefung:** Wiederholt die präzise Anfrage unverändert. Welche Aussagen bleiben
stabil? Zwei Antworten reichen nicht für eine allgemeine Qualitätsrangliste.

## U7 – Freigabe verteidigen und Transfer planen · 25–40 Minuten

**Situation:** Ein anderes Team muss euren Stand übernehmen und beurteilen können.
Prompt: `prompts/06_guardrails.txt`. Die Musterlösung folgt erst danach.

### Kern

1. Führt einen Build und alle vier Testgruppen mit aktivierten Zieltests aus.
   Dokumentiert Commit, Umgebung, Befehle und Ergebnis. Prüft `git diff --check`,
   den vollständigen Änderungsdiff gegenüber `lab-start` und neue Dateien.
2. Übergebt dem Reviewer drei unbekannte Bedien-/Importfälle zur Gegenprobe.
   Erwartung zuerst schriftlich festlegen, dann ausführen. Mindestens ein Fall
   kombiniert mehrere Regeln, etwa Leerzeile und nachfolgenden fehlerhaften Messwert.
3. Erklärt ohne Modellhilfe eine Parserentscheidung, eine Ownership-Entscheidung
   und einen Zustandsübergang. Fehlende Erklärung ist ein offener Reviewpunkt.
4. Erstellt einen höchstens einseitigen Modernisierungsplan: Problem → Maßnahme →
   Nachweis → Aufwand → Restgrenze. Priorisiert nach Risiko und Nutzen.
   Nennt einen nächsten Schritt und einen bewusst nicht gewählten Umbau.
5. Vergleicht jetzt mit der Musterlösung. Haltet eine sinnvolle Abweichung und
   einen Lernpunkt fest. Gleiche Dateistruktur ist kein Qualitätskriterium.

**Abnahme:** Testprotokoll, überprüfbare Commits, Gegenproben und eine begründete
Freigabe oder Nichtfreigabe. Offene Fehler werden nicht als erledigt ausgewiesen.

**Transfer:** Nennt ein Muster, das ihr auf eigenen Bestand übertragen könnt,
und eine Voraussetzung dafür. Verwendet keine vertraulichen Quelltexte im Labor.

## Gemeinsames Bewertungsraster

| Kriterium | Noch offen | Nachgewiesen |
| --- | --- | --- |
| Fachliche Korrektheit | Beispiele ohne unabhängige Erwartung | Vertrag, Grenzfälle und atomarer Import geprüft |
| Änderungssicherheit | Mischdiff oder abgeschwächte Tests | Refactoring und Fehlerkorrektur getrennt, Regression geprüft |
| Qt-Verhalten | Nur ein erfolgreicher Start | Stopp, Neustart, Fehler und Zerstörung geprüft |
| KI-Kompetenz | Antwort ungeprüft übernommen | Aussagen geprüft, Grenzen und Ablehnungen begründet |
| Übergabe | „Läuft bei mir“ | Stand, Umgebung, Nachweise und Restgrenzen reproduzierbar |

Das Raster dient dem Feedback, nicht dem Wettbewerb um möglichst viel Code.
Bei Zeitmangel wird Umfang reduziert oder ein Zwischenstand genutzt, nicht die
Abnahme stillschweigend abgeschwächt. Kein Kundensystem wird verändert.
