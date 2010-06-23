/*------------------------------------------------
 * 
 *   Author: Alexander Sagen <asagen@gmail.com> 
 *   (c) 2010
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ---------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char* LED_PATH = "/sys/class/leds/asus::mail/brightness";

void usage(){
	printf("Usage: setaml [on|off|blink {interval(ms) iterations}] \n");
}

void setStatus(char* status){
	if(seteuid(0) < 0){
		printf("Unable to enter super-user mode, setuid set?\n");
		exit(-1); // segfaults if trying to write without permission
	}
	FILE *fp = fopen(LED_PATH, "w");
	fprintf(fp, "%s", status);
	fclose(fp);
	seteuid(getuid());
}

int main(int argc, char **argv){
	seteuid(getuid());
	if(argc == 1){
		printf("Missing operand\n");
		usage();
		exit(0);
	}
	if(strcmp(argv[1], "on") == 0){
		setStatus("1");
	}else if(strcmp(argv[1], "off") == 0){
		setStatus("0");
	}else if(strcmp(argv[1], "blink") == 0){
		if(argc < 4){
			printf("Please provide interval and iterations\n");
			usage();
			exit(0);
		}

		
		int interval = atoi(argv[2]);
		int iterations = atoi(argv[3]);
		int x;
		for(x = 0; x < iterations; x++){
			setStatus(x % 2 == 0 ? "0" : "1");

			// Will be ignored if < 1000
			// Needes as usleep is undefined for > 1,000,000
			sleep(interval / 1000);
			
			// Will sleep for the remainder of the interval. (ms*1000=us)
			usleep((interval % 1000) * 1000);	
		}
		if(argc > 4){
			if(strcmp(argv[4], "on") == 0){
				setStatus("1");
			}else if(strcmp(argv[4], "off") == 0){
				setStatus("0");
			}
		}	
	}else{
		printf("Unknown operand\n");	
		usage();
	}	
	return 0;

}

