# esp32-fdc1004 — Dev Board

Standard projektu: **Dev Board**, Revision 0.1, opisany w lokalnym PROJECT_DIRECTORY_STRUCTURE.md.
Główne katalogi: examples/, include/, src/, hardware/. W tym repozytorium nie twórz działów 01_Electronics–06_Reports.
Repozytorium GitHub: https://github.com/JakubJasiejko/esp32-fdc1004. Zachowuj jego odrębną tożsamość i istniejącą historię.
CMakeLists.txt deklaruje komponent ESP-IDF. Publiczne API jest w include/, implementacja w src/, przykłady w examples/. Nie zakładaj, że przykład basic_usage jest samodzielną aplikacją CMake. Przed zmianą API sprawdź zależności i README.
Hardware i jego lokalne biblioteki pozostają w hardware/. Zachowuj ścieżki projektu KiCad; nie modyfikuj elektryki w ramach porządkowania. Po zmianach CAD sprawdź odpowiednio netlistę, ERC i DRC.
Cache i skrypty pomocnicze należą do work/; cache narzędzi kieruj tam jawnie. Nie publikuj .venv, build, .pio ani plików sesji KiCad.
Przed edycją sprawdź bieżącą gałąź. Rewizje sprzętu mają osobne gałęzie z pełną strukturą Dev Board; ich źródła i stan opisuj w REVISIONS.md, README.md i AGENTS.md. Nie wymyślaj numeru rewizji i nie przenoś materiałów z innego repozytorium bez potwierdzenia zgodności.
Zmiany kodu weryfikuj przez aplikację integrującą komponent z rzeczywistymi zależnościami; sam eksport lub samo istnienie plików nie dowodzi poprawnego działania.
