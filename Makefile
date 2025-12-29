# Makefile for Raspberry Pi Pico W Arduino Project

# Configuration
SKETCH = test.ino
BOARD_URL = https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
FQBN = rp2040:rp2040:rpipicow
BAUD_RATE = 115200
LIBRARIES = Pico-DMX

# Auto-detect port (macOS)
PORT = $(shell ls /dev/cu.usbmodem* 2>/dev/null | head -n 1)

# Colors for output
GREEN = \033[0;32m
BLUE = \033[0;34m
NC = \033[0m # No Color

.PHONY: help install install-libs compile upload monitor clean all

help:
	@echo "$(BLUE)Raspberry Pi Pico W Arduino Project$(NC)"
	@echo ""
	@echo "Available targets:"
	@echo "  $(GREEN)install$(NC)      - Install RP2040 board support and libraries"
	@echo "  $(GREEN)install-libs$(NC) - Install required libraries only"
	@echo "  $(GREEN)compile$(NC)      - Compile the sketch"
	@echo "  $(GREEN)upload$(NC)       - Upload sketch to board"
	@echo "  $(GREEN)monitor$(NC)      - Open serial monitor"
	@echo "  $(GREEN)all$(NC)          - Compile and upload"
	@echo "  $(GREEN)clean$(NC)        - Clean build files"
	@echo "  $(GREEN)boards$(NC)       - List connected boards"
	@echo ""

install: install-libs
	@echo "$(BLUE)Installing RP2040 board support...$(NC)"
	arduino-cli core update-index --additional-urls $(BOARD_URL)
	arduino-cli core install rp2040:rp2040 --additional-urls $(BOARD_URL)
	@echo "$(GREEN)Installation complete!$(NC)"

install-libs:
	@echo "$(BLUE)Installing required libraries...$(NC)"
	arduino-cli lib update-index
	@for lib in $(LIBRARIES); do \
		echo "Installing $$lib..."; \
		arduino-cli lib install "$$lib"; \
	done
	@echo "$(GREEN)Libraries installed!$(NC)"

compile:
	@echo "$(BLUE)Compiling $(SKETCH)...$(NC)"
	arduino-cli compile --fqbn $(FQBN) $(SKETCH)
	@echo "$(GREEN)Compilation complete!$(NC)"

upload: compile
	@echo "$(BLUE)Uploading to Pico W...$(NC)"
	@if [ -z "$(PORT)" ]; then \
		echo "Error: No Pico W board found. Please connect your board."; \
		exit 1; \
	fi
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(SKETCH)
	@echo "$(GREEN)Upload complete!$(NC)"

monitor:
	@echo "$(BLUE)Opening serial monitor at $(BAUD_RATE) baud...$(NC)"
	@if [ -z "$(PORT)" ]; then \
		echo "Error: No Pico W board found. Please connect your board."; \
		exit 1; \
	fi
	@echo "Press Ctrl+C to exit"
	arduino-cli monitor -p $(PORT) -c baudrate=$(BAUD_RATE)

boards:
	@echo "$(BLUE)Connected boards:$(NC)"
	@arduino-cli board list

clean:
	@echo "$(BLUE)Cleaning build files...$(NC)"
	@rm -rf build/
	@echo "$(GREEN)Clean complete!$(NC)"

all: compile upload
	@echo "$(GREEN)Build and upload complete!$(NC)"
