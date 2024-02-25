/*
 * SmartHome_Priv.h
 *
 * Created: 2/21/2024 7:00:01 PM
 *  Author: Beast
 */ 


#ifndef SMARTHOME_PRIV_H_
#define SMARTHOME_PRIV_H_

#define HOME_GATE_PASSWORD					{'1', '2', '3', '4', '5', '6', '7', '8'}

ES enuGateControl(boolean * copy_pbool_gatestate);
static boolean Home_bool_gate_system = True;

ES enuLoginControl();
static boolean Home_bool_login_system = False;

#endif /* SMARTHOME_PRIV_H_ */