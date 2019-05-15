#include "E101.h"

class Robot{ // declarations
private:
	int v_left, v_right, cam_tilt;
	int dv;
	double line_error;
	int quadrant;
	const int cam_width = 320;
	const int cam_height = 240;
	const int v_left_go = 52;
	const int v_right_go = 43;
	double kp = 0.05; // I think this is a good value? might change with testing
	int line_present = 1;

public:
	Rob(){} // default constructor
	int InitHardware();
	void ReadSetMotors();
	void SetMotors();
	int MeasureLine();
	int FollowLine();
};

// example of implementation
int Robot::FollowLine()(){
	MeasureLine();
	if (line_present) {
		dv = (int)(line_error*kp);
		// dv=0;
		v_left = v_left_go + dv;
		v_right = v_right_go + dv;
		cout<<" line_error = "<<line_error<<" dv = "<<dv;
		SetMotors();
	} else {
		// go back
		cout<<" Line missing"<<endl;
		v_left = 39; // value adjustable
		v_right = 55;
		SetMotors();
		sleep1(100);
	}
	return 0;
}

int main(){
	// opens gate
	char server_addr[15] = {'1','3','0','.','1','9','5','.','1','9','6'};
	char message[24] = {'P','l','e','a','s','e'};
	char password[24];
	int connect_to_server(server_addr);
	int send_to_server(message);
	int receive_from_server(password);
	int send_to_server(password);
}


