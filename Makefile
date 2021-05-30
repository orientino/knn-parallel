# FF_ROOT     points to the FastFlow root directory (i.e.
#             the one containing the ff directory).
ifndef FF_ROOT 
FF_ROOT		= ${HOME}/fastflow
endif

CXX			= g++ -std=c++17 
INCLUDES	= -I $(FF_ROOT) 
CXXFLAGS  	= -g # -DBLOCKING_MODE -DFF_BOUNDED_BUFFER

LDFLAGS 	= -pthread
OPTFLAGS	= -O3 -finline-functions -DNDEBUG

TARGETS		= knn

.PHONY: all clean
.SUFFIXES: .cpp 

knn: knn.cpp utils.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $? $(LDFLAGS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

all		: $(TARGETS)

clean		: 
	rm -f $(TARGETS)
	rm -f *.o *~