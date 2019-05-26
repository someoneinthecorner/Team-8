#include "E101.h"
#include <iostream>

using namespace std;

class Robot{ // declarations
	
private:
	int v_left, v_right, cam_tilt;
	int dv;
	double line_error = 0;
	int quadrant = 2;
	int median_white;
	int prevMedian;
	int prevDiff = 255;
	const int cam_width = 320;
	const int cam_height = 240;
	const int cam_mid = cam_height/2;
	const int cam_midPlus = (cam_height/2)+(cam_height/4);
	const int v_left_go = 52;
	const int v_right_go = 43;
	double kp = 0.05; // I think this is a good value? might change with testing
	int line_present = 1;
	int turn_left=0;
	int turn_right=0;
	int running=1;
	int prev_turn=0;
	int turns=0;
	int *indexes = new int[cam_width]; 
	int *bw  = new int[cam_width];
	int total_ahead = 0; 

public:
	Robot(){} // default constructor
	int InitHardware();
	void ReadSetMotors();
	void SetMotors();
	void update_hardware();
	int MeasureLine();
	int FollowLine();
	int OpenGate();
	int get_quadrant();
};

int Robot::get_quadrant(){
		return quadrant;
	}

int Robot::MeasureLine(){
			turn_left = 0;	
			turn_right = 0;
			int count=0;
			int white;
			int min_white = 255;
			int max_white = 0;
			while(count <= cam_width){	// go through every pixel in the mid line

				// gets min and max white values
				white = get_pixel(cam_mid,count,3);
				if (white<min_white){
					min_white = white;
					}
				if (white>max_white){
					max_white = white;
				}
				count++;
			}
			int diff = max_white - min_white;	//
			median_white = (max_white-min_white)/2; 
  			
			// checks if line present or if sharp turn
			if (max_white>prevMedian){
				if (diff < prevDiff/2){
					line_present = 0;
					if (turns>4){quadrant=4;}		// at dead end checks if it has completed the maze
				}
				else {	line_present = 1; 
					prevDiff=diff;}
			}
			else{turn_left=1;}	
			
				int total = 0;
				for (int i=0; i<=cam_width; i++){	// go through every pixel in the mid line
					
					// checks if values are black or white and adds to bw array
					
					white = get_pixel(cam_mid,i,3);
					
					if (white < median_white){
						bw[i] = 1;
						total = total+bw[i];
					}
					else{
						bw[i] = 0;
					}
					int next_white = get_pixel(cam_midPlus,i,3); // looks ahead on line
					if (next_white < median_white){
						total_ahead++;
					}

				}

				line_error = 0; // reset line error every time

				for (int i=0; i<=cam_width; i++){ // goes through bw array and calculates error
					int j = i-(cam_width/2);
					line_error = line_error + bw[i]*j;
				}

			// this might not work if the line isn't exactly in the centre bc it wont take up half the array but will it adjust properly?
			if (total>=(cam_width/2)){	// black line takes up more than half the array
				if (line_error>0){		// black line on right side	
					if (total_ahead>0){	// checks if straight line ahead
						line_error = 0;
					}
					else{		
						turn_right=1;
					}					
				}
				else if (line_error<0){	// black line on left side			
					turn_left=1;
				}
			}
			if (prev_turn == 1 && turn_left==0){ // checks if left turn has been completed and adds to count of left turns
				turns++;
			}

			return 0;
	} 


// example of implementation
int Robot::FollowLine(){
	MeasureLine();
	if (line_present) {
		if (turn_left){	// no idea if these values are correct, could be opposite
			v_left = 49;	
			v_right = v_right_go;
		}
		if (turn_right){	
			v_left = v_left_go;	
			v_right = 47;
		}
		else{
			dv = (int)(line_error*kp);
			// dv=0;
			v_left = v_left_go + dv;
			v_right = v_right_go + dv;
			cout<<" line_error = "<<line_error<<" dv = "<<dv;
		}
		SetMotors();
	} else {
		// !! should turn left until sees line !!
		cout<<" Line missing"<<endl;
		v_left = 49;	
		v_right = v_right_go;
		SetMotors();
	}
	return 0;
}

void Robot::SetMotors(){
	set_motors(5,v_left); //ask about this
	set_motors(1,v_right);
	hardware_exchange();
	}

int Robot::OpenGate(){
	// opens gate
	char server_addr[15] = {'1','3','0','.','1','9','5','.','6','.','1','9','6'};
	char message[24] = {'P','l','e','a','s','e'};
	char password[24];
	connect_to_server(server_addr,1024);
	send_to_server(message);
	receive_from_server(password);
	send_to_server(password);
	return 0;
	}

int main(){
	Robot Rob;
	Rob.InitHardware();//Initialised
	open_screen_stream();
	Rob.OpenGate();
	int quadrant = Rob.get_quadrant();
	while(quadrant!=4){
		Rob.FollowLine();
	}

	close_screen_stream();
}




