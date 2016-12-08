Szybki start:
1. Zapoznaj sie z przykladem (example/line.cpp)
   oraz ze sposobem jego kompilacji `example/compile.sh line`
2. Uruchom przyklad. Na standardowym wyjsciu pojawia sie dane w formacie
   JSON.
3. Uruchom wizualizacje (visualization/index.html),
   wrzuc otrzymane wczesniej dane do pola tekstowego
   i kliknij "Wczytaj dane".
4. Mozesz przechodzic po kolejnych krokach algorytmu uzywajac dostarczonego interfejsu.
5. W podobny sposob mozesz utworzyc wlasny algorytm i dokonac jego wizualizacji!
   Pomocna moze byc ponizsza instrukcja obslugi biblioteki C++.

## Podstawowe obiekty geometryczne

Klasa Point:

Pola:
const double x;
const double y;

W konstruktorze należy podać współrzędną x oraz y punktu

Metody:
double distance (const Point & p) const //zwraca odległość od punktu


Klasa Line:
 
Zawiera prywatną strukturę Parameters z polami double A, B, C. Odpowiadają one współrzędnym prostej w postaci Ax+By+C=0.

Pola klasy: 
Parameters parametrs
const Point point1 
const Point point2

W konstruktorze należy podać 2 punkty. Natychmiast zostaje obliczony parameters.

Metody:

bool isParallel (const Line & line) const
bool isPerpendicular (const Line & line) const
double distance (const Line & line) const //zwraca odległość od Line równoległej lub 0
double distance (const Point & p) const //zwraca odległość od punktu
double angleBetweenLines (const Line & line) const //zwraca kąt ostry między prostymi



## Obsługa biblioteki C++

Biblioteka C++ GoGUI udostępnia kontenery (gogui::vector, ...) do przechowywania kolekcji obiektów geometrycznych (gogui::Point, gogui::Line, ...), będące rozszerzeniami odpowiadających im kontenerów STL-owych. Ich ograniczeniem jest możliwość przechowywania jedynie obiektów geometryczhych biblioteki GoGUI (Point, Line, ...).

Dodatkowo, klasy gogui::vector, ... zostały wzbogacone o możliwość ustawienia trybu wizualizacji (VisualizationMethod):
* CLOUD - chmura punktów  * PATH - ścieżka otwarta  * CLOSED_PATH - ścieżka zamknięta
Domyślnie punkty są wizualizowane jako chmura.

Przykład:
```
 gogui::vector<gogui::Point> points;
 points.visualizeAs(points.VisualizationMethod::PATH);
 for(int i=0;i<10;i++)
   points.emplace_back(i, i*i);
```


Poza wizualizacją całych kontenerów, istnieje także możliwość zwizualizowania pojedynczych punktów korzystając z obiektu ActivePoint, linii - ActiveLine, ... . Te obiekty będą wyświetlane w innych kolorach niż pozostałe obiekty.

Przykład:
```
 void processLines(gogui::vector<Lines>& lines) {
   for(gogui::Line : lines) {
     gogui::ActiveLine active(line);
     // processing line
   }
 }
```


W zasadzie, to obiekty ActiveXX są tylko dla wygody użytkownika i czerpania korzyści ze wzorca RAII. Tak na prawdę, każdy obiekt geometryczny posiada metodę setStatus, która przyjąć może następujące argumenty, wpływające na sposób wizualizacji danego obiektu:
* Active - obiekt wyróżniony  * Normal - brak wyróżnień  * Processed - przetworzony
Pomocnicze obiekty ActiveXX ustawiają automatycznie status na Active.

Przykład:
```
 void processPoint(gogui::point& p) {
   //some processing
   p.setStatus(Status::Processed);
 }
```


Ogólna koncepcja jest taka, że zwizualizowane zostaną wszystkie żyjące obiekty (kolekcje gogui:: oraz obiekty pomocnicze gogui::ActiveXX). Gdyby wśród tych kolekcji istniało więcej egzemplarzy punktów o jednakowych współrzędnych, być może z różnymi statusami, punkty te zostaną scalone i przedstawione zgodnie z najwyższym spośród statusów. Podobnie linia, ... .

Przykład:
```
 void processPoints(gogui::vector<Points>& points) {
   gogui::ActivePoint center({9, 3});
   // aktualnie wizualizowane są wszystkie punkty z `points` oraz wyróżniony punkt `center`
   // o ile nie istnieją jeszcze inne obiekty
 }
```


Jednak, sama obecność wyżej wymienionych obiektów nie jest wystarczająca do uzyskania wizualizacji. Użytkownik sam, w dogodnych dla niego momentach wywołuje funkcję gogui::snapshot(), aby zapisać migawkę aktualnego stanu wizualizacji.

Przykład:
```
 void processLines(gogui::vector<gogui::Line>& lines) {
   for(gogui::Line line : lines) {
     gogui::ActiveLine active(line);
     gogui::snapshot(); // interesting moment in the algorithm
     // processing line
   }
 }
```

W ten sposób, podczas działania algorytmu, w pamięci zostanie zachowana cały jego przebieg.
Na koniec, sugeruje się wywołanie funkcji gogui::print_json(), dzięki której zostaną wypisane wszystkie migawki w formacie JSON, gotowym do uruchomienia w wizualizacji JS.


## Obsługa wizualizacji JS
(Epilog praktyczny: Wizualizacja JS - Jędrzej)
