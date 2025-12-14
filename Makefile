ifeq ($(OS),Windows_NT)

# =====================
# Toolchain
# =====================
CXX := g++

# =====================
# Vulkan SDK
# =====================
VULKAN_SDK := $(VULKAN_SDK)

# =====================
# Flags
# =====================
CFLAGS  := -std=c++17 -I. -I$(VULKAN_SDK)/Include
LDFLAGS := -L$(VULKAN_SDK)/Lib -lvulkan-1 `pkg-config --static --libs glfw3`

# =====================
# Sources
# =====================
SRCS := $(wildcard *.cpp)
HDRS := $(wildcard *.hpp)

# =====================
# Shaders
# =====================
VERT_SRCS := $(wildcard shaders/*.vert)
FRAG_SRCS := $(wildcard shaders/*.frag)
SPV_FILES := $(VERT_SRCS:.vert=.vert.spv) $(FRAG_SRCS:.frag=.frag.spv)

# =====================
# Output
# =====================
TARGET := a.exe

# =====================
# Build rules
# =====================
all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS) $(SPV_FILES)
	$(CXX) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

# Shader compilation
%.spv: %
	glslc $< -o $@

# =====================
# Utility
# =====================
.PHONY: run clean

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) shaders/*.spv

else
CFLAGS = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

# create list of all spv files and set as dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = a.out
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): *.cpp *.hpp
	g++ $(CFLAGS) -o $(TARGET) *.cpp $(LDFLAGS)

# make shader targets
%.spv: %
	./compile.sh

.PHONY: test clean

test: a.out
	./a.out

clean:
	rm -f a.out
	rm -f shaders/*.spv

endif