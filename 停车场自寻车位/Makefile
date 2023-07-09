TARGET=target
SRCS=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRCS))
CC=g++
FLAGS=-std=c++11 


#指定库文件路径
LIBS = -L ./lib/tts -lTTS -lmsc -L./lib/curl -lcurl -L./lib/json -ljsoncpp -L./lib/openssl -lcrypto -lssl -pthread

#指定头文件路径
INCLUDES = -I./inc

$(TARGET):$(OBJ)
	$(CC)  $^ -o $@  $(INCLUDES) $(LIBS)	$(FLAGS) 
%.o:%.cpp
	$(CC) -o $@ -c $< $(INCLUDES)           $(FLAGS) 


clean:
	rm *.o $(TARGET)
