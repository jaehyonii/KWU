transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/week5/_dff {C:/Users/kk200/verilog/week5/_dff/gates.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/week5/_dff {C:/Users/kk200/verilog/week5/_dff/_dlatch.v}
vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/week5/_dff {C:/Users/kk200/verilog/week5/_dff/_dff.v}

vlog -vlog01compat -work work +incdir+C:/Users/kk200/verilog/week5/_dff {C:/Users/kk200/verilog/week5/_dff/tb_dff.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb_dff

add wave *
view structure
view signals
run -all
