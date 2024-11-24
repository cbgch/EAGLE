# 定义源文件和目标文件
SOURCES = graph.cpp util.cpp oracle.cpp lockedCircuit.cpp BDE.cpp
LDFLAGS = -fopenmp       # 链接选项，启用 OpenMP 支持
OBJECTS = $(SOURCES:.cpp=.o)  # 将 .cpp 文件替换为 .o 文件
TARGET = BDE             # 最终生成的可执行文件名

# 默认目标
all: $(TARGET)

# 编译规则：如何从 .cpp 文件编译到 .o 文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

# 链接规则：如何从 .o 文件生成最终的可执行文件
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# 清理目标文件和可执行文件
clean:
	rm -f $(OBJECTS) $(TARGET)

# 安装目标（如果需要的话）
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean install