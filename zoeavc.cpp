#include "E101.h"
#include <iostream>

using namespace std;

/** 

I don't check to see if the line is there on the first go, need 
to make the robot move forward so that it can see the line 
after the gate has been opened

DO I NEED A KP VALUE?? (IN OTHER PROGRAM KP=0.5)

**/

class Robot{
	private:
		// Size of camera
		int cam_width = 320;
		int cam_height = 240;
		int cam_mid = cam_height/2;
		// used for MeasureLine
		int prev_diff;
		bool line;
		bool first = true;
		// used for SetMotors (the number each motor is plugged into)
		int left_mot = 5;
		int right_mot = 4;
		// used in multiple methods
		int num_black;
		double line_error;
		double change_mot;
		int v_right = 44;
		int v_left = 54
		
	public;
		Robot(){};
		int MeasureLine();
		int FollowLine();
		void SetMotors();
}

int Robot::MeasureLine() {
	line_error = 0; // reset every time
	int min = 255;
	int max = 0; 
	
	for (int i = 0;i < cam_width;i++) { // for every pixel in middle of camera
		// finding min and max values (of greyscale ie darkest black, lightest white)
		int white = get_pixel(cam_mid,count,3);
		if (white < min) {
			min = white;
		} else if (white > max) {
			max = white;
		}
	}
	
	int diff = max-min;
	int threshold = (max-min)/2; //for determining if pixel is black or white
	
	// Checks if line is present
	if (first = false && diff < prev_diff/2) {
		line = false;
	} else {
		prev_diff = diff;
	
		num_black; // number of black pixels
		// checks if pixel black or white and adds 1 to array if black
		for (int i = 0; i < cam_width; i++) {
			int pixel = get_pixel(cam_mid,i,3);
			if (pixel < threshold) {
				bw[i] = 1;
				num_black++;
			} else {
				bw[i] = 0;
			}
		}
		
		// calculates the line error
		for (int i = 0; i < cam_width; i++) {
			int num = i-(cam-width/2);
			line_error = line_error + bw[i]*num;
		}
		
		first = false;
	}
	
	return 0;
}

int Robot::FollowLine() {
	MeasureLine();
	if (line_present) {
		change_mot = line_error/num_black; // how much the error should change motot
		// THIS MIGHT CAUSE MOTORS TO GO BACKWARDS
		v_right = v_right + change_mot;
		v_left = v_left + change_mot;
		SetMotors();
	} /*else if (!line_present) {
		DO SOMETHING IF THE LINE ISN'T THERE
		it should turn right (otherwise will go backwards)
	}*/
	
	return 0;
}

void SetMotors() {
	set_motors(left_mot, v_left);
	set_motors(right_mot, v_right);
	hardware_exchange();
}

int main(){
	Robot Rob;
	init(0);//Initialised
	while(true){
		Rob.FollowLine();
	}
}