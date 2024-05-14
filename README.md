https://github.com/fasihh/stealth-revise/assets/47947561/ea928aa2-8866-4060-8177-46fef0c3c1a3

# Project

## Members:

### Muhammad Abser Mansoor (23K-0030)
### Muhammad Ali (23k-0052)
### Fasih Hasan Khan (23K-0018)

## Main Idea: Create a stealth game in SFML with the light physics and detection being our focus

Logic Overview:
There are 2 entities loaded onto a pre made map, the player and the enemy, both being made from the player and enemy classes respectively which are inherited from the entity class. The player emits lines in each direction (which are represented as a different shade of gray to simulate light) and the enemy detects if it is intersected by any of these lines. If the enemy is intersected then it creates a vector toward the position of the player and moves in the direction of said vector. As soon as the enemy arrives within the bounds of the player object, the game ends and the code stops.
