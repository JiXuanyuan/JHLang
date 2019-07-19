VPATH	=	.:lib
CC	=	clang++
CFLAGS	=	-I lib -std=c++11 #-D INFO
COMMONH	:=	JBinaryTree.hpp	JList.hpp	JMap.hpp	JSet.hpp \
			JGraph.hpp	JLog.hpp	JNetwork.hpp	JStack.hpp \
			JString.hpp

JHLang : JDFA.o JLex.o main.o 
	$(CC) $(CFLAGS) $? -o $@

%.o : %.cpp %.hpp $(COMMONH)
	$(CC) -c $(CFLAGS) $< -o $@
	# 编译依赖1：$?
%.o : %.cpp $(COMMONH)
	$(CC) -c $(CFLAGS) $< -o $@
	# 编译依赖2：$?
%.o : %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	# 编译依赖3：$?

.PHONY : clean
clean :
	-rm JHLang *.o