# Variables pour le nom du programme, le compilateur et les options de compilation
NAME := ft_irc
PORT := 6667
PSWD := 1234
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -g3 -std=c++98 -ansi -Wshadow
DEBUG := -g3 -fsanitize=address

# Interpreteur de commandes
SHELL := /bin/zsh -o pipefail 
# Si ne marche pas a 42 utiliser ca SHELL := /bin/sh

# Répertoires pour les fichiers objets et de dépendances
OBJDIR := obj
DEPDIR := dep

# Variables pour les fichiers sources, objets et de dépendances
SRCDIR := ./src
INCDIR := ./inc
SRCS := $(SRCDIR)/main.cpp \
		$(SRCDIR)/server.cpp \
		$(SRCDIR)/client.cpp \
		$(SRCDIR)/client2.cpp \
		$(SRCDIR)/utils.cpp \
		$(SRCDIR)/Message.cpp \
		$(SRCDIR)/replyClient.cpp \
		$(SRCDIR)/server2.cpp


OBJ := $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))
DEPS := $(addprefix $(DEPDIR)/, $(notdir $(SRCS:.cpp=.d)))

# Variables pour les messages de compilation
C_RED := \033[31m
C_GREEN := \033[32m
C_CYAN := \033[36m
C_NONE := \033[0m
CROSS := ✘
CHECK := ✔

# Fonction pour l'affichage d'un message de construction
define build_msg
	@echo "$(C_CYAN)Building $<$(C_NONE)"
	@$(1)
	@echo "$(C_GREEN)$(CHECK)$(C_NONE) Built $<"
endef

# Fonction pour l'affichage d'un message de nettoyage
define remove_msg
	@echo "$(C_RED)$(CROSS) Removing $(1)$(C_NONE)"
	@$(RM) -r $(1)
endef

# Cibles principales du Makefile
all: $(NAME)
#	make -j $(nproc)

$(NAME): $(OBJ)
	$(call build_msg,$(CXX) $(CXXFLAGS) $(OBJ) -o $@)

# Règle pour la génération des fichiers de dépendances
$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p $(DEPDIR)
	@$(CXX) $(CXXFLAGS) -I$(INCDIR) -MM -MT '$(OBJDIR)/$(notdir $(patsubst %.cpp,%.o,$<))' $< -MF $@

# Règle pour la compilation des fichiers sources
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	@mkdir -p $(OBJDIR)
	$(call build_msg,$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@)

# Inclure les fichiers de dépendances générés par le compilateur
-include $(DEPS)

# Cibles pour le nettoyage des fichiers générés
clean:
	$(call remove_msg,$(OBJ))
	$(call remove_msg,$(OBJDIR))
	$(call remove_msg,$(DEPS))
	$(call remove_msg,$(DEPDIR))

fclean: clean
	$(call remove_msg,$(NAME))

re: fclean all

# Cible pour exécuter le programme
run: $(NAME)
	./$(NAME) $(PORT) $(PSWD)

# Règle pour le formatage du code avec clang-format
format:
	@echo "Formatting source files..."
	@clang-format -i $(shell find $(SRCDIR) -type f -name "*.cpp" ! -name "*~" -exec echo "{}" \;) \
		$(shell find $(INCDIR) -type f -name "*.hpp" ! -name "*~" -exec echo "{}" \;)
	@echo "Done formatting."

# Règle pour la compilation des fichiers sources en mode debug
debug: fclean
debug: CXXFLAGS += $(DEBUG)
debug: all

.PHONY: all clean fclean re run format debug

