
void stoph();
int init(int debug_level);
// camera functions
int take_picture();
int save_picture(char fn[5]);
char get_pixel( int row,int col, int color);
int set_pixel(int row, int col, char red, char green,char blue);
void convert_camera_to_screen();
int open_screen_stream();
int close_screen_stream();
int update_screen();
int display_picture(int delay_sec,int delay_usec);
// hardware control functions
int set_motors(unsigned char num_mot,unsigned char pwm);
int sleep1(int msec);
int set_digital(unsigned char chan,unsigned char level);
int read_digital(int chan);
int read_analog(int in_ch_adc);
int hardware_exchange();
// networking functions
int connect_to_server( char server_addr[15],int port);
int send_to_server(char message[24]);
int receive_from_server(char message[24]);

