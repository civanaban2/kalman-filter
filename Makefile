CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = $(SRC_DIR)/obj
INC_DIR = inc

SIM_SRC = $(SRC_DIR)/simulation/main.c \
		  $(SRC_DIR)/simulation/initialization.c \
		  $(SRC_DIR)/simulation/record.c \

SIM_OBJ = $(OBJ_DIR)/main.o \
		  $(OBJ_DIR)/initialization.o \
		  $(OBJ_DIR)/record.o \

SIM_INC = -I$(INC_DIR)/simulation

KALMAN_SRC = $(SRC_DIR)/kalman/main.c \
			 $(SRC_DIR)/kalman/kalman.c \
			 $(SRC_DIR)/kalman/initialization.c \
			 $(SRC_DIR)/kalman/parser.c \
			 $(SRC_DIR)/kalman/gauss.c \
			 $(SRC_DIR)/kalman/ekf.c \
			 $(SRC_DIR)/kalman/matrix_operations.c \

KALMAN_OBJ = $(OBJ_DIR)/kalman_main.o \
			 $(OBJ_DIR)/kalman.o \
			 $(OBJ_DIR)/kalman_init.o \
			 $(OBJ_DIR)/parser.o \
			 $(OBJ_DIR)/gauss.o \
			 $(OBJ_DIR)/ekf.o \
			 $(OBJ_DIR)/matrix_operations.o \

KALMAN_INC = -I$(INC_DIR)/kalman

ANALYSIS_SRC = $(SRC_DIR)/analysis/analysis.c

ANALYSIS_OBJ = $(OBJ_DIR)/analysis.o

all: simulation kalman analysis

simulation: $(SIM_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/simulation $(SIM_OBJ)

kalman: $(KALMAN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/kalman $(KALMAN_OBJ) -lm

analysis: $(ANALYSIS_OBJ)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/analysis $(ANALYSIS_OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/simulation/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinc/simulation -c $< -o $@

$(OBJ_DIR)/kalman_main.o: $(SRC_DIR)/kalman/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(KALMAN_INC) -c $< -o $@

$(OBJ_DIR)/kalman.o: $(SRC_DIR)/kalman/kalman.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(KALMAN_INC) -c $< -o $@

$(OBJ_DIR)/kalman_init.o: $(SRC_DIR)/kalman/initialization.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(KALMAN_INC) -c $< -o $@

$(OBJ_DIR)/parser.o: $(SRC_DIR)/kalman/parser.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(KALMAN_INC) -c $< -o $@

$(OBJ_DIR)/gauss.o: $(SRC_DIR)/kalman/gauss.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(KALMAN_INC) -c $< -o $@

$(OBJ_DIR)/ekf.o: $(SRC_DIR)/kalman/ekf.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(KALMAN_INC) -c $< -o $@

$(OBJ_DIR)/matrix_operations.o: $(SRC_DIR)/kalman/matrix_operations.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(KALMAN_INC) -c $< -o $@

$(ANALYSIS_OBJ): $(ANALYSIS_SRC)
	$(CC) $(CFLAGS) -c $(ANALYSIS_SRC) -o $(ANALYSIS_OBJ)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BIN_DIR)

re : fclean all

.PHONY: all clean fclean re simulation kalman ekf analysis