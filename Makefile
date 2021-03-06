############################################
#
# Makefile for DataAnalysis on Linux/Cygwin
#
############################################


CXX            = g++
LD             = g++
RM             = rm -rf
ECHO           = echo

BIN_DIR        = bin
OBJ_DIR        = lib
COMMON_SRC_DIR = source/Common/src
COMMON_INC_DIR = source/Common/include
MCNP_SRC_DIR   = source/MCNPAnalysis/src
MCNP_INC_DIR   = source/MCNPAnalysis/include
SPEC_SRC_DIR   = source/SpectrumAnalysis/src
SPEC_INC_DIR   = source/SpectrumAnalysis/include
PTSIM_SRC_DIR  = source/PTSimAnalysis/src

COMMON_SRC     = $(wildcard $(COMMON_SRC_DIR)/*.cxx)
COMMON_OBJ     = $(patsubst $(COMMON_SRC_DIR)/%.cxx, $(OBJ_DIR)/%.o, $(COMMON_SRC))
MCNP_SRC       = $(wildcard $(MCNP_SRC_DIR)/*.cxx)
MCNP_OBJ       = $(patsubst $(MCNP_SRC_DIR)/%.cxx,   $(OBJ_DIR)/%.o, $(MCNP_SRC))
SPEC_SRC       = $(wildcard $(SPEC_SRC_DIR)/*.cxx)
SPEC_OBJ       = $(patsubst $(SPEC_SRC_DIR)/%.cxx,   $(OBJ_DIR)/%.o, $(SPEC_SRC))
PTSIM_SRC      = $(wildcard $(PTSIM_SRC_DIR)/*.cxx)
PTSIM_OBJ      = $(patsubst $(PTSIM_SRC_DIR)/%.cxx,   $(OBJ_DIR)/%.o, $(PTSIM_SRC))

ROOTCINT       = rootcint
ROOTCONFIG     = root-config
ROOTCXXFLAGS   = $(shell $(ROOTCONFIG) --cflags)
ROOTLIBS       = $(shell $(ROOTCONFIG) --libs) -lTreePlayer -lMinuit -lSpectrum
ROOTINC        = $(shell $(ROOTCONFIG) --incdir)

LIBS           = $(ROOTLIBS)
COMMONFLAGS    = -O2 -Wall -fPIC -I$(COMMON_INC_DIR) $(ROOTCXXFLAGS)
MCNPFLAGS      = -I$(MCNP_INC_DIR)
SPECFLAGS      = -I$(SPEC_INC_DIR)
#PTSIMFLAGS     = -I$(PTSIM_INC_DIR)

RUN_MCNPANALYSIS     = $(BIN_DIR)/MCNPAnalysis
RUN_SPECTRUMANALYSIS = $(BIN_DIR)/SpectrumAnalysis
RUN_PTSIMANALYSIS    = $(BIN_DIR)/PTSimAnalysis

# Main targets

all :  $(RUN_MCNPANALYSIS) $(RUN_SPECTRUMANALYSIS) $(RUN_PTSIMANALYSIS)
mcnp:  $(RUN_MCNPANALYSIS)
spec:  $(RUN_SPECTRUMANALYSIS)
ptsim: $(RUN_PTSIMANALYSIS)

$(OBJ_DIR)/%.o : $(COMMON_SRC_DIR)/%.cxx
	$(CXX) $(COMMONFLAGS) -c $^ -o $@
$(OBJ_DIR)/%.o : $(MCNP_SRC_DIR)/%.cxx
	$(CXX) $(COMMONFLAGS) $(MCNPFLAGS) -c $^ -o $@
$(OBJ_DIR)/%.o : $(SPEC_SRC_DIR)/%.cxx
	$(CXX) $(COMMONFLAGS) $(SPECFLAGS) -c $^ -o $@
$(OBJ_DIR)/%.o : $(PTSIM_SRC_DIR)/%.cxx
	$(CXX) $(COMMONFLAGS) -c $^ -o $@

$(RUN_MCNPANALYSIS): $(COMMON_OBJ) $(MCNP_OBJ)
	$(CXX) $(COMMONFLAGS) $(MCNPFLAGS) -o $@ -g $^ $(LIBS)
$(RUN_SPECTRUMANALYSIS): $(COMMON_OBJ) $(SPEC_OBJ)
	$(CXX) $(COMMONFLAGS) $(SPECFLAGS) -o $@ -g $^ $(LIBS)
$(RUN_PTSIMANALYSIS): $(COMMON_OBJ) $(PTSIM_OBJ)
	$(CXX) $(COMMONFLAGS) $(PTSIMFLAGS) -o $@ -g $^ $(LIBS)

clean:
	$(RM) $(OBJ_DIR)/*.o 
	$(RM) $(RUN_MCNPANALYSIS)
	$(RM) $(RUN_SPECTRUMANALYSIS)
	$(RM) $(RUN_PTSIMANALYSIS)

help:
	@$(ECHO) "Targets:"
	@$(ECHO) "  all     - compile all"
	@$(ECHO) "  mcnp    - compile MCNPAnalysis"
	@$(ECHO) "  spec    - compile SpectrumAnalysis"
	@$(ECHO) "  ptsim   - compile PTSimAnalysis"
	@$(ECHO) "  clean   - cleanup *.o and binaries"
