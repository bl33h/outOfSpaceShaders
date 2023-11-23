/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: camera.h
@version: I
Creation: 29/09/2023
Last modification: 23/11/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/
#pragma once

struct Camera {
  glm::vec3 cameraPosition;
  glm::vec3 targetPosition;
  glm::vec3 upVector;
};