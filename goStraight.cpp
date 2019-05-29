#include "E101.h"
#include <iostream>

using namespace std;

class Robot{ // declarations
	
private:
	int v_left, v_right, cam_tilt;
	const int v_left_go = 52;
	const int v_right_go = 44;
	double kp = 0.05; // I think this is a good value? might change with testing
	int line_present = 1;
	int turn_left=0;
	int turn_right=0;

public:
	Robot(){} // default constructor
	void SetMotors();
};

void Robot::SetMotors(){
	cout<<"v_left = "<<v_left<<endl;
	cout<<"v_right = "<<v_right<<endl;
	set_motors(5,v_left_go); //ask about this
	set_motors(4,v_right_go);
	hardware_exchange();
}

int main(){
	Robot Rob;
	init(0);
	Rob.SetMotors();
}
