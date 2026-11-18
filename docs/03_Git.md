# Git als lokales Sicherheitsnetz

## Variante A: GitHub-Repository klonen

```sh
git clone https://github.com/paifgx/werkstattmonitor-schulung.git
cd werkstattmonitor-schulung
git status --short
git log --oneline -1
git switch -c exercise/modernization
git tag lab-start
```

Diese Kopie ist bereits ein Repository: kein erneutes `git init`, kein neuer
Ausgangscommit. Falls `lab-start` schon existiert, dessen Ziel prüfen statt den
Tag zu überschreiben. `origin` darf auf das Schulungsrepository zeigen; Änderungen
bleiben lokal. Ein Push ist für die Übungen weder nötig noch vorgesehen.

## Variante B: ZIP oder Teilnehmerordner

Die folgenden Initialisierungsbefehle gelten nur in einer eigenen, entpackten Teilnehmerkopie. Vor `git init`
prüfen: Der Ordner darf nicht innerhalb eines bestehenden Kunden- oder privaten
Repositorys liegen. `git rev-parse --show-toplevel` muss hier zunächst fehlschlagen.
Falls es stattdessen einen übergeordneten Pfad ausgibt: andere Kopie außerhalb wählen.

```sh
git rev-parse --show-toplevel
git init -b main
git config --local user.name "Lab Teilnehmer"
git config --local user.email "training@example.invalid"
git status --short
git add .gitignore .gitattributes CMakeLists.txt README.md src tests data docs prompts tools
git diff --cached --stat
git diff --cached
git commit -m "chore: synthetischen Ausgangsstand sichern"
git tag lab-start
git remote -v
git switch -c uebung/struktur
```

Bei Variante B bleibt `git remote -v` leer. Für beide Varianten gibt es keinen
Push und keine Migration von Subversion. Ein Plattformkonto ist nicht erforderlich. Lab-Änderungen werden nicht automatisch in einen
bestehenden Arbeitsbestand übernommen.

## Ein Änderungsschritt

```sh
git status --short
git diff
git diff --check
cmake --build build --config Debug --parallel 2
ctest --test-dir build -C Debug --output-on-failure
```

Danach nur die tatsächlich geänderten Dateien gezielt stagen. Beispiel für eine
reine Änderung an vorhandenen Dateien:

```sh
git add src/legacy.cpp tests/characterization.cpp
git diff --cached
git commit -m "test: gültigen Importfall absichern"
git log --oneline --decorate -5
```

Bei neuen Dateien deren exakte Pfade ergänzen. Nicht blind `git add .` ausführen.
Konfiguration, Token, KI-Ausgaben und Buildprodukte gehören nicht in den Commit.
`.gitignore` verhindert keine Aufnahme bereits verfolgter Dateien und ersetzt kein Review.

## Eine festgeschriebene Änderung zurücknehmen

```sh
git status --short
git log --oneline -3
git revert --no-edit HEAD
```

Nur auf sauberem Arbeitsverzeichnis und bewusst auf dem gewünschten Commit.
Ein neuer Commit dokumentiert die Rücknahme. Konflikte zuerst verstehen; bei
Bedarf `git revert --abort`. Keine Löschbefehle, kein `reset --hard`, kein Force-Push.
Unfertige Änderungen erst als überprüften Zwischenstand committen oder in einer
separaten Kopie sichern; ein Branchwechsel ist kein automatisches Backup.

## Modellvorschlag prüfen

Modell zuerst um Plan oder kleinen Diff bitten. Textvorschläge manuell übernehmen.
Unbekannte Shellbefehle und Patches werden nicht automatisch ausgeführt.
Nach jedem Schritt laufen Build, Tests und menschliches Diff-Review.
Kleine Commits trennen Umstrukturierung und fachliche Änderung.

## Optional: zweites Experiment mit Worktree

Erst nach dem Ausgangscommit, ohne laufende Änderungen:

```sh
git worktree add ../werkstatt-experiment -b experiment/review
git worktree list
```

Das erzeugt eine zweite Arbeitskopie mit eigenem Branch. Es ist keine Sandbox:
ein dort ausgeführtes Tool kann dieselben Dateirechte haben. Für dieses Labor
bleibt die Ausführung in der Hand der Teilnehmenden. Ein Worktree ist keine
Voraussetzung für die Schulung.
