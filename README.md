# Werkstattmonitor: C++/Qt6 modernisieren mit KI

Ein eigenständiges Übungslabor für eine zweitägige Schulung. Start hier:
[Einrichtung und Selbsttest](docs/00_Start.md), dann [Git-Arbeitskopie](docs/03_Git.md)
und [Praxisaufgaben U0–U7](docs/01_Aufgaben.md).

Die Aufgaben führen von belegten Fehlern über abgesicherte Refactorings zu
robustem Import und bedienbarer Qt-Anzeige. Jede Übung enthält Teilaufgaben,
Abnahmekriterien und eine optionale Vertiefung. Der gemeinsame Kern bleibt von
den Zusatzaufgaben getrennt; Zeitbudgets sind Richtwerte, keine festen Uhrzeiten.

Die kleine Widgets-Anwendung spielt eine synthetische Messwertdatei ab.
Sie steuert nichts. Altbestand und Tests sind absichtlich überschaubar.
Legacy bedeutet hier gewachsene Struktur- und Verhaltensprobleme unter einer
bereits modernen Qt6-Toolchain. Es gibt keine Kundenschnittstellen, Serverzugänge, Downloads zur Buildzeit oder
vorausgesetzte KI-Erweiterungen. Die HMI selbst hat keinen Netzwerkcode.

| Pfad | Inhalt |
| --- | --- |
| `src/` | Import, Auswertung und Anzeige |
| `tests/` | Bestandstests und zuschaltbare Zieltests |
| `data/` | Fünf gültige Messwerte und ein Fehlerbeispiel |
| `docs/` | Aufbau, Vertrag, Aufgaben, Git und lokale KI |
| `prompts/` | Fertige Arbeitsaufträge für ein freigegebenes Modell |
| `tools/ai_review.py` | Optionaler API-Client; Standard ist nur Payload-Ausgabe |

Die Musterlösung wird vom Trainer separat ausgegeben. Ein roter Zieltest
im Ausgangsstand ist ein beabsichtigter Befund. Grün heißt nur, dass die
formulierten Prüfungen bestanden wurden; weitere Fehler bleiben möglich.

Erstellt am 17.11.2026 für die Durchführung am 18.–19.11.2026.
Quellcode und Beispieldaten wurden für dieses Labor neu synthetisch erstellt.
Bibliotheken und Werkzeuge sind nicht enthalten; es gelten deren eigene Nutzungsbedingungen.
