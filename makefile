# Nome do executável final
APPS = hospital_sus

# Compilador e Flags
CC = gcc
# -I./include diz ao compilador para procurar .h dentro da pasta include e subpastas
CFLAGS = -Wall -Wextra -g -I./include -I./include/tad_estruturas -I./include/tad_elementos -I./include/utils

# Pastas
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

# Encontrar todos os arquivos .c recursivamente dentro de src/
SRCS := $(shell find $(SRC_DIR) -name '*.c')

# Gerar os nomes dos objetos .o mantendo a estrutura de pastas ou simplificando
# Aqui vamos simplificar jogando todos os .o na pasta obj/ (flat object folder)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Regra principal (Default)
all: $(APPS)

# Linkagem: Cria o executável
$(APPS): $(OBJS)
	@echo "Ligando objetos..."
	$(CC) $(OBJS) -o $@

# Compilação: Transforma .c em .o
# A flag -p cria a pasta obj se não existir
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	@echo "Limpando arquivos temporários..."
	rm -rf $(OBJ_DIR) $(APPS) core
	
run: all
	./hospital_sus

# Rodar com Valgrind (opcional)
valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(APPS)

.PHONY: all clean valgrind
