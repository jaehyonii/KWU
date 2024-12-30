transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/RCA {C:/Users/kk200/verilog/RCA/ha.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/RCA {C:/Users/kk200/verilog/RCA/fa.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/RCA {C:/Users/kk200/verilog/RCA/gates.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/RCA {C:/Users/kk200/verilog/RCA/rca4.v}

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/RCA {C:/Users/kk200/verilog/RCA/tb_rca.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb_rca

add wave *
view structure
view signals
run -all
