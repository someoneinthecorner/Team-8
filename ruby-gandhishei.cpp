#include <stdio.h>
#include "E101.h"
#include <unistd.h>
#include <time.h>

int main() {
	int err;
	printf(" Hello\n");
	err = init();
	int countrun = 0;
	int countr = 0;
	int totred = 0;
	int totgreen = 0;
	int totblue = 0;
	int totint = 0;
	int countc = 0;
	float redness = 0;
	float greenness = 0;
	float blueness = 0;
	
	while (countrun<1000){
		select_IO(0, 0);
		open_screen_stream();
		take_picture();
		update_screen();
		countr = 0;
		countc = 0;
		totred = 0;
		totgreen = 0;
		totblue = 0;
		totint = 0;
		redness = 0;
		greenness = 0;
		blueness = 0;
		
		
		while(countr<240){
			countc = 0;
			while(countc<320){
				
				totred = totred + (int)get_pixel(countr,countc,0);
				totgreen = totgreen + (int)get_pixel(countr,countc,1);
				totblue = totblue + (int)get_pixel(countr,countc,2);
				totint = totint + (int)get_pixel(countr,countc,3);
				redness = (float)totred/(3.0*(float)totint);
				greenness = (float)totgreen/(3.0*(float)totint);
				blueness = (float)totblue/(3.0*(float)totint);
						
				
				/* printf("pixelr%d\n", countr);
				 * printf(""pixelc%d\n", countc);
				 * printf("totred%d\n", totred);
				 * sleep1(0,100000);
				 * */
				countc++;
				}
				countr++;
			}
			
			printf("	\n");
			printf("Total red %d\n", totred);
			printf("Total green %d\n", totgreen);
			printf("Total blue %d\n", totblue);
			printf("Total int %d\n", totint);
			printf("Redness %f\n", redness);
			printf("Greenness %f\n", greenness);
			printf("Blueness %f\n", blueness);
			sleep1(0,200000);
			countrun++;
			if (totblue < 8000000){
				printf("Ruby not detected!!!\n");
			}  else {
				printf("Ruby secure \n");
				}
		}
		close_screen_stream();
		printf("After init() error=%d\n", err);
		printf("Again\n");
		return 0;
	}
