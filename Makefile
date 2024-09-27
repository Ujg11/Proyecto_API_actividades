NAME = api

SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

# Directoris llibreria mysql connector/C++
MYSQL_INCLUDE_DIR = /home/ojimenez/Documentos/mysql-connector-c++-9.0.0-linux-glibc2.28-x86-64bit/include/jdbc
MYSQL_LIB_DIR = /home/ojimenez/Documentos/mysql-connector-c++-9.0.0-linux-glibc2.28-x86-64bit/lib64

# Llibreries a enllaçar
LIBS = -lmysqlcppconn

# Opcions de compilació
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -I$(MYSQL_INCLUDE_DIR)


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