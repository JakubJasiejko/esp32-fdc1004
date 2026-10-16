# Struktura projektu — Dev Board

Revision 0.1, 2026-10-16.

Standard dla bibliotek, sterowników i płytek uruchomieniowych, w tym komponentów ESP-IDF `esp32-*`. Główne katalogi: `examples`, `include`, `src`, `hardware`.

```text
project_name/
├── examples/
│   └── basic_usage/
│       └── basic_usage.c
├── include/
│   └── driver.h
├── src/
│   └── driver.c
├── hardware/
│   ├── README.md
│   └── board_name/                 # jeśli istnieje projekt sprzętowy
│       ├── board.kicad_pro
│       ├── board.kicad_sch
│       ├── board.kicad_pcb
│       ├── sym-lib-table
│       ├── fp-lib-table
│       └── lib/
├── CMakeLists.txt
├── README.md
├── AGENTS.md
├── PROJECT_DIRECTORY_STRUCTURE.md
└── .gitignore
```

## Przeznaczenie katalogów

- `examples/` — przykłady użycia publicznego API. Zachowuj format aktualnego projektu. Pojedynczy plik z `app_main()` może być przykładem do włączenia do aplikacji; nie opisuj go jako samodzielnej aplikacji ESP-IDF, jeśli nie ma własnego projektu CMake.
- `include/` — publiczne nagłówki sterownika lub biblioteki. Nazwy i API utrzymuj zgodnie z dokumentacją.
- `src/` — implementacja. Wewnętrzne pliki zależne od komponentu trzymaj tutaj. Nie przenoś kodu do numerowanych działów standardu Ogólna.
- `hardware/` — schematy, PCB i pliki płytki powiązanej z biblioteką. README wskazuje aktywny plik `.kicad_pro`, rewizję sprzętu i stan opracowania. Gdy brak projektu sprzętowego, pozostaw README z jawną informacją o braku materiałów; nie twórz pustych plików CAD.
- `CMakeLists.txt` w katalogu głównym — deklaracja komponentu ESP-IDF: źródła z `src`, publiczne nagłówki z `include` i rzeczywiste zależności. Zachowuj istniejący system kompilacji, jeśli projekt korzysta z innego środowiska.
- `README.md` w katalogu głównym — opis urządzenia/API, zależności, sposób włączenia komponentu, użycie przykładu i odnośnik do hardware.

## Organizacja hardware

Jedna płytka może znajdować się bezpośrednio w `hardware/` lub w nazwanym podkatalogu płytki. Zachowuj działające ścieżki istniejących projektów. Nazwa zastanego podkatalogu zawierająca numer rewizji nie zastępuje wyboru gałęzi Git.

Dla nowych materiałów używaj lokalnych katalogów płytki: `lib/symbols`, `lib/footprints`, `lib/3dmodels`, `lib/vendor_sources`, `docs`, `manufacturing_files`, `backups`, `work`, w miarę potrzeb. Tabele bibliotek pozostają przy `.kicad_pro`. Przy porządkowaniu istniejącego hardware najpierw ustal zależności i popraw odwołania; nie przemieszczaj samych plików bez sprawdzenia całego projektu.

## Cache i pliki pomocnicze

Dodatkowy `work/` służy wyłącznie narzędziom i cache; nie zastępuje czterech katalogów głównych. Cache Pythona kieruj przez `PYTHONPYCACHEPREFIX` do odpowiedniego `work/pycache`. `build`, `.pio`, `.venv`, cache i pliki sesji KiCad pomijaj w Git. Ważne dane wejściowe przechowuj w `source_files/` właściwego narzędzia albo w `hardware/<board>/docs/`, a nie wyłącznie w cache.

## Git i rewizje

Jeden komponent lub płytka ma jedno odpowiadające mu repozytorium GitHub, np. `esp32-ad4000`. Nie łącz niezależnych komponentów w jedno repozytorium tylko dlatego, że współpracują ze sobą. Zależności zapisuj w konfiguracji komponentu i README.

Rewizje sprzętu przechowuj na osobnych gałęziach jednego repozytorium, z pełną strukturą Dev Board na każdej gałęzi. Sprawdzaj bieżącą gałąź i lokalne instrukcje. Nową rewizję opisuj w `REVISIONS.md`, `README.md` i `AGENTS.md`; zachowuj źródło oraz powiązanie z rewizją bazową. Nie twórz niepotwierdzonych numerów rewizji. Repozytorium posiadające obecnie wyłącznie `main` może pozostać na `main` do czasu zlecenia nowej rewizji.

## Weryfikacja zmian

Zmiany kodu i CMake sprawdzaj w rzeczywistej aplikacji wykorzystującej komponent i właściwe zależności. Sam komponent nie musi być samodzielnym projektem uruchamianym przez `idf.py build`. Nie aktualizuj automatycznie SDK podczas zmiany struktury.

Po zmianach schematu sprawdź podgląd, netlistę i ERC; po zmianach PCB również DRC. Zachowuj układ i połączenia przy pracach organizacyjnych. Historyczne wyniki opisuj jako historyczne; obecność Gerberów nie stanowi zgody na produkcję.
