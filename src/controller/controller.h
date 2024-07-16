/*
 * controller.h
 *
 *  Created on: 27 Jun 2024
 *      Author: Maldus
 */

#ifndef CONTROLLER_CONTROLLER_H_
#define CONTROLLER_CONTROLLER_H_


#include "model/model.h"


void controller_init(mut_model_t *pmodel);
void controller_manage(mut_model_t *pmodel);


#endif /* CONTROLLER_CONTROLLER_H_ */
