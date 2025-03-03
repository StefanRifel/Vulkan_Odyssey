# Vulkan_Odyssey

## Projektübersicht
Im Rahmen eines Uni-Projekts wurde diese Vulkan-basierte Anwendung entwickelt, um eine Graslandschaft bei Sonnenuntergang zu simulieren. 
In der Szene sind verschiedene Elemente vorhanden, darunter Bäume, Terrain, Pflanzen, ein Himmel sowie Mond.

### Ausführungssschritte
1. Klone das Repository:
    ```bash
    git clone https://github.com/StefanRifel/Vulkan_Odyssey
    ```
2. Im Wurzelordner /Vulkan_Odyssey/ das Makefile ausführen:
    ```bash
    make
    ```
3. Starte die ausführbare Datei:
    ```bash
    ./Vulkan_Odyssey
    ```

## Abhängigkeiten
- **-lglfw**: Fenster- und Eingabeverwaltung
- **-vulkan**: Vulkan-API
- **-ldl**: Dynamic Linking Library
- **-lpthread**: Multithreading
- **-lX11**: X11 für Linux-Fensterverwaltung
- **-lXxf86vm**: Erweiterungen für X11
- **-lXrandr**: RandR für Bildschirmverwaltung
- **-lXi**: X Input Extension
- **glslangValidator**: Zum kompilieren der Shader
- **std_image**: Zum laden von Bilddateien
- **tiny_obj_loader**: Zum laden der Meshes

### Steuerung
- `W`: Vorwärts bewegen
- `S`: Rückwärts bewegen
- `A`: Nach links bewegen
- `D`: Nach rechts bewegen
- `Mausrad nach oben`: FOV verkleinern
- `Mausrad nach unten`: FOV vergrößern
- `Esc`: Anwendung beenden
- **360 Grad Bewegung mit der Maus**: Umschauen und Kamera drehen

## Quellen
- **Texturen**: https://polyhaven.com/all
- **Modelle**: https://polyhaven.com/all