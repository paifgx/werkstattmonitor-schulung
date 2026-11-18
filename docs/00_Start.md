# Einrichtung und Selbsttest

## Voraussetzungen

- C++17-Compiler: unter Windows Visual Studio 2022 mit „Desktopentwicklung mit C++“
  und CMake-Werkzeugen; unter Linux ein passender GCC/Clang und Make oder Ninja.
- CMake mindestens 3.21; Git mindestens 2.28 für die verwendeten Befehle.
- Qt6-Desktop-Entwicklungskit mit Shared Libraries, Core, Widgets und Test, mindestens 6.2. Verwende eine
  zusammengehörige, bereits verfügbare Version. Mindestversion 6.2 ist kein
  Nachweis für einen durchgeführten Test auf dieser Version.
- Qt-Kit und Compiler müssen zusammenpassen: MSVC-Qt für MSVC, gleiche Architektur.
  MinGW- und MSVC-Bibliotheken können nicht beliebig gemischt werden.
- Python 3.9+ ist nur für den optionalen KI-Client nötig. Das Labor läuft ohne Python.

Installiere oder ändere Software auf einem verwalteten Rechner nur im freigegebenen
Rahmen. Fehlende Rechte werden durch Trainer-Demo oder Paararbeit überbrückt.
Container, spezielle Sicherheitsmodelle und kostenpflichtige KI-Abos sind keine Voraussetzung.

## Windows: reproduzierbarer Einstieg

Entpacke den Teilnehmerordner in einen kurzen, beschreibbaren Pfad, z. B.
`C:\Training\werkstattmonitor`. Öffne eine **x64 Native Tools Command Prompt for VS 2022**.
Die folgenden Beispiele setzen ein vorhandenes MSVC-x64-Qt-Kit voraus.
Ersetze den Beispielpfad einmal durch den tatsächlichen Kitpfad.

```bat
cd C:\Training\werkstattmonitor
set "PATH=C:\Qt\6.x.y\msvc2022_64\bin;%PATH%"
cmake --version
git --version
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="C:/Qt/6.x.y/msvc2022_64"
cmake --build build --config Debug --parallel 2
ctest --test-dir build -C Debug --output-on-failure
```

Bei Kits für MSVC2019: den tatsächlich installierten Ordner angeben und die
Compiler-Kompatibilität lokal prüfen. Keine Version aus diesem Beispiel installieren.
Für die Ausführung müssen Qt-DLLs und Plugins auffindbar sein. Im selben Fenster:

```bat
set "PATH=C:\Qt\6.x.y\msvc2022_64\bin;%PATH%"
build\Debug\werkstattmonitor.exe
```

Alternativ den Ordner in Visual Studio öffnen und das passende CMake-Kit konfigurieren.
Die CLI oben ist der gemeinsame Prüfweg; Ninja ist dafür unter Windows nicht nötig.
Ein Generatorwechsel braucht ein neues Buildverzeichnis, z. B. `build-ninja`.

## Linux

Verwende ein vorhandenes Entwicklungskit. Falls Qt systemweit installiert und
für CMake auffindbar ist, entfällt `-DCMAKE_PREFIX_PATH=...`.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/pfad/zum/Qt6-Kit
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
./build/werkstattmonitor
```

Die verwendeten Befehle sind auf dem Teilnehmergerät noch zu prüfen. Der konkrete
Ausführungsnachweis dieser Lieferung steht im Trainerpaket unter `pruefung/`.

## Erfolgskriterien für den Start

1. CMake findet Qt6 Core, Widgets und Test.
2. Der Build beendet sich erfolgreich.
3. Zwei CTest-Gruppen `baseline.parser` und `baseline.widget` sind grün.
4. Die Anwendung startet. Die mitkopierte Datei `data/normal.csv` wird ohne
   Dateipfadargument neben der EXE gesucht. Fünf Zeilen werden angezeigt.
5. Beobachte beim Abspielen: Im Ausgangsstand stockt der UI-Tick-Zähler kurz.
   Der Stopp-Knopf hilft noch nicht. Das ist Übung U5, kein Einrichtungsfehler.

## Zieltests bewusst aktivieren

```sh
cmake -S . -B build -DLAB_CONTRACT_TESTS=ON
cmake --build build --config Debug --parallel 2
ctest --test-dir build -C Debug --output-on-failure
```

Nun kommen `contract.parser` und `contract.widget` hinzu. Beide sind im Ausgangsstand
absichtlich rot. Schalte sie für die Fehlersuche zu und verbessere den Code.
Die Tests dürfen zur Abnahme nicht ausgeschaltet oder abgeschwächt werden.
Für die reine Bestandsaufnahme ist `-DLAB_CONTRACT_TESTS=OFF` wieder möglich.

## Fehler eingrenzen

| Symptom | Nächste Prüfung |
| --- | --- |
| Qt6Config.cmake fehlt | Entwicklungskit statt nur Runtime vorhanden? Prefix auf Kitwurzel gesetzt? |
| Linker meldet Architekturfehler | Qt und Compiler beide x64 und passende Compilerfamilie? |
| Qt-DLL fehlt | Qt-bin im aktuellen Prozess-PATH, nicht global ändern |
| Plattformplugin fehlt | Qt-Plugin-Verzeichnis und Kit prüfen; keine zufälligen DLLs kopieren |
| CTest ohne Bildschirm schlägt fehl | `offscreen`-Plugin vorhanden? Tests setzen QT_QPA_PLATFORM selbst |
| Datei fehlt | Mitgelieferten `data`-Ordner neben EXE prüfen oder absoluten CSV-Pfad als Argument nutzen |

Ein vollständiger Lauf ohne Bildschirm prüft kein sichtbares Layout und keine Windows-Installation.
Für die Schulung einmal die sichtbare Anwendung auf dem Trainerrechner starten.
