# AAA (Anti-Aircraft Artillery) - XBox One Port

AAA (Anti-Aircraft Artillery) is a 2 player game I had made in C++ using OpenGL. This is a port of the same game for XBox One console.

AAA is a two player game in which each player defends their area of air space by shooting down enemy airplanes using their anti-aircraft guns. I worked on this individual project in the Programming-3 curriculum at FIEA, UCF (The original game was made in Programming-1).

I implemented managers to handle physics (collision) update, game object update and rendering of sprites. The colliders support collider tags in order to prevent friendly fire, etc. and call methods on game objects which can be implemented on coding custom game objects.

## How to play:

- Open **DirectX.Framework.sln** file in Visual Studio 2017 and run the project.
- Use **D-Pad** to rotate the turrets and **A** to fire using XBox controllers.
- Press **Start** to restart the game whenever anyone loses, and **Back** to end the game.

## Additional Information:

- Check out the original project in OpenGL [here](https://github.com/Siddhant628/AAA-OpenGL-Game).
