transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/mx3_64bits.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/bus_arbit.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/bus_addr.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/BUS.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/ram.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/multiplier.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/gates.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/fa_v2.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/clb4.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/cla128.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/cla32.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/cla8.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/cla4.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/cal_radix4.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/FactoCore.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/Top.v}

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/CED_Project {C:/Users/kk200/verilog/CED_Project/tb_Top.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb_Top

add wave *
view structure
view signals
run -all
