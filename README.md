# Coop Adventure

This game project was created as part of the [Unreal Engine 5 C++ Multiplayer: Make Your Own Co-Op Game](https://www.gamedev.tv/courses/ue-cpp-multiplayer) course by Kaan Alpar. The main goal was to follow along with the instructor to better understand the multiplayer Fundamentals.

There are two branches in this repository:

- `main`: Configured with the NULL subsystem.
- `steam-subsystem`: Configured with the Steam subsystem. To run the build from this branch, make sure to log into your Steam account on your computer.

## Getting Started

To get a local copy up and running, follow these steps:

1. **Clone the repository:**

    ```sh
    git clone https://github.com/sha-ridi/coop-adventure.git
    ```

2. **Checkout the desired branch:**

    - For NULL subsystem (default):
        ```sh
        git checkout main
        ```
    - For Steam subsystem:
        ```sh
        git checkout steam-subsystem
        ```
        Make sure to log into your Steam account on your computer.

3. **Open the project in Unreal Engine 5.3:**

    - Download and install Unreal Engine 5.3 from [Epic Games Launcher](https://www.unrealengine.com/en-US/download).
    - Launch Unreal Engine 5.3.
    - Open the cloned project folder.

## Running the Game

There are multiple ways to run the game:

### 1. From within the Unreal Engine Editor

1. Open the project in Unreal Engine 5.3.
2. Select the game level `Level1.umap` (avoid the menu level `MainMenu.umap` as creating a session from the menu won't work).
3. Set the number of players to 2.
4. Set the Net Mode to "Play as Listen Server".
5. Click "Play" to start the game.

### 2. Using a .bat file with the NULL Subsystem

1. Create a .bat file with the following content (adjust the paths as necessary):

    ```sh
    "<Path to UnrealEditor.exe>" "<Path to your CoopAdventure.uproject file>" -game -ResX=780 -ResY=450 -WINDOWED
    ```

    Example:
    ```sh
    "C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UnrealEditor.exe" "C:\Projects\coop-adventure\CoopAdventure.uproject" -game -ResX=780 -ResY=450 -WINDOWED
    ```

2. Run the .bat file twice to start two instances of the game.
3. Connect the second instance to the first instance.

### 3. With the Steam Subsystem

1. Checkout the `steam-subsystem` branch.
2. Build the project in Unreal Engine.
3. Transfer the built game to another computer with a different Steam account.
4. Log into Steam on both computers.
5. Run the game on both computers and play through Steam.


---

Thank you for checking out my Unreal Engine project!
