# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# Directories
BIN_DIR = bin
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = .obj
REPORT_DIR = report

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
YELLOW = \033[1;33m
NC = \033[0m # No Color

# Program names
SIMULATION = simulation
KALMAN = kalman

# Source files
SIM_SRCDIR = $(SRC_DIR)/simulation
SIM_SOURCES = main.c initialization.c record.c write.c
SIM_SRC = $(addprefix $(SIM_SRCDIR)/, $(SIM_SOURCES))
SIM_OBJ = $(addprefix $(OBJ_DIR)/simulation/, $(SIM_SOURCES:.c=.o))
SIM_INC = -I$(INC_DIR)/simulation

KALMAN_SRCDIR = $(SRC_DIR)/kalman
KALMAN_SOURCES = main.c kalman.c initialization.c parser.c gauss.c ekf.c
KALMAN_SRC = $(addprefix $(KALMAN_SRCDIR)/, $(KALMAN_SOURCES))
KALMAN_OBJ = $(addprefix $(OBJ_DIR)/kalman/, $(KALMAN_SOURCES:.c=.o))
KALMAN_INC = -I$(INC_DIR)/kalman

# Default target
all: $(BIN_DIR)/$(SIMULATION) $(BIN_DIR)/$(KALMAN)
	@echo "$(GREEN)✓ All programs compiled successfully!$(NC)"

# Individual targets
$(BIN_DIR)/$(SIMULATION): $(SIM_OBJ) | $(BIN_DIR)
	@echo "$(BLUE)Linking $(SIMULATION)...$(NC)"
	@$(CC) $(CFLAGS) -o $(BIN_DIR)/$(SIMULATION) $(SIM_OBJ)
	@echo "$(GREEN)✓ $(SIMULATION) compiled successfully!$(NC)"

$(BIN_DIR)/$(KALMAN): $(KALMAN_OBJ) | $(BIN_DIR)
	@echo "$(BLUE)Linking $(KALMAN)...$(NC)"
	@$(CC) $(CFLAGS) -o $(BIN_DIR)/$(KALMAN) $(KALMAN_OBJ) -lm
	@echo "$(GREEN)✓ $(KALMAN) compiled successfully!$(NC)"

# Object file compilation rules with dependency generation
$(OBJ_DIR)/simulation/%.o: $(SIM_SRCDIR)/%.c | $(OBJ_DIR)/simulation
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(SIM_INC) -c $< -o $@

$(OBJ_DIR)/kalman/%.o: $(KALMAN_SRCDIR)/%.c | $(OBJ_DIR)/kalman
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(KALMAN_INC) -c $< -o $@

# Directory creation
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR)/simulation:
	@mkdir -p $(OBJ_DIR)/simulation

$(OBJ_DIR)/kalman:
	@mkdir -p $(OBJ_DIR)/kalman

# Individual program targets (aliases)
simulation: $(BIN_DIR)/$(SIMULATION)

kalman: $(BIN_DIR)/$(KALMAN)

# Report generation from LaTeX files
report:
	@echo "$(BLUE)Generating PDF report from LaTeX files...$(NC)"
	@if [ -d "$(REPORT_DIR)" ]; then \
		cd $(REPORT_DIR) && \
		for tex_file in *.tex; do \
			if [ -f "$$tex_file" ]; then \
				echo "$(YELLOW)Compiling $$tex_file (1st pass)...$(NC)"; \
				pdflatex -interaction=nonstopmode $$tex_file > /dev/null 2>&1 || echo "$(RED)LaTeX pass 1 warning for $$tex_file$(NC)"; \
				if [ -f "$${tex_file%.tex}.aux" ]; then \
					echo "$(YELLOW)Processing bibliography...$(NC)"; \
					bibtex "$${tex_file%.tex}" > /dev/null 2>&1 || echo "$(YELLOW)No bibliography or bibtex warnings$(NC)"; \
				fi; \
				echo "$(YELLOW)Compiling $$tex_file (2nd pass)...$(NC)"; \
				pdflatex -interaction=nonstopmode $$tex_file > /dev/null 2>&1 || echo "$(RED)LaTeX pass 2 warning for $$tex_file$(NC)"; \
				echo "$(YELLOW)Compiling $$tex_file (3rd pass)...$(NC)"; \
				pdflatex -interaction=nonstopmode $$tex_file > /dev/null 2>&1 || echo "$(RED)LaTeX pass 3 warning for $$tex_file$(NC)"; \
				if [ -f "$${tex_file%.tex}.pdf" ]; then \
					echo "$(GREEN)✓ Successfully generated $${tex_file%.tex}.pdf$(NC)"; \
				else \
					echo "$(RED)✗ Failed to generate PDF for $$tex_file$(NC)"; \
				fi; \
			fi; \
		done; \
		echo "$(GREEN)✓ PDF report generation completed!$(NC)"; \
	else \
		echo "$(RED)Error: $(REPORT_DIR) directory not found!$(NC)"; \
	fi

# Start both programs in separate terminal windows
start: $(BIN_DIR)/$(SIMULATION) $(BIN_DIR)/$(KALMAN)
	@echo "$(BLUE)Starting simulation in first terminal...$(NC)"
	@osascript -e 'tell application "Terminal" to do script "cd $(shell pwd) && ./$(BIN_DIR)/$(SIMULATION)"'
	@sleep 1
	@echo "$(BLUE)Starting kalman filter in second terminal...$(NC)"
	@osascript -e 'tell application "Terminal" to do script "cd $(shell pwd) && ./$(BIN_DIR)/$(KALMAN)"'
	@echo "$(GREEN)✓ Both programs started in separate terminals!$(NC)"

# Plotting target
plot:
	/Applications/MATLAB_R2025a.app/bin/matlab -nosplash -nodesktop -r "cd('src/analysis'); plot_master"
plot_rmse:
	/Applications/MATLAB_R2025a.app/bin/matlab -nosplash -nodesktop -r "cd('src/analysis'); plot_rmse"

# Cleaning rules
clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(REPORT_DIR)/*.aux $(REPORT_DIR)/*.log $(REPORT_DIR)/*.out $(REPORT_DIR)/*.toc $(REPORT_DIR)/*.bbl $(REPORT_DIR)/*.blg $(REPORT_DIR)/*.fls $(REPORT_DIR)/*.fdb_latexmk $(REPORT_DIR)/*.synctex.gz 2>/dev/null || true
	@rm -f data.txt log.txt 2>/dev/null || true
	@echo "$(GREEN)✓ Object files and data files cleaned!$(NC)"

fclean: clean
	@rm -rf $(BIN_DIR)
	@rm -f $(REPORT_DIR)/*.pdf 2>/dev/null || true
	@echo "$(GREEN)✓ All generated files cleaned!$(NC)"

re: fclean all

# Help target
help:
	@echo "$(BLUE)Available targets:$(NC)"
	@echo "  $(GREEN)make$(NC)            - Compile all programs"
	@echo "  $(GREEN)make simulation$(NC) - Compile only simulation"
	@echo "  $(GREEN)make kalman$(NC)     - Compile only kalman filter"
	@echo "  $(GREEN)make report$(NC)     - Generate mathematical report PDF (with bibliography)"
	@echo "  $(GREEN)make start$(NC)      - Run simulation and kalman in separate terminals"
	@echo "  $(GREEN)make plot$(NC)       - Generate trajectory plots with MATLAB"
	@echo "  $(GREEN)make plot_rmse$(NC)  - Generate RMSE analysis plots with MATLAB"
	@echo "  $(GREEN)make clean$(NC)      - Remove object files, data.txt and log.txt"
	@echo "  $(GREEN)make fclean$(NC)     - Remove object files and binaries"
	@echo "  $(GREEN)make re$(NC)         - Clean and rebuild all"
	@echo "  $(GREEN)make help$(NC)       - Show this help message"

# Phony targets
.PHONY: all simulation kalman report start plot plot_rmse clean fclean re help
