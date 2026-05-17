/*
 * PosesEDITOR.h
 *
 * Created: 17/05/2026 17:00:37
 *  Author: grett
 */ 


#ifndef POSESEDITOR_H_
#define POSESEDITOR_H_

#include <stdint.h>

//================ FUNCIONES =================//
/*
void Pose_Save(uint8_t pose);

void Pose_Load(uint8_t pose);

void Pose_Edit(uint8_t pose);

uint8_t Pose_GetBaseAddress(uint8_t pose);
*/

//================ FUNCIONES =================//

uint8_t Pose_GetBaseAddress(uint8_t pose);

void Pose_Save(
uint8_t pose,
uint8_t s1,
uint8_t s2,
uint8_t s3,
uint8_t s4,
uint8_t s5,
uint8_t s6
);


#endif /* POSESEDITOR_H_ */