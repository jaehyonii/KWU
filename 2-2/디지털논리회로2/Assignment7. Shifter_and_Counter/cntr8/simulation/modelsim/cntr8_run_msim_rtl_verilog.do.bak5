transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/gates.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/_dff_r.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/fa_v2.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/clb4.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/cla4.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/cla8.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/cntr8.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/os_logic.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/ns_logic.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/_register3_r.v}

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/Assignment7.\ Shifter_and_Counter/cntr8 {C:/Users/kk200/verilog/Assignment7. Shifter_and_Counter/cntr8/tb_cntr8.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb_cntr8

add wave *
view structure
view signals
run -all
