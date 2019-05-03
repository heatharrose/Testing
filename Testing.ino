#include <Arduboy2.h>
#include "World.h"
#include <FixedPointsCommon.h>

Arduboy2 arduboy;

SQ15x16 playerX = 62;
SQ15x16 playerY = 58;
SQ15x16 playerVelocityY = 0;
bool playerWalking = false;
bool playerJumping = false;
bool playerStanding = false;
bool playerFalling = false;
int worldX = 0;

constexpr int playerWidth = 4;
constexpr int playerHeight = 4;
constexpr int groundLevel = (HEIGHT - 9);
constexpr SQ15x16 gravity = 0.23;
byte frame = 0;
int spriteState = 0;

enum {
  STAND_STILL,
  WALK,
  JUMP,
  FALL,
};

bool onGround = false;

bool isVacantSpace(int tile) {
  if (tile == 0) {
    return true;
  }
  return false;
}

bool canMoveLeft() {
  if(playerX == 0) {
    return false;
  } else{
    int absolutePosition = playerX.getInteger() + worldX;
    if (absolutePosition % 8 == 0) {
      uint8_t tile = getMapTile((absolutePosition - 1) / 8, playerY.getInteger() / 8);
      return isVacantSpace(tile);
    } else {
      return true;
    }
  }
  return false;
}

bool canMoveRight() {
  if(playerX == 128 - playerWidth) {
    return false;
  } else {
    int absolutePosition = playerX.getInteger() + worldX + playerWidth;
    if (absolutePosition % 8 == 7) {
      uint8_t tile = getMapTile((absolutePosition + 1) / 8, playerY.getInteger() / 8);
      return isVacantSpace(tile);
    } else {
    return true;
  }
}
return false;
}

void setup() 
{
  
 arduboy.begin();
 arduboy.setFrameRate(30);
 arduboy.clear();

}

void loop() {

  if(!arduboy.nextFrame())
  return;
  //checks button status
  arduboy.pollButtons();

  if(arduboy.pressed(LEFT_BUTTON)) {

    if (canMoveLeft()) {
      if (worldX == 0) { 
        if (playerX > 0) playerX -= 1;  
      }
      else if (worldX == (mapWidth - 16) * 8) { 
        if (playerX > 64 - (playerWidth /2)) { 
          playerX -= 1;
        }
        else {
          worldX -= 1;  
        }
       }
      else {
          worldX -= 1;  
      }

    }
    spritestate = WALK;
    playerWalking = true;

  }

  if(arduboy.pressed(RIGHT_BUTTON)) {

    if (canMoveRight()) {

      if (worldX == 0) { 

        if (playerX < 64 - (playerWidth  / 2)) {  
          playerX += 1;
        } else { 
          worldX += 1; 
        }
      } else if (worldX == (mapWidth - 16) * 8) { 
        if (playerX < 128 - playerWidth) {  
          playerX += 1;
        }
      } else {
        worldX += 1;
      }
    }
    playerWalking = true;
    spritestate = WALK;
  }
  
if(arduboy.justPressed(A_BUTTON))
{//if player is not jumping
  if(onGround)
  {//make player jump
    playerVelocityY = -2;
    
    onGround = false;
    playerJumping = true;
    SpriteState = JUMP;

  }
}
//if player is jumping
if(playerVelocityY < 0)
{
  playerVelocityY += gravity;
 }


if(playerVelocityY >= 0)
{
  playerVelocityY = gravity;
}
SQ15x16 nextPlayerY = (playerY + playerVelocityY);
SQ15x16 nextPlayerYBottom = (nextPlayerY + playerHeight);

if(nextPlayerYBottom >= groundLevel) {

  nextPlayerY = (groundLevel - playerHeight);
  onGround = true;

}

playerY = nextPlayerY;

arduboy.clear();

// Render the world ..

for(uint8_t y = 0; y < mapHeight; ++y) {

  for(uint8_t x = 0; x < mapWidth; ++x) {

   Sprites::drawOverwrite(x * tileWidth - worldX, y * tileHeight, ground,getMapTile(x, y));
     }
  //draw player
  sprite.drawSelfMasked(playerX.getInteger(), playerY.getInteger(), SpriteStanding, frame);
switch (SpriteState) {
  case STILL :
  sprite.drawPlusMask(playerX.getInteger(), playerY.getInteger(), SpriteStanding, frame);
 playerStanding = true;
  case WALK;
  sprite.drawPlusMask(playerX.getInteger(), playerY.getInteger(), SpriteWalking, frame); 
 playerWalking = true;
  case JUMP;
  sprite.drawPlusMask(playerX.getInteger(), playerY.getInteger(), SpriteJumping, frame);
  playerJumping = true;
}

}
  arduboy.display();
}
