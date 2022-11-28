#include <iostream>
#include <nlohmann/json.hpp>
#include <color.h>

extern "C" {
	#include <cpu_core.h>
}

uint8_t memory_[0xffff] = { 0 };

uint8_t cpu_fetch_byte(uint16_t addr) {
	// debugf("fetching byte at 0x%x", addr);
	return memory_[addr];
}

void cpu_write_byte(uint16_t addr, uint8_t val) {
	// debugf("writing byte 0x%x at 0x%x", val, addr);
	memory_[addr] = val;
}

uint8_t cpu_io_read(uint16_t addr) {
	// debugf("reading byte from io at 0x%x", addr);
	return 0x0;
}

void cpu_io_write(uint16_t addr, uint8_t val) {
	// debugf("writing byte 0x%x to io at 0x%x", val, addr);
}


int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " <json_config>" << std::endl;
		return 1;
	}

	FILE* config = fopen(argv[1], "r");
	if (config == NULL) {
		std::cout << "Config file not found" << std::endl;
		return 1;
	}

	nlohmann::json json_config;
	fseek(config, 0, SEEK_END);
	long fsize = ftell(config);
	fseek(config, 0, SEEK_SET);
	char* buffer = new char[fsize + 1];
	memset(buffer, 0, fsize + 1);
	fread(buffer, 1, fsize, config);
	printf("%s\n", buffer);
	json_config = nlohmann::json::parse(buffer);

	for (auto& it : json_config) {
		std::string file = it["file"];
		std::cout << YELLOW "Loading " << file << "..." COLOR_RESET << std::endl;
		memset(memory_, 0, sizeof(memory_));
		FILE* f = fopen(file.c_str(), "rb");
		if (f == NULL) {
			printf("could not open file\n");
			return 1;
		}

		fseek(f, 0, SEEK_END);
		size_t size = ftell(f);
		fseek(f, 0, SEEK_SET);

		if (size > sizeof(memory_)) {
			printf("file too large\n");
			return 1;
		}

		fread(memory_, 1, size, f);
		fclose(f);

		cpu_state_t state = { 0 };
		int timeout = 100000;
		while (cpu_tick(&state)) {
			if(timeout-- == 0) {
				std::cout << RED "--- TEST TOOK TO LONG ---" COLOR_RESET << std::endl;
				break;
			}
		}

		std::string regs[] = { "r0", "r1", "r2", "r3", "r4", "r5" };
		for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
			if (it[regs[i]].is_number_integer()) {
				int expected = it[regs[i]];
				if (state.regs[i] != expected) {
					printf(RED "--- ERROR --- expected %d in %s but was %d!\n" COLOR_RESET, expected, regs[i].c_str(), state.regs[i]);
				} else {
					printf(GREEN "--- SUCCESS --- %s was %d!\n" COLOR_RESET, regs[i].c_str(), state.regs[i]);
				}
			}
		}

	}

	return 0;
}