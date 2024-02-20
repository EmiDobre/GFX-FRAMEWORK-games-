# GFX-FRAMEWORK-games-

This repository contains a comprehensive collection of lab exercises and games designed to explore and master important concepts within the OpenGL framework.

Key Concepts Covered in the lab1-lab9: (src/lab_m1/...)
-
- Object Creation (2D and 3D): Learn how to create and manipulate objects in two- and three-dimensional spaces within the OpenGL framework.
  
- Model Matrices for Animations: usage of model matrices to understand how animations can be achieved through transformations in OpenGL.

- Understanding Vertices of Objects for a deep understanding of vertices and their role in defining the geometry of objects within the OpenGL framework.

- Object Space, World Space, and Visual Space: exploring the various coordinate spaces used in OpenGL, including object space, world space, and visual space

- Projections for Camera Work: integrate first-person and third-person camera perspectives into OpenGL projects.

- Understanding Graphic Pipeline: Delve into the graphic pipeline to comprehend the sequence of stages in rendering graphics with OpenGL.

- Working with Shaders, Textures, and Lights: using the power of shaders, textures, and lights to enhance the visual quality of OpenGL applications.

The more complex games built using these concepts are:
-

1) src/lab_m1/Tema1: Plants vs Zombies (2D game)
   -
   - this project focuses on creating interactivity between the user and the game 
   - the rhombs kill the hexagons of the same color if the user has enough money for them to buy and can occupy only an unused place
   - when the enemies(hexagons) pass the red barrier the user loses one life
    
2) src/lab_m1/Tema2: World of Tanks (3D game)
   -
  - the challenge for this project comes from working with a third-person camera behind the tank and synchronizing its movement to the tank
  - the tank is designed using Blender
  - the tank can move W A S D, the turret is moving exactly like the mouse, and on click the tank can throw projectiles
  - the enemies move randomly and can't get out of the game space
  - the enemies deteriorate after each collision with a projectile and this is done in Fragment Shader
   
3) src/lab_m1/Tema3: LightHouse (3D game)
   -
   - the projects focus more on the visual part of a game and also the realism
   - the virtual scene consists of a lighthouse, moon, ocean, an isle, and boats that rotate around the lighthouse's isle
   - the challenges in this project are:
     1) water movement effect using the shader
     2) lighting: interaction of 3 types of light punctiform, directional, and spotlight
        punctiform light: the reflector of the lighthouse, each boat
        directional light: the moon
        spotlight: the lighthouse's reflector light upon the ocean
