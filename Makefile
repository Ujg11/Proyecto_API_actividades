NAME = api

SRCS = main.cpp Usuari.cpp Activitat.cpp SqlManager.cpp

OBJS = $(SRCS:.cpp=.o)

# Directoris llibreria mysql connector/C++
MYSQL_INCLUDE_DIR = /home/ojimenez/Documentos/mysql-connector-c++-9.0.0-linux-glibc2.28-x86-64bit/include/jdbc
MYSQL_LIB_DIR = /home/ojimenez/Documentos/mysql-connector-c++-9.0.0-linux-glibc2.28-x86-64bit/lib64

# Llibreries a enllaçar
LIBS = -lmysqlcppconn -lboost_system

# Opcions de compilació
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -I$(MYSQL_INCLUDE_DIR) 

include db_config.mk

DB_SQL = create_database.sql

create_db:
	mysql -u $(DB_USER) -p$(DB_PASS) < $(DB_SQL)


all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $(NAME) $(OBJS) -L$(MYSQL_LIB_DIR) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re create_db
