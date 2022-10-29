#include <programing.h>
#include <tusb.h>
#include <stdio.h>
#include <readline.h>
#include <cpu_core.h>
#include <setclock.h>
#include <pico_hal.h>

char* parse_number(char* input, int* output) {
	int idx = 0;
	int number_system_base = 10;

	if (input[0] == '0') {
		if (input[1] == 'x') {
			number_system_base = 16;
			idx = 2;
		} else if (input[1] == 'b') {
			number_system_base = 2;
			idx = 2;
		}
	}

	int _number = 0;

	while (input[idx] != '\0') {
		if (input[idx] >= '0' && input[idx] <= '9') {
			_number = _number * number_system_base + (input[idx] - '0');
		} else if (input[idx] >= 'a' && input[idx] <= 'f') {
			_number = _number * number_system_base + (input[idx] - 'a' + 10);
		} else if (input[idx] >= 'A' && input[idx] <= 'F') {
			_number = _number * number_system_base + (input[idx] - 'A' + 10);
		} else {
			break;
		}

		idx++;
	}

	*output = _number;

	return &input[idx];
}

extern uint8_t memory_[0xffff];

void programing_mode() {
	while (!tud_cdc_connected()) {}

	bool running = true;
	while (running) {
        char* buf = readLine();

        if (strcmp(buf, "PING") == 0) {
        } else if(strncmp(buf, "READ ", 5) == 0) {
            int addr;
            parse_number(&buf[5], &addr);
            char out[32] = { 0 };
            sprintf(out, "%d\n", cpu_fetch_byte(addr));
            printf(out);
            continue;
        } else if(strncmp(buf, "WRITE ", 6) == 0) {
            int addr;
            char* new = parse_number(&buf[6], &addr);
            int val;
            parse_number(&new[1], &val);
            cpu_write_byte(addr, val);
        } else if (strncmp(buf, "CLOCK ", 6) == 0) {
			int clock;
            parse_number(&buf[6], &clock);
			clock_set_mhz(clock);
        } else if (strcmp(buf, "FORMAT") == 0) {
            pico_unmount();
            if (pico_mount(true) < 0) {
                goto error;
            }
        } else if (strcmp(buf, "SAVE") == 0) {
            int file = pico_open("prog", LFS_O_RDWR | LFS_O_CREAT);
            pico_write(file, memory_, sizeof(memory_));
            pico_close(file);

        } else if (strcmp(buf, "EXIT") == 0) {
            running = false;
        } else {
            goto error;
        }

        printf("OK\n");
        continue;

    error:
        printf("ERROR\n");
	}
}
