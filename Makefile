NAME = api

SRCS = main.cpp src/Usuari.cpp src/Activitat.cpp src/SqlManager.cpp src/Server.cpp

OBJS = $(SRCS:.cpp=.o)

# Directoris llibreria mysql connector/C++
MYSQL_INCLUDE_DIR = ./lib/mysql/include/jdbc
MYSQL_LIB_DIR = ./lib/mysql/lib64

# Llibreries a enllaçar
LIBS = -lmysqlcppconn -lboost_system

# Opcions de compilació
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -I$(MYSQL_INCLUDE_DIR)

include db_config.mk

DB_SQL = create_database.sql

all: create_db $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $(NAME) $(OBJS) -L$(MYSQL_LIB_DIR) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

create_db:
	mysql -u $(DB_USER) -p$(DB_PASS) < $(DB_SQL)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re create_db
