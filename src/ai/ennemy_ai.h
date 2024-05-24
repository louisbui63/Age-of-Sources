//! @file ennemy_ai.h
#include "../data_structures/ecs.h"
#include <SDL2/SDL_render.h>

//! The current state the ai is in, i.e., which behavior it should adopt. This
//! is essentially a finite state machine
typedef enum {
  //! The ai focuses on strengthening its economy
  Eco,
  //! The ai focuses on attacking its opponent
  Offense,
  //! The ai focuses on repsonding to an immediate threat
  Defense
} AiState;

//! Reconsiders the current `AiState` depending on the current state of the game
void reconsider_ai_state(World *w, AiState *ais);

//! Returns true iff ai is being attacked
char is_ai_attacked(World *w);

//! Cause the ai to act
void take_ai_action(World *w, AiState *ais, SDL_Renderer *renderer,
                    SDL_Window *window);

void ai_defends_itself(World *w);
