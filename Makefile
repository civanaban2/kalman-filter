# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -O2

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
ANALYSIS = analysis

# Source files
SIM_SRCDIR = $(SRC_DIR)/simulation
SIM_SOURCES = main.c initialization.c record.c write.c
SIM_SRC = $(addprefix $(SIM_SRCDIR)/, $(SIM_SOURCES))
SIM_OBJ = $(addprefix $(OBJ_DIR)/simulation/, $(SIM_SOURCES:.c=.o))
SIM_INC = -I$(INC_DIR)/simulation

KALMAN_SRCDIR = $(SRC_DIR)/kalman
KALMAN_SOURCES = main.c kalman.c initialization.c parser.c gauss.c ekf.c matrix_operations.c
KALMAN_SRC = $(addprefix $(KALMAN_SRCDIR)/, $(KALMAN_SOURCES))
KALMAN_OBJ = $(addprefix $(OBJ_DIR)/kalman/, $(KALMAN_SOURCES:.c=.o))
KALMAN_INC = -I$(INC_DIR)/kalman

ANALYSIS_SRCDIR = $(SRC_DIR)/analysis
ANALYSIS_SOURCES = analysis.c
ANALYSIS_SRC = $(addprefix $(ANALYSIS_SRCDIR)/, $(ANALYSIS_SOURCES))
ANALYSIS_OBJ = $(addprefix $(OBJ_DIR)/analysis/, $(ANALYSIS_SOURCES:.c=.o))

# Default target
all: $(BIN_DIR)/$(SIMULATION) $(BIN_DIR)/$(KALMAN) $(BIN_DIR)/$(ANALYSIS)
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

$(BIN_DIR)/$(ANALYSIS): $(ANALYSIS_OBJ) | $(BIN_DIR)
	@echo "$(BLUE)Linking $(ANALYSIS)...$(NC)"
	@$(CC) $(CFLAGS) -o $(BIN_DIR)/$(ANALYSIS) $(ANALYSIS_OBJ) -lm
	@echo "$(GREEN)✓ $(ANALYSIS) compiled successfully!$(NC)"

# Object file compilation rules with dependency generation
$(OBJ_DIR)/simulation/%.o: $(SIM_SRCDIR)/%.c | $(OBJ_DIR)/simulation
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(SIM_INC) -c $< -o $@

$(OBJ_DIR)/kalman/%.o: $(KALMAN_SRCDIR)/%.c | $(OBJ_DIR)/kalman
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(KALMAN_INC) -c $< -o $@

$(OBJ_DIR)/analysis/%.o: $(ANALYSIS_SRCDIR)/%.c | $(OBJ_DIR)/analysis
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Directory creation
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR)/simulation:
	@mkdir -p $(OBJ_DIR)/simulation

$(OBJ_DIR)/kalman:
	@mkdir -p $(OBJ_DIR)/kalman

$(OBJ_DIR)/analysis:
	@mkdir -p $(OBJ_DIR)/analysis

# Individual program targets (aliases)
simulation: $(BIN_DIR)/$(SIMULATION)

kalman: $(BIN_DIR)/$(KALMAN)

analysis: $(BIN_DIR)/$(ANALYSIS)

# Report generation from LaTeX files
report:
	@echo "$(BLUE)Generating PDF report from LaTeX files...$(NC)"
	@if [ -d "$(REPORT_DIR)" ]; then \
		cd $(REPORT_DIR) && \
		for tex_file in *.tex; do \
			if [ -f "$$tex_file" ]; then \
				echo "$(YELLOW)Compiling $$tex_file...$(NC)"; \
				pdflatex $$tex_file; \
				pdflatex $$tex_file; \
			fi; \
		done; \
		echo "$(GREEN)✓ PDF reports generated successfully!$(NC)"; \
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

# Cleaning rules
clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(REPORT_DIR)/*.aux $(REPORT_DIR)/*.log $(REPORT_DIR)/*.out $(REPORT_DIR)/*.toc $(REPORT_DIR)/*.fls $(REPORT_DIR)/*.fdb_latexmk $(REPORT_DIR)/*.synctex.gz 2>/dev/null || true
	@echo "$(GREEN)✓ Object files cleaned!$(NC)"

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
	@echo "  $(GREEN)make analysis$(NC)   - Compile only analysis"
	@echo "  $(GREEN)make report$(NC)     - Generate PDF from LaTeX files"
	@echo "  $(GREEN)make start$(NC)      - Run simulation and kalman in separate terminals"
	@echo "  $(GREEN)make clean$(NC)      - Remove object files"
	@echo "  $(GREEN)make fclean$(NC)     - Remove object files and binaries"
	@echo "  $(GREEN)make re$(NC)         - Clean and rebuild all"
	@echo "  $(GREEN)make help$(NC)       - Show this help message"

# Phony targets
.PHONY: all simulation kalman analysis report start clean fclean re help
