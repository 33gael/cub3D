*This project has been created as part of the 42 curriculum by gaeducas, thbosvie.*

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
