# Studienprojekt Sommersemester 2023

## Implementierung und Visualisierung des Min-Cost-Flow Problems mithilfe der C++-Bibliothek [LEDA](https://algorithmic-solutions.info/site/leda/about/index.htm) mit zwei Algorithmen:

### 1. Cycle Canceling Algorithmus
### 2. Successive Shortest Path Algorithmus

## Zusammenfassung der Algorithmen
### 1. Cycle Canceling Algorithmus
Der Cycle-Canceling-Algorithmus ist ein Algorithmus zur Lösung des Minimum Cost Flow Problems. Er basiert auf dem Successive Shortest Path Algorithmus, erweitert diesen jedoch, um negative Zyklen im Restgraphen zu finden und den Fluss entlang dieser Zyklen anzupassen, um die Kosten weiter zu reduzieren. Hier ist eine Schritt-für-Schritt-Erklärung des Algorithmus:

Schritt 1: Initialisierung

    Setze den Fluss auf 0 für alle Kanten im Graphen.
    Wähle einen gültigen Fluss (der den Kapazitätsbeschränkungen genügt) als Startpunkt. Dies kann zufällig oder mit Hilfe eines anderen Algorithmus geschehen.

Schritt 2: Restgraph konstruieren

    Konstruiere den Restgraphen basierend auf dem aktuellen Fluss und den Kapazitäten der Kanten. Der Restgraph enthält nur Kanten, auf denen noch Fluss erhöht werden kann.

Schritt 3: Negative Zyklen finden

    Suche im Restgraphen nach einem negativen Zyklus. Dies kann mit Hilfe eines geeigneten Algorithmus wie dem Bellman-Ford Algorithmus oder dem Floyd-Warshall Algorithmus erfolgen. Ein negativer Zyklus ist ein Zyklus, dessen Gesamtkosten negativ sind.

Schritt 4: Fluss anpassen

    Passe den Fluss entlang des gefundenen negativen Zyklus an, um die Kosten weiter zu reduzieren. Dazu verringere den Fluss entlang des Zyklus um den kleinsten Kapazitätswert auf dem Zyklus und erhöhe gleichzeitig den Fluss in umgekehrter Richtung um den gleichen Betrag.

Schritt 5: Aktualisierung der Kosten

    Aktualisiere die Kosten für den aktuellen Fluss basierend auf den Kosten der Kanten und den aktuellen Flusswerten.

Schritt 6: Wiederholung

    Wiederhole die Schritte 2-5, bis kein negativer Zyklus mehr im Restgraphen gefunden werden kann oder bis eine Abbruchbedingung erreicht ist (z.B. ein bestimmter maximaler Flusswert).

Der Algorithmus terminiert, wenn kein negativer Zyklus mehr im Restgraphen gefunden werden kann. Zu diesem Zeitpunkt ist der Fluss optimal und die Kosten sind minimiert.

### 2. Successive Shortest Path Algorithmus

Der Successive Shortest Path Algorithmus ist ein iterativer Algorithmus zur Lösung des Minimum Cost Flow Problems. Er basiert auf dem Ford-Fulkerson Algorithmus für das Max-Flow Problem und passt den Fluss entlang kürzester Pfade im Restgraphen an, um die Kosten zu minimieren.

Hier ist eine Schritt-für-Schritt-Erklärung des Algorithmus:

Schritt 1: Initialisierung

    Setze den Fluss auf 0 für alle Kanten im Graphen.
    Wähle einen gültigen Fluss (der den Kapazitätsbeschränkungen genügt) als Startpunkt. Dies kann zufällig oder mit Hilfe eines anderen Algorithmus geschehen.

Schritt 2: Restgraph konstruieren

    Konstruiere den Restgraphen basierend auf dem aktuellen Fluss und den Kapazitäten der Kanten. Der Restgraph enthält nur Kanten, auf denen noch Fluss erhöht werden kann.

Schritt 3: Kürzesten Pfad finden

    Suche einen kürzesten Pfad im Restgraphen vom Quellknoten zum Senkenknoten. Dies kann mit Hilfe eines geeigneten kürzesten Pfad Algorithmus wie dem Dijkstra-Algorithmus oder dem Bellman-Ford Algorithmus erfolgen.

Schritt 4: Fluss erhöhen

    Erhöhe den Fluss entlang des kürzesten Pfades, indem du den Flusswert um den kleinsten Kapazitätswert auf dem Pfad erhöhst. Verringere gleichzeitig die Kapazität entlang des Pfades um den gleichen Betrag.

Schritt 5: Aktualisierung der Kosten

    Aktualisiere die Kosten für den aktuellen Fluss basierend auf den Kosten der Kanten und den aktuellen Flusswerten.

Schritt 6: Wiederholung

    Wiederhole die Schritte 2-5, bis kein kürzester Pfad mehr im Restgraphen gefunden werden kann oder bis eine Abbruchbedingung erreicht ist (z.B. ein bestimmter maximaler Flusswert).

Der Algorithmus terminiert, wenn kein kürzester Pfad mehr im Restgraphen gefunden werden kann. Zu diesem Zeitpunkt ist der Fluss optimal und die Kosten sind minimiert.