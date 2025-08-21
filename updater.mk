PLATFORM = xilinx_u280_gen3x16_xdma_1_202211_1
KERNELS = increase_graph secure_graph 
XO_FILES := $(addsuffix .xo,$(KERNELS))
XILINX_XRT = /opt/xilinx/xrt
ROOT=/home/sail/lyf/pma_demo/disaggregate_kernel
COMMON_SRC=pma.cpp pma_top.cpp memory_pool_in_HBM.cpp util.cpp
define kernel_src
$(1)_SRC := $(ROOT)/$(1).cpp
endef
$(foreach k,$(KERNELS),$(eval $(call kernel_src,$(k))))


MODE ?= hw

prepare_headers:
	cp *.h _x/.

all: increase_graph.xo secure_graph.xo prepare_headers
	v++ -l -t $(MODE)  --config updater.cfg  -g -I$(ROOT)  --platform $(PLATFORM) $(XO_FILES) -o test_update_$(MODE).xclbin  

# $(XO_FILES): %.xo: $($(basename $@)_SRC)
# 	v++ -c -t $(MODE)  --config updater.cfg --platform $(PLATFORM) -k $(basename $@) $^ -o $@
# shared_kernel.xo: $(COMMON_SRC) $(shared_kernel_SRC) prepare_headers
	#  v++ -c -t $(MODE) --platform $(PLATFORM) -g -I$(ROOT) -k shared_kernel $(COMMON_SRC) $(shared_kernel_SRC)  -o $@

ifeq ($(MODE), hw)
increase_graph.xo: $(increase_graph_SRC) prepare_headers
	v++ -c -t $(MODE) --platform $(PLATFORM) -g -I$(ROOT) -k increase_graph $(increase_graph_SRC) $(COMMON_SRC) -o $@
secure_graph.xo: $(secure_graph_SRC) prepare_headers
	v++ -c -t $(MODE) --platform $(PLATFORM) -g -I$(ROOT)  -k secure_graph $(secure_graph_SRC) $(COMMON_SRC)  -o $@
else ifeq ($(MODE), sw_emu)
increase_graph.xo: $(increase_graph_SRC) prepare_headers
	v++ -c -t $(MODE) --platform $(PLATFORM) -g -I$(ROOT) -k increase_graph $(increase_graph_SRC) $(COMMON_SRC) -o $@
secure_graph.xo: $(secure_graph_SRC) prepare_headers
	v++ -c -t $(MODE) --platform $(PLATFORM) -g -I$(ROOT)  -k secure_graph $(secure_graph_SRC) $(COMMON_SRC)  -o $@
endif

host: host.cpp
	g++ -std=c++17 -I${XILINX_XRT}/include -L${XILINX_XRT}/lib -o $@ $^ -lxrt_core -lxrt_coreutil -lOpenCL -lpthread -lrt -ldl -g
