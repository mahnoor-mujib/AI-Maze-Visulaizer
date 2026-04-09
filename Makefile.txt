# ==============================
# COMPILERS
# ==============================
CXX = g++
NVCC = nvcc
MPICXX = mpic++

# ==============================
# FLAGS
# ==============================
CXXFLAGS = -std=c++17 -fopenmp -Wall
SDLFLAGS = -lSDL2

# ==============================
# FILES
# ==============================
CPP_FILES = main.cpp maze.cpp solver_omp.cpp visualizer.cpp
CPP_OBJ = $(CPP_FILES:.cpp=.o)

CUDA_FILE = solver_cuda.cu
CUDA_OBJ = solver_cuda.o

MPI_FILE = solver_mpi.cpp

# ==============================
# OUTPUTS
# ==============================
TARGET = maze
MPI_TARGET = mpi_solver

# ==============================
# DEFAULT TARGET
# ==============================
all: $(TARGET) $(MPI_TARGET)

# ==============================
# MAIN BUILD (OpenMP + CUDA + SDL)
# ==============================
$(TARGET): $(CPP_OBJ) $(CUDA_OBJ)
	$(CXX) $(CPP_OBJ) $(CUDA_OBJ) $(CXXFLAGS) $(SDLFLAGS) -o $(TARGET)

# Compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile CUDA file
$(CUDA_OBJ): $(CUDA_FILE)
	$(NVCC) -c $(CUDA_FILE) -o $(CUDA_OBJ)

# ==============================
# MPI BUILD
# ==============================
$(MPI_TARGET): $(MPI_FILE)
	$(MPICXX) $(MPI_FILE) -o $(MPI_TARGET)

# ==============================
# RUN COMMANDS
# ==============================
run:
	./$(TARGET)

run-mpi:
	mpirun -np 4 ./$(MPI_TARGET)

# ==============================
# CLEAN
# ==============================
clean:
	rm -f *.o $(TARGET) $(MPI_TARGET)