*This project has been created as part of the 42 curriculum by gaeducas, thbosvie.*

## Version bonus

```bash
make bonus
./cub3D_bonus bonus/maps/simple.cub
```

Le dossier `bonus/` contient une version simple : minimap, collisions,
portes coulissantes et affichage optionnel des FPS. Le chemin d'une carte
`.cub` est obligatoire au lancement.

| Touche | Action |
| --- | --- |
| WASD | Se déplacer |
| Flèches gauche / droite | Tourner |
| E | Ouvrir / fermer une porte proche devant soi |
| M | Afficher / masquer la minimap |
| F | Afficher / masquer les FPS |
| R | Revenir au départ et refermer les portes |
| Échap | Quitter |

La souris reste libre. La simulation se met en pause lorsque la fenêtre perd
le focus. Une porte ne se referme pas sur le joueur.

```bash
./cub3D_bonus bonus/maps/simple.cub
```

Les cartes bonus utilisent les en-têtes habituels `NO`, `SO`, `WE`, `EA`,
`F` et `C`, et ajoutent `D` pour les portes. Chaque porte doit être encadrée
par deux murs opposés. Les textures murales utilisent un chemin XPM
relatif au répertoire de lancement. Les textures bonus sont dans
`Textures/bonus_*.xpm`, dont `Textures/bonus_door.xpm` pour les portes.
Une carte doit contenir exactement un départ `N`, `S`, `E` ou `W`, être fermée
et tenir dans 127 colonnes et 128 lignes.

La fenêtre nécessite Linux avec X11/XWayland et les dépendances MiniLibX
déjà utilisées par le projet. La version obligatoire reste accessible avec `make`.

<details>
<summary> <strong>DESCRIPTION</strong> </summary>

The goal of this project is to code a raycasting like [Wolfenstein 3D](https://fr.wikipedia.org/wiki/Wolfenstein_3D) to display a map where you can move around.
This project is called <mark>cub3D</mark>.

</details>
------------------------------------------------------
<details>
<summary><strong>INSTRUCTIONS</strong></summary>

To use our program you need to use :
```bash
make
```

to compile it, after the compilation you can launch the executable with the command:
```bash 
./cub3D name_of_map.cub
```
However, the maps must comply with several rules to be valid:

- The map must be composed of only 6 possible characters: 0 for an empty space,
1 for a wall, and N,S,E or W for the player’s start position and spawning
orientation.
- They must have only one player position.
- The player must not be able to leave the map.
- There is a specific order to follow for the textures,<br> 

the north texture:
```bash
  NO ./path_to_north_texture
```
  the south texture:
```bash
  SO ./path_to_south_texture
```
  the west texture:
```bash
  WE ./path_to_west_texture
```
  the east texture:
```bash
  EA ./path_to_east_texture
```
  floor color:
```bash
  F 0,255,0
```
  ceiling color:
```bash
 C 0,255,255
```
*The numbers following the F and C represent RGB, respectively; the numbers can range from 0 to 255.*

</details>
------------------------------------------------------
<details>
<summary><strong>RESOURCES</strong></summary>

To do this project we used some resources like: **[Gemini AI](https://gemini.google.com/)** to do a plan before start coding. We use this resources to understand [raycast](https://lodev.org/cgtutor/raycasting.html) and this [documentation](https://ibon-ira-cub3d.mintlify.app/concepts/textures) of minilibx.

</details>
-----------------------------------------------------
<details>
<summary><strong>TECHNICAL CHOICES</strong></summary>
We chose to use <mark>Minilibx</mark> because it doesn't leak and we are more comfortable with it.
</details>
