# Smart Vending Machine (SVM)

## About the project
Smart Vending Machine is a compact wall-mountable vending machine that does not require anyone to insert cash in it. We designed this machine keeping in mind the vending ecosystem of India and the vulnerabilities that impede the proliferation of these machines in the country. The main aim if the project was to make these economic and compact to aid towards the aforementioned cause. This machine also sported an advertisement screen to increase the net revenue of the vendor. 

![](https://github.com/rhitvik/Smart_Vending_Machine/blob/master/Images_and_Screenshots/Smart_Vending_Machine_Gen-2.jpg)

## How to purchase from the machine?
Simply download the application from the app store and login/sign up to your account. The application will automatically search and locate the nearest SVM and will tell the user all the inventories inside of it. Simply click on the product that you want to purchase and it will give you a secret code after you have made the payment. Now simply enter the code into the machine and you will have it in your hand.

## How far did we take this project?
10 of these machines were deployed in Rajasthan IT Day 2017 and were used to distribute goodies and gifts to the participants. Also, showcased in the Jaipur Literature Fest 2016, it was admired by the top entrepreneurs of the state and the Department of Information Technology. Our Startup also secured 3rd position at the Startup Inspire, Indian Industrial Fair 2016 for being one of the best business ideas amongst 102 participants from all over the country.

## System 
The entire system was can be divided into 4 subsystems:
1. Master Control and Supervisory System
	- This was the main board where all the other subsystem information was sent and processed. 
	- It consisted of 3 AVR microcontrollers
		* Atmega 328pu for establishing the primary communication between every device including the Ethernet Controller, Motor Driver Controller, Sensor Grid Controller, and Human Machine Interface (HMI) Controller.
		* Atmega16 was the controller and was responsible for reading the input data (PIN Code) from the user and display the feedback and status on to the screen.
		* Atmega32 was responsible for:
	 		a) Controlling the Motor Drivers which then made the respective motor rotate till the product was successfully dispatched. 
			b) Talking to the IR sensor Grid to be acknowledged about the product dispatch status and when it drops, stop the motor immediately and communicate back to the primary communication controller.
	- Sensor Services (IR Sensor Grid)
		This was the system that acknowledged the Master Control System about the product dispatch status.
		It also generated alert signals when it was tried to be tempered with.
		Atmega8 microcontroller was responsible to keep a check on 16 IR sensors at the same time.
2. Ethernet Controller for web communication with a W5500 chip was responsible for taking the PIN code entered by the user and communicate to the server for authentication and then, communicate if the PIN was right and what product to dispatch.
3. The power system was responsible for powering the microcontrollers and drivers with 5 volts and 12 volts to the motors. Employed a modified PC PSU

## Codes
[Master Control Board Master Communication Control](https://github.com/rhitvik/Smart_Vending_Machine/blob/master/Codes/Control_Board_ATmega328pu/10%20motors%20master%20control/10%20motors%20master%20control.cpp)

[Master Control Board Motor Control](https://github.com/rhitvik/Smart_Vending_Machine/blob/master/Codes/Control_Board_Atmega32/10motors/10motors.cpp)

[Master Control Board HMI and IO](https://github.com/rhitvik/Smart_Vending_Machine/blob/master/Codes/Control_Board_ATmega16/password%20based/password%20based.cpp)

[Sensor Control Services](https://github.com/rhitvik/Smart_Vending_Machine/blob/master/Codes/Sensor_ATmega8/MR.SENSOR/MR.SENSOR.cpp)

[Socket-Client for authorization](https://github.com/rhitvik/Smart_Vending_Machine/blob/master/Codes/SocketClient_Ethernet_StateMachine_polled/SocketClient_Ethernet_StateMachine_polled.ino)

## Schematics and PCB layout
![](https://github.com/rhitvik/Smart_Vending_Machine/blob/master/Images_and_Screenshots/System_Controller_Schematic.JPG "Schematic of the Control Circuit")

## Actual pictures

## Further Potential

 
