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
	int prevDiff = 255;
	const int cam_width = 320;
	const int cam_height = 240;
	const int cam_mid = cam_height/2;
	const int cam_midPlus = (cam_height/2)+(cam_height/4);
	const int v_left_go = 52;
	const int v_right_go = 41;
	double kp = 0.05; // I think this is a good value? might change with testing
	int line_present = 1;
	int intersection = false;
	int *indexes = new int[cam_width]; 
	int *bw  = new int[cam_width];
	int total_ahead = 0;
	int turn_total = 0;
	

public:
	Robot(){} // default constructor
	void SetMotors();
	int MeasureLine();
	int FollowLine();
	int MeasureColor();
	int OpenGate();
	int get_quadrant();
	int stop();
};

int Robot::get_quadrant(){
	count = 0;
	while(count <= cam_width){	// go through every pixel in the mid line
       totred = totred+(int)get_pixel(cam_mid,count,0);
       cout<<"Red now ="<<totred<<"\n";
    }
	if(totred >= 50){
		quadrant = 3;
		cout<<"Quadrant 3"<<endl;
	}
	return quadrant;
}
	

int Robot::MeasureLine(){
			int count=0;
			int white;
			int min_white = 255;
			int max_white = 0;
			line_present = 1;
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
			//int diff = max_white - min_white;	
			median_white = (max_white+min_white)/2; 
			int diff = max_white-min_white;
			
			// check if intersection
			if (diff< prevDiff/2){
				line_present = 0;
			}
			
				//int total = 0;
				for (int i=0; i<=cam_width; i++){	// go through every pixel in the mid line
					
					// checks if values are black or white and adds to bw array
					
					white = get_pixel(cam_mid,i,3);
					
					if (white < median_white){
						bw[i] = 1;
						//total++; // dont know why i use total here
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
				//total = 0;
				for (int i=0; i<cam_width; i++){ // goes through bw array and calculates error
					//total = total+bw[i];
					int j = i-(cam_width/2);
					line_error = line_error + bw[i]*j;
				}
				line_error = line_error/10;

			return 0;
	} 

	

// example of implementation
int Robot::FollowLine(){
	MeasureLine();
	if (line_present) {
			dv = (int)(line_error*kp);
			v_left = v_left_go + dv;
			v_right = v_right_go + dv;
			cout<<" line_error = "<<line_error<<" dv = "<<dv;
			SetMotors();
		}
		
	 else {
		// turns left until finds line again but also if intersection
		cout<<" Line missing or intersection"<<endl;
		v_left = v_right_go;	
		v_right = v_right_go;
		SetMotors();
	}
	return 0;
}

//Checking which direction
int Robot::FollowMaze(){
	MeasureLine();
	if (line_present) {
			dv = (int)(line_error*kp);
			v_left = v_left_go + dv;
			v_right = v_right_go + dv;
			cout<<" line_error = "<<line_error<<" dv = "<<dv;
			if(turn_total = 0 and line_error < -200){
				cout<<"turn 1 - right"<<"\n";
				set_motors(5,50); 
				set_motors(1,42);
				sleep1(10000);
				turn_total = 1;
			}
			else if(turn_total = 1 and line_error < -200){
				cout<<"turn 2 - right"<<"\n";
				set_motors(5,50); 
				set_motors(1,42);
				sleep1(10000);
				turn_total = 2;
			}
			else if(turn_total = 2 and line_error < 200){
				cout<<"turn 3 - left"<<"\n";
				set_motors(5,42); 
				set_motors(1,50);
				sleep1(10000);
				turn_total = 3;
			}
			else if(turn_total = 3 and line_error < -800){
				cout<<"turn 4 - left"<<"\n";
				set_motors(5,42); 
				set_motors(1,50);
				sleep1(10000);
				turn_total = 4;
			}
			else if(turn_total = 4 and line_error < -200){
				cout<<"turn 5 - right"<<"\n";
				set_motors(5,50); 
				set_motors(1,42);
				sleep1(10000);
				turn_total = 5;
			}
			else if(turn_total = 5 and line_error < -800){
				cout<<"turn 6 - left"<<"\n";
				set_motors(5,42); 
				set_motors(1,50);
				sleep1(10000);
				turn_total = 6;
			}
			else if(turn_total = 6 and line_error < -800){
				cout<<"turn 7 - right"<<"\n";
				set_motors(5,50); 
				set_motors(1,42);
				sleep1(10000);
			}
			else{
				SetMotors();
			}
		}
		
	 else {
		// turns left until finds line again but also if intersection
		cout<<" Line missing or intersection"<<endl;
		v_left = 49;	
		v_right = v_right_go;
		SetMotors();
	}
	return 0;
}

void Robot::SetMotors(){
	cout<<"v_left = "<<v_left<<endl;
	cout<<"v_right = "<<v_right<<endl;
	if (v_left>60){v_left=60;}
	if (v_left<49){v_left=49;}
	if (v_right<35){v_right=35;}
	if (v_right>47){v_right=47;}
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
	
int Robot::stop(){
	set_motors(5,48);
	set_motors(1,48);
	hardware_exchange();
	return(0);
	}
	
int main(){
	Robot Rob;
	init(0);//Initialised
	open_screen_stream();
	Rob.OpenGate();
	//int quadrant = Rob.get_quadrant();
	int count =0;
	while(count<100){
		take_picture();
		update_screen();
		if(quadrant == 2){
			Rob.FollowLine();
		} else if(quadrant = 3){
			Rob.FollowMaze();
		}	
		
		count++;
	}
	Rob.stop();

	close_screen_stream();
}
