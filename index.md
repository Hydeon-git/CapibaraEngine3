
<h1 align="center">Capibara Engine 3
</h1>
<p align="center">
<img src="capilogo.png" width="360" height="360">
</p>

## Intro

In this page you will find every piece of information regarding CapibaraEngine3, an university project made by a team of three students of Video Game Design and Development Degree, for the subject "3D Engines" at the Centre de la Imatge i la Tecnologia Multimedia (CITM) in Terrassa, Catalunya.


## Team Members
### [Albert Pou](https://github.com/Hydeon-git)

<img src="fotoAlbert.png" width="192" height="256">

#### Tasked with:


- Tank Scene implementation

- Tank Movement and Rotation systems

- Scripting Module

- Capibara Engine Custom Icons

- Webpage Polishing


###  [Arnau Bonada](https://github.com/arnaubonada)

<img src="fotoArnau.png" width="220" height="256">

#### Tasked with:

- Tank Movement, Rotation and Shooting systems

- Capibara Logo implementation

- Scripting Module

- Capibara Icons In-Engine implementation

- ImGui Script Text Editor


### [Pol Pallares](https://github.com/Zeta115)

<img src="fotoPol.png" width="260" height="256">

#### Tasked with:


- Webpage Creation & Management



## Main Core Sub-systems

## 1st Assignment - Geometry Viewer

### Models and .fbx Import

- Basic Shape primitives: Cube, Pyramid and Sphere.

- .fbx Objects can be loaded and be visible in the engine.

- 3 different .fbx models included in the engine (/Assets folder).

- Drag & Drop of FBX & DDS/PNG files.

- GameObject structure, one for each loaded mesh. Each GameObject having 3 display-only components: Transform, Mesh and Texture.


### Camera

- While Right clicking, WASD to move around the scene.

- Mouse wheel for zoom in and zoom out.

- Holding SHIFT duplicates movement speed.

- Alt+Left click to orbit around the object.

- "F" to focus the camera around the geometry.


### Editor Windows

- The console LOGS the geometry loading process.


### Configuration

- A graph for the frames per second.

- Configuration for all variables on each module (renderer, window, input, textures).

- Information output of memory consumption, hardware detection and software versions (SDL, OpenGL, DevIL).

- Exit option in general menu to exit the engine.


### Hierarchy window

- Displaying a list of all GameObjects

- GameObject selection


### Inspector

- Displays information about the components of the selected GameObject

- Transform: only for display purposes. Show position, rotation and scale for the selected GameObject.

- Mesh: information about the loaded mesh. There should be an option to display its normals (per-triangle and per-face).

- Texture: display texture size and path. There should be an option to view the selected GameObject with the checkers texture.


### Menus

- About window with Github link and information about the Engine

- Option to turn on/off editor windows


## 2nd Assignment - Engine Editor
On the second assignment we forked our project from our teacher [Rubén Ajenjo](https://github.com/solidajenjo/), who provided us a fresh project to continue developing CapibaraEngine under all 1st Assignment demands.

This project was forked from https://github.com/solidajenjo/Engine3D.

- The user can now use the inspector to modify a Gameobject's:

· *Hierarchy*: Deleting, reparenting, creating emptys or creating children.

· *Transform*: Traslading, rotating and scaling GameObjects.

· *Mesh* :  Selecting and/or droping any imported mesh.

· *Texture*: Selecting and/or dropping any imported texture.

· The Camera as a component.

- GameObjects can be picked using the mouse.

- All meshes use AABB and can be discarted using Frustrum Culling(visualized in the editor).

- Models, Meshes and Textures saved in our own "Library" folder

- Scene serialized to a file to be loaded.

- Play/Pause/Stop mechanics added to the simulation and recovering original state.

- Editor with 2 windows: Editor view / Main Game camera viewing at the same time.


### Resource Management

- “Assets” window showing all user assets.

- The user can drop/import new files.

- Asset files can be deleted through the window.

- Upon start, all resources not managed inside Assets are generated in Library.

- All resources use reference counting, this process is visualized in the editor.



## 3rd Assignment - High Level System

In the 3rd assignment the diferent teams from the class had a diferent sub system to develop, in our case our task was the scripting system for our engine. In our case the team developing [Ragnar Engine](https://github.com/UriKurae/Ragnar-Engine) kindly provided us their project to be forked.

Their respective Github usernames are:

- Oriol Bernal ([UriKurae](https://github.com/UriKurae))

- Lucas Pérez ([LucasPG14](https://github.com/LucasPG14))

###  General Contents

- Engine's Website

- Default window at 1024 x 768 and centered on the screen

- Resizable window.

- Build loads directly from the scene with the required demo

- All systems work using time controls (Play/Pause/Stop)


### Scripting Contents


(Image of the in-game Tank)

- Tank moves and rotates using the keyboard (W, A, S, D).

- Tank turret rotates with the mouse.

- Tank shoots with Left Click on the mouse (Bullets are new GameObjects and have a lifetime).

- In-editor text window to edit scripts.


(Image of the Tank and turret in the scene)


## Explanatory video

Link to the video [here](https://youtu.be/67RvIryQf3w) or watch it in our page:

<iframe width="560" height="315" src="https://www.youtube.com/embed/dQw4w9WgXcQ" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


## Release Links

- Link to the code repository [here](https://github.com/Hydeon-git/CapibaraEngine3)

- Link to the latest release ready to download [here](https://github.com/Hydeon-git/CapibaraEngine3/releases)


## License

- This project is suported under an MIT license link [here](https://github.com/Hydeon-git/CapibaraEngine3/blob/main/LICENSE.md) or read the complete license below:


MIT License

Copyright (c) 2021 CapibaraEngine

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
